from conans import ConanFile, CMake, tools


class HiredisConan(ConanFile):
    name = "hiredis"
    version = "2020.10.29"
    license = "https://github.com/redis/hiredis/blob/master/COPYING"
    author = "Jeremy Zheng"
    url = "https://github.com/redis/hiredis.git"
    description = "Hiredis is a minimalistic C client library for the Redis database."
    topics = ("redis")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        git = tools.Git(folder="hiredis")
        git.clone("https://github.com/redis/hiredis.git")
        git.checkout("e3f88eb")

    def build(self):
        cmake = CMake(self)
        if self.options.shared:
            cmake.definitions["BUILD_SHARED_LIBS"] = "ON"
            cmake.definitions["BUILD_STATIC_LIBS"] = "OFF"
        else:
            cmake.definitions["BUILD_SHARED_LIBS"] = "OFF"
            cmake.definitions["BUILD_STATIC_LIBS"] = "ON"
        cmake.definitions["DISABLE_TESTS"] = "ON"
        cmake.configure(source_folder="hiredis")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="hiredis")
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["hiredis_static"]
