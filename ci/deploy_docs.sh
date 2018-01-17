#!/bin/bash
set -e

# Create docs from 'master' and push them to 'gh-pages'.
SOURCE_BRANCH="master"
TARGET_BRANCH="gh-pages"

# Prevent multiple or unwanted deployments.
if [ "$TRAVIS" != "true" ]                         \
	|| [ "$DEPLOY_DOCS" != "true" ]            \
	|| [ "$TRAVIS_OS_NAME" != "linux" ]        \
	|| [ "$CC" != "clang" ]                    \
	|| [ "$TRAVIS_PULL_REQUEST" != "false" ]   \
	|| [ "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
	exit 0
fi

# Store some useful information.
REPO=`git config remote.origin.url`
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
SHA=`git rev-parse --verify HEAD`

# Directory where this script is located.
SCRIPT_DIR=$( cd $(dirname $0); pwd -P)

# Verify that this script is next to 'compile.sh'.
if [ ! -f "$SCRIPT_DIR/compile.sh" ]; then
    echo "Unable to find 'compile.sh'; aborting."
    exit -1
fi
# See 'compile.sh'.
BUILD_DIR_FIXED="$SCRIPT_DIR/$BUILD_DIR"

# Path to the docs that will be deployed.
DOXYGEN_HTML_DIR="$BUILD_DIR_FIXED/doxygen/html"
# Verify that docs have been created.
if [ ! -d "$DOXYGEN_HTML_DIR" ]; then
	echo "Doxygen HTML output is not available; aborting."
	exit -1
fi

# Path to unencrypted deploy key.
DEPLOY_KEY_PATH="$SCRIPT_DIR/deploy_key"
# Path to encrypted deploy key.
ENC_DEPLOY_KEY_PATH="$DEPLOY_KEY_PATH.enc"
# Verify that encrypted deploy is available.
if [ ! -f "$ENC_DEPLOY_KEY_PATH" ]; then
    echo "Unable to find encrypted deploy key; aborting."
    exit -1
fi
# Decrypt and install deploy key.
openssl aes-256-cbc \
	-K $encrypted_6b814194a991_key \
	-iv $encrypted_6b814194a991_iv \
	-in "$ENC_DEPLOY_KEY_PATH"     \
	-out "$DEPLOY_KEY_PATH"        \
	-d
chmod 600 "$DEPLOY_KEY_PATH"
eval `ssh-agent -s`
ssh-add "$DEPLOY_KEY_PATH"

# Directory where TARGET_BRANCH is cloned to.
TARGET_DIR="$SCRIPT_DIR/$TARGET_BRANCH"
# Verify that TARGET_DIR does not exist.
if [ -d "$TARGET_DIR" ]        \
	|| [ -f "$TARGET_DIR" ]; then
	echo "'$TARGET_DIR' already exists; aborting."
	exit -1
fi
# Clone TARGET_BRANCH to TARGET_DIR.
git clone -b $TARGET_BRANCH $SSH_REPO $TARGET_DIR
pushd "$TARGET_DIR"
	# Copy docs.
	cp -a "$DOXYGEN_HTML_DIR/." ./

	# Set user name and email for commit.
	git config user.name "Travis CI"
	git config user.email "travis@tinyspline.org"

	# Commit all changes.
	git add --all
	git commit -m "Deploy docs for: ${SHA}"

	# Now that we're all set up, we can push.
	git push --force
popd

# Remove decrypted deploy key.
rm "$DEPLOY_KEY_PATH"
