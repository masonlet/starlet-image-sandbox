#pragma once

#include "StarletSerializer/data/bmpData.hpp"

#include <string>
#include <optional>

namespace Starlet {
	namespace Sandbox {
		std::optional<Serializer::BmpData> loadImage(const std::string& filePath);
	}
}
