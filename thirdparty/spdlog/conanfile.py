from conans import ConanFile, CMake

class Spdlog(ConanFile):
    name = "spdlog"
    version = "1.6.1"
    description = """Logging library for C++"""
    license = "MIT"
    author = "gabime"
    settings = "os", "compiler", "build_type", "arch"
    scm = {
        "type": "git",
        "url": "https://github.com/gabime/spdlog.git",
        "revision": "v%s" % version
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.definitions["SPDLOG_FMT_EXTERNAL"] = "ON"
        cmake.definitions["SPDLOG_ENABLE_PCH"] = "ON"
        cmake.build()

    def package(self):
        self.copy("*", "include", "include")
        if self.settings.os == "Windows":
            self.copy("*", "lib", "%s" % self.settings.build_type)
        if self.settings.os == "Linux":
            self.copy("*.so*", "lib", ".")
            self.copy("*.a", "lib", ".")

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["spdlogd"]
        else:
            self.cpp_info.libs = ["spdlog"]
