#include "starlet-image-sandbox/argParser.hpp"

#include "starlet-logger/logger.hpp"

#include <iostream>

namespace Starlet::Sandbox {
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
				Logger::error("Image Sandbox", "parseArg", argName + " must be a positive integer");
				return std::nullopt;
			}
			return value;
		}
		catch (const std::exception&) {
			Logger::error("Image Sandbox", "parseArg", "Invalid integer for: " + argName);
			return std::nullopt;
		}
	}

	std::optional<Config> parseArgs(const int argc, char* argv[]) {
		Config config;

		for (int i = 1; i < argc; ++i) {
			const std::string arg{ argv[i] };

			if (arg == "--help" || arg == "-h") {
				displayHelp();
				return std::nullopt;
			}

			else if (arg == "--path" || arg == "-p") {
				if (i + 1 >= argc) {
					Logger::error("Image Sandbox", "parseArgs", "No path specified after " + arg);
					return std::nullopt;
				}

				config.filePath = argv[++i];
			}

			else if (arg == "--scale-x" || arg == "-x") {
				if (i + 1 >= argc) {
					Logger::error("Image Sandbox", "parseArgs", "No scale specified after " + arg);
					return std::nullopt;
				}

				std::optional<int> value = parseIntArg(argv[++i], "scale-x");
				if (!value.has_value()) return std::nullopt;
				config.scaleX = value.value();
			}

			else if (arg == "--scale-y" || arg == "-y") {
				if (i + 1 >= argc) {
					Logger::error("Image Sandbox", "parseArgs", "No scale specified after " + arg);
					return std::nullopt;
				}

				std::optional<int> value = parseIntArg(argv[++i], "scale-y");
				if (!value.has_value()) return std::nullopt;
				config.scaleY = value.value();
			}

			else if (arg == "--gradient" || arg == "-g") {
				if (i + 1 >= argc) {
					Logger::error("Image Sandbox", "parseArgs", "No gradient specified after " + arg);
					return std::nullopt;
				}

				config.gradient = argv[++i];
				if (config.gradient.empty()) {
					Logger::error("Image Sandbox", "parseArgs", "Gradient string cannot be empty");
					return std::nullopt;
				}
			}

			else if (arg == "--mode" || arg == "-m") {
				if(i + 1 >= argc) {
					Logger::error("Image Sandbox", "parseArgs", "No mode specified after " + arg);
					return std::nullopt;
				}

				std::string mode = argv[++i];
				if (mode == "ascii") config.outputMode = OutputMode::Ascii;
				else if (mode == "ascii_colour") config.outputMode = OutputMode::AsciiColour;
				else {
					Logger::error("Image Sandbox", "parseArgs", "Invalid mode: " + mode);
					return std::nullopt;
				}
			}

			else {
				Logger::error("Image Sandbox", "parseArgs", "Unknown argument: " + arg);
				return std::nullopt;
			}
		}

		return config;
	}
}