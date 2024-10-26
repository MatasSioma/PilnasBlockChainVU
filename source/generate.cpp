#include "../headers/block.h"

using namespace std;

#define USER_NR 1'000
#define TX_NR 10'000

vector<User> generateUsers(int userCount) {
    vector<User> users(userCount);
    for (auto it = users.begin(); it != users.end(); it++) {
        User gen;
        *it = gen;
    }
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
    return txs;
}

int main() {
    vector<User> users = generateUsers(USER_NR);
    vector<Transaction> txs = generateTxs(users, TX_NR);

    for (int i = 0; i < 10; i++) {
        txs.at(i).print();
    }
}