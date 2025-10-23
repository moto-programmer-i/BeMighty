#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

#インクルードフォルダ、パッケージによってインストールされている
#/usr/include/GLFW

#ライブラリ
#/usr/lib/libglfw.so.3.4

# glfwがvulkanを参照しているようなので、vulkanを/usr/local/includeに置いた
# 本来パッケージからvulkanもインストールするべき？
#/usr/include/GLFW/glfw3.h:111:12: 致命的エラー: vulkan/vulkan.h: そのようなファイルやディレクトリはありません
#  111 |   #include <vulkan/vulkan.h>


# サブモジュールからコンパイル
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/glfw/glfw.window_settings.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/glfw/glfw.window.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/glfw/glfw.structs.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/glfw/glfw.utils.ixx

# モジュールをコンパイル
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/glfw/glfw.ixx

#動的ライブラリ作成
g++ -std=c++23 -fmodules -shared -o Glfw.so glfw*.o
