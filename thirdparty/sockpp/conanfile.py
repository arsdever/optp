import os
from conans import ConanFile, CMake

class Sockpp(ConanFile):
    name = "sockpp"
    version = "0.7"
    description = """Sockpp - socket wrapper library C++"""
    license = "MIT"
    author = "nlohmann"
    settings = "os", "compiler", "build_type", "arch"
    scm = {
        "type": "git",
        "url": "https://github.com/arsdever/sockpp.git",
        "revision": "develop"
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*", "include", "include")
        if self.settings.os == "Windows":
            self.copy("*", "lib", "%s" % self.settings.build_type)
        if self.settings.os == "Linux":
            self.copy("*.so*", "lib", ".")

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        if self.settings.os == "Windows":
            self.cpp_info.libs = ["sockpp-static"]
            self.cpp_info.system_libs = ["ws2_32"]
        if self.settings.os == "Linux":
            self.cpp_info.libs = ["sockpp"]