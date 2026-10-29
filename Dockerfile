###############################################################################
# Docker image with the complete CHASE toolchain:
#   - CHASE core-library, chase-backends, DSL_tool, logics_tool (+ Python API)
#   - external solvers used by the backends: gr1c, slugs, NuSMV, Yosys
#     (yowasp-yosys), and nuXmv (x86_64 only, when its license is accepted,
#     see below)
#
# The CHASE modules are built from the git submodules of this repository,
# which must be initialized before building the image:
#   git submodule update --init --recursive
#   docker build -t chase .
#
# nuXmv may be used only for academic and non-commercial purposes
# (https://nuxmv.fbk.eu/license.html). It is downloaded into the image only
# when its license is accepted:
#   docker build --build-arg ACCEPT_NUXMV_LICENSE=yes -t chase .
# Without it, the checks use NuSMV (no real variables, no IC3). In a
# container, `chase-cps install nuxmv --accept-license` adds nuXmv.
#
# NuSMV is built from its sources without zChaff, whose license forbids
# redistribution: the binary is under the LGPL 2.1 (NuSMV) and the MIT
# license (MiniSat), which come with it in /usr/local/share/doc/nusmv.
#
# Run the demos:
#   docker run --rm -it chase
#   docker run --rm chase /opt/chase/tests/demo-suite.sh
###############################################################################

ARG UBUNTU_VERSION=24.04

###############################################################################
# NuSMV 2.6.0, from https://nusmv.fbk.eu/distrib/NuSMV-2.6.0.tar.gz. Its build
# needs Python 2 and an older compiler, hence Ubuntu 18.04 (the binary needs
# only libc and libstdc++). Without zChaff, readline (GPL), and libxml2; two
# fixes for newer compilers (MiniSat patch, `union wait` in CUDD).
###############################################################################
FROM ubuntu:18.04 AS nusmv
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake curl ca-certificates python bison flex patch \
    && rm -rf /var/lib/apt/lists/*
RUN curl -fsSL https://nusmv.fbk.eu/distrib/NuSMV-2.6.0.tar.gz | tar xz -C /tmp \
    && sed -i 's/extern "C"void/extern "C" void/' /tmp/NuSMV-2.6.0/MiniSat/MiniSat_v37dc6c6_nusmv.patch \
    && sed -i 's/union wait status;/int status;/' /tmp/NuSMV-2.6.0/cudd-2.4.1.1/util/pipefork.c \
    && mkdir /tmp/nusmv-build && cd /tmp/nusmv-build \
    && cmake /tmp/NuSMV-2.6.0/NuSMV -DCMAKE_BUILD_TYPE=Release \
        -DENABLE_ZCHAFF=OFF -DENABLE_READLINE=OFF -DENABLE_LIBXML2=OFF \
        -DENABLE_STATIC_LINK=ON -DBUILD_CMD_HELP=OFF -DCMAKE_INSTALL_PREFIX=/opt/nusmv \
    && (make -j"$(nproc)" || make) && make install \
    && cd / \
    && mkdir -p /opt/nusmv/doc \
    && cp /tmp/NuSMV-2.6.0/NuSMV/LGPL-2.1 /tmp/NuSMV-2.6.0/NuSMV/README_MiniSat.txt /opt/nusmv/doc/ \
    && echo "NuSMV 2.6.0 built from https://nusmv.fbk.eu/distrib/NuSMV-2.6.0.tar.gz without zChaff." \
        > /opt/nusmv/doc/SOURCE \
    && rm -rf /tmp/NuSMV-2.6.0 /tmp/nusmv-build

FROM ubuntu:${UBUNTU_VERSION} AS base
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake git ca-certificates curl pkg-config \
        python3 python3-dev python3-pip python3-pytest python3-z3 pybind11-dev uuid-dev \
        bison flex libboost-dev graphviz less libxml2 libedit2 xz-utils \
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
    && mkdir -p /usr/local/share/doc/gr1c && cp LICENSE.txt /usr/local/share/doc/gr1c/ \
    && rm -rf /tmp/gr1c

# slugs (GR(1) synthesis) and its structured-slugs compiler
RUN git clone https://github.com/VerifiableRobotics/slugs /opt/slugs \
    && cd /opt/slugs && git checkout ${SLUGS_REF} \
    && sed -i 's/#ifndef __x86_64/#if !defined(__x86_64) \&\& !defined(__aarch64__)/' \
        src/BFAbstractionLibrary/compilerOptionGenerator.c \
    && cd src && make -j"$(nproc)" \
    && ln -s /opt/slugs/src/slugs /usr/local/bin/slugs

# NuSMV (above), and Yosys for RTL input (yowasp-yosys, ISC license; the first
# run compiles it, so it is run once here)
COPY --from=nusmv /opt/nusmv/bin/NuSMV /usr/local/bin/NuSMV
COPY --from=nusmv /opt/nusmv/doc /usr/local/share/doc/nusmv
RUN pip3 install --no-cache-dir --break-system-packages yowasp-yosys \
    && yowasp-yosys -V

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
    CHASE_HOME=/opt/chase/tools \
    PATH=/opt/chase/tools/bin:/opt/chase/chase/bin:${PATH} \
    PYTHONPATH=/opt/chase/python:/opt/chase/chase/lib \
    SLUGSCOMPILER=/opt/slugs/tools/StructuredSlugsParser/compiler.py

# the command line of the Python package (chase-cps install, doctor, check)
RUN printf '#!/bin/sh\nexec python3 -m chase_cps "$@"\n' > /usr/local/bin/chase-cps \
    && chmod +x /usr/local/bin/chase-cps

# nuXmv (model checking), installed by chase-cps when its license is
# accepted. FBK ships it for x86_64 Linux only.
ARG ACCEPT_NUXMV_LICENSE=no
RUN if [ "${ACCEPT_NUXMV_LICENSE}" = "yes" ] && [ "$(uname -m)" = "x86_64" ]; then \
        python3 -m chase_cps install nuxmv --accept-license; \
    elif [ "${ACCEPT_NUXMV_LICENSE}" = "yes" ]; then \
        echo "nuXmv: no build for $(uname -m), not installed."; \
    else \
        echo "nuXmv: license not accepted, not installed" \
             "(docker build --build-arg ACCEPT_NUXMV_LICENSE=yes)."; \
    fi

LABEL org.opencontainers.image.source=https://github.com/chase-cps/chase \
      org.opencontainers.image.description="CHASE: contract-based design, with its tools, solvers, and demos" \
      org.opencontainers.image.licenses=BSD-3-Clause
WORKDIR /opt/chase/demos
CMD ["/bin/bash"]
