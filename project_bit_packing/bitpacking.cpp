#include "bitpacking.hpp"
#include <iostream>
#include <cmath>
#include <filesystem>
#include <string>
#include <fstream>
#include <istream>
#include <bitset>
#include <vector>

namespace file = std::filesystem;

std::string mult(const std::string& a, unsigned int n) { //Multiplicador de strings
    std::string res = "";
    for(int i = 0; i < n; i++) {
        res += a;
    }
    return res;
}

unsigned int bit_size(const unsigned int& n) { //Mede o tamanho minimo de bits necessarios para guardar um numero binario
    unsigned int i = n, RES = 0;
    while(i != 0){
        i = (i >> 1); //Reduz o valor binario ate 0 e conta quantas vezes fez a reducao
        RES++;
    }
    return RES;
}

std::string min_bit_string(unsigned short& n) { //Transforma um binario em string com o menor numero de caracteres possivel
    unsigned int k = bit_size(n), p = 0, l = 0, r = 0;
    std::string res = "";
    char aux;
    bool N = 0b0;
    for(int i = 0; i < k; i++) {
        p = std::pow(2, i);
        N = n & p; //Funciona por forma de residuo de mascara de porta AND
        if(N) res += "1";
        else res += "0";
    }

    r = res.length() - 1;
    while (l < r) { //Inverte a saida para ficar no formato padrao
        aux = res[l];
        res[l] = res[r];
        res[r] = aux;
        l++;
        r--;
    }

    return res;
}

unsigned short string_binary_to_short(std::string& n) { //Transforma uma string contendo um binario em um short do binario em forma decimal
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

unsigned short string_binary_to_short_increment(std::string& n,const unsigned short& a) { //Transforma um binario em um valor short com um complemento de potencia para fechar um numero d 8 bits (1 byte)
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

unsigned short binary8t_to_short(const std::bitset<8>& byte) { //Transforma um byte puro em um valor short
    unsigned short res;
    std::bitset<8> mask = 0b1, bye = byte;
    for(int i = 0; i < 8; i++) {
        if((bye & mask) == mask) {
            res += std::pow(2, i);
        }
        mask = mask << 1; //Mascara para avaliar as potencias de 2
    }
    return res;
}


bool numerical(std::ifstream& input) {
    input.clear();
    input.seekg(0);
    char c;
    while (input.get(c)) {
        if (!isdigit(c) && !isspace(c)) {
            return false;
        }
    }
    return true;
}

void encode(const file::path& file_path, const file::path& output_folder, const std::string& custom_name) { //Funcao de compactar
    std::ifstream input(file_path, std::ios::in);
    input.clear();
    input.seekg(0);     //Garantia
    std::string name = custom_name + ".bin";
    file::path out = output_folder / name;
    std::ofstream output(out, std::ios::out |std::ios::trunc | std::ios::binary);
    if(!input) {                                            //Verificacoes
        std::cerr<<"Invalid input!"<<std::endl;
    }
    else if(!input.is_open()) {
        std::cerr<<"File couldn't be open!"<<std::endl;     //Verificacoes
    }
    else if (!file::is_regular_file(file_path)) {
        std::cerr<<"File is not a regular file!"<<std::endl;    //Verificacoes
    }
    else {
        bool dec = numerical(input);
        input.clear();
        input.seekg(0);
        std::string line, code = "", encoded = "", temp1;
        unsigned char a;
        unsigned short temp2, size = 0, test_size = 0;
        signed long long size_temp = 0;
        unsigned long long count = 0;

        if(dec) {
            std::vector<unsigned short> values;
            unsigned short v;
            while(input >> v) values.push_back(v);
            for(unsigned short n : values) {
                temp1 = min_bit_string(n);
                if (temp1.length() > size) size = temp1.length();
            }
            for(unsigned short n : values) {
                temp1 = min_bit_string(n);
                if(temp1.length()<size) {
                    temp1 = mult("0", (size-(temp1.length()))) + temp1; //Padroniza os tamanhos para que os padroes possam ser descompactados de forma mais eficiente
                }
                encoded += temp1;
            }
            a = dec;
            output << a;
            a = size;
            output << a;
            size_temp = encoded.length();
            do {
                temp1 = encoded.substr(count, 8); //Pega de 8-em-8 bits pra compactar
                a = string_binary_to_short_increment(temp1, 8); //Transforma em um caracter de 8 bits 
                output << a;
                size_temp = size_temp - 8;
                count = count + 8;
            }while(size_temp>0);
        }

        else{
            while(getline(input, line)) {
                code += line;
                code += "\n";
            }
            for(int i = 0; i < code.length(); i++) {
                a = code[i];
                temp2 = a;
                temp1 = min_bit_string(temp2);
                if(temp1.length() > size) size = temp1.length(); //Procura maximo de bit necessarios para compactar
            }
            for(int i = 0; i < code.length(); i++) {
                a = code[i];
                temp2 = a;
                temp1 = min_bit_string(temp2);
                if(temp1.length()<size) {
                    temp1 = mult("0", (size-(temp1.length()))) + temp1; //Padroniza os tamanhos para que os padroes possam ser descompactados de forma mais eficiente
                }
                encoded += temp1;
                size_temp = encoded.length();
            }
            a = dec;
            output << a;
            a = size;
            output<<a;

            while(encoded.length() % 8 != 0) encoded += "0";

            do {
                temp1 = encoded.substr(count, 8); //Pega de 8-em-8 bits pra compactar
                a = string_binary_to_short_increment(temp1, 8); //Transforma em um caracter de 8 bits 
                output << a;
                size_temp = size_temp - 8;
                count = count + 8;
            }while(size_temp>0);
        }
        
    }
    input.close();
    output.close();
}

void decode(const file::path& file_path, const file::path& output_folder, const std::string& custom_name) { 
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
        bool dec = 0b0;
        unsigned short count = 0, jump, full_size, temp2, hate_you;
        unsigned char temp3, ah;
        std::string line, coded = "", decoded = "", temp1, hate;
        while(input.read(reinterpret_cast<char*>(&temp3), 1)) { //Leitura de caracteres individualmente
            if(count==0) dec = temp3; //O primeiro define o tamanho
            else if(count == 1) jump = temp3;
            else {
                ah = temp3;
                temp2 = binary8t_to_short(std::bitset<8>(ah)); //Binario para short
                temp1 = min_bit_string(temp2);                 //Short para String
                if(temp1.length()<8) {
                    hate_you = (8-(temp1.length()));           //Padroniza os tamanhos para 8 (bits)
                    hate = mult("0", hate_you);
                    temp1 = hate + temp1;
                }
                coded += temp1;
            }
            count++;
        }
        full_size = coded.length()/jump; //Calcula a quantidade provavel de caracteres na mensagem original
        full_size = std::floor(full_size);
        count = 0;
        if(dec) {
            for(int i = 0; i < full_size; i++) {
                temp1 = coded.substr(count, jump); //Pega os bits de 'jump'-em-'jump'
                temp2 = string_binary_to_short(temp1);  //String para Short
                output << std::to_string(temp2) << " ";
                count += jump;
            }
        }
        else {
            for(int i = 0; i < full_size; i++) {
                temp1 = coded.substr(count, jump); //Pega os bits de 'jump'-em-'jump'
                temp2 = string_binary_to_short(temp1);  //String para Short
                temp3 = static_cast<char>(temp2);       //Short para caracter
                output << temp3;
                count += jump;
            }
        }
    }
    input.close();
    output.close();
}