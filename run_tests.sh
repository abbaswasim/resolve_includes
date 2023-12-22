#!/bin/bash

clang++ -g -O0 -std=c++17 test/test.cpp -I. -o test/shader_reader && cd test && ./shader_reader main.frag && cd ..
