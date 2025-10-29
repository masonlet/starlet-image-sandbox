#include "StarletSerializer/parser/bmpParser.hpp" 
#include "StarletSerializer/data/bmpData.hpp"
#include "StarletSerializer/utils/log.hpp"

#include <iostream>
#include <optional>

namespace Starlet::Sandbox {
	constexpr std::string_view DEFAULT_GRADIENT{ "@%#*+=-:. " };
	constexpr std::string_view DEFAULT_PATH{ "skull.bmp" };
	constexpr int DEFAULT_SCALE_X{ 16 };
	constexpr int DEFAULT_SCALE_Y{ 32 };

	constexpr size_t BYTES_PER_PIXEL{ 3 };

	// Rec. 709 Luminance coefficients
	constexpr double LUMINANCE_R{ 0.2126 };
	constexpr double LUMINANCE_G{ 0.7152 };
	constexpr double LUMINANCE_B{ 0.0722 };

	struct Config {
		std::string filePath{ DEFAULT_PATH };
		int scaleX{ DEFAULT_SCALE_X };
		int scaleY{ DEFAULT_SCALE_Y };
		std::string gradient{ DEFAULT_GRADIENT };
	};

	static double calculateBrightness(const uint8_t r, const uint8_t g, const uint8_t b) {
		return (LUMINANCE_R * r + LUMINANCE_G * g + LUMINANCE_B * b) / 255.0;
	}

	static char brightnessToChar(const double brightness, const std::string_view& gradient) {
		const int index{ static_cast<int>((1.0 - brightness) * (gradient.length() - 1)) };
		return gradient[index];
	}

	static void displayAsAscii(const Starlet::Serializer::BmpData& data, const Config& config) {
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

	static void displayHelp() {
		std::cout << "Usage: Image Sandbox [options]\n"
			<< "Options:\n"
			<< "  --help, -h           Show help message\n"
			<< "  --path, -p <path>    Specify the image file path           (default: skull.bmp)\n"
			<< "  --scale-x, -x <int>  Specify the horizontal scaling factor (default: 16)\n"
			<< "  --scale-y, -y <int>  Specify the vertical scaling factor   (default: 32)\n"
			<< "  --gradient, -g <str> Specify the ASCII gradient string     (default: '@%#*+=-:. ')\n";
	}

	static std::optional<int> parseIntArg(const char* arg, const std::string& argName) {
		try {
			int value = std::stoi(arg);

			if (value <= 0) {
				Serializer::error("Image Sandbox", "parseArg", argName + " must be a positive integer");
				return std::nullopt;
			} 
			return value;
		} catch (const std::exception&) {
			Serializer::error("Image Sandbox", "parseArg", "Invalid integer for: " + argName);
			return std::nullopt;
		}
	}

	static std::optional<Config> parseArgs(const int argc, char* argv[]) {
		Config config;

		for (int i = 1; i < argc; ++i) {
			const std::string arg{ argv[i] };

			if (arg == "--help" || arg == "-h") {
				displayHelp();
				return std::nullopt;
			}

			else if (arg == "--path" || arg == "-p") {
				if (i + 1 >= argc) {
					Serializer::error("Image Sandbox", "parseArgs", "No path specified after " + arg);
					return std::nullopt;
				}

				config.filePath = argv[++i];
			}

			else if (arg == "--scale-x" || arg == "-x") {
				if (i + 1 >= argc) {
					Serializer::error("Image Sandbox", "parseArgs", "No scale specified after " + arg);
					return std::nullopt;
				}

				std::optional<int> value = parseIntArg(argv[++i], "scale-x");
				if (!value.has_value()) return std::nullopt;
				config.scaleX = value.value();
			}

			else if (arg == "--scale-y" || arg == "-y") {
				if (i + 1 >= argc) {
					Serializer::error("Image Sandbox", "parseArgs", "No scale specified after " + arg);
					return std::nullopt;
				}

				std::optional<int> value = parseIntArg(argv[++i], "scale-y");
				if (!value.has_value()) return std::nullopt;
				config.scaleY = value.value();
			}

			else if (arg == "--gradient" || arg == "-g") {
				if (i + 1 >= argc) {
					Serializer::error("Image Sandbox", "parseArgs", "No gradient specified after " + arg);
					return std::nullopt;
				}

				config.gradient = argv[++i];
				if (config.gradient.empty()) {
					Serializer::error("Image Sandbox", "parseArgs", "Gradient string cannot be empty");
					return std::nullopt;
				}
			}
		}

		return config;
	}

	static std::optional<Serializer::BmpData> loadImage(const std::string& filePath) {
		Starlet::Serializer::BmpParser parser;
		Starlet::Serializer::BmpData data;

		if (!parser.parse(filePath, data)) {
			Starlet::Serializer::error("Image Sandbox", "loadImage", "Failed to parse image: " + filePath);
			return std::nullopt;
		}

		return data;
	}
}

int main(int argc, char* argv[]) {
	namespace SSandbox = Starlet::Sandbox;

	std::optional<SSandbox::Config> config = SSandbox::parseArgs(argc, argv);
	if (!config.has_value()) {
		return (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))
			? EXIT_SUCCESS : EXIT_FAILURE;
	}

	const std::string fullPath{ std::string(ASSET_DIR) + '/' + config->filePath };

	std::optional<Starlet::Serializer::BmpData> data = SSandbox::loadImage(fullPath);
	if (!data.has_value()) 
		return EXIT_FAILURE;

	SSandbox::displayAsAscii(data.value(), config.value());
	return EXIT_SUCCESS;
}

