# ashoka

Saraca asoca (the ashoka tree)

## Build

```bash
$ docker build -t ashoka .
$ docker run --rm -it --network host -v `pwd`:/workspace ashoka
> ./deb.sh
> dpkg -i ashoka.deb # show
> dpkg -L ashoka # install
> dpkg -r ashoka # uninstall
```

```bash
$ openssl rand -base64 32 # generate a random 32-bit base64 encoded key
```

## Documents

- [format](https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/DataTypesSummary.pdf)
- [vspkg](https://github.com/microsoft/vcpkg)
- [conan](https://conan.io/center/)
- [grpc](https://grpc.io/docs/languages/cpp/quickstart/)
- [protobuf](https://developers.google.com/protocol-buffers/docs/reference/overview)

- [sfml](https://www.sfml-dev.org/index.php)
- [libsodium](https://doc.libsodium.org/)
