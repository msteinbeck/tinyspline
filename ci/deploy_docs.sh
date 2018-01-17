#!/bin/bash
set -e

# Create docs from 'master' and push them to 'gh-pages'.
SOURCE_BRANCH="master"
TARGET_BRANCH="gh-pages"

# Check environment to prevent multiple or unwanted deployments.
if [ "$DEPLOY_DOCS" != "yes" ]                     \
	|| [ "$TRAVIS_OS_NAME" != "linux" ]        \
	|| [ "$CC" != "clang" ]                    \
	|| [ "$TRAVIS_PULL_REQUEST" != "false" ]   \
	|| [ "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
	exit 0
fi

# Check required variables.
if [ -z "$BUILD_DIR" ] || [ ! -d "$BUILD_DIR" ]; then
	echo "Build directory is not available; aborting."
	exit -1
fi

# Locations of docs to deploy.
DOXYGEN_HTML_DIR="$BUILD_DIR/doxygen/html"

# Make sure docs have been created.
if [ ! -d "$DOXYGEN_HTML_DIR" ]; then
	echo "HTML output ($DOXYGEN_HTML_DIR) is not available; aborting."
	exit -1
fi

# Store some useful information.
REPO=`git config remote.origin.url`
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
SHA=`git rev-parse --verify HEAD`

# Decrypt and install deploy key.
openssl aes-256-cbc \
	-K $encrypted_6b814194a991_key \
	-iv $encrypted_6b814194a991_iv \
	-in deploy_key.enc -out deploy_key -d
chmod 600 deploy_key
eval `ssh-agent -s`
ssh-add deploy_key

# Clone the docs branch to 'TARGET_BRANCH'.
git clone -b $TARGET_BRANCH $SSH_REPO $TARGET_BRANCH
pushd $TARGET_BRANCH
	# Copy docs.
	cp -a "../$DOXYGEN_HTML_DIR/." ./

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
rm deploy_key
