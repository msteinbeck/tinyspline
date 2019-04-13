#!/bin/sh
set -e

SCRIPT_DIR=$( cd $(dirname $0); pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
DIST_DIR="${SCRIPT_DIR}/dist"
mkdir -p ${DIST_DIR}

# Docker image repository.
REPOSITORY="tinyspline"

APT_GET_CMDS=$(cat << END
RUN apt-get update && apt-get install -y --no-install-recommends cmake swig
COPY . /tinyspline
WORKDIR /tinyspline
END
)

# Expects that the artifacts are located at /tinyspline/dist/
COPY_ARTIFACTS_AND_DELETE() {
	CONTAINER_ID=$(docker ps -aqf "name=${1}")
	docker cp "${CONTAINER_ID}":/tinyspline/dist/. ${DIST_DIR}
	docker rm "${CONTAINER_ID}"
	docker rmi "${REPOSITORY}:${1}"
}

################################# C#, D, Java #################################
NAME="misc"
docker build -t "${REPOSITORY}:${NAME}" -f - ${ROOT_DIR} <<-END
	FROM buildpack-deps:stretch
	${APT_GET_CMDS}
	RUN apt-get install -y --no-install-recommends 	\
		mono-mcs nuget \
		dub \
		default-jdk maven
	END
docker run --name "${NAME}" "${REPOSITORY}:${NAME}" \
	/bin/bash -c "mkdir -p dist && cmake \
	-DTINYSPLINE_ENABLE_CSHARP=True \
	-DTINYSPLINE_ENABLE_DLANG=True \
	-DTINYSPLINE_ENABLE_JAVA=True . && \
	cmake --build . --target tinysplinecsharp && \
		nuget pack && mv ./*.nupkg dist && \
	dub build && tar cJf dist/tinysplinedlang.tar.xz dub && \
	mvn package && mv ./target/*.jar dist"
	COPY_ARTIFACTS_AND_DELETE ${NAME}

##################################### Lua #####################################
BUILD_LUA() {
	NAME="lua${1}"
	docker build -t "${REPOSITORY}:${NAME}" -f - ${ROOT_DIR} <<-END
		FROM buildpack-deps:stretch
		${APT_GET_CMDS}
		RUN apt-get install -y --no-install-recommends \
		luarocks liblua${1}-dev
		END
	docker run --name "${NAME}" "${REPOSITORY}:${NAME}" \
		/bin/bash -c "cmake -DTINYSPLINE_ENABLE_LUA=True . && \
		luarocks make --local && luarocks pack --local tinyspline && \
		mkdir -p dist && mv ./*.rock dist"
	COPY_ARTIFACTS_AND_DELETE ${NAME}
	for file in "${DIST_DIR}/"*.rock
	do
		if [[ "${file}" != *"lua"* ]];then
			mv $file ${file/.rock/.${NAME}.rock}
		fi
	done
}

BUILD_LUA 5.1
BUILD_LUA 5.2
BUILD_LUA 5.3

################################### Python ####################################
BUILD_PYTHON() {
	NAME="python${1}"
	docker build -t "${REPOSITORY}:${NAME}" -f - ${ROOT_DIR} <<-END
		FROM python:${1}-stretch
		${APT_GET_CMDS}
		END
	docker run --name "${NAME}" "${REPOSITORY}:${NAME}" \
		/bin/bash -c "cmake -DTINYSPLINE_ENABLE_PYTHON=True . && \
		python setup.py bdist_wheel"
	COPY_ARTIFACTS_AND_DELETE ${NAME}
}

BUILD_PYTHON 2.7
BUILD_PYTHON 3.5
BUILD_PYTHON 3.6
BUILD_PYTHON 3.7
