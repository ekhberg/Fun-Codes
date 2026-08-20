#include <iostream>
#include <cmath>
#include <filesystem>
#include <string>
#include <fstream>
#include <istream>
#include <bitset>
#include <vector>

namespace file = std::filesystem;

std::string mult(const std::string& a, unsigned int n) {
    std::string res = "";
    for(int i = 0; i < n; i++) {
        res += a;
    }
    return res;
}

unsigned int bit_size(const unsigned int& n) {
    unsigned int i = n;
    unsigned int RES = 0;
    while(i != 0){
        i = (i >> 1);
        RES++;
    }
    return RES;
}

std::string min_bit_string(unsigned short& n) {
    unsigned int k = bit_size(n), p = 0, l = 0, r = 0;
    std::string res = "";
    char aux;
    bool N = 0b0;
    for(int i = 0; i < k; i++) {
        p = std::pow(2, i);
        N = n & p;
        if(N) res += "1";
        else res += "0";
    }

    r = res.length() - 1;
    while (l < r) {
        aux = res[l];
        res[l] = res[r];
        res[r] = aux;
        l++;
        r--;
    }

    return res;
}

unsigned short string_binary_to_short(std::string& n) {
    unsigned int k = n.length();
    unsigned short res = 0, p = 0;
    for(int i = 0; i<k; i++) {
        if(n.at(i)=='0') {}
        else {
            p = std::pow(2, k-i-1);
            res += p;
        }
    }
    return res;
}

unsigned short string_binary_to_short_increment(std::string& n,const unsigned short& a) {
    unsigned int k = n.length();
    unsigned short res = 0, p = 0;
    for(int i = 0; i<k; i++) {
        if(n.at(i)=='0') {}
        else {
            p = std::pow(2, k-i-1);
            res += p;
        }
    }
    if(k<a) {
        res = res * std::pow(2, a-k);
    }
    return res;
}

unsigned short binary8t_to_short(const std::bitset<8>& byte) {
    unsigned short res;
    std::bitset<8> mask = 0b1, bye = byte;
    for(int i = 0; i < 8; i++) {
        if((bye & mask) == mask) {
            res += std::pow(2, i);
        }
        mask = mask << 1;
    }
    return res;
}

void encode(const file::path& file_path, const file::path& output_folder, const std::string& custom_name) {
    std::ifstream input(file_path, std::ios::in);

    std::string name = custom_name + ".bin";
    file::path out = output_folder / name;
    std::ofstream output(out, std::ios::out |std::ios::trunc | std::ios::binary);
    if(!input) {
        std::cerr<<"Invalid input!"<<std::endl;
    }
    else if(!input.is_open()) {
        std::cerr<<"File couldn't be open!"<<std::endl;
    }
    else if (!file::is_regular_file(file_path)) {
        std::cerr<<"File is not a regular file!"<<std::endl;
    }
    else {
        input.clear();
        input.seekg(0);
        std::string line, code = "", encoded = "", temp1;
        unsigned char a;
        unsigned short temp2, size = 0, test_size = 0, count = 0;
        signed short size_temp;
        while(getline(input, line)) {
            code += line;
        }
        for(int i = 0; i < code.length(); i++) {
            a = code[i];
            temp2 = a;
            temp1 = min_bit_string(temp2);
            if(temp1.length() > size) size = temp1.length();
        }
        for(int i = 0; i < code.length(); i++) {
            a = code[i];
            temp2 = a;
            temp1 = min_bit_string(temp2);
            if(temp1.length()<size) {
                temp1 = mult("0", (size-(temp1.length()))) + temp1;
            }
            encoded += temp1;
        }
        size_temp = encoded.length();

        a = size; //Changed
        output<<a;

        do {
            temp1 = encoded.substr(count, 8);
            a = string_binary_to_short_increment(temp1, 8);
            output << a;
            size_temp = size_temp - 8;
            count = count + 8;
        }while(size_temp>0);
    }
    input.close();
    output.close();
}

void decode(const file::path& file_path, const file::path& output_folder, const std::string& custom_name) { //Change everything :/
    std::ifstream input(file_path, std::ios::in | std::ios::binary);

    std::string name = custom_name + ".txt";
    file::path out = output_folder / name;
    std::ofstream output(out, std::ios::out |std::ios::trunc);
    if(!input) {
        std::cerr<<"Invalid input!"<<std::endl;
    }
    else if(!input.is_open()) {
        std::cerr<<"File couldn't be open!"<<std::endl;
    }
    else if (!file::is_regular_file(file_path)) {
        std::cerr<<"File is not a regular file!"<<std::endl;
    }
    else {
        input.clear();
        input.seekg(0);
        unsigned short count = 0, jump, full_size, temp2, hate_you;
        unsigned char temp3, ah;
        std::string line, coded_1 = "", coded_2= "", decoded = "", temp1, hate;
        while(input.read(reinterpret_cast<char*>(&temp3), 1)) {
            
            if(count==0) {
                jump = temp3;
            }
            else {
                //std::cout << std::setw(2) <<std::bitset<8>(temp3) << ' ';
                ah = temp3;
                temp2 = binary8t_to_short(std::bitset<8>(ah));
                temp1 = min_bit_string(temp2);
                if(temp1.length()<8) {
                    hate_you = (8-(temp1.length()));
                    hate = mult("0", hate_you);
                    temp1 = hate + temp1;
                }
                coded_2 += temp1;
            }
            count++;
        }


        
        full_size = coded_2.length()/jump;
        full_size = std::floor(full_size);
        count = 0;
        for(int i = 0; i < full_size; i++) {
            temp1 = coded_2.substr(count, jump);
            count += jump;
            temp2 = string_binary_to_short(temp1);
            temp3 = static_cast<char>(temp2);
            decoded += temp3;
        }
        output << decoded;
    }
    input.close();
    output.close();
}


int main() {
    file::path cur = file::current_path();
    file::path input1 = cur / "input_file";
    file::path input2 = cur / "output_file";


    std::string cool = "heyy";
    encode(input1 / "test.txt", input2, cool);
    
    cool = "heyya";
    decode(input2 / "heyy.bin", input1, cool);

    return 0;
}