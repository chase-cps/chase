###############################################################################
# Docker image with the complete CHASE toolchain:
#   - CHASE core-library, chase-backends, DSL_tool, logics_tool (+ Python API)
#   - external solvers used by the backends: gr1c, slugs, NuSMV (x86_64 only)
#
# The CHASE modules are built from the git submodules of this repository,
# which must be initialized before building the image:
#   git submodule update --init --recursive
#   docker build -t chase .
#
# Run the demos:
#   docker run --rm -it chase
#   docker run --rm chase /opt/chase/tests/demo-suite.sh
###############################################################################

ARG UBUNTU_VERSION=22.04

FROM ubuntu:${UBUNTU_VERSION} AS base
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake git ca-certificates curl pkg-config \
        python3 python3-dev python3-pytest pybind11-dev uuid-dev \
        bison flex libboost-dev graphviz less \
    && rm -rf /var/lib/apt/lists/*

###############################################################################
# External solvers.
###############################################################################
FROM base AS solvers
ARG GR1C_REF=v0.13.3
ARG SLUGS_REF=ac09bf652d4ff47a7018d6a41627af73a67a049c
ARG TARGETARCH

# gr1c (GR(1) synthesis)
RUN git clone https://github.com/tulip-control/gr1c /tmp/gr1c \
    && cd /tmp/gr1c && git checkout ${GR1C_REF} \
    && ./get-deps.sh && ./build-deps.sh \
    && make -j"$(nproc)" && make install prefix=/usr/local \
    && rm -rf /tmp/gr1c

# slugs (GR(1) synthesis) and its structured-slugs compiler
RUN git clone https://github.com/VerifiableRobotics/slugs /opt/slugs \
    && cd /opt/slugs && git checkout ${SLUGS_REF} \
    && sed -i 's/#ifndef __x86_64/#if !defined(__x86_64) \&\& !defined(__aarch64__)/' \
        src/BFAbstractionLibrary/compilerOptionGenerator.c \
    && cd src && make -j"$(nproc)" \
    && ln -s /opt/slugs/src/slugs /usr/local/bin/slugs

# NuSMV (model checking). Upstream only ships x86_64 Linux binaries.
RUN if [ "${TARGETARCH}" = "amd64" ]; then \
        curl -fsSL -o /tmp/nusmv.tgz \
            https://nusmv.fbk.eu/distrib/NuSMV-2.6.0-linux64.tar.gz \
        && echo "11b12e42bc04566c4ec1f7cc205fa8704382ac4d69e052154cbb59c9b7e8e08c  /tmp/nusmv.tgz" | sha256sum -c - \
        && tar xzf /tmp/nusmv.tgz -C /opt \
        && ln -s /opt/NuSMV-2.6.0-Linux/bin/NuSMV /usr/local/bin/NuSMV \
        && rm /tmp/nusmv.tgz; \
    else \
        echo "NuSMV: no prebuilt binary for ${TARGETARCH}, skipping."; \
    fi

###############################################################################
# CHASE.
###############################################################################
FROM solvers AS chase
ARG BUILD_TYPE=Release

COPY CMakeLists.txt /src/chase/
COPY modules /src/chase/modules

RUN for m in third_party core-library chase-backends DSL_tool logics_tool; do \
        test -f /src/chase/modules/$m/CMakeLists.txt || { \
            echo "modules/$m is empty: run 'git submodule update --init --recursive'"; \
            exit 1; }; \
    done \
    && cmake -S /src/chase -B /build \
        -DCMAKE_INSTALL_PREFIX=/opt/chase \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -Dpybind11_DIR=/usr/lib/cmake/pybind11 \
        -DPython_EXECUTABLE=/usr/bin/python3 \
        -DCHASE_BUILD_TESTS=ON \
    && cmake --build /build \
    && (cd /build && ctest --output-on-failure) \
    && rm -rf /build /src

COPY demos /opt/chase/demos
COPY tests/demo-suite.sh /opt/chase/tests/demo-suite.sh
COPY python /opt/chase/python

ENV CHASE_INSTALL_DIR=/opt/chase \
    PATH=/opt/chase/chase/bin:${PATH} \
    PYTHONPATH=/opt/chase/python:/opt/chase/chase/lib \
    SLUGSCOMPILER=/opt/slugs/tools/StructuredSlugsParser/compiler.py

WORKDIR /opt/chase/demos
CMD ["/bin/bash"]
