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


Cheetsheet for Perfetto SQL
-----------------

> * Statistics for Context Switching per Core
~~~~sql
 drop view context_switch_count;
 create view context_switch_count as select id,ts,name,cpu,count(CASE WHEN cpu=0 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu0,count(CASE WHEN cpu=1 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu1,count(CASE WHEN cpu=2 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu2,count(CASE WHEN cpu=3 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu3,count(CASE WHEN cpu=4 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu4,count(CASE WHEN cpu=5 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu5,count(CASE WHEN cpu=6 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu6,count(CASE WHEN cpu=7 THEN 1 END) over (ROWS 1400 PRECEDING) as cpu7 from ftrace_event;
select cast(ts as double)/1000000000 as ts,cpu0,cpu1,cpu2,cpu3,cpu4,cpu5,cpu6,cpu7 from context_switch_count where id % 1400 = 0;
~~~~

License
-------

Neptune-Profiler-Boot is released under the Apache License, Version 2.0. See [LICENSE](LICENSE) for the full license text.

Copyright
---------

Copyright 2024 Coding Nerd
