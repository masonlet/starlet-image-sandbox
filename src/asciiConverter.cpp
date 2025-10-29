#include "asciiConverter.hpp"
#include "config.hpp"

#include "StarletSerializer/data/bmpData.hpp"

#include <iostream>

namespace Starlet::Sandbox {
	static double calculateBrightness(const uint8_t r, const uint8_t g, const uint8_t b) {
		return (LUMINANCE_R * r + LUMINANCE_G * g + LUMINANCE_B * b) / 255.0;
	}

	static char brightnessToChar(const double brightness, const std::string_view& gradient) {
		const int index{ static_cast<int>((1.0 - brightness) * (gradient.length() - 1)) };
		return gradient[index];
	}

	void displayAsAscii(const Starlet::Serializer::BmpData& data, const Config& config) {
		const size_t scaledWidth = static_cast<size_t>(data.width + config.scaleX - 1) / config.scaleX;
		const size_t scaledHeight = static_cast<size_t>(data.height + config.scaleY - 1) / config.scaleY;
		const size_t estimatedSize = (scaledWidth + 1) * scaledHeight; // +1 for newline

		std::vector<char> output;
		output.reserve(estimatedSize);

		for (size_t y = 0; y < data.height; y += config.scaleY) {
			for (size_t x = 0; x < data.width; x += config.scaleX) {
				const size_t pixelIndex{ (y * data.width + x) * BYTES_PER_PIXEL };
				const uint8_t r = data.pixels[pixelIndex + 2];
				const uint8_t g = data.pixels[pixelIndex + 1];
				const uint8_t b = data.pixels[pixelIndex + 0];

				const double brightness = calculateBrightness(r, g, b);
				output.push_back(brightnessToChar(brightness, config.gradient));
			}

			output.push_back('\n');
		}

		std::cout.write(output.data(), output.size());
	}
}