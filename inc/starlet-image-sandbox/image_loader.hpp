#pragma once

#include "starlet-serializer/data/image_data.hpp"

#include <string>
#include <optional>

namespace Starlet {
	namespace Sandbox {
		std::optional<Serializer::ImageData> loadImage(const std::string& filePath);
	}
}
