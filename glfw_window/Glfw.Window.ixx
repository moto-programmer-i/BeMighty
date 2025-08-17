export module Glfw:Window;

import std;

import :WindowSettings;
import :Structs;


#define GLFW_INCLUDE_VULKAN // REQUIRED only for GLFW CreateWindowSurface.
#include <GLFW/glfw3.h>

namespace Glfw {
    // GLFWをモジュール化したクラス
    export class Window {
    public:
        /// <summary>
        /// ウィンドウ作成と実行
        /// </summary>
        /// <param name="settings"></param>
        Window(WindowSettings settings) {
            this->settings = settings;

            // GLFW初期化
            glfwInit();
            // GLFWは元々OpenGL用なので、OpenGLを使わないことを指定する必要がある
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            window = glfwCreateWindow(settings.width, settings.height, settings.name.c_str(), nullptr, nullptr);
        }

        // ウィンドウを閉じるまで待機
        void waitUntilClose() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();

                // 一旦すべてをdraw、将来的には必要なときだけ描画したい
                for (auto& draw : drawFunctions) {
                    draw();
                }
            }
        }

        WindowSettings getSettings() {
            return this->settings;
        }

        ~Window() {
            // GLFWの終了
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        // ウィンドウへの参照。このインスタンス内で管理されるので、解放しないこと
        GLFWwindow& getWindow() {
            return *window;
        }

        Glfw::Size getFramebufferSize() {
            Glfw::Size size{};
            // CのコードはさすがにC++で隠す方針で
            glfwGetFramebufferSize(window, &size.width, &size.height);

            // 一応コピーはされないらしい
            // https://cpprefjp.github.io/lang/cpp17/guaranteed_copy_elision.html
            return size;
        }

        
        void addDraw(std::function<void(void)> draw) {
            // 関数は元々ポインタなので参照は不要らしい
            drawFunctions.emplace_back(draw);
        }
        

    private:
        GLFWwindow* window = nullptr;
        WindowSettings settings;
        std::vector<std::function<void(void)>> drawFunctions;
    };
}
