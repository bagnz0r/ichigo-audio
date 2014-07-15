#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Missing platform argument"
    echo "Usage: build.sh [osx|linux]"
    exit 1
fi
if [ $1 == "osx" ]; then
	cp -rvf output/*.dylib ../ichigo-player/node_modules/nodewebkit/nodewebkit
fi
if [ $1 == "linux" ]; then
	cp -rvf output/*.so ../ichigo-player/node_modules/nodewebkit/nodewebkit
fi