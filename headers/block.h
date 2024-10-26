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
    double getBalance() const {return balance;};

    void setName(const string name) {this->name = name;};
    void setPublicKey(const string publicKey) {this->publicKey = publicKey;};
    void setBalance(const double balance) {this->balance = balance;};

    void print();
};

class Transaction {
private:
    string transactionID;
    string sender;
    string receiver;
    double amount; 

public:
    Transaction() : transactionID(""), sender(""), receiver(""), amount(0.0) {}

    Transaction(string sender, string receiver, double amount){
        string hashInput = sender + receiver + to_string(amount);
        this->transactionID = bitsetToHexStr(hashStr(hashInput, ""));
        this->sender = sender;
        this->receiver = receiver;
        this->amount = amount;
    }

    void setTransactionID() {
        string hashInput = getSender() + getReceiver() + to_string(getAmount());
        this->transactionID = bitsetToHexStr(hashStr(hashInput, ""));

    };
    void setSender(string sender) {this->sender = sender;};
    void setReceiver(string receiver) {this->receiver = receiver;};
    void setAmount(double amount) {this->amount = amount;};
    
    string getTransactionID() const {return transactionID;};
    string getSender() const {return sender;};
    string getReceiver() const {return receiver;};
    double getAmount() const {return amount;};

    void print();
};

string generateRandomString(int length);
double generateRandomDouble(double low, double high);
int generateRandomInt(int low, int high);

#endif