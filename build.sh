#!/bin/sh -ex
# 出力フォルダをカレントに
cd out

#/usr/lib以下の、.so は自動的に探索される

# 実行ファイル作成
#g++ -std=c++23 -fmodules -fPIC ../main.cpp -L. std.o  Glfw.so Vulkan.so -o main
# g++ -std=c++23 -fmodules -fPIC ../main.cpp -l libglfw.so -o main
# libglfw.so をリンクしたいときは -lglfw （先頭のlibと、拡張子は不要）
#g++ -std=c++23 -fmodules -fPIC ../main.cpp -lglfw  -L. std.o Glfw.so VulkanTutorial.so -o main
#g++ -std=c++23 -fmodules -fPIC ../main.cpp -lglfw -L. Glfw.so VulkanTutorial.so -o main
g++ -std=c++23 -fmodules -fPIC ../main.cpp -lglfw -L. Glfw.so -o BeMighty

