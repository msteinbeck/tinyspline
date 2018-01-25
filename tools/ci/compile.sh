#!/bin/bash
set -e

# Check required variables.
if [ -z "$BUILD_DIR" ]; then
	echo "BUILD_DIR has not been set or is empty; aborting."
	exit -1
fi

# Directory where this script is located.
SCRIPT_DIR=$( cd $(dirname $0); pwd -P)

# Make BUILD_DIR next to this script.
BUILD_DIR_FIXED="$SCRIPT_DIR/$BUILD_DIR"

# Build all targets.
mkdir -p "$BUILD_DIR_FIXED"
pushd "$BUILD_DIR_FIXED"
	cmake "$SCRIPT_DIR/.." $CMAKE_FLAGS
	cmake --build .
popd
