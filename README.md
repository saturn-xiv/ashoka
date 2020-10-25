# ashoka

Saraca asoca (the ashoka tree)

## Development

```bash
$ sudo apt-get update
$ sudo apt-get install git build-essential tar curl zip unzip
$ bash env.sh # ensure all packages installed
$ mkdir build
$ cd build
$ CXX=clang++ CC=clang cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/local/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j
```

```bash
$ openssl rand -base64 32 # generate a random 32-bit base64 encoded key
```

## Documents

- [format](https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/DataTypesSummary.pdf)
- [vspkg](https://github.com/microsoft/vcpkg)
