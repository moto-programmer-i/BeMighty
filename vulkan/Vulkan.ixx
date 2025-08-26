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

        Vulkan(Glfw::Window& window, std::uint32_t maxFrames = Settings::DEFAULT_MAX_FRAMES_IN_FLIGHT) :
            window(window),
            maxFrames(maxFrames),
            information(window),
            device(information.getInstance(), information.getSurface()),
            swapChain(device, information.getSurface(), window),
            uniformBufferManager(device, swapChain, maxFrames),
            descriptor(device, uniformBufferManager),
            graphicsPipeline(device, swapChain, descriptor, spvFilename, vertName, fragName),
            rendering(device, swapChain, maxFrames)

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
        Descriptor descriptor;
        GraphicsPipeline graphicsPipeline;
        Rendering rendering;


#ifdef NDEBUG
#else
        DebugManager debugManager;
#endif
	};
}