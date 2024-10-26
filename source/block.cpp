#include "../headers/block.h"

using namespace std;

User::User() {
    this->name = generateRandomString(6);
    string random32Str = generateRandomString(32);
    this->publicKey = bitsetToHexStr(hashStr(random32Str, ""));
    this->balance = generateRandomDouble(100.00, 1000000.00);
}

void User::print() {
    cout << "Vardas: " << this->name << endl;
    cout << "PK: " << this->publicKey << endl;
    cout << "Balansas: " << this->balance << std::fixed << std::setprecision(2) << endl;
    cout << endl;
}

void Transaction::print() {
    cout << "ID: " << this->transactionID << endl;
    cout << "Siuntėjas (PK): " << this->sender << endl;
    cout << "Gavėjas (PK): " << this->receiver << endl;
    cout << "Suma: " << this->amount << std::fixed << std::setprecision(2) << endl;
    cout << endl;
}

void Block::print() {
    cout << "Hash'as: " << this->hash << endl;
    cout << "Praeito hash'as: " << this->previousHash << endl;
    cout << "Data: " << this->timestamp << endl;
    cout << "Versija: " << this->version << endl;
    cout << "Merkle hash'as: " << this->merkleHash << endl;
    cout << "Nonce: " << this->nonce << endl;
    cout << "Sudėtingumas: " << this->difficulty << endl;
    cout << "Iškastas: ";
    if(this->mined) cout << "Taip";
    else cout << "Ne";
    cout << endl;
    cout << "Transakcijų kiekis: " << this->transactions.size() << endl;
    cout << "Paskutinė transacija" << this->transactions.end()->getTransactionID() << endl;
    cout << endl;
}

string generateRandomString(int length) {
    // const std::string characters = "abcdefghijklmnopqrstuvwxyz";
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHJKLMNOPRSTUVWXYZ";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string randomString;
    for (int i = 0; i < length; i++) {
        randomString += characters[distribution(generator)];
        // bool capitalize = rand() % 2;
        // if (capitalize) *randomString.end().toUpper();
    }

    return randomString;
}

double generateRandomDouble(double low, double high) { // [low, high]
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(low, high);
    return distribution(generator);
}

int generateRandomInt(int low, int high) { // [low, high)
    high -= 1;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(low, high);
    return distribution(generator);
}