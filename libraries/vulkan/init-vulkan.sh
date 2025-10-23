#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

# vulkan環境設定 https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html
# source /run/media/user/linux-data/vulkan/1.4.321.1/setup-env.sh
# Vulkan Header Files
#  sudo cp -r $VULKAN_SDK/include/vulkan/ /usr/local/include/
# Vulkan Loader Files
#  sudo cp -P $VULKAN_SDK/lib/libvulkan.so* /usr/local/lib/
# Vulkan Layer Files
#  sudo cp $VULKAN_SDK/lib/libVkLayer_*.so /usr/local/lib/
#  sudo mkdir -p /usr/local/share/vulkan/explicit_layer.d
#  sudo cp $VULKAN_SDK/share/vulkan/explicit_layer.d/VkLayer_*.json /usr/local/share/vulkan/explicit_layer.d



# vulkan.cppmビルド
source /run/media/user/linux-data/vulkan/1.4.321.1/setup-env.sh
# vulkanモジュールを読み込み（一度だけ実行すればよい）
# define参考https://qiita.com/bdpumpkin/items/867851cb5df2d836f940
g++ -std=c++23 -fmodules -D VULKAN_HPP_NO_STRUCT_CONSTRUCTORS = 1 -lvulkan -c ${VULKAN_SDK}/include/vulkan/vulkan.cppm


# linker input file not found のエラーがでるが、何がnot foundなのかでない
# 対応方法不明
