#!/bin/sh -ex
source /run/media/user/linux-data/vulkan/1.4.321.1/setup-env.sh

# outフォルダにビルド
slangc shader.slang -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name -entry vertMain -entry fragMain -o ../out/slang.spv
