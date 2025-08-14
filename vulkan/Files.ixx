export module Files;

import std;

namespace Files {
    // C++�̓f�t�H���g�ł͗�O���łȂ����ߐݒ�
    export void initForException(std::ifstream& file) {
        //�G���[�̂Ƃ��͗�O��throw����悤�ɐݒ�
        // https://dormolin.livedoor.blog/archives/50353376.html
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }
    export std::vector<char> readFileToChar(std::string_view filename) {
        // ���̏������Ő��������͕s���A����char�łƂ�Ȃ��̂��H
        // https://docs.vulkan.org/tutorial/latest/_attachments/09_shader_modules.cpp
        std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);

        initForException(file);
        
        
        // ���̏������Ő��������͕s��
        std::vector<char> buffer(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

        // �f�X�g���N�^��close�����͂�
        // file.close();
        return buffer;
    }

    export std::vector<std::string> readAllLines(std::string_view filename) {
        std::vector<std::string> lines;
        std::ifstream file(filename.data());
        initForException(file);
        
        while (!file.eof()) {
            // �����ɒ��ڍ\�z
            // https://zenn.dev/mafafa/articles/370875167e4a3a
            lines.emplace_back();
            std::getline(file, lines.back());
        }
        
        return lines;
    }
}