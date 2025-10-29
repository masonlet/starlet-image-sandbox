#pragma once

#include <string_view>

namespace Starlet::Sandbox {
	constexpr std::string_view DEFAULT_GRADIENT { "@%#*+=-:. " };
	constexpr std::string_view DEFAULT_PATH { "skull.bmp" };
	constexpr int DEFAULT_SCALE_X { 16 };
	constexpr int DEFAULT_SCALE_Y { 32 };

	constexpr size_t BYTES_PER_PIXEL { 3 };

	// Rec. 709 Luminance coefficients
	constexpr double LUMINANCE_R { 0.2126 };
	constexpr double LUMINANCE_G { 0.7152 };
	constexpr double LUMINANCE_B { 0.0722 };
}