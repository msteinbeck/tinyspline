#!/bin/sh
set -e

SCRIPT_DIR=$( cd $(dirname $0); pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
VOLUME="${SCRIPT_DIR}/build.docs"
mkdir -p ${VOLUME}

REPOSITORY="tinyspline"
TAG="build.docs"
IMAGE_NAME="${REPOSITORY}:${TAG}"
STORAGE="/dist"

SETUP_CMDS=$(cat << END
RUN apt-get update && apt-get install -y --no-install-recommends cmake
COPY . /tinyspline
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

################################### Doxygen ###################################
BUILD_DOXYGEN() {
	BUILD_RUN_DELETE \
	"FROM buildpack-deps:stretch
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends 	\
		doxygen graphviz" \
	"cmake . && \
	cmake --build . --target docs && \
		chown -R $(id -u):$(id -g) docs/doxygen/html && \
		cp -a -R docs/doxygen/html ${STORAGE}"
}

BUILD_DOXYGEN
