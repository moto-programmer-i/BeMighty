export module Vulkan;


export import :Settings;
export import :Information;
export import :Device;
export import :SwapChain;
export import :Buffer;
export import :Descriptor;
export import :GraphicsPipeline;
export import :Rendering;
export import :Vertex;
export import :Texture;



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

        // テクスチャを自由に読み込めるようにしたいが、
        // 今は方法が不明なのでチュートリアルからコピペ
        static constexpr std::string_view textureFilename = "D:/VisualStudio/repository/BeMighty/textures/texture.jpg";

        Vulkan(Glfw::Window& window, std::uint32_t maxFrames = Settings::DEFAULT_MAX_FRAMES_IN_FLIGHT) :
            // なぜかここで初期化しなければならない
            window(window),
            maxFrames(maxFrames),
            information(window),
            device(information.getInstance(), information.getSurface()),
            swapChain(device, information.getSurface(), window),
            uniformBufferManager(device, swapChain, maxFrames),
            graphicsPipeline(device, swapChain, spvFilename, vertName, fragName),
            rendering(device, swapChain, maxFrames),

            // 本来は後から追加できるべき？
            texture(device, rendering, textureFilename),
            descriptor(device, graphicsPipeline, uniformBufferManager, texture)

#ifdef NDEBUG
#else
        , debugManager(information)
#endif
      
        {
            
        }

        std::uint32_t getMaxFrames() {
            return maxFrames;
        }

        Information& getInformation() {
            return information;
        }

        Device& getDevice() {
            return device;
        }
        SwapChain& getSwapChain() {
            return swapChain;
        }
        UniformBufferManager& getUniformBufferManager() {
            return uniformBufferManager;
        }
        Descriptor& getDescriptor() {
            return descriptor;
        }
        GraphicsPipeline& getGraphicsPipeline() {
            return graphicsPipeline;
        }

        Rendering& getRendering() {
            return rendering;
        }
	private:
        // 外部で管理されているウィンドウ。このクラス内では解放しないこと
        Glfw::Window& window;

        std::uint32_t maxFrames;

        Information information;
        Device device;
        SwapChain swapChain;
        UniformBufferManager uniformBufferManager;
        GraphicsPipeline graphicsPipeline;
        Rendering rendering;
        Texture texture;
        Descriptor descriptor;


#ifdef NDEBUG
#else
        DebugManager debugManager;
#endif
	};
}