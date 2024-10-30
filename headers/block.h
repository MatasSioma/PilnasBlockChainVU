#ifndef BLOCK_H
#define BLOCK_H

#include "../hash/hash.h"
#include "../headers/timer.h"
#include <list>
#include <random>
#include <ctime>

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

    bool doTx(vector<User> &users);
    void print();
};

class Block {
private:
    string hash;
    string previousHash;
    time_t timestamp;
    string version;
    string merkleHash;
    long nonce = 0;
    int difficulty = 0;
    bool mined = false;
    vector<Transaction> transactions;

public:
    //Genesis
    Block(vector<User> users) : hash("0"), previousHash(""), version("1.0"), nonce(0), difficulty(0), mined(1) {
        time_t timestamp;
        this->timestamp = time(&timestamp);

        vector<Transaction> newAcc(users.size());
        for (size_t i = 0; i < users.size(); i++) {
            User user = users.at(i);
            Transaction start("0", user.getPublicKey(), user.getBalance());
            newAcc.at(i) = start;
        }

        this->transactions = newAcc;
        this->merkleHash = calcMerkleHash(newAcc);
        
    };

    Block(string previousHash, string version, int difficulty, vector<Transaction> transactions){
        this->previousHash = previousHash;
        this->version = version;
        this->difficulty = difficulty;
        this->transactions = transactions;

        time_t timestamp;
        this->timestamp = time(&timestamp);

        this->merkleHash = calcMerkleHash(transactions);
    }

    void setHash(string hash) {this->hash = hash;};
    void setPreviousHash(string previousHash) {this->previousHash = previousHash;};
    void setTimestamp(time_t timestamp) {this->timestamp = timestamp;};
    void setVersion(string version) {this->version = version;};
    void setMerkleHash(string merkleHash) {this->merkleHash = merkleHash;};
    void setNonce(long nonce) {this->nonce = nonce;};
    void setDifficulty(int difficulty) {this->difficulty = difficulty;};
    void setMined(bool mined) {this->mined = mined;};
    void setTransactions(vector<Transaction> transactions) {this->transactions = transactions;};

    string getHash() const {return hash;};
    string getPreviousHash() const {return previousHash;};
    time_t getTimestamp() const {return timestamp;};
    string getVersion() const {return version;};
    string getMerkleHash() const {return merkleHash;};
    long getNonce() const {return nonce;};
    int getDifficulty() const {return difficulty;};
    bool getMined() const {return mined;};
    vector<Transaction> getTransactions() const {return transactions;};

    void print();
    void printTxs(int amount);

    string getBlockString();
    string calcMerkleHash(vector<Transaction> &txs);
    long mine(long start = 0, int maxTime = INT32_MAX);
};

string generateRandomString(int length);
double generateRandomDouble(double low, double high);
int generateRandomInt(int low, int high);
vector<User> generateUsers(int userCount);
vector<Transaction> generateTxs(vector<User> &users, int transactionCount);

void printBlockChain(list<Block> &chain);

#endif