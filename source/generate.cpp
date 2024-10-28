#include "../headers/block.h"

using namespace std;

#define USER_NR 1'0
#define TX_NR 10'0

int main() {
    vector<User> users = generateUsers(USER_NR);
    vector<Transaction> txs = generateTxs(users, TX_NR);
    vector<Block> blockChain;

    cout << endl;
    Block genesis(users);
    
    Block next("0", "1.0", 3, txs);
    next.print();
    cout << "Kasamas blokas..." << endl;
    next.mine();
    next.print();
}