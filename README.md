# ashoka

Saraca asoca (the ashoka tree)

## Development

```bash
$ docker build -t ashoka .
$ docker run --rm -it --network host -v `pwd`:/workspace ashoka
> CXX=clang++ CC=clang cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/local/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release ..
> make -j
```

```bash
$ openssl rand -base64 32 # generate a random 32-bit base64 encoded key
```

## Documents

- [format](https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/DataTypesSummary.pdf)
- [vspkg](https://github.com/microsoft/vcpkg)
