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
JDK8_URL="https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u212-b03/OpenJDK8U-jdk_x64_mac_hotspot_8u212b03.tar.gz"

BUILD_CSHARP_D_JAVA() {
	BUILD_RUN_DELETE \
	"FROM fzwoch/osxcross:latest
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends 	\
		mono-mcs nuget \
		dub \
		default-jdk maven" \
	"wget ${JDK8_URL} -O /opt/jdk8.tar.gz && mkdir /opt/java && \
		tar -C /opt/java -xf /opt/jdk8.tar.gz --strip 1 && \
	CC=o64-clang CXX=o64-clang++ JAVA_HOME=/opt/java/Contents/Home \
		cmake . \
		-DCMAKE_SYSTEM_NAME=Darwin \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_CSHARP=True \
		-DTINYSPLINE_ENABLE_DLANG=True \
		-DTINYSPLINE_ENABLE_JAVA=True \
		-DJava_JAVAC_EXECUTABLE=/usr/bin/javac \
		-DJava_JAR_EXECUTABLE=/usr/bin/jar && \
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

#BUILD_CSHARP_D_JAVA

##################################### Lua #####################################
LUA51_URL="https://homebrew.bintray.com/bottles/lua@5.1-5.1.5_8.el_capitan.bottle.tar.gz"
LUA53_URL="https://homebrew.bintray.com/bottles/lua-5.3.5_1.el_capitan.bottle.tar.gz"

BUILD_LUA() {
	url="LUA5${1}_URL"
	BUILD_RUN_DELETE \
	"FROM fzwoch/osxcross:latest
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends \
		luarocks" \
	"wget ${!url} -O /opt/lua.tar.gz && mkdir /opt/lua && \
		tar -C /opt/lua -xf /opt/lua.tar.gz --strip 2 && \
	CC=o64-clang CXX=o64-clang++ \
		cmake . \
		-DCMAKE_SYSTEM_NAME=Darwin \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_LUA=True \
		-DLUA_INCLUDE_DIR=/opt/lua/include/lua5.${1} \
		-DLUA_LIBRARY=/opt/lua/lib/liblua5.${1}.dylib && \
	sed -i '/supported_platforms/,/}/d' *.rockspec && \
		luarocks make  --pack-binary-rock && \
		for f in ./*.rock; do mv \$f \${f/.rock/-5.${1}.rock}; done && \
		for f in ./*.rock; do mv \$f \${f/linux/macosx}; done && \
		chown $(id -u):$(id -g) *.rock && \
		cp -a *.rock ${STORAGE}"
}

BUILD_LUA 1
BUILD_LUA 3
