#include "imageLoader.hpp"

#include "StarletSerializer/parser/bmpParser.hpp"
#include "StarletSerializer/utils/log.hpp"

namespace Starlet::Sandbox {
	std::optional<Serializer::BmpData> loadImage(const std::string& filePath) {
		Starlet::Serializer::BmpParser parser;
		Starlet::Serializer::BmpData data;

		if (!parser.parse(filePath, data)) {
			Starlet::Serializer::error("Image Sandbox", "loadImage", "Failed to parse image: " + filePath);
			return std::nullopt;
		}

		return data;
	}
}
