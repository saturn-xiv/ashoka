# ASHOKA

A web application by C++ and React.

## Usage

```bash
git clone https://github.com/saturn-xiv/ashoka.git
cd ashoka/docker && ./build.sh && cd -
docker run --rm -it --network host -v `pwd`:/workspace ashoka-xenial # bionic, focal
./docker/scripts/deb.sh x64 demo # build for x86_64
./docker/scripts/deb.sh linaro demo # build for arm7hf
```

- generate a random 32-bit base64 encoded key

```bash
openssl rand -base64 32
```

- enable sudo with-out-password

```bash
echo 'deploy ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/101-deploy
```

## Documents

- [toml](https://toml.io/en/)
- [strftime](http://www.cplusplus.com/reference/ctime/strftime/)
- [crontab guru](https://crontab.guru/)
- [libsodium](https://doc.libsodium.org/)
- [A Template Engine for Modern C++](https://pantor.github.io/inja/)
