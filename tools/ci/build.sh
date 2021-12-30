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

MINGW_LIBS="\
/usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libstdc++-6.dll\
\;/usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libgcc_s_seh-1.dll"

docker run \
	--rm \
	--volume "${VOLUME}:${STORAGE}" \
	${IMAGE_NAME} /bin/bash -c \
	"source ~/.bashrc && \
	 mkdir -p ${STORAGE}/linux64 && \
	 chown $(id -u):$(id -g) ${STORAGE}/linux64 && \
		mkdir linux64 && pushd linux64 && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DBUILD_SHARED_LIBS=On \
				-DCMAKE_INSTALL_PREFIX=/usr \
				-DTINYSPLINE_INSTALL_LIBRARY_DIR=lib && \
			cmake --build . --target tinyspline && \
			cmake --build . --target tinysplinecxx && \
			strip --strip-unneeded lib/libtinyspline* && \
				cpack -G DEB && \
				chown $(id -u):$(id -g) *.deb && \
				cp -a *.deb ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
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
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/linux/lua54/include \
				-DLUA_LIBRARY=/opt/linux/lua54/liblua54.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python37/include/python3.7m \
				-DPYTHON_LIBRARY=/opt/linux/python37/lib/libpython3.7m.so && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
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
	mkdir -p ${STORAGE}/wasm && \
	chown $(id -u):$(id -g) ${STORAGE}/wasm && \
		mkdir emsdk && pushd emsdk && \
			PYTHON=/opt/linux/python39/bin/python3 emcmake cmake .. && \
			PYTHON=/opt/linux/python39/bin/python3 npm pack --unsafe-perm && \
			chown $(id -u):$(id -g) *.tgz && \
			cp -a *.tgz ${STORAGE}/wasm && \
		popd && \
	mkdir -p ${STORAGE}/macosx64 && \
	chown $(id -u):$(id -g) ${STORAGE}/macosx64 && \
		mkdir macosx64 && pushd macosx64 && \
			x86_64-apple-darwin17-cmake .. \
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
			mvn package && \
				chown $(id -u):$(id -g) target/*.jar && \
				cp -a target/*.jar ${STORAGE}/macosx64 && \
				chown $(id -u):$(id -g) pom.xml && \
				cp -a pom.xml ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			x86_64-apple-darwin17-cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/include/lua-5.1 \
				-DLUA_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/lib/lua-5.1/liblua-5.1.dylib \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/2.7/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/2.7/Python && \
			cmake --build . --target tinysplinelua && \
				find lib -name '*tinysplinelua*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/lua@5.1/lib \
						{} \\; && \
				sed -i '/supported_platforms/,/}/d' *.rockspec && \
				sed -i '/dependencies/,/}/d' *.rockspec && \
				luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/macosx}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/macosx64 && \
			cmake --build . --target tinysplinepython && \
				find lib -name '*tinysplinepython*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/python/Frameworks/Python.framework/Versions/2.7 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/System/Library/Frameworks/Python.framework/Versions/2.7 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/Library/Developer/CommandLineTools/Library/Frameworks/Python2.framework/Versions/2.7 \
						{} \\; && \
				python2 setup.py bdist_wheel && \
				for w in dist/*.whl; do mv \$w \${w/mu-linux/m-macosx_10_13}; done && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			x86_64-apple-darwin17-cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/include/lua-5.2 \
				-DLUA_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/lib/lua-5.2/liblua-5.2.dylib \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.5/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.5/Python && \
			cmake --build . --target tinysplinelua && \
				find lib -name '*tinysplinelua*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/lua@5.2/lib \
						{} \\; && \
				sed -i '/supported_platforms/,/}/d' *.rockspec && \
				sed -i '/dependencies/,/}/d' *.rockspec && \
				luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/macosx}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/macosx64 && \
			cmake --build . --target tinysplinepython && \
				find lib -name '*tinysplinepython*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/python/Frameworks/Python.framework/Versions/3.5 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/System/Library/Frameworks/Python.framework/Versions/3.5 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.5 \
						{} \\; && \
				python3 setup.py bdist_wheel && \
				for w in dist/*.whl; do mv \$w \${w/linux/macosx_10_13}; done && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			x86_64-apple-darwin17-cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/include \
				-DLUA_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/lib/liblua.5.3.dylib \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.6/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.6/Python && \
			cmake --build . --target tinysplinelua && \
				find lib -name '*tinysplinelua*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/lua@5.3/lib \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/lua/lib \
						{} \\; && \
				sed -i '/supported_platforms/,/}/d' *.rockspec && \
				sed -i '/dependencies/,/}/d' *.rockspec && \
				luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/macosx}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/macosx64 && \
			cmake --build . --target tinysplinepython && \
				find lib -name '*tinysplinepython*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/python/Frameworks/Python.framework/Versions/3.6 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/System/Library/Frameworks/Python.framework/Versions/3.6 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.6 \
						{} \\; && \
				/opt/linux/python36/bin/python3 setup.py bdist_wheel && \
				for w in dist/*.whl; do mv \$w \${w/linux/macosx_10_13}; done && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			x86_64-apple-darwin17-cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.7/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.7/Python && \
			cmake --build . --target tinysplinepython && \
				find lib -name '*tinysplinepython*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/python/Frameworks/Python.framework/Versions/3.7 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/System/Library/Frameworks/Python.framework/Versions/3.7 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.7 \
						{} \\; && \
				/opt/linux/python37/bin/python3 setup.py bdist_wheel && \
				for w in dist/*.whl; do mv \$w \${w/linux/macosx_10_13}; done && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			x86_64-apple-darwin17-cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.8/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.8/Python && \
			cmake --build . --target tinysplinepython && \
				find lib -name '*tinysplinepython*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/python/Frameworks/Python.framework/Versions/3.8 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/System/Library/Frameworks/Python.framework/Versions/3.8 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.8 \
						{} \\; && \
				/opt/linux/python38/bin/python3 setup.py bdist_wheel && \
				for w in dist/*.whl; do mv \$w \${w/linux/macosx_10_13}; done && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		rm -rf ..?* .[!.]* * && \
			x86_64-apple-darwin17-cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.9/Headers \
				-DPYTHON_LIBRARY=/opt/osxcross/target/macports/pkgs/opt/local/Library/Frameworks/Python.framework/Versions/3.9/Python && \
			cmake --build . --target tinysplinepython && \
				find lib -name '*tinysplinepython*' \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						@@HOMEBREW_PREFIX@@/opt/python/Frameworks/Python.framework/Versions/3.9 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/System/Library/Frameworks/Python.framework/Versions/3.9 \
						{} \\; \
					-exec x86_64-apple-darwin17-install_name_tool -add_rpath \
						/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9 \
						{} \\; && \
				/opt/linux/python39/bin/python3 setup.py bdist_wheel && \
				for w in dist/*.whl; do mv \$w \${w/linux/macosx_10_13}; done && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/macosx64 && \
		popd && \
	mkdir -p ${STORAGE}/windows64 && \
	chown $(id -u):$(id -g) ${STORAGE}/windows64 && \
		mkdir windows64 && pushd windows64 && \
			CC=x86_64-w64-mingw32-gcc-win32 \
			CXX=x86_64-w64-mingw32-g++-win32 \
			JAVA_HOME=/opt/wincross/java \
			cmake .. \
				-DCMAKE_SYSTEM_NAME=Windows \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_RUNTIME_LIBRARIES=${MINGW_LIBS} \
				-DTINYSPLINE_ENABLE_CSHARP=True \
				-DTINYSPLINE_ENABLE_DLANG=True \
				-DTINYSPLINE_ENABLE_JAVA=True \
				-DJava_JAVAC_EXECUTABLE=/usr/bin/javac \
				-DJava_JAR_EXECUTABLE=/usr/bin/jar && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/windows64 && \
			dub build && \
				tar czf tinysplinedlang.tar.gz dub && \
				chown $(id -u):$(id -g) tinysplinedlang.tar.gz && \
				cp -a tinysplinedlang.tar.gz ${STORAGE}/windows64 && \
			mvn package && \
				chown $(id -u):$(id -g) target/*.jar && \
				cp -a target/*.jar ${STORAGE}/windows64 && \
				chown $(id -u):$(id -g) pom.xml && \
				cp -a pom.xml ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			CC=x86_64-w64-mingw32-gcc-win32 \
			CXX=x86_64-w64-mingw32-g++-win32 \
			cmake .. \
				-DCMAKE_SYSTEM_NAME=Windows \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_RUNTIME_LIBRARIES=${MINGW_LIBS} \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/wincross/lua51/include \
				-DLUA_LIBRARY=/opt/wincross/lua51/lua5.1.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			CC=x86_64-w64-mingw32-gcc-win32 \
			CXX=x86_64-w64-mingw32-g++-win32 \
			cmake .. \
				-DCMAKE_SYSTEM_NAME=Windows \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_RUNTIME_LIBRARIES=${MINGW_LIBS} \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/wincross/lua52/include \
				-DLUA_LIBRARY=/opt/wincross/lua52/lua52.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			CC=x86_64-w64-mingw32-gcc-win32 \
			CXX=x86_64-w64-mingw32-g++-win32 \
			cmake .. \
				-DCMAKE_SYSTEM_NAME=Windows \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_RUNTIME_LIBRARIES=${MINGW_LIBS} \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/wincross/lua53/include \
				-DLUA_LIBRARY=/opt/wincross/lua53/lua53.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			CC=x86_64-w64-mingw32-gcc-win32 \
			CXX=x86_64-w64-mingw32-g++-win32 \
			cmake .. \
				-DCMAKE_SYSTEM_NAME=Windows \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_RUNTIME_LIBRARIES=${MINGW_LIBS} \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/wincross/lua54/include \
				-DLUA_LIBRARY=/opt/wincross/lua54/lua54.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
		popd"

docker rmi ${IMAGE_NAME}
