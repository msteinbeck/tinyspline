#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)

### Check build directories.
LINUX_X86_64="${SCRIPT_DIR}/build.linux-x86_64"
if [ ! -d "${LINUX_X86_64}" ]; then
	echo "Linux (x86_64) is missing.  Aborting."
	exit -1
fi
MACOSX_X86_64="${SCRIPT_DIR}/build.macosx-x86_64"
if [ ! -d "${MACOSX_X86_64}" ]; then
	echo "MacOSX (x86_64) is missing.  Aborting."
	exit -1
fi
WINDOWS_X86_64="${SCRIPT_DIR}/build.windows-x86_64"
if [ ! -d "${WINDOWS_X86_64}" ]; then
	echo "Windows (x86_64) is missing.  Aborting."
	exit -1
fi

### Prepare output directory.
OUTPUT="${SCRIPT_DIR}/tinyspline"
mkdir -p "${OUTPUT}"

### Copy and merge files.
# C#
NUPKG_TMP_DIR="${SCRIPT_DIR}/nuget"
find "${WINDOWS_X86_64}" -name '*.nupkg' -print0 | \
	xargs -0 -I{} unzip -d "${NUPKG_TMP_DIR}" -o {}
find "${MACOSX_X86_64}" -name '*.nupkg' -print0 | \
	xargs -0 -I{} unzip -d "${NUPKG_TMP_DIR}" -o {}
find "${LINUX_X86_64}" -name '*.nupkg' -print0 | \
	xargs -0 -I{} unzip -d "${NUPKG_TMP_DIR}" -o {}
# Remove 'lib' prefix from the native library of the Windows DLL.
find  "${SCRIPT_DIR}/nuget/runtimes/" \
	-name 'libtinyspline*.dll' \
	-exec /bin/bash -c \
		'DIRNAME=$( dirname "${0}" ) && \
		 BASENAME=$( basename "${0}" ) && \
		 mv "${0}" "${DIRNAME}/${BASENAME#lib}"' {} \;
# Add 'lib' prefix to the native library of the Linux/OSX so file.
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
	zip -r "${OUTPUT}/${NUPKG_NAME}" *
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
	zip -r "${JAVA_ZIP_TMP_DIR}/${JAR_NAME}" *
popd
pushd "${JAVA_ZIP_TMP_DIR}"
	zip -r "${OUTPUT}/tinyspline-java.zip" *
popd

# Lua
find "${LINUX_X86_64}" -name '*.rock' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
find "${MACOSX_X86_64}" -name '*.rock' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"

# Python
find "${LINUX_X86_64}" -name '*.whl' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
find "${MACOSX_X86_64}" -name '*.whl' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"

# Ruby
find "${LINUX_X86_64}" -name '*.gem' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
find "${MACOSX_X86_64}" -name '*.gem' -print0 | \
	xargs -0 -I{} cp {} "${OUTPUT}"
