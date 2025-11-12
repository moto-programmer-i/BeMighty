#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

# glmモジュール作成（最初に１回だけやれば良い）
g++ -std=c++23 -fmodules -fsearch-include-path -c glm/glm.cppm


