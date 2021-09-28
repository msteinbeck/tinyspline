#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)

### Check build directories.
LINUX_X86_64="${SCRIPT_DIR}/build/linux64"
if [ ! -d "${LINUX_X86_64}" ]; then
	echo "Linux (x86_64) is missing.  Aborting."
	exit 1
fi
MACOSX_X86_64="${SCRIPT_DIR}/build/macosx64"
if [ ! -d "${MACOSX_X86_64}" ]; then
	echo "MacOSX (x86_64) is missing.  Aborting."
	exit 1
fi
WINDOWS_X86_64="${SCRIPT_DIR}/build/windows64"
if [ ! -d "${WINDOWS_X86_64}" ]; then
	echo "Windows (x86_64) is missing.  Aborting."
	exit 1
fi
WASM="${SCRIPT_DIR}/build/wasm"
if [ ! -d "${WASM}" ]; then
	echo "WebAssembly is missing.  Aborting."
	exit 1
fi

### Prepare output directory.
OUTPUT="${SCRIPT_DIR}/tinyspline"
mkdir -p "${OUTPUT}"

### Copy and merge files.
# C/C++
find "${LINUX_X86_64}" -name '*.deb' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"

# C#
NUPKG_TMP_DIR="${SCRIPT_DIR}/nuget"
find "${WINDOWS_X86_64}" -name '*.nupkg' -print0 | \
	xargs -0 -I{} unzip -d "${NUPKG_TMP_DIR}" -o {}
find "${MACOSX_X86_64}" -name '*.nupkg' -print0 | \
	xargs -0 -I{} unzip -d "${NUPKG_TMP_DIR}" -o {}
find "${LINUX_X86_64}" -name '*.nupkg' -print0 | \
	xargs -0 -I{} unzip -d "${NUPKG_TMP_DIR}" -o {}
# Add 'lib' prefix to the native library of the Linux/OSX .so file.
find  "${SCRIPT_DIR}/nuget/runtimes/" \
	-name 'tinyspline*.so' \
	-exec /bin/bash -c \
		'DIRNAME=$( dirname "${0}" ) && \
		 BASENAME=$( basename "${0}" ) && \
		 mv "${0}" "${DIRNAME}/lib${BASENAME}"' {} \;
# See https://github.com/NuGet/Home/issues/5956
find  "${SCRIPT_DIR}/nuget/runtimes/" \
	-name '*\%2B*' \
	-exec /bin/bash -c \
		'DIRNAME=$( dirname "${0}" ) && \
		 BASENAME=$( basename "${0}" ) && \
		 mv "${0}" "${DIRNAME}/${BASENAME/\%2B\%2B/++}"' {} \;
NUPKG_NAME=$( find "${LINUX_X86_64}" -name "*.nupkg" -exec basename {} \; )
pushd "${NUPKG_TMP_DIR}"
	zip -r "${OUTPUT}/${NUPKG_NAME}" ./*
popd

# Java
JAVA_ZIP_TMP_DIR="${SCRIPT_DIR}/java"
mkdir -p "${JAVA_ZIP_TMP_DIR}"
find "${LINUX_X86_64}" -name '*javadoc.jar' -print0 | \
	xargs -0 -I{} cp {} "${JAVA_ZIP_TMP_DIR}"
find "${LINUX_X86_64}" -name '*sources.jar' -print0 | \
	xargs -0 -I{} cp {} "${JAVA_ZIP_TMP_DIR}"
find "${LINUX_X86_64}" -name 'pom.xml' -print0 | \
	xargs -0 -I{} cp {} "${JAVA_ZIP_TMP_DIR}"
JAR_TMP_DIR="${SCRIPT_DIR}/jar"
find "${WINDOWS_X86_64}" \( -name '*.jar' \
	-and -not -name '*javadoc*' \
	-and -not -name '*sources*' \) \
	-print0 | xargs -0 -I{} unzip -d "${JAR_TMP_DIR}" -o {}
find "${MACOSX_X86_64}" \( -name '*.jar' \
	-and -not -name '*javadoc*' \
	-and -not -name '*sources*' \) \
	-print0 | xargs -0 -I{} unzip -d "${JAR_TMP_DIR}" -o {}
find "${LINUX_X86_64}" \( -name '*.jar' \
	-and -not -name '*javadoc*' \
	-and -not -name '*sources*' \) \
	-print0 | xargs -0 -I{} unzip -d "${JAR_TMP_DIR}" -o {}
JAR_NAME=$( find "${LINUX_X86_64}" \( -name '*.jar' \
	-and -not -name '*javadoc*' \
	-and -not -name '*sources*' \) \
	-exec basename {} \; )
pushd "${JAR_TMP_DIR}"
	zip -r "${JAVA_ZIP_TMP_DIR}/${JAR_NAME}" ./*
popd
pushd "${JAVA_ZIP_TMP_DIR}"
	zip -r "${OUTPUT}/tinyspline-java.zip" ./*
popd

# Javascript
cp -a ${WASM}/*.tgz ${OUTPUT}

# Lua
LUAROCKS_TMP_DIR="${SCRIPT_DIR}/luarocks"
LUAROCKS_PLATFORMS=( "${LINUX_X86_64}" "${MACOSX_X86_64}" "${WINDOWS_X86_64}" )
for platform in "${LUAROCKS_PLATFORMS[@]}"
do
	p=$(basename "${platform}")
	LUAROCKS_PLATFORM_TMP_DIR="${LUAROCKS_TMP_DIR}/${p}"
	mkdir -p "${LUAROCKS_PLATFORM_TMP_DIR}"
	find "${platform}" -name '*.rock' -print0 | \
		xargs -0 -I{} unzip -d "${LUAROCKS_PLATFORM_TMP_DIR}" -o {}
	if ! ls "${LUAROCKS_PLATFORM_TMP_DIR}"/*.rockspec >/dev/null 2>&1; then
		continue
	fi
	pushd "${LUAROCKS_PLATFORM_TMP_DIR}"
		rm -f rock_manifest
		find . -regex '.*/[a-zA-Z]*[0-9]+.*rockspec' | sort \
			| sed -e 1d | xargs rm
		pkgn=$(find . -regex '.*/[a-zA-Z]*[0-9]+.*rockspec' -print0 \
			| xargs -0 basename | cut -d- -f1 | tr -dc 'a-zA-Z')
		pkgv=$(find . -regex '.*/[a-zA-Z]*[0-9]+.*rockspec' -print0 \
			| xargs -0 basename | cut -d- -f1 | tr -dc '0-9')
		for file in "${pkgn}${pkgv}"*.rockspec
		do
			mv "$file" "${file/${pkgv}/}"
		done
		luas=$(find * -iname '*.lua' | sed 's/.*/"&",/g' \
			| sed ':a;N;$!ba;s/\n/ /g' | sed '$s/,$//')
		libs=$(find * -iname '*.so' -or -iname '*.dll' \
			| sed 's/.*/"&",/g' \
			| sed ':a;N;$!ba;s/\n/ /g' | sed '$s/,$//')
		sed -i "s/${pkgn}${pkgv}/${pkgn}/g" ./*.rockspec
		sed -i '/supported_platforms/,/}/d' ./*.rockspec
		sed -i '/dependencies/,/}/d' ./*.rockspec
		sed -i '/build_command/,/]],/d' ./*.rockspec
		sed -i "s~lua[[:space:]]\{1,\}=[[:space:]]\{1,\}{.*}~lua = { ${luas} }~" ./*.rockspec
		sed -i "s~lib[[:space:]]\{1,\}=[[:space:]]\{1,\}{.*}~lib = { ${libs} }~" ./*.rockspec
		luarocks make --pack-binary-rock
		os=$(find * -name '*.rock' | rev | cut -d. -f2 \
			| rev | cut -d- -f1)
		if [[ ${p} = *"linux"* ]]; then
			for file in ./*.rock; do
				mv "$file" "${file/${os}/linux}" || true;
			done
		elif [[ ${p} = *"macosx"* ]]; then
			for file in ./*.rock; do
				mv "$file" "${file/${os}/macosx}" || true;
			done
		elif [[ ${p} = *"windows"* ]]; then
			for file in ./*.rock; do
				mv "$file" "${file/${os}/windows}" || true;
			done
		fi
	popd
	find "${LUAROCKS_PLATFORM_TMP_DIR}" -name '*.rock' -print0 | \
		xargs -0 -I{} cp {} "${OUTPUT}"
done

# Python
find "${LINUX_X86_64}" -name '*.whl' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
find "${MACOSX_X86_64}" -name '*.whl' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
find "${WINDOWS_X86_64}" -name '*.whl' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"

# Ruby
find "${LINUX_X86_64}" -name '*.gem' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
find "${MACOSX_X86_64}" -name '*.gem' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
