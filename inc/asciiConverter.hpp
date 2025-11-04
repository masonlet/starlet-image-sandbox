#pragma once

namespace Starlet {
	namespace Serializer {
		struct BmpData;
	}

	namespace Sandbox {
		struct Config;

		void displayAsAscii(const Serializer::BmpData& data, const Config& config);
		void displayAsAsciiColour(const Serializer::BmpData& data, const Config& config);
	}
}