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

class value {
    private:
    double a = 0.0;
    double delta_a = 0.0;

    public:
    value(double a, double delta_a) {
        this->a = a;
        this->delta_a = delta_a;
    };
    double get_a() {
        return this->a;
    }
    double get_delta_a() {
        return this->delta_a;
    }

    value operator+(const value& v2) {
        double c = this->a + v2.a;
        double delta_c = this->delta_a + v2.delta_a;
        return value(c, delta_c);
    }
    value operator-(const value& v2) {
        double c = this->a - v2.a;
        double delta_c = this->delta_a + v2.delta_a;
        return value(c, delta_c);
    }
    value operator*(const value& v2) { //product by another uncertainty
        double c = (double) this->a * v2.a;
        double delta_c = (double) (this->delta_a*v2.a) + (this->a*v2.delta_a);
        return value(c, delta_c);
    }
    value operator/(const value& v2) {
        double c = (double) this->a / v2.a;
        double delta_c = (double) ((this->delta_a*v2.a) + (this->a*v2.delta_a))/std::pow(v2.a, 2);
        return value(c, delta_c);
    }
    value operator*(const double x) { //product by a const.
        double c = (double) this->a * x;
        double delta_c = (double) this->delta_a * x;
        return value(c, delta_c);
    }
    value operator/(const double x) { //division by a const.
        double c = (double) (this->a)/x;
        double delta_c = (double) (this->delta_a)/x;
        return value(c, delta_c);
    }

    void view() {
        std::cout << std::setprecision(10) << get_a() << " +- " << get_delta_a()<<std::endl;
    }
    ~value() {};
};

value cos(value& B) {
    double a = B.get_a();
    double delta_a = B.get_delta_a();
    double c = (double) std::cos(a);
    double delta_c = (double) std::sin(delta_a);
    return value(c, delta_c);
}

value sin(value& B) {
    double a = B.get_a();
    double delta_a = B.get_delta_a();
    double c = (double) std::sin(a);
    double delta_c = (double) std::cos(delta_a);
    return value(c, delta_c);
}

value mean(std::vector<value> vec) {
    value c(0,0);
    double m = 0, delta_m = 0;
    for(auto& el : vec) {
        m += (double) el.get_a();
        delta_m += (double) el.get_delta_a();
    }
    m = (double) m/vec.size();
    delta_m = (double) delta_m/vec.size();
    c =value(m, delta_m);
    return c;
}