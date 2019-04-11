#!/bin/bash
set -e

SCRIPT_DIR=$( cd $(dirname $0); pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
DIST_DIR="${SCRIPT_DIR}/dist"
mkdir -p ${DIST_DIR}

STRETCH_SETUP_CMDS=$(cat << END
RUN apt-get update && apt-get install -y --no-install-recommends cmake swig
COPY . /tinyspline
WORKDIR /tinyspline
END
)

##################################### C# ######################################
docker build -t "tinyspline:mono" -f - ${ROOT_DIR} <<-END
	FROM gcc:7
	${STRETCH_SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends mono-mcs nuget
	END
docker run --name "mono" "tinyspline:mono" \
	/bin/bash -c "cmake -DTINYSPLINE_ENABLE_CSHARP=True . && \
	cmake --build . --target tinysplinecsharp && \
	nuget pack && mkdir dist && mv ./*.nupkg dist/"
CONTAINER_ID=$(docker ps -aqf "name=mono")
docker cp "${CONTAINER_ID}":/tinyspline/dist/. ${DIST_DIR}
docker rm "${CONTAINER_ID}"
docker rmi "tinyspline:mono"

################################### Python ####################################
BUILD_PYTHON() {
	NAME="python${1}"
	docker build -t "tinyspline:${NAME}" -f - ${ROOT_DIR} <<-END
		FROM python:${1}-stretch
		${STRETCH_SETUP_CMDS}
		END
	docker run --name "${NAME}" "tinyspline:${NAME}" \
		/bin/bash -c "cmake -DTINYSPLINE_ENABLE_PYTHON=True . && \
		python setup.py bdist_wheel"
	CONTAINER_ID=$(docker ps -aqf "name=${NAME}")
	docker cp "${CONTAINER_ID}":/tinyspline/dist/. ${DIST_DIR}
	docker rm "${CONTAINER_ID}"
	docker rmi "tinyspline:${NAME}"
}

BUILD_PYTHON 2.7
BUILD_PYTHON 3.5
BUILD_PYTHON 3.6
BUILD_PYTHON 3.7
