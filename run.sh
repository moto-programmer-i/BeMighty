#!/bin/sh
# 出力フォルダをカレントに
cd out

source /run/media/user/linux-data/vulkan/1.4.321.1/setup-env.sh

# カレントディレクトリをライブラリパスに追加
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

# 実行
./BeMighty
