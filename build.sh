#!/bin/sh

###############################################
# Convenience script for building with CMake. #
###############################################

# Make sure we are in the correct directory.
cd $(dirname "$0")

# Output diractory name / location
OUTPUT="out"

# Create output directory if it doesn't exist
if [ ! -d $OUTPUT ]
then
    mkdir $OUTPUT
fi

# Enter output directory
cd $OUTPUT

# CMake
cmake .. 

# Stopping if CMake fails.
if [ $? -ne 0 ]; then
    echo "CMake failed, stopping build."
    exit 1
fi

# Make
make 

# Stopping if Make fails.
if [ $? -ne 0 ]; then
    echo "Make failed, stopping build."
    exit 1
fi

