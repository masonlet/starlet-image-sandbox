#pragma once

#include "defaults.hpp"

#include <string>

namespace Starlet::Sandbox {
	enum class OutputMode {
		Ascii,
		AsciiColour
	};

	struct Config {
		std::string filePath  { DEFAULT_PATH };
		std::string gradient  { DEFAULT_GRADIENT };
		OutputMode outputMode { OutputMode::Ascii };
		int scaleX { DEFAULT_SCALE_X };
		int scaleY { DEFAULT_SCALE_Y };
	};
}