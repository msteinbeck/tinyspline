from distutils.core import setup
from distutils.command.build_py import build_py as _build_py

from shutil import rmtree, copy2
from os import path, chdir
from glob import glob
import subprocess
import sys

cmake_bin = "cmake"
cmake_build_config = "Release"
cmake_build_target = "_tinysplinepython"


class build_py(_build_py):
	def run(self):
		script_dir = path.dirname(path.realpath(__file__))
		src_dir = path.join(script_dir, "library")
		build_dir = path.join(script_dir, self.build_lib, "build")
		bin_dir = path.join(script_dir, self.build_lib, "tinyspline")

		if not path.exists(build_dir):
			print("creating cmake build directory")
			self.mkpath(build_dir)

		if not path.exists(bin_dir):
			print("creating binary directory")
			self.mkpath(bin_dir)

		# generate make files (or any kind of project)
		print("generating cmake tree")
		chdir(build_dir)
		cmake_cmd = [cmake_bin, src_dir,
					 "-DCMAKE_BUILD_TYPE=" + cmake_build_config,
					 "-DTINYSPLINE_PYTHON_VERSION=" + str(sys.version_info[0]),
					 "-Wno-dev"]
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

		# copy resulting files into bin_dir
		for bin_file in glob(path.join(build_dir, "lib", "*tinyspline*")):
			copy2(bin_file, bin_dir)

		# create __init__.py in bin_dir
		print("creating file: __init__.py")
		init_file = open(path.join(bin_dir, "__init__.py"), "w+")
		init_file.writelines("from .tinyspline import *\n")
		init_file.close()

		# distutils uses old-style classes, so no super()
		_build_py.run(self)


setup(name="tinyspline",
	  version="0.1.3",
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
	  classifiers=[
		  "Topic :: Scientific/Engineering :: Mathematics",
		  "Topic :: Software Development :: Libraries :: Python Modules",
		  "Programming Language :: Python",
		  "Operating System :: OS Independent",
		  "License :: OSI Approved :: MIT License"
	  ],
	  cmdclass={"build_py": build_py},
	  packages=["tinyspline"],
	  package_dir={"tinyspline": "."}
	  )
