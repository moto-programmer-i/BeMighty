export module Glfw:Structs;

import std;

namespace Glfw {
	export struct Size {
		int width;
		int height;
	};
	
	export struct Extensions {
		const char ** names;
		const std::uint32_t count;
	};
}
