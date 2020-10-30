import os

from conans import ConanFile,  tools, AutoToolsBuildEnvironment


class LibsodiumConan(ConanFile):
    name = "libsodium"
    version = "1.0.18"
    license = "ISC"
    author = "Jeremy Zheng"
    url = "https://github.com/jedisct1/libsodium"
    description = "Sodium is a new, easy-to-use software library for encryption, decryption, signatures, password hashing and more."
    topics = ("NaCI")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "make"

    def source(self):
        tools.get(
            "https://download.libsodium.org/libsodium/releases/libsodium-1.0.18.tar.gz")

        os.rename(self.name + "-" + self.version, self.name)
        # git = tools.Git(folder="libsodium")
        # git.clone("https://github.com/jedisct1/libsodium.git", "1.0.18")

    def build(self):
        with tools.chdir("libsodium"):
            autotools = AutoToolsBuildEnvironment(self)
            autotools.configure(host=None)
            autotools.make()

    def package(self):
        self.copy("*.h", dst="include", src="libsodium")
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["sodium"]
