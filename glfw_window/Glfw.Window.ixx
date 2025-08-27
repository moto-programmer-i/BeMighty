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

            // インスタンスを登録
            glfwSetWindowUserPointer(window, this);

            // ウィンドウサイズ変更時などのコールバック
            glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        }

        

        // ウィンドウを閉じるまで待機
        void waitUntilClose() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();

                //// 一旦すべてをdraw、将来的には必要なときだけ描画したい
                //for (auto& draw : drawFunctions) {
                //    draw();
                //}
            }

            // ウィンドウを閉じるときは全てのスレッドを停止
            std::for_each(std::execution::par, threads.begin(), threads.end(), [&](auto& thread) {
                thread.request_stop();
                thread.join();
            });
        }

        WindowSettings getSettings() {
            return this->settings;
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

        // 
        Glfw::Size waitUntilSetFramebufferSize() {
            // ウィンドウ最小化のときなどは、FramebufferSizeが0になってしまう
            // https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/04_Swap_chain_recreation.html#_handling_minimization
            Glfw::Size size{};
            do {
                glfwGetFramebufferSize(window, &size.width, &size.height);
                glfwWaitEvents();
            } while (size.width == 0 || size.height == 0);
            return size;
        }

        
        //void addDraw(std::function<void(void)> draw) {
        //    // 関数は元々ポインタなので参照は不要らしい
        //    drawFunctions.emplace_back(draw);
        //}

        /// <summary>
        /// スレッドを追加
        /// 例：
        /// [](std::stop_token token) {
        ///     while (!token.stop_requested()) {
        ///         //停止要求がくるまで処理を継続する…
        ///     }
        /// }
        /// </summary>
        /// <typeparam name="...Args"></typeparam>
        /// <param name="...args">std::jthreadのコンストラクタの引数</param>
        void emplaceThread(std::function<void(std::stop_token)> argForjthread) {
            threads.emplace_back(argForjthread);
        }

        void addResizeCallbacks(std::function<void(void)> resizeCallback) {
            // 関数は元々ポインタなので参照は不要らしい
            resizeCallbacks.emplace_back(resizeCallback);
        }

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
            // ウィンドウ最小化の場合はwidth, heightともに0
            if (width == 0 && height == 0) {
                // 最小化のときに実行したい場合があるかもしれない
                return;
            }

            auto instance = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

            // チュートリアルでは即座にrecreateSwapChainしていなかったが、
            // 現状即座に実行するように変更した
            // https://docs.vulkan.org/tutorial/latest/_attachments/17_swap_chain_recreation.cpp
            //app->framebufferResized = true;

            for (auto& resizeCallback : instance->resizeCallbacks) {
                resizeCallback();
            }
        }
        
        ~Window() {
            // GLFWの終了
            glfwDestroyWindow(window);
            glfwTerminate();
        }

    private:
        GLFWwindow* window = nullptr;
        WindowSettings settings;
        //std::vector<std::function<void(void)>> drawFunctions;
        std::vector<std::jthread> threads;
        std::vector<std::function<void(void)>> resizeCallbacks;
    };
}
