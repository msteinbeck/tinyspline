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

################################### Python ####################################
BUILD_PYTHON() {
	docker build -t "tinyspline:python${1}-linux" -f - ${ROOT_DIR} <<-END
		FROM python:${1}-stretch
		${STRETCH_SETUP_CMDS}
		END
	docker run --name "python${1}-linux" "tinyspline:python${1}-linux" \
		/bin/bash -c "cmake -DTINYSPLINE_ENABLE_PYTHON=True . && python setup.py bdist_wheel"
	CONTAINER_ID=$(docker ps -aqf "name=python${1}-linux")
	docker cp "${CONTAINER_ID}":/tinyspline/dist/. ${DIST_DIR}
	docker rm "${CONTAINER_ID}"
	docker rmi "tinyspline:python${1}-linux"
}

BUILD_PYTHON 2.7
BUILD_PYTHON 3.5
BUILD_PYTHON 3.6
BUILD_PYTHON 3.7
