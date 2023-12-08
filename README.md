# Terraria Boss Rush
by Denis Anisimov and Stepan Mizyurin
### Configure
C++ 17 & CMake 3.11 or higher are required

Might not work with MSVC compiler
```cmake
cmake -D CMAKE_BUILD_TYPE:STRING=Release -B ./build
```
GCC compiler is preffered (specify the path if needed)
```cmake
cmake -D CMAKE_BUILD_TYPE:STRING=Release -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -B ./build -G "MinGW Makefiles"
```
### Build

```cmake
cmake --build ./build --config Release --target TerrariaBossRush
```
