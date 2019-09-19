#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)
ROOT_DIR="${SCRIPT_DIR}/../.."
VOLUME="${SCRIPT_DIR}/build.windows-x86_64"
mkdir -p "${VOLUME}"

REPOSITORY="tinyspline"
TAG="build.windows-x86_64"
IMAGE_NAME="${REPOSITORY}:${TAG}"
STORAGE="/dist"

SETUP_CMDS=$(cat << END
RUN echo 'debconf debconf/frontend select Noninteractive' \
		| debconf-set-selections && \
	apt-get update && \
	apt-get install -y --no-install-recommends cmake swig mingw-w64
COPY src/. /tinyspline
WORKDIR /tinyspline
END
)

BUILD_RUN_DELETE() {
	docker build -t ${IMAGE_NAME} -f - "${ROOT_DIR}" <<-END
		${1}
	END
	docker run \
		--rm \
		--volume "${VOLUME}:${STORAGE}" \
		${IMAGE_NAME} \
		/bin/bash -c "${2}"
	docker rmi ${IMAGE_NAME}
}

RUNTIME_LIBRARIES="\
/usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libstdc++-6.dll\
\;/usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libgcc_s_seh-1.dll"

################################# C#, D, Java #################################
JDK8_URL="https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u222-b10/OpenJDK8U-jdk_x64_windows_hotspot_8u222b10.zip"

BUILD_CSHARP_JAVA() {
	BUILD_RUN_DELETE \
	"FROM buildpack-deps:stretch
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends 	\
		mono-mcs nuget \
		dub \
		default-jdk maven" \
	"wget ${JDK8_URL} -O /opt/jdk8.zip && mkdir /opt/java && \
		unzip -d /opt /opt/jdk8.zip && mv /opt/jdk*/* /opt/java && \
	CC=x86_64-w64-mingw32-gcc-win32 CXX=x86_64-w64-mingw32-g++-win32 \
		JAVA_HOME=/opt/java \
		cmake . \
		-DCMAKE_SYSTEM_NAME=Windows \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_RUNTIME_LIBRARIES=${RUNTIME_LIBRARIES} \
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

BUILD_CSHARP_JAVA

##################################### Lua #####################################
LUA51_URL="https://homebrew.bintray.com/bottles/lua@5.1-5.1.5_8.el_capitan.bottle.tar.gz"
LUA53_URL="https://homebrew.bintray.com/bottles/lua-5.3.5_1.el_capitan.bottle.tar.gz"

BUILD_LUA() {
	url="LUA5${1}_URL"
	BUILD_RUN_DELETE \
	"FROM liushuyu/osxcross:latest
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
		luarocks make --pack-binary-rock && \
		for f in ./*.rock; do mv \$f \${f/.rock/-5.${1}.rock}; done && \
		for f in ./*.rock; do mv \$f \${f/linux/macosx}; done && \
		chown $(id -u):$(id -g) *.rock && \
		cp -a *.rock ${STORAGE}"
}

#BUILD_LUA 1
#BUILD_LUA 3

################################### Python ####################################
PYTHON27_URL="https://homebrew.bintray.com/bottles/python@2-2.7.16.sierra.bottle.1.tar.gz"
PYTHON37_URL="https://homebrew.bintray.com/bottles/python-3.7.3.sierra.bottle.tar.gz"

BUILD_PYTHON() {
	url="PYTHON${1}${2}_URL"
	if [ "${1}" = "3" ]; then v="3"; else v=""; fi
	if [ "${1}" = "3" ]; then m="m"; else m=""; fi
	if [ "${1}" = "3" ]; then s="36"; else s="27"; fi
	basedir="/opt/python/Frameworks/Python.framework/Versions/${1}.${2}"
	BUILD_RUN_DELETE \
	"FROM liushuyu/osxcross:latest
	${SETUP_CMDS}
	RUN apt-get install -y --no-install-recommends \
		python${v} python${v}-setuptools python${v}-wheel" \
	"wget ${!url} -O /opt/python.tar.gz && mkdir /opt/python && \
		tar -C /opt/python -xf /opt/python.tar.gz --strip 2 && \
	CC=o64-clang CXX=o64-clang++ \
		cmake . \
		-DCMAKE_SYSTEM_NAME=Darwin \
		-DCMAKE_BUILD_TYPE=Release \
		-DTINYSPLINE_ENABLE_PYTHON=True \
		-DPYTHON_INCLUDE_DIR=${basedir}/include/python${1}.${2}${m} \
		-DPYTHON_LIBRARY=${basedir}/lib/libpython${1}.${2}.dylib && \
	python${v} setup.py bdist_wheel && \
		for f in dist/*.whl; do mv \$f \${f/${s}/${1}${2}$}; done && \
		for f in dist/*.whl; do mv \$f \${f/${s}/${1}${2}$}; done && \
		for f in dist/*.whl; do mv \$f \${f/$/}; done && \
		for f in dist/*.whl; do mv \$f \${f/$/}; done && \
		for f in dist/*.whl; do mv \$f \${f/linux/macosx_10_14}; done && \
		chown $(id -u):$(id -g) dist/*.whl && \
		cp -a dist/*.whl ${STORAGE}"
}

#BUILD_PYTHON 2 7
#BUILD_PYTHON 3 7
