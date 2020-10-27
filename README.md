# ashoka

Saraca asoca (the ashoka tree)

## Build

Insure install [protocolbuf v3.11.4](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) at first.

```bash
$ docker build -t ashoka .
$ docker run --rm -it --network host -v `pwd`:/workspace ashoka
> ./build.sh
> dpkg -i ashoka.deb # show
> dpkg -L ashoka # install
> dpkg -r ashoka # uninstall
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
- [grpc](https://grpc.io/docs/languages/cpp/quickstart/)
- [protobuf](https://developers.google.com/protocol-buffers/docs/reference/overview)
- [sfml](https://www.sfml-dev.org/index.php)
