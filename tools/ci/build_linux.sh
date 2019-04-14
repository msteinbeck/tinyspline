#!/bin/sh
set -e

SCRIPT_DIR=$( cd $(dirname $0); pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
DIST_DIR="${SCRIPT_DIR}/dist"
mkdir -p ${DIST_DIR}

REPOSITORY="tinyspline"
TAG="build.linux-x86_64"
IMAGE_NAME="${REPOSITORY}:${TAG}"

# Initial setup commands. Expects that 'apt-get' is present.
SETUP_CMDS=$(cat << END
RUN apt-get update && apt-get install -y --no-install-recommends cmake swig
COPY . /tinyspline
WORKDIR /tinyspline
END
)

# Expects that the artifacts are located in: /tinyspline/dist/
COPY_ARTIFACTS_AND_DELETE() {
	CONTAINER_ID=$(docker ps -aqf "name=${TAG}")
	docker cp ${CONTAINER_ID}:/tinyspline/dist/. ${DIST_DIR}
	docker rm ${CONTAINER_ID}
	docker rmi ${IMAGE_NAME}
}

################################# C#, D, Java #################################
BUILD_CSHARP_D_JAVA() {
	docker build -t ${IMAGE_NAME} -f - ${ROOT_DIR} <<-END
		FROM buildpack-deps:stretch
		${SETUP_CMDS}
		RUN apt-get install -y --no-install-recommends 	\
			mono-mcs nuget \
			dub \
			default-jdk maven
		END
	docker run --name ${TAG} ${IMAGE_NAME} \
		/bin/bash -c "mkdir -p dist && cmake \
			-DTINYSPLINE_ENABLE_CSHARP=True \
			-DTINYSPLINE_ENABLE_DLANG=True \
			-DTINYSPLINE_ENABLE_JAVA=True . && \
		cmake --build . --target tinysplinecsharp && \
			nuget pack && mv ./*.nupkg dist && \
		dub build && tar cJf dist/tinysplinedlang.tar.xz dub && \
		mvn package && mv ./target/*.jar dist"
	COPY_ARTIFACTS_AND_DELETE
}

BUILD_CSHARP_D_JAVA

##################################### Lua #####################################
BUILD_LUA() {
	docker build -t ${IMAGE_NAME} -f - ${ROOT_DIR} <<-END
		FROM buildpack-deps:stretch
		${SETUP_CMDS}
		RUN apt-get install -y --no-install-recommends \
			luarocks liblua${1}-dev
		END
	docker run --name ${TAG} ${IMAGE_NAME} \
		/bin/bash -c "cmake -DTINYSPLINE_ENABLE_LUA=True . && \
		luarocks make --local && luarocks pack --local tinyspline && \
		mkdir -p dist && mv ./*.rock dist"
	COPY_ARTIFACTS_AND_DELETE
	for file in "${DIST_DIR}/"*.rock
	do
		if [[ "${file}" != *"lua"* ]];then
			mv $file ${file/.rock/.lua${1}.rock}
		fi
	done
}

BUILD_LUA 5.1
BUILD_LUA 5.2
BUILD_LUA 5.3

################################### Python ####################################
BUILD_PYTHON() {
	docker build -t ${IMAGE_NAME} -f - ${ROOT_DIR} <<-END
		FROM python:${1}-stretch
		${SETUP_CMDS}
		END
	docker run --name ${TAG} ${IMAGE_NAME} \
		/bin/bash -c "cmake -DTINYSPLINE_ENABLE_PYTHON=True . && \
		python setup.py bdist_wheel"
	COPY_ARTIFACTS_AND_DELETE
}

BUILD_PYTHON 2.7
BUILD_PYTHON 3.5
BUILD_PYTHON 3.6
BUILD_PYTHON 3.7

################################## Octave, R ##################################
BUILD_OCTAVE_R_UBUNTU() {
	docker build -t ${IMAGE_NAME} -f - ${ROOT_DIR} <<-END
		FROM buildpack-deps:${1}
		RUN echo 'debconf debconf/frontend select Noninteractive' \
			| debconf-set-selections
		${SETUP_CMDS}
		RUN apt-get install -y --no-install-recommends \
			liboctave-dev octave \
			r-base r-cran-rcpp
		END
	docker run --name ${TAG} ${IMAGE_NAME} \
		/bin/bash -c "mkdir -p dist && cmake \
			-DTINYSPLINE_ENABLE_OCTAVE=True \
			-DTINYSPLINE_ENABLE_R=True . && \
		cmake --build . --target tinysplineoctave && \
			find ./lib -name '*octave*' \
			| tar cJf dist/tinysplineoctave.utnubu.tar.xz -T - && \
		cmake --build . --target tinyspliner && \
			find ./lib -name 'tinyspliner*' -o -name '*.R' \
			| tar cJf dist/tinyspliner.utnubu.tar.xz -T -"
	COPY_ARTIFACTS_AND_DELETE
	for file in "${DIST_DIR}/"*utnubu.tar.xz
	do
		if [[ "${file}" != *"ubuntu"* ]];then
			mv $file ${file/utnubu/ubuntu-${1}}
		fi
	done
}

BUILD_OCTAVE_R_UBUNTU 16.04
BUILD_OCTAVE_R_UBUNTU 18.04
