#!/bin/sh
set -e
cd "$(dirname "$0")"
git pull
git submodule update
./build.sh configure
./build.sh build
