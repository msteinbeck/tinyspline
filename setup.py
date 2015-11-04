from os import path, makedirs, chdir
import subprocess
from distutils.core import setup
from distutils.command.build import build as _build

cmake_bin = "cmake"
cmake_dir = "build-python-distutils"
cmake_build_config = "Release"
cmake_build_target = "_tinysplinepython"

script_dir = path.dirname(path.realpath(__file__))
build_dir = script_dir + path.sep + cmake_dir


class BuildWithCmake(_build):
    def run(self):
        # create the build directory if necessary
        if not path.exists(build_dir):
            makedirs(build_dir)

        # generate make files
        chdir(build_dir)
        cmake_cmd = [cmake_bin, "..",
                     "-DCMAKE_BUILD_TYPE=" + cmake_build_config]
        if subprocess.call(cmake_cmd) != 0:
            raise EnvironmentError("error calling cmake")
        chdir(script_dir)

        # build the python binding
        cmake_cmd = [cmake_bin, "--build", build_dir,
                     "--config", cmake_build_config,
                     "--target", cmake_build_target]
        if subprocess.call(cmake_cmd) != 0:
            raise EnvironmentError("error building project")

        # can't use super() here because _build is an
        # old style class in 2.7
        _build.run(self)


setup(name='tinyspline',
      version='0.1.0.dev',
      description='Python binding for TinySpline',
      long_description='''
      TinySpline is a C library for NURBS, B-Splines and Bezier curves
      (even lines and points) with a modern C++11 wrapper and bindings
      for C#, Java and Python (via Swig). The goal of this project is
      to provide a small library with a minimum set of dependencies
      which is easy and intuitively to use.''',
      author='Marcel Steinbeck',
      author_email='github@retux.de',
      license='MIT',
      url='https://github.com/retuxx/tinyspline',
      platforms='Any',
      cmdclass={'build': BuildWithCmake},
      packages=['tinyspline'],
      package_dir={'tinyspline': cmake_dir + '/library'},
      package_data={'tinyspline': ['*tinysplinepython*']}
)
