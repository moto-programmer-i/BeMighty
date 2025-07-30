export module Glfw:Window;
import :WindowSettings;
import std;

#include <GLFW/glfw3.h>

// GLFWをモジュール化したクラス
export class Window {
public:
    /// <summary>
    /// ウィンドウ作成と実行
    /// </summary>
    /// <param name="settings"></param>
    /// <param name="main">作成後の処理</param>
    Window(WindowSettings settings, std::function<void()> main) {
        this->settings = settings;

        // GLFW初期化
        glfwInit();
        // GLFWは元々OpenGL用なので、OpenGLを使わないことを指定する必要がある
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(settings.width, settings.height, settings.name.c_str(), nullptr, nullptr);


        // 処理実行
        main();


        // ウィンドウを閉じるまで待機
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

private:
    GLFWwindow* window = nullptr;
    WindowSettings settings;
};
