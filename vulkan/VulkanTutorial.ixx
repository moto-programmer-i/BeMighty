export module VulkanTutorial;

import Glfw;

// Vulkan�̃`���[�g���A��
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
    // �O���ŊǗ�����Ă���E�B���h�E�B���̃N���X���ł͉�����Ȃ�����
    Glfw::Window *  window;
};
