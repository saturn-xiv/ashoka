from conans import ConanFile, CMake, tools


class JwtppConan(ConanFile):
    name = "jwtpp"
    version = "2.0.3"
    license = "MIT"
    author = "Jeremy Zheng"
    url = "https://github.com/troian/jwtpp"
    description = "JSON Object Signing and Encryption library for C++"
    topics = ("jwt")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake_find_package_multi", "cmake", "pkg_config"

    def requirements(self):
        self.requires("openssl/1.1.1h")
        self.requires("jsoncpp/1.9.4")

    def configure(self):
        self.options["openssl"].shared = self.options.shared

    def source(self):
        git = tools.Git(folder="jwtpp")
        git.clone("https://github.com/troian/jwtpp.git", "v"+self.version)

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="jwtpp")
        cmake.build()

    def package(self):
        self.copy("*.hh", dst="include", src="jwtpp/include/export")
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["jwtpp"]
