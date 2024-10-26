#ifndef BLOCK_H
#define BLOCK_H

#include "../hash/hash.h"
#include <random>
using namespace std;

class User {
private:
    string name;
    string publicKey;
    double balance;
public:
    User();
    User(string &name, string &publicKey, double &balance) {
        this->name = name;
        this->publicKey = publicKey;
        this->balance = balance;
    }
    
    string getName() const {return name;};
    string getPublicKey() const {return publicKey;};
    double getBalansas() const {return balance;};

    void setName(const string name) {this->name = name;};
    void setPublicKey(const string publicKey) {this->publicKey = publicKey;};
    void setBalansas(const double balance) {this->balance = balance;};

    void printUser();
};

string generateRandomString(int length);
double generateRandomDouble(double low, double high);


#endif