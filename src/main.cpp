#include "StarletSerializer/parser/bmpParser.hpp" 
#include "StarletSerializer/data/bmpData.hpp"
#include "StarletSerializer/utils/log.hpp"

#include <iostream>
#include <sstream>

constexpr std::string_view GRADIENT{ "@%#*+=-:. " };
constexpr int SCALE_X = 2;
constexpr int SCALE_Y = 4;

static void displayAsAscii(const Starlet::Serializer::BmpData& data) {
	std::vector<char> output;
	output.reserve(static_cast<size_t>(data.width * data.height * 3));

	for (size_t y = 0; y < data.height; y += SCALE_Y) {
		for (size_t x = 0; x < data.width; x += SCALE_X) {
			if(x >= data.width || y >= data.height) continue;

			const size_t pixelIndex{ (y * data.width + x) * 3 };
			const uint8_t r = data.pixels[pixelIndex + 2];
			const uint8_t g = data.pixels[pixelIndex + 1];
			const uint8_t b = data.pixels[pixelIndex + 0];

			// Relative Luminance Formula
			const double brightness = (0.2126 * r + 0.7152 * g + 0.0722 * b) / 255.0;
			const int index{ static_cast<int>((1.0 - brightness) * (GRADIENT.length() - 1)) };

			output.push_back(GRADIENT[index]);
		}

		output.push_back('\n');
	}

	std::cout.write(output.data(), output.size());
}

int main(int argc, char* argv[]) {
	const std::string assetPath = std::string(ASSET_DIR);
	Starlet::Serializer::BmpParser parser;
	Starlet::Serializer::BmpData data;

	if (!parser.parse(assetPath + "/skull.bmp", data)) {
		Starlet::Serializer::error("Image Tester", "parse", "Failed to parse image");
		return EXIT_FAILURE;
	}

	displayAsAscii(data);
	return EXIT_SUCCESS;
}

