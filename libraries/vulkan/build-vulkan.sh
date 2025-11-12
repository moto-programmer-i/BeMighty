#!/bin/sh -ex
# 出力フォルダをカレントに
cd ../../out

#コンパイル
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.patch.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.settings.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.information.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.device.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.buffer.ixx
g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.image.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.swap_chain.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.rendering.ixx
g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.texture.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.vertex.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.graphics_pipeline.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.descriptor.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.model.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.validation.ixx
#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.ixx

#g++ -std=c++23 -fmodules -fPIC -Wall -c ../libraries/vulkan/vulkan.test.ixx

#動的ライブラリ作成
g++ -std=c++23 -fmodules -fPIC -shared  -o VulkanWrapper.so vulkan*.o
