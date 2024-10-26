#include "../headers/block.h"

using namespace std;

int main() {
    srand(time(nullptr));
    int usersToGen = 1000;
    int txsToGen = 10000;

    vector<User> users(usersToGen);
    for (auto it = users.begin(); it != users.end(); it++) {
        User gen;
        *it = gen;
    }

    vector<Transaction> txs(txsToGen);
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
        it->print();
    }
}