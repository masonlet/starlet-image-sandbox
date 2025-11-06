#include "asciiConverter.hpp"
#include "config.hpp"

#include "StarletSerializer/data/imageData.hpp"

#include <iostream>

namespace Starlet::Sandbox {

namespace {
	constexpr const char* ANSI_RESET = "\x1b[0m";

	double calculateBrightness(const uint8_t r, const uint8_t g, const uint8_t b) {
		return (LUMINANCE_R * r + LUMINANCE_G * g + LUMINANCE_B * b) / 255.0;
	}

	char brightnessToChar(const double brightness, const std::string_view& gradient) {
		const int index{ static_cast<int>((1.0 - brightness) * (gradient.length() - 1)) };
		return gradient[index];
	}

	struct PixelData {
		uint8_t r, g, b;
		double brightness;
		char asciiChar;
	};

	PixelData getPixelData(const Starlet::Serializer::ImageData& data,
		const size_t x, const size_t y,
		const std::string_view& gradient) {
		const size_t pixelIndex{ (y * data.width + x) * BYTES_PER_PIXEL };
		const uint8_t r{ data.pixels[pixelIndex + 0] };
		const uint8_t g{ data.pixels[pixelIndex + 1] };
		const uint8_t b{ data.pixels[pixelIndex + 2] };
		const double brightness = calculateBrightness(r, g, b);
		return { r, g, b, brightness, brightnessToChar(brightness, gradient) };
	}

	void appendAnsiColour(std::vector<char>& output, const uint8_t r, const uint8_t g, const uint8_t b) {
		const std::string colorCode = "\x1b[38;2;" + std::to_string(r) + ';'
																							 + std::to_string(g) + ';'
																							 + std::to_string(b) + 'm';
		output.insert(output.end(), colorCode.begin(), colorCode.end());
	}
}

void displayAsAscii(const Starlet::Serializer::ImageData& data, const Config& config) {
	const size_t scaledWidth = static_cast<size_t>(data.width + config.scaleX - 1) / config.scaleX;
	const size_t scaledHeight = static_cast<size_t>(data.height + config.scaleY - 1) / config.scaleY;

	std::vector<char> output;
	output.reserve((scaledWidth + 1) * scaledHeight ); // +1 for newline

	for (size_t y = 0; y < data.height; y += config.scaleY) {
		for (size_t x = 0; x < data.width; x += config.scaleX) {
			const PixelData pixel = getPixelData(data, x, y, config.gradient);
			output.push_back(pixel.asciiChar);
		}

		output.push_back('\n');
	}

	std::cout.write(output.data(), output.size());
}

void displayAsAsciiColour(const Starlet::Serializer::ImageData& data, const Config& config) {
	const size_t scaledWidth = static_cast<size_t>(data.width + config.scaleX - 1) / config.scaleX;
	const size_t scaledHeight = static_cast<size_t>(data.height + config.scaleY - 1) / config.scaleY;

	std::vector<char> output;
	output.reserve((scaledWidth * 24 + 1) * scaledHeight); // 24 for ANSI codes, +1 for newline

	for (size_t y = 0; y < data.height; y += config.scaleY) {
		for (size_t x = 0; x < data.width; x += config.scaleX) {
			const PixelData pixel = getPixelData(data, x, y, config.gradient);
			appendAnsiColour(output, pixel.r, pixel.g, pixel.b);
			output.push_back(pixel.asciiChar);
		}

		output.insert(output.end(), { '\x1b', '[', '0', 'm', '\n' });
	}

	std::cout.write(output.data(), output.size());
}

}