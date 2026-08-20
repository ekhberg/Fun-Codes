#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <filesystem>
#include <regex>
#include <set>
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>

const long double M = 0.63011;

double sum(std::vector<double>& v) {
    double f = 0;
    for(double va : v) f += va;
    return f;
}

double mean(std::vector<double>& v) {
    double f = sum(v);
    f = (double) f/v.size();
    return f;
}

double var(std::vector<double>& v) {
    double m = mean(v), f = 0;
    for(double va : v) f += (double) std::pow((va - m), 2);
    f = (double) std::sqrt(f/(v.size()-1));
    return f;
}

double varcoe(std::vector<double>& v) {
    double f = 0;
    f = (double) (var(v)/mean(v))*100;
    return f;
}

/*int main() {
    double t = 0;
    std::vector<double> L {0.199, 0.198, 0.196, 0.195, 0.20};
    std::vector<double> CB {0.124, 0.178, 0.12, 0.104, 0.13};
    std::vector<double> m {0.9604, 0.9938, 1.0299, 0.9542, 0.9154};
    std::vector<double> T1;
    std::vector<double> T2;

    std::cout<<std::setprecision(10)<<"L\tCB\tT"<<std::endl;
    for(int i = 0; i<5; i++) {
        t = (M*G*L[i])/CB[i];
        std::cout<<std::setprecision(10)<<L[i]<<"\t"<<CB[i]<<"\t"<<t<<std::endl;
        T1.push_back(t);
    }
    std::cout<<std::setprecision(10)<<"m\tT"<<std::endl;
    for (int i = 0; i<5; i++) {
        t = m[i]*G;
        std::cout<<std::setprecision(10)<<m[i]<<"\t"<<t<<std::endl;
        T2.push_back(t);
    }
    std::cout<<"T1) Media da tensao da parte 'a' do experimento 2: "<<std::setprecision(10)<<mean(T1)<<"\t| Desvio padrao: "<<var(T1)<<std::endl;
    std::cout<<"T2) Media da tensao da parte 'b' do experimento 2: "<<std::setprecision(10)<<mean(T2)<<"\t| Desvio padrao: "<<var(T2)<<std::endl;
    return 0;
}*/