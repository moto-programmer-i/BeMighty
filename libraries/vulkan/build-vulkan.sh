#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

#コンパイル
g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan_tutorial.ixx

#動的ライブラリ作成
g++ -std=c++23 -fmodules -shared -o VulkanTutorial.so vulkan_tutorial.o
