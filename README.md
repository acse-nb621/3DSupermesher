# 3DSupermesher
This project develops a supermeshing algorithm to aid the conservative interpolation between two 3D tetrahedralized meshes.

# Set Up
It was developed with and tested on WSL 1 shell on Windows 10, with Ubuntu 20.04.4 LTS. The CPU on the HP machine used has 4 cores and 8 logical processors, with a base speed of 2.80 GHz.
The code is wholly in C++, and the compiler used is GNU compiler GCC version 9.4.0.

# Usage
To compile the tests found in irp-nb621/code/library/tests, run the following command two commands:
```
g++ ../source/node.cpp ../source/face.cpp ../source/ele.cpp ../source/supermesh.cpp test_random_tetrahedra.cpp -o test
./test
```
The code is documented sufficiently in the header files in irp-nb621/code/library/source.
For compiling the file bar_mer/driver.cpp, please check irp-nb621/documents/tetgen_manual.pdf page 23 and compile TetGen in the same directory. Then run:
```
g++ ../source/node.cpp ../source/face.cpp ../source/ele.cpp -o test tetcall.cxx -L./ -ltet
./test
```
