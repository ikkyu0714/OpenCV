#!/bin/bash

# このスクリプトの場所を取得

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# X11許可

xhost +local:docker

docker run -it --rm \
    --name opencv_cpp_container \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v "$SCRIPT_DIR":/workspace \
    -w /workspace \
    opencv_cpp
