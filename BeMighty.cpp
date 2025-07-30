// BeMighty.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

import std;
import Glfw;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;


void run() {
    std::cout << "nice module" << std::endl;
}


int main()
{    
    // Glfw:WindowSettingsの変数と、Glfw:Windowの変数を同時に定義するとなぜかエラーになる
    /*
    Glfw:WindowSettings settings;
    std::cout << settings.name << std::endl;
    */

    // ウィンドウの作成と同時に実行。わけた方が良いかは要検討
    Glfw:Window myWindow{
        {
            .width = WIDTH,
            .height = HEIGHT,
            .name = "vulkan"
        },
        run
    };
}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
