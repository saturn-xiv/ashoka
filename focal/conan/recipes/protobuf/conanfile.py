from conans import ConanFile, CMake, tools


class ProtobufConan(ConanFile):
    name = "protobuf"
    version = "3.13.0"
    license = "https://github.com/protocolbuffers/protobuf/blob/master/LICENSE"
    author = "Jeremy Zheng"
    url = "https://github.com/protocolbuffers/protobuf"
    description = "Google's data interchange format"
    topics = ("protobuf")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        git = tools.Git(folder="protobuf")
        git.clone("https://github.com/protocolbuffers/protobuf.git",
                  "v"+self.version)

    def build(self):
        cmake = CMake(self)
        cmake.definitions["protobuf_BUILD_TESTS"] = False
        cmake.configure(source_folder="protobuf/cmake")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="protobuf/src")
        self.copy("*.inc", dst="include", src="protobuf/src")
        self.copy("*.proto", dst="include", src="protobuf/src")
        self.copy("protoc", dst="bin", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["protobuf"]
