###############################################################################
### Sanity checks and initial setup.
###############################################################################
if ([string]::IsNullOrEmpty($Env:BUILD_DIR)) {
	"BUILD_DIR has not been set or is empty; aborting."
	exit 1
}

if ([string]::IsNullOrEmpty($Env:COMPILER)) {
	"COMPILER has not been set or is empty; aborting."
	exit 1
}
if ($Env:COMPILER -ne "mingw" -and $Env:COMPILER -ne "msvc") {
	"Unsupported value: COMPILER = '$Env:COMPILER'; aborting."
	exit 1
}

if ([string]::IsNullOrEmpty($Env:PLATFORM)) {
	"PLATFORM has not been set or is empty; aborting."
	exit 1
}
if ($Env:PLATFORM -ne "Win32" -and $Env:PLATFORM -ne "Win64") {
	"Unsupported value: PLATFORM = '$Env:PLATFORM'; aborting."
	exit 1
}

# Make BUILD_DIR next to this script.
$BUILD_DIR_FIXED = "$PSScriptRoot\$Env:BUILD_DIR"

# Setup CMake generator.
if ($Env:COMPILER -eq "mingw") {
	# sh.exe must not be in PATH when compiling with MinGW.
	Rename-Item -Path "C:\Program Files\Git\usr\bin\sh.exe" -NewName "sh2.exe"
	$GENERATOR = "MinGW Makefiles"
} else {
	$GENERATOR = "Visual Studio 15 2017"
	if ($Env:PLATFORM -eq "Win64") {
		$GENERATOR = "$GENERATOR Win64"
	}
}



###############################################################################
### Install dependencies.
###############################################################################
choco install -y -r swig --version 3.0.9
choco install -y -r lua53
if ($Env:COMPILER -eq "mingw") {
	if ($Env:PLATFORM -eq "Win64") {
		choco install -y -r mingw
	} else {
		$Env:Path += ";C:\MinGW\bin"
	}
}
refreshenv



###############################################################################
### Compile all targets.
###############################################################################
mkdir $BUILD_DIR_FIXED
pushd $BUILD_DIR_FIXED
	cmake -G "$GENERATOR" "$Env:CMAKE_FLAGS" "$PSScriptRoot\..\.."
	cmake --build .
popd
