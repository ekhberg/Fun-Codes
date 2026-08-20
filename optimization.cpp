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

const int POP = 1000;
const int GEN = 200;
const int EL = 30;
const double M_RATE = 0.1;
const double M_STRENGTH = 0.005;
const double DIST = 0.02;

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<> uniform_dist(0.0, 1.0);
std::normal_distribution<> normal_dist(0.0, 0.5);

std::vector<double> justaguy() {
    std::vector<double> x(6);
    for(double& s : x) {
        s = (double) std::abs(normal_dist(rng)); //Try changing to uniform_dist to observe convergence rates later
    }
    return x;
}

std::vector<double> sex(const std::vector<double>& mommy, const std::vector<double>& daddy) {
    std::vector<double> child;
    double a = uniform_dist(rng);
    double v = 0;
    for(int i = 0; i < 6; i++) {
        v = (double) a*(mommy.at(i)) + (1-a)*(daddy.at(i));
        child.push_back(v);
    }
    return child;
}

double dist(const std::vector<double>& mommy, const std::vector<double>& daddy) {
    double d1 = 0;
    for(int i = 0; i < 6; i++) {
        d1 += (double) pow((mommy.at(i) - daddy.at(i)), 2);
    }
    d1 = (double) std::sqrt(d1);
    return d1;
}

void mutate(std::vector<double>& weirdo) {
    if (uniform_dist(rng) < M_RATE) {
        for(double& s : weirdo) {
            s += M_STRENGTH * normal_dist(rng);
        }
    }
}

void toline(std::vector<double>& guy) {
    double h1 = guy.at(0);
    double l1 = guy.at(1);
    double h2 = guy.at(2);
    double l2 = guy.at(3);
    double h3 = guy.at(4);
    double l3 = guy.at(5);
    double lambda = 0, v = 0, t1 = 0, t2 = 0, t3 = 0,
    ft = 0, A = 0, Iz = 0, Iy = 0, ycg = 0;
    double Mz = -60.0;
    double Vy = 30.0;

    A = (double) h1*l1 + h2*l2 + h3*l3;
    ycg = (double) (h1*l1*(h3+h2+(h1/2)) + h2*l2*(h3+(h2/2)) + h3*l3*(h3/2))/(A);
    Iz = (double) (l1*pow(h1, 3)/12.0) + l1*h1*pow((h3+h2+(h1/2)-ycg), 2) + (l2*pow(h2, 3)/12.0) + l2*h2*pow((h3+(h2/2)-ycg), 2) + (l3*pow(h3, 3)/12.0) + l3*h3*pow(((h3/2)-ycg), 2);
    Iy = (double) (h1*pow(l1, 3)/12.0) + (h2*pow(l2, 3)/12.0) + (h3*pow(l3, 3)/12.0);
    t1 = (double) -(Mz*(h1+h2+h3-ycg))/(Iz);
    t2 = (double) (Mz*ycg)/(Iz);
    t3 = (double) (Vy/(Iz*l2))*(l3*h3*(ycg-(h3/2)) + (l2*pow((ycg-h3), 2))/2.0);
    t1 = (double) t1 / 5000.0;
    t2 = (double) t2 / 45000.0;
    t3 = (double) t3 / 8000.0;

    std::cout << "h1: " << h1 << "\tl1: " << l1 
    << "\nh2: " << h2 << "\tl2: " << l2 <<
    "\nh3: " << h3 << "\tl3: " << l3 << "\n\nA: " << A <<
    "\nt1: " << t1 << "\nt2: " << t2 << "\nt3: " << t3 <<std::endl;
}

double score(std::vector<double>& guy, int gen) {
    //Get dimentions info
    double h1 = (double) std::abs(guy.at(0));
    double l1 = (double) std::abs(guy.at(1));
    double h2 = (double) std::abs(guy.at(2));
    double l2 = (double) std::abs(guy.at(3));
    double h3 = (double) std::abs(guy.at(4));
    double l3 = (double) std::abs(guy.at(5));

    //Initializations
    double lambda = 0, v = 0, t1 = 0, t2 = 0, t3 = 0,
    ft = 0, A = 0, Iz = 0, Iy = 0, ycg = 0, x = 0, y = 0, z = 0;
    double Mz = -60.0;
    double Vy = 30.0;

    x = 0.6;
    y = 1.0;
    z = 0.8;
    lambda = (double) gen*(7/20) + 10.0;
    A = (double) h1*l1 + h2*l2 + h3*l3;
    ycg = (double) (h1*l1*(h3+h2+(h1/2)) + h2*l2*(h3+(h2/2)) + h3*l3*(h3/2))/(A);
    Iz = (double) (l1*pow(h1, 3)/12.0) + l1*h1*pow((h3+h2+(h1/2)-ycg), 2) + (l2*pow(h2, 3)/12.0) + l2*h2*pow((h3+(h2/2)-ycg), 2) + (l3*pow(h3, 3)/12.0) + l3*h3*pow(((h3/2)-ycg), 2);
    Iy = (double) (h1*pow(l1, 3)/12.0) + (h2*pow(l2, 3)/12.0) + (h3*pow(l3, 3)/12.0);
    t1 = (double) -(Mz*(h1+h2+h3-ycg))/(Iz);
    t2 = (double) (Mz*ycg)/(Iz);
    t3 = (double) (Vy/(Iz*l2))*(l3*h3*(ycg-(h3/2)) + (l2*pow((ycg-h3), 2))/2.0);
    t1 = (double) std::abs(t1 / 5000.0);
    t2 = (double) std::abs(t2 / 45000.0);
    t3 = (double) std::abs(t3 / 8000.0);
    ft = (double) std::abs(x*(pow(t1 - 1, 2)) + y*(pow(t2 - 1, 2)) + z*(pow(t3 - 1, 2)));// + std::abs((t1 - 1)*(t2 - 1)*(t3 - 1))
    if(ft>0.01) {
        v = (double) (lambda)*pow((1/ft), 2);
    }
    else {
        v = (double) 1/A + (lambda)*pow((1/ft), 2);
    }
    return v;
}

int main() {
    //Initializations
    std::vector<std::pair<double, std::vector<double>>> checked;
    std::vector<double> bestguy;
    double bestscore = 0, comp = 0;
    int a1 = 0, a2 = 0;
    
    //Initial population
    std::vector<std::vector<double>> city;
    for(int i = 0; i < POP; i++) {
        city.push_back(justaguy());
    }

    //Main Cycle
    for (int gen = 0; gen <= GEN; gen++) {
        checked.clear();
        for(auto& guy : city) {
            checked.push_back({score(guy, gen), guy});
        }
        std::sort(checked.begin(), checked.end());
        std::cout << "\nGeneration: " << gen << "\t| Best score: " << checked[POP-1].first << "\n";
        toline(checked[POP-1].second);
        std::vector<std::vector<double>> new_city;
        new_city.clear();
        for (int i = 0; i < EL; ++i) new_city.push_back(checked[POP-i-1].second);
        while((int)new_city.size() < POP) {
            a1 = std::uniform_int_distribution<>(POP-50, POP-1)(rng);
            a2 = std::uniform_int_distribution<>(POP-50, POP-1)(rng);
            const std::vector<double>& p1 = checked[a1].second;
            const std::vector<double>& p2 = checked[a2].second;
            if(dist(p1, p2) < DIST) {
                std::vector<double> baby = sex(p1, p2);
                mutate(baby);
                new_city.push_back(baby);
            }
        }
        city.clear();
        for(const auto& fucker : new_city) {
            city.push_back(fucker);
        }
    }

    //Final search
    for(auto& guy : city) {
        comp = score(guy, GEN);
        if(comp > bestscore) {
            bestscore = comp;
            bestguy = guy;
        }
    }
    //User output
    std::cout << "\n\nWINNER!!\n";
    toline(bestguy);

    std::cout << "\nFinal Score: "<< std::setprecision(15) << bestscore << std::endl;
    return 0;
}