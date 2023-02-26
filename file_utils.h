#pragma once
#include <string>
#include <fstream>
#include <filesystem>

/*
* Function to load the contents of a file into a string
*/
std::string load_file(const std::filesystem::path &path);