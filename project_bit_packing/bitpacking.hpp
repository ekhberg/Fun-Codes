#include <iostream>
#include <cmath>
#include <filesystem>
#include <string>
#include <fstream>
#include <istream>
#include <bitset>
#include <vector>

#ifndef BITPACKING_HPP

#define BITPACKING_HPP

std::string mult(const std::string& a, unsigned int n);

unsigned int bit_size(const unsigned int& n);

std::string min_bit_string(unsigned short& n);

unsigned short string_binary_to_short(std::string& n);

unsigned short string_binary_to_short_increment(std::string& n,const unsigned short& a);

unsigned short binary8t_to_short(const std::bitset<8>& byte);

bool numerical(std::ifstream& input);

void encode(const std::filesystem::path& file_path, const std::filesystem::path& output_folder, const std::string& custom_name);

void decode(const std::filesystem::path& file_path, const std::filesystem::path& output_folder, const std::string& custom_name);

#endif