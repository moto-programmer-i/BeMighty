// BeMighty.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <memory>

import std;
import Glfw;
import Vulkan;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


/// <summary>
        /// チュートリアル用の三角形を描画するコマンドを記録
        /// </summary>
        /// <param name="imageIndex"></param>
void recordCommandBufferToTutorial(Vulkan::Vulkan& vulkan, uint32_t imageIndex) {
    auto& commandBuffer = vulkan.getCommand().getCommandBuffer();
    commandBuffer.begin({});
    // Before starting rendering, transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL
    vulkan.getCommand().transitionImageLayout(
        imageIndex,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal,
        {},                                                     // srcAccessMask (no need to wait for previous operations)
        vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
        vk::PipelineStageFlagBits2::eTopOfPipe,                   // srcStage
        vk::PipelineStageFlagBits2::eColorAttachmentOutput        // dstStage
    );


    
    vk::ClearValue clearColor = vk::ClearColorValue(
        // 黒色
        0.0f, 0.0f, 0.0f, 1.0f);
    vk::RenderingAttachmentInfo attachmentInfo = {
        .imageView = vulkan.getSwapChain().getSwapChainImageViews()[imageIndex],
        .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .clearValue = clearColor
    };

    // チュートリアル用のコードなので、マジックナンバーでも許容
    vk::RenderingInfo renderingInfo = {
        .renderArea = {.offset = { 0, 0 }, .extent = vulkan.getSwapChain().getSwapChainExtent()},
        .layerCount = 1,

        // 本来は配列と要素数だが、1個しかないのでこう書くしかない
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentInfo
    };

    // 3????? 1????? 0???????
    commandBuffer.beginRendering(renderingInfo);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *vulkan.getGraphicsPipeline().getGraphicsPipeline());
    commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(vulkan.getSwapChain().getSwapChainExtent().width), static_cast<float>(vulkan.getSwapChain().getSwapChainExtent().height), 0.0f, 1.0f));

    // 領域を指定
    // https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/03_Drawing/01_Command_buffers.html#_basic_drawing_commands
    commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vulkan.getSwapChain().getSwapChainExtent()));





    
    
    
    
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




    commandBuffer.endRendering();


    // After rendering, transition the swapchain image to PRESENT_SRC
    vulkan.getCommand().transitionImageLayout(
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


// チュートリアルの三角形を描画
void drawTriangleTutorial(Vulkan::Vulkan& vulkan) {
    vulkan.getRendering().drawFrame(
        [&](uint32_t imageIndex) {
            recordCommandBufferToTutorial(vulkan, imageIndex);
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

    // std::cout << "nice module" << std::endl;


    Vulkan::Vulkan vulkan(window);
    drawTriangleTutorial(vulkan);


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
