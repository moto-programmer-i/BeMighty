// BeMighty.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <memory>
#include <chrono>
#include <stdarg.h>

// なぜかvulkan_hppに入っていない
#include <vulkan/vulkan_core.h>

// 効いてるのかどうか不明
// https://docs.vulkan.org/tutorial/latest/07_Depth_buffering.html#_depth_image_and_view
#define GLM_FORCE_DEPTH_ZERO_TO_ONE




import std;
import Glfw;
import Vulkan;



constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


// チュートリアル用モデル
const std::string_view MODEL_PATH = "D:/VisualStudio/repository/BeMighty/thirdparties/models/viking_room/viking_room.obj";
// 暫定でVulkanの textureFilename に入ってしまっている
// const std::string_view TEXTURE_PATH = "thirdparties/models/viking_room/viking_room.png";


// チュートリアルの三角形を描画
void drawTriangleTutorial(Vulkan::Vulkan& vulkan, Vulkan::VertexManager& vertexManager) {
    vulkan.getRendering().drawFrame(
        // チュートリアルのupdateUniformBuffer
        // https://docs.vulkan.org/tutorial/latest/_attachments/22_descriptor_layout.cpp
        [&](uint32_t currentImage) {
            // void updateUniformBuffer(Vulkan::UniformBufferManager & manager, Vulkan::SwapChain & swapChain, uint32_t currentImage) {
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float>(currentTime - startTime).count();


            // チュートリアル用マジックナンバー祭り
            Vulkan::UniformBufferObject ubo{};

            // チュートリアル用、変換
            // https://docs.vulkan.org/tutorial/latest/05_Uniform_buffers/00_Descriptor_set_layout_and_buffer.html
            // Intellisenseではエラーになるが、ビルドは通る（glmがモジュール正式対応していないせい？）
            ubo.model = glm::ext::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            ubo.view = glm::ext::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.proj = glm::ext::perspective(glm::radians(45.0f), static_cast<float>(vulkan.getSwapChain().getSwapChainExtent().width) / static_cast<float>(vulkan.getSwapChain().getSwapChainExtent().height), 0.1f, 10.0f);

            // GLMは元々OpenGL用に設計されたもので、クリップ座標のY座標が反転している
            ubo.proj[1][1] *= -1;

            // UBOをこのように使用するのは、頻繁に変化する値をシェーダーに渡す最も効率的な方法ではありません。
            // 小さなデータバッファをシェーダーに渡すより効率的な方法は、プッシュ定数です。
            // これについては、今後の章で取り上げる予定です。
            std::memcpy(vulkan.getUniformBufferManager().getUniformBufferMapped(currentImage), &ubo, sizeof(ubo));
        },

        // チュートリアルのrecordCommandBuffer
        [&](uint32_t imageIndex) {
            auto& commandBuffer = vulkan.getRendering().getCurrentCommandBuffer();
            commandBuffer.begin({});
            // Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
            vulkan.getRendering().transitionImageLayout(
                imageIndex,
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::eColorAttachmentOptimal,
                {},                                                     // srcAccessMask (no need to wait for previous operations)
                vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
                vk::PipelineStageFlagBits2::eTopOfPipe,                   // srcStage
                vk::PipelineStageFlagBits2::eColorAttachmentOutput        // dstStage
            );


            // https://docs.vulkan.org/tutorial/latest/_attachments/27_depth_buffering.cpp
            // Transition depth image to depth attachment optimal layout
            vk::ImageMemoryBarrier2 depthBarrier = {
                .srcStageMask = vk::PipelineStageFlagBits2::eTopOfPipe,
                .srcAccessMask = {},
                .dstStageMask = vk::PipelineStageFlagBits2::eEarlyFragmentTests | vk::PipelineStageFlagBits2::eLateFragmentTests,
                .dstAccessMask = vk::AccessFlagBits2::eDepthStencilAttachmentRead | vk::AccessFlagBits2::eDepthStencilAttachmentWrite,
                .oldLayout = vk::ImageLayout::eUndefined,
                .newLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image = vulkan.getSwapChain().getDepthImage(),
                .subresourceRange = {
                    // 他とaspectMaskだけ違う
                    .aspectMask = vk::ImageAspectFlagBits::eDepth,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1
                }
            };
            vk::DependencyInfo depthDependencyInfo = {
                .dependencyFlags = {},
                .imageMemoryBarrierCount = 1,
                .pImageMemoryBarriers = &depthBarrier
            };
            commandBuffer.pipelineBarrier2(depthDependencyInfo);



            vk::ClearValue clearColor = vk::ClearColorValue(
                // 黒色
                0.0f, 0.0f, 0.0f, 1.0f);
            vk::ClearValue clearDepth = vk::ClearDepthStencilValue(1.0f, 0);

            vk::RenderingAttachmentInfo colorAttachmentInfo = {
                .imageView = vulkan.getSwapChain().getSwapChainImageViews()[imageIndex],
                .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
                .loadOp = vk::AttachmentLoadOp::eClear,
                .storeOp = vk::AttachmentStoreOp::eStore,
                .clearValue = clearColor
            };

            vk::RenderingAttachmentInfo depthAttachmentInfo = {
               .imageView = vulkan.getSwapChain().getDepthImageView(),
               .imageLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal,
               .loadOp = vk::AttachmentLoadOp::eClear,
               .storeOp = vk::AttachmentStoreOp::eDontCare,
               .clearValue = clearDepth
            };

            // チュートリアル用のコードなので、マジックナンバーでも許容
            vk::RenderingInfo renderingInfo = {
                .renderArea = {.offset = { 0, 0 }, .extent = vulkan.getSwapChain().getSwapChainExtent()},
                .layerCount = 1,

                // 本来は配列と要素数だが、1個しかないのでこう書くしかない
                .colorAttachmentCount = 1,
                .pColorAttachments = &colorAttachmentInfo,
                .pDepthAttachment = &depthAttachmentInfo
            };


            commandBuffer.beginRendering(renderingInfo);
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *vulkan.getGraphicsPipeline().getGraphicsPipeline());
            commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(vulkan.getSwapChain().getSwapChainExtent().width), static_cast<float>(vulkan.getSwapChain().getSwapChainExtent().height), 0.0f, 1.0f));

            // 領域を指定
            // https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/03_Drawing/01_Command_buffers.html#_basic_drawing_commands
            commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vulkan.getSwapChain().getSwapChainExtent()));


            commandBuffer.bindVertexBuffers(0, *vertexManager.getVertexBuffer(), { 0 });
            commandBuffer.bindIndexBuffer(*vertexManager.getIndexBuffer(), 0, vertexManager.getIndexType());
            

            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                vulkan.getGraphicsPipeline().getPipelineLayout(),
                0,
                *vulkan.getDescriptor().getDescriptorSet(vulkan.getRendering().getCurrentFrame()),
                nullptr);

            // Vulkanにインデックスバッファを使用するよう指示
            //  本来1つのinstanceを引数にとるオーバーロードがあるべき
            commandBuffer.drawIndexed(
                // indexCount
                vertexManager.getIndices().size(),
                // instanceCount
                1,
                // firstIndex
                0,
                // vertexOffset
                0,
                // firstInstance
                0);



            /*
            commandBuffer.draw(
                // vertexCount: Even though we don’t have a vertex buffer, we technically still have 3 vertices to draw.
                // 現状はshader.slangのpositionの数と合わせないとうまく動作しない
                3,

                // instanceCount : Used for instanced rendering, use 1 if you’re not doing that.
                1,

                // firstVertex : Used as an offset into the vertex buffer, defines the lowest value of SV_VertexId.
                0,

                // firstInstance : Used as an offset for instanced rendering, defines the lowest value of SV_InstanceID.
                0
            );
            */




            commandBuffer.endRendering();


            // After rendering, transition the swapchain image to PRESENT_SRC
            vulkan.getRendering().transitionImageLayout(
                imageIndex,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::ImageLayout::ePresentSrcKHR,
                vk::AccessFlagBits2::eColorAttachmentWrite,                 // srcAccessMask
                {},                                                      // dstAccessMask
                vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
                vk::PipelineStageFlagBits2::eBottomOfPipe                  // dstStage
            );
            commandBuffer.end();
        }
    );
}


int main()
{
    // ウィンドウの作成と同時に実行。わけた方が良いかは要検討
    Glfw::Window window{
        {
            .width = WIDTH,
            .height = HEIGHT,
            .name = "vulkan"
        }
    };


    // std::cout << "ああああ" << std::endl;


    Vulkan::Vulkan vulkan(window);

    Vulkan::Model model(MODEL_PATH);


    Vulkan::VertexManager vertexManager(
        vulkan.getDevice(),
        vulkan.getRendering(),
        model.getVertices(),
        model.getIndices()
        
        //{
        //    // 座標、色、UV座標（1が画像のサイズ）
        //    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        //    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        //    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        //    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        //    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        //    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        //    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
        //},

        //// 頂点再利用のためのインデックス。これ自分で書くの？？？？
        //{
        //    0, 1, 2, 2, 3, 0,
        //    4, 5, 6, 6, 7, 4
        //}
    );

    
    // drawTriangleTutorial(vulkan, vertexManager);

    // Vulkan::Texture(vulkan.getDevice(), vulkan.getRendering(), "D:/VisualStudio/repository/BeMighty/textures/texture.jpg");
    
    
    // 別スレッドを作る
    window.emplaceThread(
        [&](std::stop_token token) {
            while (!token.stop_requested()) {
                // 適当に待ちながら描画する
                std::this_thread::sleep_for(std::chrono::milliseconds(33));
                // std::cout << "別スレッド" << std::endl;
                drawTriangleTutorial(vulkan, vertexManager);
            }
        }
    );
    
    


    window.waitUntilClose();
}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
