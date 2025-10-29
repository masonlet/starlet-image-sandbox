#pragma once

#include "config.hpp"
#include <optional>

namespace Starlet::Sandbox {
	std::optional<Config> parseArgs(const int argc, char* argv[]);
}
