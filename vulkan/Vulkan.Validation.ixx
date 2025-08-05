#include <vulkan/vk_platform.h>

export module Vulkan:Validation;

import std;
import vulkan_hpp;

namespace Vulkan {
    // ���C���[���Ȃ���������ŕ�����w�肵�Ȃ���΂����Ȃ�
    export const std::vector VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

    export void addDebugExtensions(std::vector<const char*>& extensions) {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    // �`���[�g���A������쐬
    // https://docs.vulkan.org/tutorial/latest/_attachments/06_swap_chain_creation.cpp


    export class DebugManager {
    public:
        DebugManager(vk::raii::Instance& instance) {
            vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
            vk::DebugUtilsMessageTypeFlagsEXT    messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
            vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
                .messageSeverity = severityFlags,
                .messageType = messageTypeFlags,

                // �f�o�b�O�̃R�[���o�b�N�͎b��A��������`���[�g���A���ɍ��킹��
                .pfnUserCallback = &debugCallback
            };
            debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);

        }

        // �{���͂ǂ����ׂ����s��
        static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type, const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData, void*) {
            if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning) {
                std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;
            }

            return vk::False;
        }

    private:
        // raii�ɂ���ĉ�������
        vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;
    };
}


