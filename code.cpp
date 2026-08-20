#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "avgdp.cpp"
#include "uncertainty.cpp"

const long double PI = 3.141592653589793238462643383279502884197169399375105820974944;

std::vector<double> fucku(std::vector<double> vec, double c) {
    double temp, temp2;
    std::vector<double> haha;
    for(int i = 0; i<vec.size(); i++){
        temp = (double) (vec.at(i))/(c);
        haha.push_back(temp);
    }
    return haha;
}

int main() {
    double D = 0.07, temp = 0.0;

    std::vector<double> times {0.026482, 0.023355, 0.025324, 0.024411, 0.023658};
    std::vector<double> velo;
    for(double v: times) {
        temp = (double) D/v;
        velo.push_back(temp);
    }
    std::cout<<std::setprecision(10)<<mean(velo)<<"   "<<var(velo)<<std::endl;
    return 0;
}