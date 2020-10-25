# ashoka

Saraca asoca (the ashoka tree)

## Development

```bash
$ docker build -t ashoka .
$ docker run --rm -it --network host -v `pwd`:/workspace ashoka
> mkdir build && cd build
> conan install --build=missing .. --profile ../arm
> conan install --build=missing .. --profile ../x64
> cmake -DCMAKE_BUILD_TYPE=Release ..
> make -j
```

```bash
$ openssl rand -base64 32 # generate a random 32-bit base64 encoded key
$ conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
$ conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI
```

## Documents

- [format](https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/DataTypesSummary.pdf)
- [vspkg](https://github.com/microsoft/vcpkg)
- [conan](https://conan.io/center/)
