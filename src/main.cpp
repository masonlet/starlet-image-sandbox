#include "StarletSerializer/parser/bmpParser.hpp" 
#include "StarletSerializer/data/bmpData.hpp"
#include "StarletSerializer/utils/log.hpp"

#include <iostream>
#include <sstream>

namespace Starlet::Sandbox {
	constexpr std::string_view GRADIENT{ "@%#*+=-:. " };

	constexpr int SCALE_X = 16;
	constexpr int SCALE_Y = 32;

	// Rec. 709 Luminance coefficients
	constexpr double LUMINANCE_R = 0.2126;
	constexpr double LUMINANCE_G = 0.7152;
	constexpr double LUMINANCE_B = 0.0722;

	static double calculateBrightness(const uint8_t r, const uint8_t g, const uint8_t b) {
		return (LUMINANCE_R * r + LUMINANCE_G * g + LUMINANCE_B * b) / 255.0;
	}

	static char brightnessToChar(const double brightness) {
		const int index{ static_cast<int>((1.0 - brightness) * (GRADIENT.length() - 1)) };
		return GRADIENT[index];
	}

	static void displayAsAscii(const Starlet::Serializer::BmpData& data) {
		const size_t scaledWidth = static_cast<size_t>(data.width + SCALE_X - 1) / SCALE_X;
		const size_t scaledHeight = static_cast<size_t>(data.height + SCALE_Y - 1) / SCALE_Y;
		const size_t estimatedSize = (scaledWidth + 1) * scaledHeight; // +1 for newline

		std::vector<char> output;
		output.reserve(estimatedSize);

		for (size_t y = 0; y < data.height; y += SCALE_Y) {
			for (size_t x = 0; x < data.width; x += SCALE_X) {
				if (x >= data.width || y >= data.height) continue;

				const size_t pixelIndex{ (y * data.width + x) * 3 };
				const uint8_t r = data.pixels[pixelIndex + 2];
				const uint8_t g = data.pixels[pixelIndex + 1];
				const uint8_t b = data.pixels[pixelIndex + 0];

				const double brightness = calculateBrightness(r, g, b);
				output.push_back(brightnessToChar(brightness));
			}

			output.push_back('\n');
		}

		std::cout.write(output.data(), output.size());
	}
}

int main(int argc, char* argv[]) {
	const std::string assetPath = std::string(ASSET_DIR);
	Starlet::Serializer::BmpParser parser;
	Starlet::Serializer::BmpData data;

	if (!parser.parse(assetPath + "/skull.bmp", data)) {
		Starlet::Serializer::error("Image Tester", "parse", "Failed to parse image");
		return EXIT_FAILURE;
	}

	Starlet::Sandbox::displayAsAscii(data);
	return EXIT_SUCCESS;
}

