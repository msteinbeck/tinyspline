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

# Setup CMAKE_FLAGS.
$CMAKE_FLAGS = "$Env:CMAKE_FLAGS -DCMAKE_BUILD_TYPE=Release"
if ($Env:COMPILER -eq "mingw") {
	$GENERATOR = "MinGW Makefiles"
} else {
	$GENERATOR = "Visual Studio 15 2017"
	if ($Env:PLATFORM -eq "Win64") {
		$GENERATOR = "$GENERATOR Win64"
	}
}
$CMAKE_FLAGS = "$CMAKE_FLAGS -G ""$GENERATOR"""



###############################################################################
### Install dependencies and setup system.
###############################################################################
choco install -y -r swig --version 3.0.9
choco install -y -r lua
choco install -y -r python3
if ($Env:COMPILER -eq "mingw") {
	if ($Env:PLATFORM -eq "Win64") {
		choco install -y -r mingw
	} else {
		$Env:Path += ";C:\MinGW\bin"
	}
	# sh.exe must not be in PATH when compiling with MinGW.
	Rename-Item -Path "C:\Program Files\Git\usr\bin\sh.exe" -NewName "sh2.exe"
}
refreshenv

# Enable installed and supported interfaces.
$CMAKE_FLAGS = "$CMAKE_FLAGS -DCMAKE_BUILD_TYPE=Release"
$CMAKE_FLAGS = "$CMAKE_FLAGS -DTINYSPLINE_ENABLE_CSHARP=True"
$CMAKE_FLAGS = "$CMAKE_FLAGS -DTINYSPLINE_ENABLE_D=True"
$CMAKE_FLAGS = "$CMAKE_FLAGS -DTINYSPLINE_ENABLE_JAVA=True"
$CMAKE_FLAGS = "$CMAKE_FLAGS -DTINYSPLINE_ENABLE_LUA=True"
if ($Env:COMPILER -eq "msvc") {
	if ($Env:PLATFORM -eq "Win64") {
		$CMAKE_FLAGS = "$CMAKE_FLAGS -DTINYSPLINE_ENABLE_PYTHON=True"
	}
}



###############################################################################
### Compile.
###############################################################################
$OUT_LOG = "out.log"
$ERR_LOG = "err.log"
mkdir $BUILD_DIR_FIXED
pushd $BUILD_DIR_FIXED
	$PROCESS = Start-Process cmake `
		-ArgumentList "$CMAKE_FLAGS $PSScriptRoot\..\.." `
		-PassThru `
		-RedirectStandardOutput "$OUT_LOG" `
		-RedirectStandardError "$ERR_LOG"
	$PROCESS | Wait-Process
	Get-Content -Path "$OUT_LOG"
	Get-Content -Path "$ERR_LOG"
	if ($PROCESS.ExitCode -ne 0) {
		exit $PROCESS.ExitCode
	}

	$PROCESS = Start-Process cmake `
		-ArgumentList "--build . --config Release" `
		-PassThru `
		-RedirectStandardOutput "$OUT_LOG" `
		-RedirectStandardError "$ERR_LOG"
	$PROCESS | Wait-Process
	Get-Content -Path "$OUT_LOG"
	Get-Content -Path "$ERR_LOG"
	if ($PROCESS.ExitCode -ne 0) {
		exit $PROCESS.ExitCode
	}
popd
