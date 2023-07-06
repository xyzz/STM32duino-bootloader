#!/bin/bash

docker build -f Dockerfile -t bootloader_build .

docker run -it -v $(pwd):/source bootloader_build bash -c "cd /source && mkdir -p build && cd build && cmake .. && make -j $(nproc)"
