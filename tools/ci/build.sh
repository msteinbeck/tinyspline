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
	COPY . /tinyspline
	WORKDIR /tinyspline
END

docker run \
	--rm \
	--volume "${VOLUME}:${STORAGE}" \
	${IMAGE_NAME} /bin/bash -c \
	"source ~/.bashrc && \
	 mkdir -p ${STORAGE}/android-arm && \
	 chown $(id -u):$(id -g) ${STORAGE}/android-arm && \
		mkdir android-arm && pushd android-arm && \
			cmake .. \
				-DCMAKE_TOOLCHAIN_FILE=/opt/android/android-ndk-r23b/build/cmake/android.toolchain.cmake \
				-DANDROID_ABI=armeabi-v7a \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/android-arm && \
		rm -rf ..?* .[!.]* * && \
		popd && \
	 mkdir -p ${STORAGE}/android-arm64 && \
	 chown $(id -u):$(id -g) ${STORAGE}/android-arm64 && \
		mkdir android-arm64 && pushd android-arm64 && \
			cmake .. \
				-DCMAKE_TOOLCHAIN_FILE=/opt/android/android-ndk-r23b/build/cmake/android.toolchain.cmake \
				-DANDROID_ABI=arm64-v8a \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/android-arm64 && \
		rm -rf ..?* .[!.]* * && \
		popd && \
	 mkdir -p ${STORAGE}/android-x86 && \
	 chown $(id -u):$(id -g) ${STORAGE}/android-x86 && \
		mkdir android-x86 && pushd android-x86 && \
			cmake .. \
				-DCMAKE_TOOLCHAIN_FILE=/opt/android/android-ndk-r23b/build/cmake/android.toolchain.cmake \
				-DANDROID_ABI=x86 \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/android-x86 && \
		rm -rf ..?* .[!.]* * && \
		popd && \
	 mkdir -p ${STORAGE}/android-x86_64 && \
	 chown $(id -u):$(id -g) ${STORAGE}/android-x86_64 && \
		mkdir android-x86_64 && pushd android-x86_64 && \
			cmake .. \
				-DCMAKE_TOOLCHAIN_FILE=/opt/android/android-ndk-r23b/build/cmake/android.toolchain.cmake \
				-DANDROID_ABI=x86_64 \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True && \
			cmake --build . --target tinysplinecsharp && \
				nuget pack && \
				chown $(id -u):$(id -g) *.nupkg && \
				cp -a *.nupkg ${STORAGE}/android-x86_64 && \
		rm -rf ..?* .[!.]* * && \
		popd && \
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
				chown $(id -u):$(id -g) *go.zip && \
				cp -a *go.zip ${STORAGE}/linux64 && \
			mvn package && \
				chown $(id -u):$(id -g) target/*.jar && \
				cp -a target/*.jar ${STORAGE}/linux64 && \
				chown $(id -u):$(id -g) pom.xml && \
				cp -a pom.xml ${STORAGE}/linux64 && \
			cmake --build . --target tinysplineoctave && \
			cmake --build . --target tinysplinephp && \
				chown $(id -u):$(id -g) php*.zip && \
				cp -a php*.zip ${STORAGE}/linux64 && \
			cmake --build . --target tinyspliner && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/usr/include/lua5.1 \
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.1.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/usr/include/python2.7 \
				-DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			python2 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				chown $(id -u):$(id -g) *.gemspec && \
				cp -a *.gem ${STORAGE}/linux64 && \
				cp -a *.gemspec ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby24/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/usr/include/lua5.2 \
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.2.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python37/include/python3.7m \
				-DPYTHON_LIBRARY=/opt/linux/python37/lib/libpython3.7m.so \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			/opt/linux/python37/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby25/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/usr/include/lua5.3 \
				-DLUA_LIBRARY=/usr/lib/x86_64-linux-gnu/liblua5.3.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python38/include/python3.8 \
				-DPYTHON_LIBRARY=/opt/linux/python38/lib/libpython3.8.so \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			/opt/linux/python38/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby26/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/linux/lua54/include \
				-DLUA_LIBRARY=/opt/linux/lua54/liblua54.so \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python39/include/python3.9 \
				-DPYTHON_LIBRARY=/opt/linux/python39/lib/libpython3.9.so \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/linux64 && \
			/opt/linux/python39/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby27/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python310/include/python3.10 \
				-DPYTHON_LIBRARY=/opt/linux/python310/lib/libpython3.10.so \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			/opt/linux/python310/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby30/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_PYTHON=True \
				-DPYTHON_INCLUDE_DIR=/opt/linux/python311/include/python3.11 \
				-DPYTHON_LIBRARY=/opt/linux/python311/lib/libpython3.11.so \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			/opt/linux/python311/bin/python3 setup.py bdist_wheel && \
				chown $(id -u):$(id -g) dist/*.whl && \
				cp -a dist/*.whl ${STORAGE}/linux64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby31/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
		rm -rf ..?* .[!.]* * && \
			PATH=/opt/linux/ruby32/bin:$PATH cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_RUBY=True && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/linux64 && \
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
		popd && \
	mkdir -p ${STORAGE}/windows64 && \
	chown $(id -u):$(id -g) ${STORAGE}/windows64 && \
		export CMAKE_TOOLCHAIN_FILE=/opt/windows-x86_64/toolchain-mingw64.cmake && \
		mkdir windows64 && pushd windows64 && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_CSHARP=True \
				-DTINYSPLINE_ENABLE_DLANG=True \
				-DTINYSPLINE_ENABLE_GO=True \
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
			cmake --build . --target tinysplinego && \
				cp -a tinyspline-go.zip ${STORAGE}/windows64 && \
			mvn package && \
				chown $(id -u):$(id -g) target/*.jar && \
				cp -a target/*.jar ${STORAGE}/windows64 && \
				chown $(id -u):$(id -g) pom.xml && \
				cp -a pom.xml ${STORAGE}/windows64 && \
			cmake --build . --target demo && \
				chown $(id -u):$(id -g) examples/cxx/fltk/demo.exe && \
				cp -a examples/cxx/fltk/demo.exe ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/windows-x86_64/lua51/include \
				-DLUA_LIBRARY=/opt/windows-x86_64/lua51/liblua5.1.a \
				-DTINYSPLINE_ENABLE_RUBY=True \
				-DRuby_EXECUTABLE=/opt/linux/ruby26/bin/ruby \
				-DRuby_INCLUDE_DIR=/opt/windows-x86_64/ruby26/include/ruby-2.6.0 \
				-DRuby_CONFIG_INCLUDE_DIR=/opt/windows-x86_64/ruby26/include/ruby-2.6.0/x64-mingw32 \
				-DRuby_LIBRARY=/opt/windows-x86_64/ruby26/bin/x64-msvcrt-ruby260.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				chown $(id -u):$(id -g) *.gemspec && \
				cp -a *.gem ${STORAGE}/windows64 && \
				cp -a *.gemspec ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/windows-x86_64/lua52/include \
				-DLUA_LIBRARY=/opt/windows-x86_64/lua52/liblua52.a \
				-DTINYSPLINE_ENABLE_RUBY=True \
				-DRuby_EXECUTABLE=/opt/linux/ruby27/bin/ruby \
				-DRuby_INCLUDE_DIR=/opt/windows-x86_64/ruby27/include/ruby-2.7.0 \
				-DRuby_CONFIG_INCLUDE_DIR=/opt/windows-x86_64/ruby27/include/ruby-2.7.0/x64-mingw32 \
				-DRuby_LIBRARY=/opt/windows-x86_64/ruby27/bin/x64-msvcrt-ruby270.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/windows-x86_64/lua53/include \
				-DLUA_LIBRARY=/opt/windows-x86_64/lua53/liblua53.a \
				-DTINYSPLINE_ENABLE_RUBY=True \
				-DRuby_EXECUTABLE=/opt/linux/ruby30/bin/ruby \
				-DRuby_INCLUDE_DIR=/opt/windows-x86_64/ruby30/include/ruby-3.0.0 \
				-DRuby_CONFIG_INCLUDE_DIR=/opt/windows-x86_64/ruby30/include/ruby-3.0.0/x64-mingw32 \
				-DRuby_LIBRARY=/opt/windows-x86_64/ruby30/bin/x64-msvcrt-ruby300.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_LUA=True \
				-DLUA_INCLUDE_DIR=/opt/windows-x86_64/lua54/include \
				-DLUA_LIBRARY=/opt/windows-x86_64/lua54/liblua54.a \
				-DTINYSPLINE_ENABLE_RUBY=True \
				-DRuby_EXECUTABLE=/opt/linux/ruby31/bin/ruby \
				-DRuby_INCLUDE_DIR=/opt/windows-x86_64/ruby31/include/ruby-3.1.0 \
				-DRuby_CONFIG_INCLUDE_DIR=/opt/windows-x86_64/ruby31/include/ruby-3.1.0/x64-mingw-ucrt \
				-DRuby_LIBRARY=/opt/windows-x86_64/ruby31/bin/x64-ucrt-ruby310.dll && \
			sed -i '/supported_platforms/,/}/d' *.rockspec && \
			sed -i '/dependencies/,/}/d' *.rockspec && \
			luarocks make --pack-binary-rock && \
				for r in ./*.rock; do mv \$r \${r/linux/windows}; done && \
				chown $(id -u):$(id -g) *.rock && \
				cp -a *.rock ${STORAGE}/windows64 && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/windows64 && \
		rm -rf ..?* .[!.]* * && \
			cmake .. \
				-DCMAKE_BUILD_TYPE=Release \
				-DTINYSPLINE_ENABLE_RUBY=True \
				-DRuby_EXECUTABLE=/opt/linux/ruby32/bin/ruby \
				-DRuby_INCLUDE_DIR=/opt/windows-x86_64/ruby32/include/ruby-3.2.0 \
				-DRuby_CONFIG_INCLUDE_DIR=/opt/windows-x86_64/ruby32/include/ruby-3.2.0/x64-mingw-ucrt \
				-DRuby_LIBRARY=/opt/windows-x86_64/ruby32/bin/x64-ucrt-ruby320.dll && \
			gem build tinyspline.gemspec && \
				chown $(id -u):$(id -g) *.gem && \
				cp -a *.gem ${STORAGE}/windows64 && \
		popd"

docker rmi ${IMAGE_NAME}
