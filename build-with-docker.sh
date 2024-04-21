#!/bin/bash

SRC=$(dirname $0)
SRC=$(realpath "$SRC")

docker build \
	-t emception_build \
	.

mkdir -p $(pwd)/build/emsdk_cache

docker run \
	-i --rm \
	-v /var/run/docker.sock:/var/run/docker.sock \
	-v $(pwd):$(pwd) \
	-v $(pwd)/build/emsdk_cache:/emsdk/upstream/emscripten/cache \
	emception_build:latest \
	bash -c "cd $(pwd) && ./build.sh"
