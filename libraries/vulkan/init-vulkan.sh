#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

# stdモジュール作成（最初に１回だけやれば良い）
#g++ -std=c++23 -fmodules -fsearch-include-path -c bits/std.cc
#g++ -std=c++23 -fmodules -fsearch-include-path -c bits/std.compat.cc


# vulkan.cppmビルド
source /run/media/user/linux-data/vulkan/1.4.321.1/setup-env.sh

# vulkanモジュールを読み込み（一度だけ実行すればよい）
# define参考https://qiita.com/bdpumpkin/items/867851cb5df2d836f940
g++ -std=c++23 -fmodules -D VULKAN_HPP_NO_STRUCT_CONSTRUCTORS -lvulkan -c ${VULKAN_SDK}/include/vulkan/vulkan.cppm

# 最新の328はmax_align_t の重複？によりエラーがでる、今は動かない？

