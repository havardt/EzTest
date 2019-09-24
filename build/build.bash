#!/bin/bash
# Build script: see README.md for more usage. 

# Make sure we are in the correct directory so the script can be called from anywhere.
cd $(dirname "$0")

OUTPUT="out"

echo "Starting build.."

# Remove output directory if it exists.
if [ -d $OUTPUT ]
then
    rm -r $OUTPUT
fi

# Make and enter output directory.
mkdir $OUTPUT
cd $OUTPUT

# CMake, only displaying errors.
cmake ../ > /dev/null

# Stopping if CMake fails.
if [ $? -ne 0 ]; then
    echo "CMake failed, stopping build."
    exit 1
fi

# Make, only displaying errors.
make > /dev/null

# Stopping if Make fails.
if [ $? -ne 0 ]; then
    echo "Make failed, stopping build."
    exit 1
fi

# Go back out of output dir.
cd ..

# Remove the output files.
if [ -d $OUTPUT ]
then
    rm -r $OUTPUT
fi


echo "Build complete."

