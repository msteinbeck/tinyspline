#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
IMAGE_NAME="tinyspline/build:latest"
STORAGE="/storage"
VOLUME="${SCRIPT_DIR}/build"
mkdir -p "${VOLUME}"

docker build -t ${IMAGE_NAME} -f - "${ROOT_DIR}" <<-END
	FROM tinyspline/build-deps:latest
	COPY src/. /tinyspline
	WORKDIR /tinyspline
END

docker run \
	--rm \
	--volume "${VOLUME}:${STORAGE}" \
	${IMAGE_NAME} /bin/bash -c \
	"mkdir -p ${STORAGE}/linux64 && \
	 chown $(id -u):$(id -g) ${STORAGE}/linux64 && \
	 	mkdir linux64 && cd linux64 && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True \
				-DTINYSPLINE_ENABLE_DLANG=True \
				-DTINYSPLINE_ENABLE_GO=True \
				-DTINYSPLINE_ENABLE_JAVA=True \
				-DTINYSPLINE_ENABLE_OCTAVE=True \
				-DTINYSPLINE_ENABLE_PHP=True \
				-DTINYSPLINE_ENABLE_R=True && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/linux64 && \
			dub build && \
				tar czf tinysplinedlang.tar.gz dub && \
				chown $(id -u):$(id -g) tinysplinedlang.tar.gz && \
				cp -a tinysplinedlang.tar.gz ${STORAGE}/linux64 && \
			cmake --build . --target tinysplinego && \
			mvn package && \
				chown $(id -u):$(id -g) target/*.jar && \
				cp -a target/*.jar ${STORAGE}/linux64 && \
				chown $(id -u):$(id -g) pom.xml && \
				cp -a pom.xml ${STORAGE}/linux64 && \
			cmake --build . --target tinysplineoctave && \
			cmake --build . --target tinysplinephp && \
			cmake --build . --target tinyspliner && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/usr/include/lua5.1 \
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.1.so && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64"

docker rmi ${IMAGE_NAME}
