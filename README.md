# SSD Maintainer

Small utility to read a whole SSD disk.
This is since there is a data degradation in SSDs, 

where the electrical current in cells slowly fades and dissipates.

With this utility, it will read all data to "reset" the cells charge.

I might need to add a write feature which will rewrite the whole disk, but since it is risky, I'm still exploring this feature.

## Build

Requires CMake and a C++17 compliant (GCC preferably) compiler

```
git clone https://github.com/TheLoneDev/ssd-maintainer
cd ssd-maintainer
mkdir build
cmake -B build .
make -C build -j 32   (replace 32 with number of logical cores)
```

After that the binary will be compiled in the bin directory

## Usage

Just follow the instructions given in console, make sure to run as root or have full permissions to read the block device.

![Example Image](https://i.imgur.com/xureaL2.png)