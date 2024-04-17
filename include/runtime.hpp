#pragma once

#include <filesystem>

#include "utils/project.hpp"

struct runtime {
	std::wstring filepath;
	std::filesystem::path working_directory;
	library* external_library; // import_lib

	struct {
		language lang;
	} debug;
};