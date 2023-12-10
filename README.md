Neptune-Profiler-Boot
===========

A boot profiler for hypervisor based system

Prerequisites
-----------------

> * 

How to Build
-----------------

> * mkdir build && cd build
> * for x86 testing purpose
>> * cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../
> * for QNX
  ```bash
  cmake -DCMAKE_CROSSCOMPILING=1 -DCMAKE_TOOLCHAIN_FILE=$PROJECT_ROOT/cmake/QNXToolchain.cmake \
  find . -name "link.txt" -exec sed -i "s/-lrt//g" {} + && \
  find . -name "link.txt" -exec sed -i "s/-lpthread//g" {} + && \
  make -j$(nproc)
  ```
> * make install

Documentation
-----------------

Read the API Docs [here](doc/neptune-profiler-boot-doc.md)

License
-------

Neptune-Profiler-Boot is released under the Apache License, Version 2.0. See [LICENSE](LICENSE) for the full license text.

Copyright
---------

Copyright 2024 Coding Nerd
