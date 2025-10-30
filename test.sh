#!/bin/sh -ex
# 出力フォルダをカレントに
cd out

#/usr/lib以下の、.so は自動的に探索される

# 実行ファイル作成
# libglfw.so をリンクしたいときは -lglfw （先頭のlibと、拡張子は不要）
g++ -g -O0 -std=c++23 -fmodules -fPIC ../test.cpp -L. Files.so -o test

# カレントディレクトリをライブラリパスに追加
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# デバッグ
gdb ./test

