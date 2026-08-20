#include <iostream>
#include <atomic>
#include <cmath>
#include <iomanip>
#include <string>

class obj {
public:
    double value = 0.0;
    obj* next = nullptr;
    obj* prev = nullptr;

    obj(double value) {
        this->value = value;
    }
    ~obj() {
        //std::cout<<"Object with value "<<this->toString()<<" was destroyed."<<std::endl;
    }

    std::string toString() {
        std::string a = std::to_string(value);
        return a;
    }
};

class objList {
public:
    unsigned int size = 0;
    obj* head = nullptr;
    obj* tail = nullptr;

    void add_tail(double value) {
        obj* a = new obj(value);
        if(size==0) {
            head = a;
            tail = a;
        }
        else {
            (*tail).next = a;
            (*a).prev = tail;
            tail = a;
        }
        size++;
    }

    void add_head(double value) {
        obj* a = new obj(value);
        if(size==0) {
            head = a;
            tail = a;
        }
        else {
            (*head).prev = a;
            (*a).next = head;
            head = a;
        }
        size++;
    }

    void remove_tail() {
        obj* temp = (*tail).prev;
        delete tail;
        tail = temp;
        (*tail).next = nullptr;
        size--;
    }

    void remove_head() {
        obj* temp = (*head).next;
        delete head;
        head = temp;
        (*head).prev = nullptr;
        size--;
    }

    void bubble_sort() {
        unsigned int n = 1;
        double temp;
        obj *it = head;
        while(n>0) {
            n = 0;
            it = head;
            for(int i = 0; i < (size-1); i++) {
                if(it->value > it->next->value) {
                    temp = it->value;
                    this->remove_idx(i);
                    this->add_idx(i, temp);
                    n++;
                }
                else{
                    it = it->next;
                }
            }
        }
    }

    void add_idx(int n, double value) {
        try{
            if(n==(size-1)) {
                this->add_tail(value);
            }
            else{
                obj* it = head, *a = new obj(value);
                if(n > size-1) {
                    throw std::out_of_range("Number must be smaller than vector size which is " + std::to_string(size));
                }
                for(int i = 0; i < n; i++) {
                    it = it->next;
                }

                a->next = it->next;
                it->next = a;
                a->prev = it;
                a->next->prev = a;
                size++;
            }  
        }
        catch(std::out_of_range& e) {
            std::cout<<"Out of range."<<std::endl;
        }
    }

    void remove_idx(int n) {
        try{
            if(n==0){
                this->remove_head();
            }
            else if(n==(size-1)) {
                this->remove_tail();
            }
            else{
                obj* it = head;
                if(n > size-1) {
                    throw std::out_of_range("Number must be smaller than vector size which is " + std::to_string(size));
                }
                for(int i = 0; i < n; i++) {
                    it = it->next;
                }
                it->prev->next = it->next;
                it->next->prev = it->prev;
                delete it;
                it = nullptr;
                size--;
            }
            
        }
        catch(std::out_of_range& e) {
            std::cout<<"Out of range."<<std::endl;
        }
    }

    std::string toString() {
        std::string H = "[";
        obj* it = head;
        for(int i = 0; i < (size-1); i++) {
            H += std::to_string((*it).value) + ", ";
            it = (*it).next;
        }
        H += std::to_string((*it).value);
        H += "]";
        it = nullptr;
        delete it;
        return H;
    }

    std::string reverse_toString() {
        std::string H = "[";
        obj* it = tail;
        for(int i = 0; i < (size-1); i++) {
            H += std::to_string((*it).value) + ", ";
            it = (*it).prev;
        }
        H += std::to_string((*it).value);
        H += "]";
        it = nullptr;
        delete it;
        return H;
    }

    obj get_idx(int n) {
        try{
            obj* it = head;
            if(n > size-1) {
                throw std::out_of_range("Number must be smaller than vector size which is " + std::to_string(size));
            }
            for(int i = 0; i < n; i++) {
                it = (*it).next;
            }
            return *it;
            it = nullptr;
            delete it;
        }
        catch(std::out_of_range& e) {
            std::cout<<"Out of range; returning null."<<std::endl;
            return obj(0.0);
        }
    }

    std::string find_idx(obj a) {
        obj* it = head;
        std::string H = "There are occurrences at the index(es) (None if empty): ";
        for(int i = 0; i < size; i++) {
            if((*it).value == a.value){
                H+= std::to_string(i) + " ";
            }
            it = (*it).next;
        }
        it = nullptr;
        delete it;
        return H;
    }

    std::string findFirst_idx(obj a) {
        obj* it = head;
        std::string H = "The first occurrences is the index (None if empty): ";
        for(int i = 0; i < size; i++) {
            if((*it).value == a.value){
                H+= std::to_string(i) + " ";
                break;
            }
            it = (*it).next;
        }
        it = nullptr;
        delete it;
        return H;
    }

    obj max() {
        obj a = *head, *it = head;
        for(int i = 0; i < size; i++) {
            if((*it).value >= a.value){
                a = *it;
            }
            it = (*it).next;
        }
        it = nullptr;
        delete it;
        return a;
    }

    obj min() {
        obj a = *head, *it = head;
        for(int i = 0; i < size; i++) {
            if((*it).value <= a.value){
                a = *it;
            }
            it = (*it).next;
        }
        it = nullptr;
        delete it;
        return a;
    }

    double mode() {
        unsigned int n = 0;
        std::string H = "";
        obj* it1 = head, *it2 = head, ans(0.0);
        for(int i = 0; i < size; i++) {
            it2 = head;
            H = "";
            for(int i = 0; i < size; i++) {
                if((*it2).value == (*it1).value){
                    H+= "0";
                }
                it2 = (*it2).next;
            }
            if(H.length() > n){
                n = H.length();
                ans = (*it1);
            }
            it1 = (*it1).next;
        }
        if(n==1) {
            std::cout<<"No mode"<<std::endl;
            return -1.0;
        }
        else{
            return ans.value;
        }
    }

    ~objList() {
        while(size>1) {
            this->remove_tail();
        }
        delete head;
        //std::cout<<"List was destroyed."<<std::endl;
    }
};



int main() {
    objList A;
    A.add_tail(6.0);
    A.add_tail(2.0);
    A.add_tail(3.0);
    A.add_tail(5.0);


    std::cout<<A.toString()<<std::endl;
    A.bubble_sort();
    std::cout<<A.toString()<<std::endl;
    A.remove_head();
    std::cout<<A.toString()<<std::endl;

    return 0;
}
