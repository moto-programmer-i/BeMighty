export module Vulkan;

export import :Information;
export import :Device;
export import :SwapChain;
export import :SwapChainSettings;
export import :GraphicsPipeline;
export import :Command;
export import :Rendering;


#ifdef NDEBUG
#else
export import :Validation;
#endif

import std;

import Glfw;


namespace Vulkan {
	export class Vulkan {
    public:
        // チュートリアルからコピペなのでそのまま。
        // どうすべきかわかったら削除する。
        static constexpr std::string_view spvFilename = "D:/VisualStudio/repository/BeMighty/shaders/slang.spv";
        static constexpr std::string_view vertName = "vertMain";
        static constexpr std::string_view fragName = "fragMain";

        Vulkan(Glfw::Window& window) :
            window(window),
            information(window),
            device(information.getInstance(), information.getSurface()),
            swapChain(device, information.getSurface(), window),
            graphicsPipeline(device, swapChain, spvFilename, vertName, fragName),
            command(device, swapChain, graphicsPipeline),
            rendering(device, swapChain, command)

#ifdef NDEBUG
#else
        , debugManager(information)
#endif
      
        {

        }

        void draw() {
            // チュートリアルの状態。本当は指定したものを描画できるようにする
            rendering.drawFrame();
        }
	private:
        // 外部で管理されているウィンドウ。このクラス内では解放しないこと
        Glfw::Window& window;

        Information information;
        Device device;
        SwapChain swapChain;
        GraphicsPipeline graphicsPipeline;
        Command command;
        Rendering rendering;


#ifdef NDEBUG
#else
        DebugManager debugManager;
#endif
	};
}