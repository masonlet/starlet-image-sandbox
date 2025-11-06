#include "argParser.hpp"
#include "imageLoader.hpp"
#include "asciiConverter.hpp"
#include "config.hpp"

int main(int argc, char* argv[]) {
	namespace SSandbox = Starlet::Sandbox;

	std::optional<SSandbox::Config> config = SSandbox::parseArgs(argc, argv);
	if (!config.has_value()) {
		return (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))
			? EXIT_SUCCESS : EXIT_FAILURE;
	}

	const std::string fullPath{ std::string(ASSET_DIR) + '/' + config->filePath };

	std::optional<Starlet::Serializer::ImageData> data = SSandbox::loadImage(fullPath);
	if (!data.has_value()) 
		return EXIT_FAILURE;

	switch (config->outputMode) {
	case SSandbox::OutputMode::Ascii:
		SSandbox::displayAsAscii(data.value(), config.value());
		break;
	case SSandbox::OutputMode::AsciiColour:
		SSandbox::displayAsAsciiColour(data.value(), config.value());
		break;
	}

	return EXIT_SUCCESS;
}

