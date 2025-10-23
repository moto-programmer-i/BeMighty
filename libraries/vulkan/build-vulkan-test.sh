#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

#コンパイル
#g++ -std=c++23 -fmodules -fPIC -Wall -lglfw -lvulkan -c ../libraries/vulkan/vulkan_test.ixx -o main
g++ -std=c++23  -fmodules -c ../libraries/vulkan/vulkan_test.cpp  -o main 

#動的ライブラリ作成
#g++ -std=c++23 -fmodules -shared -o VulkanTutorial.so vulkan_tutorial.o
