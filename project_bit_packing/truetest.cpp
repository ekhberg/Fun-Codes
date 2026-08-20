#include <iostream>
#include <cmath>
#include <filesystem>
#include <string>
#include <fstream>
#include <istream>
#include <bitset>
#include <vector>
#include "bitpacking.hpp"

namespace file = std::filesystem;

int main() {
    file::path cur = file::current_path();
    file::path input1 = cur / "input_file";
    file::path input2 = cur / "output_file";


    std::string cool = "heyy";
    encode(input1 / "Dados_B.txt", input2, cool);

    cool = "Dados_B_decoded";
    decode(input2 / "heyy.bin", input1, cool);



    return 0;
}