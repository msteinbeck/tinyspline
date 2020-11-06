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
		mkdir linux64 && pushd linux64 && \
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
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.1.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/usr/include/python2.7 \
				-DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			python2 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/usr/include/lua5.2 \
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.2.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/usr/include/python3.5m \
				-DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.5m.so && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/usr/include/lua5.3 \
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.3.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python36/include/python3.6m \
				-DPYTHON_LIBRARY=/opt/linux/python36/lib/libpython3.6m.so && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			/opt/linux/python36/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python37/include/python3.7m \
				-DPYTHON_LIBRARY=/opt/linux/python37/lib/libpython3.7m.so && \
			/opt/linux/python37/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python38/include/python3.8 \
				-DPYTHON_LIBRARY=/opt/linux/python38/lib/libpython3.8.so && \
			/opt/linux/python38/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python39/include/python3.9 \
				-DPYTHON_LIBRARY=/opt/linux/python39/lib/libpython3.9.so && \
			/opt/linux/python39/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
		popd && \
	mkdir -p ${STORAGE}/macosx64 && \
	chown $(id -u):$(id -g) ${STORAGE}/macosx64 && \
		mkdir macosx64 && pushd macosx64 && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True \
				-DTINYSPLINE_ENABLE_DLANG=True \
				-DTINYSPLINE_ENABLE_GO=True \
				-DTINYSPLINE_ENABLE_JAVA=True && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/macosx64 && \
			dub build && \
				tar czf tinysplinedlang.tar.gz dub && \
				chown $(id -u):$(id -g) tinysplinedlang.tar.gz && \
				cp -a tinysplinedlang.tar.gz ${STORAGE}/macosx64 && \
			cmake --build . --target tinysplinego && \
			mvn package && \
				chown $(id -u):$(id -g) target/*.jar && \
				cp -a target/*.jar ${STORAGE}/macosx64 && \
				chown $(id -u):$(id -g) pom.xml && \
				cp -a pom.xml ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/include/lua-5.1 \
				-DLUA_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/lib/lua-5.1/liblua-5.1.dylib \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/2.7/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/2.7/Python && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/macosx64 && \
			python2 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/include/lua-5.2 \
				-DLUA_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/lib/lua-5.1/liblua-5.2.dylib \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.5/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.5/Python && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/macosx64 && \
			python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/include \
				-DLUA_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/lib/liblua.5.3.dylib \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.6/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.6/Python && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/macosx64 && \
			/opt/linux/python36/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.7/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.7/Python && \
			/opt/linux/python37/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.8/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.8/Python && \
			/opt/linux/python38/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.9/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.9/Python && \
			/opt/linux/python39/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		popd"

docker rmi ${IMAGE_NAME}
