export module VulkanTutorial;

import Glfw;

// Vulkanのチュートリアル
// https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/00_Setup/00_Base_code.html
export class HelloTriangleApplication {
public:

    HelloTriangleApplication(Glfw::Window* window) {
        this->window = window;
    }
    void run() {
        /*initWindow();
        initVulkan();
        mainLoop();
        cleanup();*/
    }

private:
    // 外部で管理されているウィンドウ。このクラス内では解放しないこと
    Glfw::Window *  window;
};
