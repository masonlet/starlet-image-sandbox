#pragma once

#include "defaults.hpp"

#include <string>

namespace Starlet::Sandbox {
	struct Config {
		std::string filePath{ DEFAULT_PATH };
		int scaleX{ DEFAULT_SCALE_X };
		int scaleY{ DEFAULT_SCALE_Y };
		std::string gradient{ DEFAULT_GRADIENT };
	};
}