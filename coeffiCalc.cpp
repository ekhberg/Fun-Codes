#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

int ordSum(std::vector<int> in, int i, int f) {
    int n = in.size(), sum = 0, ri = 0, rf = 0;
    ri = i - 1;
    rf = f - 1;
    if(ri<0) ri=0;
    if(rf>n) rf=n;
    if(i<f){
        for(int i = ri; i < f; i++) {
            sum += in.at(i);
        }
        return sum;
    }
    else if(i==f){
        return in.at(ri);
    }
    else return 1;
}

int prodSum(std::vector<int> in, int p, int lim) {
    int n = in.size(), sum = 0, np = 0, temp = 0;
    n++;
    np = p - 2;
    if(p==1) {
        return 1;
    }
    else if(p==2) {
        return ordSum(in, 1, lim);
    }
    else {
        for(int i = np; i<lim; i++) {
            sum += (in.at(i))*(prodSum(in, (p-1), i));
        }
        return sum;
    }
}
int pasTri(int h, int p) {
    if(p>(h+1)) return 0;
    else {
        std::vector<int> in;
        for(int i = 0; i < h; i++) {
            in.push_back(1);
        }
        return prodSum(in, p, h);
    }

}

int main() {
    std::vector<int> boy {-2, 5,6};
    int n = boy.size();
    int k = 5;
    for(int p = 1; p<=n; p++){
        std::cout<<"("<<prodSum(boy, p, n)<<")x^"<<(n+1-p)<<" + ";
    }
    std::cout<<"("<<prodSum(boy, (n+1), n)<<")"<<std::endl;

    /*for(int m = 1; m<=7;m++) {
        for(int i = 1; i<=(m+1); i++) {
            std::cout<<pasTri(m,i)<<"   ";
        }
        std::cout<<std::endl;
    }*/
    return 0;
}