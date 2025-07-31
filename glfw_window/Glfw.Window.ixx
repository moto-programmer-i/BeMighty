export module Glfw:Window;
import :WindowSettings;
import std;

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
        void wait_until_close() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }

        WindowSettings get_settings() {
            return this->settings;
        }

        ~Window() {
            // GLFWの終了
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        // ウィンドウへのポインタ。このインスタンス内で管理されるので、解放しないこと
        GLFWwindow* get_window() {
            return window;
        }

    private:
        GLFWwindow* window = nullptr;
        WindowSettings settings;
    };
}
