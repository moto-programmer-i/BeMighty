#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

#コンパイル
g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/files/files.cppm

#動的ライブラリ作成
g++ -std=c++23 -fmodules -shared -o Files.so files.o
