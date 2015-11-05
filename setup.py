from distutils.core import setup
from distutils.command.build import build as _build

from shutil import rmtree
from os import path, makedirs, chdir
import subprocess
import sys

cmake_bin = "cmake"
cmake_build_config = "Release"
cmake_build_target = "_tinysplinepython"

build_dir_name = "build-python-distutils"

script_dir = path.dirname(path.realpath(__file__))
src_dir = path.join(script_dir, "library")
build_dir = path.join(script_dir, build_dir_name)


class build(_build):
    def run(self):
        try:
            # create build directory if necessary
            if not path.exists(build_dir):
                print("creating directory: " + build_dir_name)
                makedirs(build_dir)

            # generate make files (or any kind of project)
            print("generating cmake tree")
            chdir(build_dir)
            cmake_cmd = [cmake_bin, src_dir,
                         "-DCMAKE_BUILD_TYPE=" + cmake_build_config]
            if subprocess.call(cmake_cmd) != 0:
                raise EnvironmentError("error calling cmake")
            chdir(script_dir)

            # build the python binding
            print("generating python binding")
            cmake_cmd = [cmake_bin, "--build", build_dir,
                         "--config", cmake_build_config,
                         "--target", cmake_build_target]
            if subprocess.call(cmake_cmd) != 0:
                raise EnvironmentError("error building project")

            # create __init__.py
            print("creating file: __init__.py")
            init_file = open(path.join(build_dir, "__init__.py"), "w+")
            init_file.writelines("from tinyspline import *")
            init_file.close()

            # distutils uses old-style classes, so no super()
            _build.run(self)
        except EnvironmentError as e:
            print(e.strerror)
        finally:
            # remove build directory if necessary
            if path.exists(build_dir):
                print("removing directory: " + build_dir_name)
                rmtree(build_dir)


# ********************************************************
# *                                                      *
# * Main                                                 *
# *                                                      *
# ********************************************************
if len(sys.argv) >= 2 and sys.argv[1] == "sdist":
    print("preparing for sdist")
    package_dir = "."
    package_data = []
else:
    package_dir = build_dir_name
    package_data = ["*tinysplinepython*"]

setup(name="tinyspline",
      version="0.1.0.dev",
      description="Python binding for TinySpline",
      long_description="""
      TinySpline is a C library for NURBS, B-Splines and Bezier curves
      (even lines and points) with a modern C++11 wrapper and bindings
      for C#, Java and Python (via Swig). The goal of this project is
      to provide a small library with a minimum set of dependencies
      which is easy and intuitively to use.""",
      author="Marcel Steinbeck",
      author_email="github@retux.de",
      license="MIT",
      url="https://github.com/retuxx/tinyspline",
      platforms="Any",
      cmdclass={"build": build},
      packages=["tinyspline"],
      package_dir={"tinyspline": package_dir},
      package_data={"tinyspline": package_data}
      )
