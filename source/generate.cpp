#include "../headers/block.h"

using namespace std;

#define USER_NR 1'0
#define TX_NR 10'000

int main() {
    vector<User> users = generateUsers(USER_NR);
    vector<Transaction> txs = generateTxs(users, TX_NR);
    vector<Block> blockChain;

    cout << endl;
    Block genesis(users);
    genesis.printTxs(-2);
}