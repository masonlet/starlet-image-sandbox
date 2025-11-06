#pragma once

namespace Starlet {
	namespace Serializer {
		struct ImageData;
	}

	namespace Sandbox {
		struct Config;

		void displayAsAscii(const Serializer::ImageData& data, const Config& config);
		void displayAsAsciiColour(const Serializer::ImageData& data, const Config& config);
	}
}