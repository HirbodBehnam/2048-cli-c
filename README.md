# 2048 CLI
A simple 2048 game written in C
## Compiling
Before compiling make sure that you have ncurses on your sytem

At first clone this repository:
```bash
git clone https://github.com/HirbodBehnam/2048-cli-c
```
### Manual Compiling
```bash
gcc -O3 -o 2048-cli main.c -lncursesw
```
### CMake
Cmake 2.8.10 or higher is needed
```bash
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```