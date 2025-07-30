export module Glfw:Window;
import :WindowSettings;
import std;

#include <GLFW/glfw3.h>

// GLFW�����W���[���������N���X
export class Window {
public:
    /// <summary>
    /// �E�B���h�E�쐬�Ǝ��s
    /// </summary>
    /// <param name="settings"></param>
    /// <param name="main">�쐬��̏���</param>
    Window(WindowSettings settings, std::function<void()> main) {
        this->settings = settings;

        // GLFW������
        glfwInit();
        // GLFW�͌��XOpenGL�p�Ȃ̂ŁAOpenGL���g��Ȃ����Ƃ��w�肷��K�v������
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(settings.width, settings.height, settings.name.c_str(), nullptr, nullptr);


        // �������s
        main();


        // �E�B���h�E�����܂őҋ@
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    WindowSettings get_settings() {
        return this->settings;
    }

    ~Window() {
        // GLFW�̏I��
        glfwDestroyWindow(window);
        glfwTerminate();
    }

private:
    GLFWwindow* window = nullptr;
    WindowSettings settings;
};
