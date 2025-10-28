#include "StarletSerializer/parser/bmpParser.hpp" 
#include "StarletSerializer/data/bmpData.hpp"
#include "StarletSerializer/utils/log.hpp"

int main(int argc, char* argv[]) {
	const std::string assetPath = std::string(ASSET_DIR);
	Starlet::Serializer::BmpParser parser;
	Starlet::Serializer::BmpData data;

	if (!parser.parse(assetPath + "/skull.bmp", data)) {
		Starlet::Serializer::error("Image Tester", "parse", "Failed to parse image");
		return EXIT_FAILURE;
	}

	Starlet::Serializer::debugLog("Image Tester", "main", "Image parsed successfully");
	return EXIT_SUCCESS;
}

