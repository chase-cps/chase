The C++ API provided by the CHASE Library can be accessed by using Python.
The interfacing between the two languages relies on [**pybind**][PYBIND]. Pybind is
already provided in the `third_party` directory, while the bindings are defined 
in the following file: 

`src/core/bindings/bindings.cc`

The bindings expose the same interface provided by the C++ API. Thus, refer
to the (doxygen-based) documentation of the library for details about the provided
functionalities.

The binding is based on Python 3. Thus, it requires Python 3.x, and it has been
tested using Python 3.6.

In order to use the bindings it is necessary that the python 3 interpreter is
able to see the file:

`chasecorebnd.cpython-36m-x86_64-linux-gnu.so`

The file is generated during the compilation, and installed according to the
following path:

`$INSTALL_DIRECTORY/core/pylib/chasecorebnd.cpython-36m-x86_64-linux-gnu.so`

Thus, it is necessary to set the `PYTHONPATH` environment variable in order to
make the library visible to the python interpreter. For instance, in Unix-based
systems:

`$> export PYTHONPATH=$PYTHONPATH:$INSTALL_DIRECTORY/core/pylib`

Some examples showing the usage of the Python API are available in the 
`demo/python_examples/` directory.


[PYBIND]: https://github.com/pybind/pybind11 
[BINDINGS]: src/core/bindings/bindings.cc