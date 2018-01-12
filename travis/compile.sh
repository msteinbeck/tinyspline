#!/bin/bash
set -e

# Directory where the script is located.
SCRIPT_DIR=$( cd $(dirname $0); pwd -P)

# Check required variables.
if [ -z "$BUILD_DIR" ]; then
	echo "'BUILD_DIR' has not been set or is empty; aborting."
	exit -1
fi

# Build all targets.
mkdir -p $BUILD_DIR
pushd $BUILD_DIR
	cmake "$SCRIPT_DIR/.." $CMAKE_FLAGS
	cmake --build .
popd
