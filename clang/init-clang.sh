#!/bin/sh -ex

# Geanyで情報を表示するためのclangの初期設定
# 現状はコンパイルはgccで行うが、IDEの情報表示はclangになってしまっている
# clangが今後改善されれば全てclangに移行するのも手

# コンパイルコマンド
# https://clang.llvm.org/docs/ClangCommandLineReference.html

# モジュールの指定方法
# https://clang.llvm.org/docs/StandardCPlusPlusModules.html#specifying-bmi-dependencies

# 事前にlibc++をインストール
# yay -S libc++

# 事前にstdをビルド
# https://0xstubs.org/using-the-c23-std-module-with-clang-18/
#clang++ -std=c++23 -stdlib=libc++  -Wno-reserved-module-identifier --precompile /usr/share/libc++/v1/std.cppm  -o clang/std.pcm

# vulkanをビルド
clang++ -std=c++23  --precompile -D VULKAN_HPP_NO_STRUCT_CONSTRUCTORS /usr/local/include/vulkan/vulkan.cppm -o vulkan_hpp.pcm


# 自前のモジュールに関しては設定が面倒なので省略する


