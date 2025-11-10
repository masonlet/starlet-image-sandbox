#include "starlet-image-sandbox/imageLoader.hpp"

#include "starlet-serializer/parser/imageParser.hpp"
#include "starlet-logger/logger.hpp"

namespace Starlet::Sandbox {
	std::optional<Serializer::ImageData> loadImage(const std::string& filePath) {
		Starlet::Serializer::ImageParser parser;
		Starlet::Serializer::ImageData data;

		if (!parser.parse(filePath, data)) {
			Starlet::Logger::error("Image Sandbox", "loadImage", "Failed to parse image: " + filePath);
			return std::nullopt;
		}

		return data;
	}
}
