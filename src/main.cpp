#include "StarletSerializer/parser/bmpParser.hpp" 
#include "StarletSerializer/data/bmpData.hpp"
#include "StarletSerializer/utils/log.hpp"

#include <iostream>

namespace Starlet::Sandbox {
	constexpr std::string_view DEFAULT_GRADIENT { "@%#*+=-:. " };
	constexpr std::string_view DEFAULT_PATH { "skull.bmp" };
	constexpr int DEFAULT_SCALE_X { 16 };
	constexpr int DEFAULT_SCALE_Y { 32 };

	// Rec. 709 Luminance coefficients
	constexpr double LUMINANCE_R { 0.2126 };
	constexpr double LUMINANCE_G { 0.7152 };
	constexpr double LUMINANCE_B { 0.0722 };

	static double calculateBrightness(const uint8_t r, const uint8_t g, const uint8_t b) {
		return (LUMINANCE_R * r + LUMINANCE_G * g + LUMINANCE_B * b) / 255.0;
	}

	static char brightnessToChar(const double brightness, const std::string_view& gradient) {
		const int index{ static_cast<int>((1.0 - brightness) * (gradient.length() - 1)) };
		return gradient[index];
	}

	static void displayAsAscii(const Starlet::Serializer::BmpData& data, 
														 const int scaleX, const int scaleY, 
														 const std::string_view gradient) {
		const size_t scaledWidth = static_cast<size_t>(data.width + scaleX - 1) / scaleX;
		const size_t scaledHeight = static_cast<size_t>(data.height + scaleY - 1) / scaleY;
		const size_t estimatedSize = (scaledWidth + 1) * scaledHeight; // +1 for newline

		std::vector<char> output;
		output.reserve(estimatedSize);

		for (size_t y = 0; y < data.height; y += scaleY) {
			for (size_t x = 0; x < data.width; x += scaleX) {
				if (x >= data.width || y >= data.height) continue;

				const size_t pixelIndex{ (y * data.width + x) * 3 };
				const uint8_t r = data.pixels[pixelIndex + 2];
				const uint8_t g = data.pixels[pixelIndex + 1];
				const uint8_t b = data.pixels[pixelIndex + 0];

				const double brightness = calculateBrightness(r, g, b);
				output.push_back(brightnessToChar(brightness, gradient));
			}

			output.push_back('\n');
		}

		std::cout.write(output.data(), output.size());
	}
}

int main(int argc, char* argv[]) {
	namespace SSandbox = Starlet::Sandbox;
	namespace SSerializer = Starlet::Serializer;

	const std::string assetPath { std::string(ASSET_DIR) + '/' };
	std::string filePath { SSandbox::DEFAULT_PATH };
	int scaleX { SSandbox::DEFAULT_SCALE_X };
	int scaleY { SSandbox::DEFAULT_SCALE_Y };
	std::string gradient { SSandbox::DEFAULT_GRADIENT };

	for (int i = 1; i < argc; ++i) {
		const std::string arg{ argv[i] };

		if (arg == "--help" || arg == "-h") {
			std::cout << "Usage: ImageTester [options]\n"
								<< "Options:\n"
								<< "  --help, -h           Show help message\n"
								<< "  --path, -p <path>    Specify the image file path           (default: skull.bmp)\n"
								<< "  --scale-x, -x <int>  Specify the horizontal scaling factor (default: 16)\n"
								<< "  --scale-y, -y <int>  Specify the vertical scaling factor   (default: 32)\n"
								<< "  --gradient, -g <str> Specify the ASCII gradient string     (default: '@%#*+=-:. ')\n";

			return EXIT_SUCCESS;
		}

		else if (arg == "--path" || arg == "-p") {
			if(i + 1 >= argc) {
				SSerializer::error("Image Tester", "main", "No path specified after " + arg);
				return EXIT_FAILURE;
			}

			filePath = argv[++i];
		}

		else if (arg == "--scale-x" || arg == "-x") {
			if(i + 1 >= argc) {
				SSerializer::error("Image Tester", "main", "No scale specified after " + arg);
				return EXIT_FAILURE;
			}

			scaleX = std::stoi(argv[++i]);
			if(scaleX <= 0) {
				SSerializer::error("Image Tester", "main", "Scale X must be a positive integer");
				return EXIT_FAILURE;
			}
		}

		else if (arg == "--scale-y" || arg == "-y") {
			if(i + 1 >= argc) {
				SSerializer::error("Image Tester", "main", "No scale specified after " + arg);
				return EXIT_FAILURE;
			}

			scaleY = std::stoi(argv[++i]);
			if(scaleY <= 0) {
				SSerializer::error("Image Tester", "main", "Scale Y must be a positive integer");
				return EXIT_FAILURE;
			}
		}

		else if (arg == "--gradient" || arg == "-g") {
			if(i + 1 >= argc) {
				SSerializer::error("Image Tester", "main", "No gradient specified after " + arg);
				return EXIT_FAILURE;
			}

			gradient = argv[++i];
			if(gradient.empty()) {
				SSerializer::error("Image Tester", "main", "Gradient string cannot be empty");
				return EXIT_FAILURE;
			}
		}
	}

	SSerializer::BmpParser parser;
	SSerializer::BmpData data;

	if (!parser.parse(assetPath + filePath, data)) {
		SSerializer::error("Image Tester", "main", "Failed to parse image");
		return EXIT_FAILURE;
	}

	SSandbox::displayAsAscii(data, scaleX, scaleY, gradient);
	return EXIT_SUCCESS;
}

