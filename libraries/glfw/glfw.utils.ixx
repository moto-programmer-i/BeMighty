export module Glfw:Utils;

import std;
import :Structs;

#include <GLFW/glfw3.h>

namespace Glfw {	
	export Extensions getRequiredInstanceExtensions() {
		std::uint32_t count;
		// https://www.glfw.org/docs/3.3/group__vulkan.html#ga99ad342d82f4a3421e2864978cb6d1d6
        auto names = glfwGetRequiredInstanceExtensions(&count);
		return {names, count};
	}
}
