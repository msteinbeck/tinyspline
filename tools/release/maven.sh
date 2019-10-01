#!/bin/sh
set -e

SCRIPT_DIR=$( cd "$(dirname "$0")"; pwd -P)
TMP_DIR="${SCRIPT_DIR}/maven"
ZIP_NAME="tinyspline-java.zip"
ZIP_FILE="${SCRIPT_DIR}/${ZIP_NAME}"
SIGNED_ZIP_FILE="${SCRIPT_DIR}/tinyspline-java-signed.zip"
URL="https://github.com/msteinbeck/tinyspline/releases/download/snapshot/${ZIP_NAME}"

wget -O "${ZIP_FILE}" "${URL}"
unzip -d "${TMP_DIR}" -o "${ZIP_FILE}"
pushd "${TMP_DIR}"
	for file in *
	do
		gpg2 --no-tty \
			--local-user 06B668EB \
			--detach-sign \
			--output "${file}.asc" "${file}"
	done
	zip -r "${SIGNED_ZIP_FILE}" *
popd
