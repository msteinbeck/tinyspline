#!/bin/bash
set -e

# Directory where this script is located.
SCRIPT_DIR=$( cd $(dirname $0); pwd -P)

###############################################################################
### Sanity checks and initial setup.
###############################################################################
# Configure branches.
SOURCE_BRANCH="master"
if [ -z "$TRAVIS_OS_NAME" ]; then
	exit 0
fi
if [ "$(getconf LONG_BIT)" = "64" ]; then
	BUILD_BRANCH="build_${TRAVIS_OS_NAME}_64"
else
	BUILD_BRANCH="build_${TRAVIS_OS_NAME}_32"
fi
DOCS_BRANCH="gh-pages"

# Avoid unwanted deployments.
if [ "$TRAVIS" != "true" ]                         \
	|| [ "$TRAVIS_PULL_REQUEST" != "false" ]   \
	|| [ "$TRAVIS_BRANCH" != "$SOURCE_BRANCH" ]; then
	exit 0
fi

# Verify that this script is located next to 'compile.sh'.
if [ ! -f "$SCRIPT_DIR/compile.sh" ]; then
    echo "Unable to find 'compile.sh'; aborting."
    exit -1
fi
# See 'compile.sh'.
if [ -z "$BUILD_DIR" ]; then
	echo "BUILD_DIR has not been set or is empty; aborting."
	exit -1
fi
BUILD_DIR_FIXED="$SCRIPT_DIR/$BUILD_DIR"

# The following CI configuration files and directories will be copied to
# DOCS_BRANCH and BUILD_BRANCH to prevent that these branches are tested (which
# fails due to missing tests).
APPVEYOR_CONFIG_PATH="$SCRIPT_DIR/../../.appveyor.yml"
if [ ! -f "$APPVEYOR_CONFIG_PATH" ]; then
	echo "AppVeyor config file is not available; aborting."
	exit -1
fi
TRAVISCI_CONFIG_PATH="$SCRIPT_DIR/../../.travis.yml"
if [ ! -f "$TRAVISCI_CONFIG_PATH" ]; then
	echo "TravisCI config file is not available; aborting."
	exit -1
fi
CIRCLECI_CONFIG_DIR="$SCRIPT_DIR/../../.circleci"
if [ ! -d "$CIRCLECI_CONFIG_DIR" ]; then
	echo "CircleCI config directory is not available; aborting."
	exit -1
fi

# Store some git-related information.
REPO=`git config remote.origin.url`
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
SHA=`git rev-parse --verify HEAD`



###############################################################################
### Decrypt and install deploy key.
###############################################################################
# Path to the unencrypted deploy key.
DEPLOY_KEY_PATH="$SCRIPT_DIR/deploy_key"
# Path to the encrypted deploy key.
ENC_DEPLOY_KEY_PATH="$DEPLOY_KEY_PATH.enc"
# Verify that the encrypted deploy key is available.
if [ ! -f "$ENC_DEPLOY_KEY_PATH" ]; then
    echo "Unable to find encrypted deploy key; aborting."
    exit -1
fi
openssl aes-256-cbc \
	-K $encrypted_6b814194a991_key \
	-iv $encrypted_6b814194a991_iv \
	-in "$ENC_DEPLOY_KEY_PATH"     \
	-out "$DEPLOY_KEY_PATH"        \
	-d
# Remove decrypted deploy key at the end of this script.
trap "{ rm $DEPLOY_KEY_PATH; }" EXIT
chmod 600 "$DEPLOY_KEY_PATH"
eval `ssh-agent -s`
ssh-add "$DEPLOY_KEY_PATH"



###############################################################################
### Deploy build.
###############################################################################
# Path to the libs that will be deployed.
LIB_DIR="$BUILD_DIR_FIXED/lib"
# Verify that libs have been created.
if [ ! -d "$LIB_DIR" ]; then
	echo "Directory 'lib' is not available; aborting."
	exit -1
fi

# Path to the binaries that will be deployed.
BIN_DIR="$BUILD_DIR_FIXED/bin"
# Verify that binaries have been created.
if [ ! -d "$BIN_DIR" ]; then
	echo "Directory 'bin' is not available; aborting."
	exit -1
fi

# Path to the C# package that will be deployed.
CSHARP_FILE=$(find "$BUILD_DIR_FIXED" -maxdepth 1 -name '*.nupkg')
# Verify that the C# package has been created.
if [ ! -f "$CSHARP_FILE" ]; then
	echo "C# package is not available; aborting."
	exit -1
fi

# Path to the Lua package that will be deployed.
LUAROCKS_FILE=$(find "$BUILD_DIR_FIXED" -maxdepth 1 -name '*.rock')
# Verify that the Lua package has been created.
if [ ! -f "$LUAROCKS_FILE" ]; then
	echo "Lua package is not available; aborting."
	exit -1
fi

# Path to the Java package that will be deployed.
JAVA_FILE=$(find "$BUILD_DIR_FIXED/target" -maxdepth 1 -name '*.jar')
# Verify that the Java package has been created.
if [ ! -f "$JAVA_FILE" ]; then
	echo "Java package is not available; aborting."
	exit -1
fi

# Path to the Python package that will be deployed.
PYTHON_DIR="$BUILD_DIR_FIXED/dist"
# Verify that the Python package has been created.
if [ ! -d "$PYTHON_DIR" ]; then
	echo "Directory 'dist' is not available; aborting."
	exit -1
fi

# Path to the Ruby package that will be deployed.
RUBY_FILE=$(find "$BUILD_DIR_FIXED" -maxdepth 1 -name '*.gem')
# Verify that the Ruby package has been created.
if [ ! -f "$RUBY_FILE" ]; then
	echo "Ruby package is not available; aborting."
	exit -1
fi

# Directory where BUILD_BRANCH is cloned to.
BUILD_BRANCH_DIR="$SCRIPT_DIR/$BUILD_BRANCH"
# Verify that BUILD_BRANCH_DIR does not exist.
if [ -d "$BUILD_BRANCH_DIR" ]        \
	|| [ -f "$BUILD_BRANCH_DIR" ]; then
	echo "'$BUILD_BRANCH_DIR' already exists; aborting."
	exit -1
fi

# Clone BUILD_BRANCH to BUILD_BRANCH_DIR.
git clone -b $BUILD_BRANCH $SSH_REPO $BUILD_BRANCH_DIR
pushd "$BUILD_BRANCH_DIR"
	# Reset this branch to it's first commit.
	git reset $(git rev-list --max-parents=0 HEAD) --hard

	# Copy libs.
	cp -R "$LIB_DIR" ./

	# Copy binaries.
	cp -R "$BIN_DIR" ./

	# Copy packages.
	mkdir dist
	cp "$CSHARP_FILE" ./dist
	cp "$LUAROCKS_FILE" ./dist
	cp "$JAVA_FILE" ./dist
	cp -a "$PYTHON_DIR/." ./dist
	cp "$RUBY_FILE" ./dist

	# Copy AppVeyor config file.
	cp "$APPVEYOR_CONFIG_PATH" ./

	# Copy TravisCI config file.
	cp "$TRAVISCI_CONFIG_PATH" ./

	# Copy CircleCI config directory.
	cp -R "$CIRCLECI_CONFIG_DIR" ./

	# Set user name and email for commit.
	git config user.name "Travis CI"
	git config user.email "travis@tinyspline.org"

	# Commit all changes.
	git add --all
	git commit -m "Deploy build for: ${SHA}"

	# Now that we're all set up, we can push.
	git push --force
popd



###############################################################################
### Deploy docs.
###############################################################################
# Only one environment may deploy docs.
if [ "$DEPLOY_DOCS" != "true" ]            \
	|| [ "$TRAVIS_OS_NAME" != "linux" ]; then
	exit 0
fi

# Path to the docs that will be deployed.
DOXYGEN_HTML_DIR="$BUILD_DIR_FIXED/docs/doxygen/html"
# Verify that docs have been created.
if [ ! -d "$DOXYGEN_HTML_DIR" ]; then
	echo "Doxygen HTML output is not available; aborting."
	exit -1
fi

# Directory where DOCS_BRANCH is cloned to.
DOCS_BRANCH_DIR="$SCRIPT_DIR/$DOCS_BRANCH"
# Verify that DOCS_BRANCH_DIR does not exist.
if [ -d "$DOCS_BRANCH_DIR" ]        \
	|| [ -f "$DOCS_BRANCH_DIR" ]; then
	echo "'$DOCS_BRANCH_DIR' already exists; aborting."
	exit -1
fi

# Clone DOCS_BRANCH to DOCS_BRANCH_DIR.
git clone -b $DOCS_BRANCH $SSH_REPO $DOCS_BRANCH_DIR
pushd "$DOCS_BRANCH_DIR"
	# Copy docs.
	cp -a "$DOXYGEN_HTML_DIR/." ./

	# Copy AppVeyor config file.
	cp "$APPVEYOR_CONFIG_PATH" ./

	# Copy TravisCI config file.
	cp "$TRAVISCI_CONFIG_PATH" ./

	# Copy CircleCI config directory.
	cp -R "$CIRCLECI_CONFIG_DIR" ./

	# Set user name and email for commit.
	git config user.name "Travis CI"
	git config user.email "travis@tinyspline.org"

	# Commit all changes.
	git add --all
	git commit -m "Deploy docs for: ${SHA}"

	# Now that we're all set up, we can push.
	git push --force
popd
