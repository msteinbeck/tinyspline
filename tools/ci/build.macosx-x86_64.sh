#!/bin/sh
set -e

SCRIPT_DIR=$( cd $(dirname $0); pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
VOLUME="${SCRIPT_DIR}/build.macosx-x86_64"
mkdir -p ${VOLUME}

REPOSITORY="tinyspline"
TAG="build.macosx-x86_64"
IMAGE_NAME="${REPOSITORY}:${TAG}"
STORAGE="/dist"

JDK8_URL="https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u212-b03/OpenJDK8U-jdk_x64_mac_hotspot_8u212b03.tar.gz"
LUA51_URL="https://homebrew.bintray.com/bottles/lua@5.1-5.1.5_8.el_capitan.bottle.tar.gz"

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

BUILD_RUN_DELETE \
"FROM fzwoch/osxcross:latest
RUN apt-get update && apt-get install -y --no-install-recommends \
	cmake swig \
	mono-mcs nuget \
	dub \
	default-jdk maven
COPY src/. /tinyspline
WORKDIR /tinyspline" \
"wget ${JDK8_URL} -O /opt/jdk8.tar.gz && mkdir /opt/java && \
	tar -C /opt/java -xf /opt/jdk8.tar.gz --strip 1 && \
wget ${LUA51_URL} -O /opt/lua51.tar.gz && mkdir /opt/lua51 && \
	tar -C /opt/lua51 -xf /opt/lua51.tar.gz --strip 2 && \
CC=o64-clang CXX=o64-clang++ JAVA_HOME=/opt/java/Contents/Home \
	cmake . \
	-DCMAKE_SYSTEM_NAME=Darwin \
	-DCMAKE_BUILD_TYPE=Release \
	-DTINYSPLINE_ENABLE_CSHARP=True \
	-DTINYSPLINE_ENABLE_DLANG=True \
	-DTINYSPLINE_ENABLE_JAVA=True \
	-DJava_JAVAC_EXECUTABLE=/usr/bin/javac \
	-DJava_JAR_EXECUTABLE=/usr/bin/jar \
	-DTINYSPLINE_ENABLE_LUA=True \
	-DLUA_INCLUDE_DIR=/opt/lua51/include/lua5.1 \
	-DLUA_LIBRARY=/opt/lua51/lib/liblua5.1.dylib && \
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
	cp -a target/*.jar ${STORAGE} && \
cmake --build . --target tinysplinelua"
