#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
SRC_DIR="/tinyspline"
IMAGE_NAME="tinyspline/docs:latest"
STORAGE="/storage"
VOLUME="${SCRIPT_DIR}/docs"
mkdir -p "${VOLUME}"

docker build -t ${IMAGE_NAME} -f - "${ROOT_DIR}" <<-END
	FROM tinyspline/build-deps:latest
	COPY . ${SRC_DIR}
	WORKDIR /
END

docker run \
	--rm \
	--volume "${VOLUME}:${STORAGE}" \
	${IMAGE_NAME} /bin/bash -c \
	"mkdir build && pushd build && \
		cmake ${SRC_DIR} && \
		cmake --build . --target docs coverage && \
		chown -R $(id -u):$(id -g) docs && \
		chown -R $(id -u):$(id -g) coverage && \
		cp -a docs/doxygen ${STORAGE}/doxygen && \
		cp -a coverage ${STORAGE}/doxygen/html && \
	popd"

docker rmi ${IMAGE_NAME}
