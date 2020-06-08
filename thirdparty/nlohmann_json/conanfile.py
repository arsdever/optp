import os
from conans import ConanFile, CMake

class NlohmannJson(ConanFile):
    name = "nlohmann_json"
    version = "3.7.3"
    description = """Json library for C++"""
    url = "https://github.com/nlohmann/json.git"
    license = "MIT"
    author = "nlohmann"
    scm = {
        "type": "git",
        "url": "https://github.com/nlohmann/json.git",
        "revision": "develop"
    }
    options = {
        "testing": [True, False]
    }
    default_options = {
        "testing": False
    }

    def build(self):
        cmake = CMake(self)

        cmake.definitions["JSON_BuildTests"] = self.options.testing
        
        cmake.configure()
        cmake.build()

    def package(self):
        print(os.getcwd())
        self.copy("*", "include", "single_include")

    def package_info(self):
        self.cpp_info.includedirs = ["include"]