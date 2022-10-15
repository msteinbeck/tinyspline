#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)
TMP_DIR="${SCRIPT_DIR}/maven"
ZIP_FILE="${SCRIPT_DIR}/tinyspline/tinyspline-java.zip"
SIGNED_ZIP_FILE="${SCRIPT_DIR}/tinyspline-java-signed.zip"

unzip -d "${TMP_DIR}" -o "${ZIP_FILE}"
pushd "${TMP_DIR}"
    for file in *
    do
        gpg2 --no-tty \
             --local-user 1BA70C7A62392ED9372845302DBCE0097DC4D259 \
             --detach-sign \
             --output "${file}.asc" "${file}"
    done
    zip -r "${SIGNED_ZIP_FILE}" ./*
popd
rm -r "${TMP_DIR}"
