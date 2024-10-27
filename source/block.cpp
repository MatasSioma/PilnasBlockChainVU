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
    cout << "Data: " << ctime(&this->timestamp);
    cout << "Versija: " << this->version << endl;
    cout << "Merkle hash'as: " << this->merkleHash << endl;
    cout << "Nonce: " << this->nonce << endl;
    cout << "Sudėtingumas: " << this->difficulty << endl;
    cout << "Iškastas: ";
    if(this->mined) cout << "Taip";
    else cout << "Ne";
    cout << endl;
    cout << "Transakcijų kiekis: " << this->transactions.size() << endl;
    cout << "Paskutinės transacijos ID: " << (this->transactions.end()-1)->getTransactionID() << endl;
    cout << endl;
}

string Block::calcMerkleHash(vector<Transaction> &txs) {
    string hashInput = "";
    for(auto tx : txs) {
        hashInput += tx.getSender();
        hashInput += tx.getReceiver();
        hashInput += tx.getAmount();
    }
    return bitsetToHexStr(hashStr(hashInput, ""));
}

bool checkIfTxValid(Transaction &tx, vector<User> &users) {
    for (auto user : users) {
        if(user.getPublicKey() == tx.getSender() && user.getBalance() >= tx.getAmount()) {
            return true;
        }
    }
    return false;
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

vector<User> generateUsers(int userCount) {
    vector<User> users(userCount);
    for (auto it = users.begin(); it != users.end(); it++) {
        User gen;
        *it = gen;
    }

    cout << "Sugeneruota " << userCount << " vartotojų." << endl;
    cout << userCount << "-tojo duomenys:\n" << endl;
    (users.end()-1)->print();

    return users;
}

vector<Transaction> generateTxs(vector<User> &users, int transactionCount) {
    vector<Transaction> txs(transactionCount);
    for (auto it = txs.begin(); it != txs.end(); it++) {
        while (true) {
            User sender = users.at(generateRandomInt(0, users.size()));
            User receiver = users.at(generateRandomInt(0, users.size()));
            if (sender.getPublicKey() == receiver.getPublicKey()) continue;
            double amount = generateRandomDouble(0.10, sender.getBalance());
            Transaction newTx(sender.getPublicKey(), receiver.getPublicKey(), amount); 
            *it = newTx;
            break;
        }
    }

    cout << "Sugeneruota " << transactionCount << " transakcijų." << endl;
    cout << transactionCount << "-tos duomenys:\n" << endl;
    (txs.end()-1)->print();

    return txs;
}