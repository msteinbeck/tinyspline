#!/bin/sh
set -e

SCRIPT_DIR=$( cd $(dirname $0); pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
VOLUME="${SCRIPT_DIR}/build.linux-x86_64"
mkdir -p ${VOLUME}

REPOSITORY="tinyspline"
TAG="build.linux-x86_64"
IMAGE_NAME="${REPOSITORY}:${TAG}"
STORAGE="/dist"

SETUP_CMDS=$(cat << END
RUN apt-get update && apt-get install -y --no-install-recommends cmake swig
COPY src/. /tinyspline
WORKDIR /tinyspline
END
)

BUILD_RUN_DELETE() {
	docker build -t ${IMAGE_NAME} -f - ${ROOT_DIR} <<-END
		${1}
	END
	docker run \
		--rm \
		--volume "${VOLUME}:${STORAGE}" \
		${IMAGE_NAME} \
		/bin/bash -c "${2}"
	docker rmi ${IMAGE_NAME}
}

################################# C#, D, Java #################################
BUILD_CSHARP_D_JAVA() {
	BUILD_RUN_DELETE \
	"FROM buildpack-deps:stretch
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends 	\
		mono-mcs nuget \
		dub \
		default-jdk maven" \
	"cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_CSHARP=True \
		-DTINYSPLINE_ENABLE_DLANG=True \
		-DTINYSPLINE_ENABLE_JAVA=True && \
	cmake --build . --target tinysplinecsharp && \
		nuget pack && \
		chown $(id -u):$(id -g) *.nupkg && \
		cp -a *.nupkg ${STORAGE} && \
	dub build && \
		tar czf tinysplinedlang.tar.gz dub && \
		chown $(id -u):$(id -g) tinysplinedlang.tar.gz && \
		cp -a tinysplinedlang.tar.gz ${STORAGE} && \
	mvn package && \
		chown $(id -u):$(id -g) target/*.jar && \
		cp -a target/*.jar ${STORAGE}"
}

BUILD_CSHARP_D_JAVA

##################################### Lua #####################################
BUILD_LUA() {
	BUILD_RUN_DELETE \
	"FROM buildpack-deps:stretch
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends \
		luarocks liblua${1}-dev" \
	"cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_LUA=True && \
	luarocks make --local && \
		luarocks pack --local tinyspline && \
		for f in ./*.rock; do mv \$f \${f/.rock/-${1}.rock}; done && \
		chown $(id -u):$(id -g) *.rock && \
		cp -a *.rock ${STORAGE}"
}

BUILD_LUA 5.1
BUILD_LUA 5.2
BUILD_LUA 5.3

################################## Octave, R ##################################
BUILD_OCTAVE_R_UBUNTU() {
	BUILD_RUN_DELETE \
	"FROM buildpack-deps:${1}
	RUN echo 'debconf debconf/frontend select Noninteractive' \
		| debconf-set-selections
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends \
		liboctave-dev octave \
		r-base r-cran-rcpp" \
	"cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_OCTAVE=True \
		-DTINYSPLINE_ENABLE_R=True && \
	cmake --build . --target tinysplineoctave && \
		find lib -name '*.oct' \
			| tar czf tinysplineoctave.ubuntu-${1}.tar.gz -T - && \
		chown $(id -u):$(id -g) tinysplineoctave.ubuntu-${1}.tar.gz && \
		cp -a tinysplineoctave.ubuntu-${1}.tar.gz ${STORAGE} && \
	cmake --build . --target tinyspliner && \
		find lib -name 'tinyspliner*' -o -name '*.R' \
			| tar czf tinyspliner.ubuntu-${1}.tar.gz -T - && \
		chown $(id -u):$(id -g) tinyspliner.ubuntu-${1}.tar.gz && \
		cp -a tinyspliner.ubuntu-${1}.tar.gz ${STORAGE}"
}

BUILD_OCTAVE_R_UBUNTU 16.04
BUILD_OCTAVE_R_UBUNTU 18.04

##################################### PHP #####################################
BUILD_PHP_7() {
	BUILD_RUN_DELETE \
	"FROM buildpack-deps:18.04
	RUN echo 'debconf debconf/frontend select Noninteractive' \
		| debconf-set-selections
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends \
		php-dev" \
	"cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_PHP=True && \
	cmake --build . --target tinysplinephp && \
		find lib -name '*php*' \
			| tar czf tinysplinephp7.tar.gz -T - && \
		chown $(id -u):$(id -g) tinysplinephp7.tar.gz && \
		cp -a tinysplinephp7.tar.gz ${STORAGE}"
}

BUILD_PHP_7

################################### Python ####################################
BUILD_PYTHON() {
	BUILD_RUN_DELETE \
	"FROM python:${1}-stretch
	${SETUP_CMDS}" \
	"cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_PYTHON=True && \
	python setup.py bdist_wheel && \
		chown $(id -u):$(id -g) dist/*.whl && \
		cp -a dist/*.whl ${STORAGE}"
}

BUILD_PYTHON 2.7
BUILD_PYTHON 3.5
BUILD_PYTHON 3.6
BUILD_PYTHON 3.7

#################################### Ruby #####################################
BUILD_RUBY() {
	BUILD_RUN_DELETE \
	"FROM ruby:${1}-stretch
	${SETUP_CMDS}" \
	"cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_RUBY=True && \
	gem build *.gemspec && \
		for f in ./*.gem; do mv \$f \${f/.gem/.${1}.gem}; done && \
		chown $(id -u):$(id -g) ./*.gem && \
		cp -a ./*.gem ${STORAGE}"
}

BUILD_RUBY 2.4
BUILD_RUBY 2.5
BUILD_RUBY 2.6
