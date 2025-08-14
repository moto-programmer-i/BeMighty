export module Files;

import std;

namespace Files {
    // C++はデフォルトでは例外がでないため設定
    export void initForException(std::ifstream& file) {
        //エラーのときは例外をthrowするように設定
        // https://dormolin.livedoor.blog/archives/50353376.html
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }
    export std::vector<char> readFileToChar(std::string_view filename) {
        // この書き方で正しいかは不明、直接charでとれないのか？
        // https://docs.vulkan.org/tutorial/latest/_attachments/09_shader_modules.cpp
        std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

        initForException(file);
        
        
        // この書き方で正しいかは不明
        std::vector<char> buffer(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

        // デストラクタでcloseされるはず
        // file.close();
        return buffer;
    }

    export std::vector<std::string> readAllLines(std::string_view filename) {
        std::vector<std::string> lines;
        std::ifstream file(filename.data());
        initForException(file);
        
        while (!file.eof()) {
            // 末尾に直接構築
            // https://zenn.dev/mafafa/articles/370875167e4a3a
            lines.emplace_back();
            std::getline(file, lines.back());
        }
        
        return lines;
    }
}