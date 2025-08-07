export module Glfw:Window;

import :WindowSettings;
import :Structs;
import std;

#define GLFW_INCLUDE_VULKAN // REQUIRED only for GLFW CreateWindowSurface.
#include <GLFW/glfw3.h>

namespace Glfw {
    // GLFW�����W���[���������N���X
    export class Window {
    public:
        /// <summary>
        /// �E�B���h�E�쐬�Ǝ��s
        /// </summary>
        /// <param name="settings"></param>
        Window(WindowSettings settings) {
            this->settings = settings;

            // GLFW������
            glfwInit();
            // GLFW�͌��XOpenGL�p�Ȃ̂ŁAOpenGL���g��Ȃ����Ƃ��w�肷��K�v������
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            window = glfwCreateWindow(settings.width, settings.height, settings.name.c_str(), nullptr, nullptr);
        }

        // �E�B���h�E�����܂őҋ@
        void waitUntilClose() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }

        WindowSettings getSettings() {
            return this->settings;
        }

        ~Window() {
            // GLFW�̏I��
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        // �E�B���h�E�ւ̎Q�ƁB���̃C���X�^���X���ŊǗ������̂ŁA������Ȃ�����
        GLFWwindow& getWindow() {
            return *window;
        }

        Glfw::Size getFramebufferSize() {
            Glfw::Size size{};
            // C�̃R�[�h�͂�������C++�ŉB�����j��
            glfwGetFramebufferSize(window, &size.width, &size.height);

            // �ꉞ�R�s�[�͂���Ȃ��炵��
            // https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html
            return size;
        }
        

    private:
        GLFWwindow* window = nullptr;
        WindowSettings settings;
    };
}
