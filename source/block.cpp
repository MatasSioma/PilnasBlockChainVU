#include "../headers/block.h"

using namespace std;

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

double generateRandomDouble(double low, double high) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(low, high);
    return distribution(generator);
}

User::User() {
    this->name = generateRandomString(6);
    string random32Str = generateRandomString(32);
    this->publicKey = bitsetToHexStr(hashStr(random32Str, ""));
    this->balance = generateRandomDouble(100.00, 1000000.00);
}

void User::printUser() {
    cout << "Vardas: " << this->name << endl;
    cout << "PK: " << this->publicKey << endl;
    cout << "Balansas: " << this->balance << std::fixed << std::setprecision(2) << endl;
    cout << endl;
}