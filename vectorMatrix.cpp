#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <filesystem>
#include <regex>
#include <set>
#include <cmath>

namespace fs = std::filesystem;

const long double PI = 3.141592653589793238462643383279502884197169399375105820974944;

class atomVector {
    private:
    std::string name = "";
    double xcoord = 0.0;
    double ycoord = 0.0;
    double zcoord = 0.0;

    public:
    atomVector(std::string name, double x, double y, double z) {
        this->name = name;
        this->xcoord = x;
        this->ycoord = y;
        this->zcoord = z;
    }
    std::vector<double> getXYZ() {
        std::vector<double> car;
        car.push_back(this->xcoord);
        car.push_back(this->ycoord);
        car.push_back(this->zcoord);
        return car;
    }
    std::string getName() {
        return this->name;
    }
    atomVector operator+(const atomVector& atom2) const {
        double x, y, z;
        std::string name;
        name = atom2.name + " + " + this->name;
        x = this->xcoord + atom2.xcoord;
        y = this->ycoord + atom2.ycoord;
        z = this->zcoord + atom2.zcoord;
        return atomVector(name, x, y, z);
    }
    atomVector operator-(const atomVector& atom2) const {
        double x, y, z;
        std::string name;
        name = atom2.name + " -> " + this->name;
        x = this->xcoord - atom2.xcoord;
        y = this->ycoord - atom2.ycoord;
        z = this->zcoord - atom2.zcoord;
        return atomVector(name, x, y, z);
    }
    std::string toString() {
        std::string det = "";
        std::vector<double> coord = this->getXYZ();
        det = "Name: " + this->getName() + "\nX: " + std::to_string(coord.at(0)) + "\tY: " + std::to_string(coord.at(1)) + "\tZ: " + std::to_string(coord.at(2));
        return det;
    }
    double operator*(const atomVector& atom2) const {
        double x, y, z, f = 0;
        std::string name;
        name = atom2.name + " . " + this->name;
        x = (double) this->xcoord * atom2.xcoord;
        y = (double) this->ycoord * atom2.ycoord;
        z = (double) this->zcoord * atom2.zcoord;
        f = (x + y) + z;
        return f;
    }
    ~atomVector() {};
};

double angleVector(atomVector a, atomVector b) {
    double f = 0;
    f = std::acos((a*b)/(sqrt(a*a)*sqrt(b*b)));
    return f;

}

double triangleAngle(atomVector a, atomVector b, atomVector c) {
    atomVector I = (a-b);
    atomVector J = (c-b);
    return angleVector(I, J);
}

std::vector<std::string> splitString(std::string &input, std::string ws) {
    std::vector<std::string> words;
    std::regex w (ws);
    const int fu = -1;
    std::regex_token_iterator<std::string::iterator> it(input.begin(), input.end(), w, fu);
    std::regex_token_iterator<std::string::iterator> end;
    while (it!=end) {
        if(!it->str().empty()){
            words.push_back(*it);
        }
        it++;
    }
    return words;
}

int findLast(std::string ptrn, std::istream& input){
    input.clear();
    input.seekg(0);
    if (!input) {
        std::cerr << "Invalid input" << std::endl;
        return -1;
    }
    std::size_t fnd = 0;
    int lc = 1, re = -1;
    std::string line;
    for(std::string line; std::getline(input, line); ) {
        fnd = line.find(ptrn);
        if(fnd!=std::string::npos) {   
            re=lc;
        }
        lc++;
    }
    return re;
}

std::string cpLine(std::istream& input, int &line) {
    int ln = 1;
    input.clear();
    input.seekg(0);
    std::string lines = "";
    for(std::string lines; std::getline(input, lines); ) {
        if(ln==line){
            return lines;
            break;
        }
        ln++;
    }
    return "Not found";
}

int main() {
    int n = 0;                                        //ALWAYS INFORM n AS THE PROGRAM CANNOT DO IT AUTOMATICALLY
    const double limit = 2.0;                               //SUBJECT TO CHANGE
    std::string main_name = "acetylcholine";
    std::string input = ".out", pattern = "Final atomic structure:", name = "", name2 = ".xyz";
    fs::path main = "aim", file, vesta_file, vesta = "aim2";
    input = main_name + input;
    name2 = main_name + name2;
    file = main / input;
    vesta_file = main / vesta / name2;
    int lc = 1, ln = 0;
    double dMatrix[n][n];
    double x = 0, y = 0, z = 0, ip = 0, ei = 0, Se = 0, ef = 0;
    std::vector<atomVector> aMatrix;
    std::vector<std::string> words;
    std::ifstream aimin(file, std::ios::in);
    std::ofstream vestas(vesta_file, std::ios::out | std::ios::trunc);
    ln = findLast(pattern, aimin);
    ln = ln + 2;
    aimin.clear();
    aimin.seekg(0);
    for(std::string line; std::getline(aimin, line); ) {
            if(lc>=ln) {
                if(line=="------------------------------------------------------------") {
                    break; 
                }
                else {
                    n++;
                }
            }
            
            lc++;
        }
    lc = 1;
    aimin.clear();
    aimin.seekg(0);
    vestas<<n<<"\n\n";
    for(std::string line; std::getline(aimin, line); ) {
        if(lc>=ln && lc<=(ln+n-1)) {
            words = splitString(line, "\\s+");
            x = stod(words.at(1));
            y = stod(words.at(2));
            z = stod(words.at(3));
            name = words.at(4);
            vestas << words.at(4) << " " << words.at(1) + "00000" << " " << words.at(2) + "00000" << " " << words.at(3) + "00000" << "\n"; 
            aMatrix.push_back(atomVector(name, x, y, z));
        }
        lc++;
    }
    aimin.clear();
    aimin.seekg(0);
    words.clear();
    pattern = "| Total energy of the DFT / Hartree-Fock s.c.f. calculation";
    ln = findLast(pattern, aimin);
    name = cpLine(aimin, ln);
    words = splitString(name, "\\s+:\\s+");
    ei = stod(words.at(1));
    ei = abs(ei);
    for(atomVector& atom: aMatrix) {
        name = atom.getName();
        if(name=="Br") Se += 71401.416024606;
        else if(name=="H") Se += 13.602610804;
        else if(name=="C") Se += 1025.095015244;
        else if(name=="N") Se += 1485.31304101;
        else if(name=="O") Se += 2043.221363519;
        else if(name=="P") Se += 9317.758602055;
        else if(name=="K") Se += 16417.828058414;
        else if(name=="I") Se += 196590.981926065;
        else if(name=="S") Se += 10875.868257939;
        else if(name=="Cl") Se += 12577.705581371;
        else if(name=="Fe") Se += 34762.097596533;
        else if(name=="Na") Se += 4422.011462878;
    }
    ef = (double) (ei-Se)/n;
    aimin.close();
    for(int i = 0; i<n; i++) {
        for(int j = 0; j<n; j++) {
            if(i==j) dMatrix[i][j]=0.0;
            else if (i<j) {
                atomVector I = aMatrix.at(i);
                atomVector J = aMatrix.at(j);
                atomVector temp = (I-J);
                ip = sqrt(temp*temp);
                dMatrix[i][j]=ip;
                I.~atomVector();
                J.~atomVector();
            }
        }
    }
    for(int i = 0; i<n; i++) {
        for(int j = 0; j<n; j++) {
            if(i<j) dMatrix[j][i] = dMatrix[i][j];
        }
    }




    //Display
    std::cout<<"\nBond energy (in eV): "<<std::setprecision(15)<<std::to_string(ef)<<std::endl;

    std::cout<<"\t";
    for(int i = 0; i < n; i++) {
        atomVector K = aMatrix[i];
        std::cout<<K.getName()<<"["<<i<<"]"<<"\t\t";
    }
    std::cout<<std::endl;
    for(int i = 0; i<n; i++) {
        atomVector K = aMatrix[i];
        std::cout<<K.getName()<<"["<<i<<"]"<<"\t";
        for(int j = 0; j<n; j++) {
            std::cout<<std::to_string(dMatrix[i][j])<<"\t";
        }
        std::cout<<std::endl;
    }
    int a = 0, b = 2, c = 1;
    if(dMatrix[b][a] > limit || dMatrix[b][c] > limit) {
        std::cout<<"Higher than the limit; likely there isn't a connection!"<<std::endl;
    }
    else {
        ip = triangleAngle(aMatrix[a], aMatrix[b], aMatrix[c]);
        ip = (360/(2*PI))*ip;
        std::cout<<"\n"<<std::to_string(ip)<<std::endl;
    }
    return 0;
}