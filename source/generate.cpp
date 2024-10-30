#include "../headers/block.h"

using namespace std;

#define USER_NR 100
#define TX_IN_BLOCK 100
#define BLOCK_NR 3
#define DIFFICULTY 3

int main() {
    vector<User> users = generateUsers(USER_NR);
    saveUsersToFile(users, "usersStart");
    list<Block> blockChain;
    Block genesis(users);
    blockChain.push_back(genesis);
    string prevHash = "0";

    for(int i = 0; i < BLOCK_NR; i++) {
        vector<Transaction> txs = generateTxs(users, 10000);
        vector<Transaction> rTx;
        rTx.reserve(TX_IN_BLOCK);
        cout << "Atsitiktinai parenkamos 100." << endl;

        std::vector<int> indices(10000);
        std::iota(indices.begin(), indices.end(), 0);
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);

        for(int tx = 0; tx < TX_IN_BLOCK*1.2; tx++) {
            int index = indices[tx];
            if(txs.at(index).doTx(users)) {
                rTx.push_back(txs.at(index));
            }
        }
        rTx.shrink_to_fit();
        Block next(prevHash, "1.0", DIFFICULTY, rTx);
        cout << "Paskutinė transakcija:" << endl;
        next.printTxs(1);
        cout << "Kasamas blokas..." << endl;
        next.mine();
        cout << endl;
        prevHash = next.getHash();
        blockChain.push_back(next);
    }

    printBlockChain(blockChain);
    saveUsersToFile(users, "usersEnd");
}