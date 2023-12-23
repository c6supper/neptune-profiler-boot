Neptune-Profiler-Boot
===========

A boot profiler for hypervisor based system

Prerequisites
-----------------

> * Install VSCode/LLVM-16 or 17(include clang & tools)/Python3
> * pip3 install pycairo

How to Build
-----------------

> * mkdir build && cd build
> * for Linux testing purpose
  ```bash
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../
  make -j$(nproc)
  ```
> * for QNX
  ```bash
  cmake -DCMAKE_CROSSCOMPILING=1 -DCMAKE_TOOLCHAIN_FILE=$PROJECT_ROOT/cmake/QNXToolchain.cmake \
  find . -name "link.txt" -exec sed -i "s/-lrt//g" {} + && \
  find . -name "link.txt" -exec sed -i "s/-lpthread//g" {} + && \
  make -j$(nproc)
  ```
> * make install

How to Run
-----------------

> * 

License
-------

Neptune-Profiler-Boot is released under the Apache License, Version 2.0. See [LICENSE](LICENSE) for the full license text.

Copyright
---------

Copyright 2024 Coding Nerd
