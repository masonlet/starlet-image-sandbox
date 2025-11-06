#pragma once

#include "StarletSerializer/data/imageData.hpp"

#include <string>
#include <optional>

namespace Starlet {
	namespace Sandbox {
		std::optional<Serializer::ImageData> loadImage(const std::string& filePath);
	}
}
