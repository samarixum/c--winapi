Write-Output "Compilation uses g++, make sure you have it installed!"

# Create build directory if it doesn't exist
$buildDir = "build"
if (-Not (Test-Path -Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir
}

windres src/resources.rc -O coff -o $buildDir/resources.o
g++ -o $buildDir/win.exe src/main.cpp $buildDir/resources.o -mwindows
