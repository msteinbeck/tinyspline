#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)
TMP_DIR="${SCRIPT_DIR}/pypi"
WHL_DIR="${SCRIPT_DIR}/tinyspline"

mkdir -p "${TMP_DIR}"
for file in "${WHL_DIR}"/*.whl; do
    BASENAME=$( basename "${file}" )
    cp "${file}" "${TMP_DIR}/${BASENAME/linux/manylinux_2_24}"
done
