#include "../headers/block.h"

using namespace std;

#define USER_NR 100
#define TX_IN_BLOCK 100
#define BLOCK_NR 3
#define DIFFICULTY 3

int main() {
    vector<User> users = generateUsers(USER_NR);
    list<Block> blockChain;

    Block genesis(users);
    blockChain.push_back(genesis);
    string prevHash = "0";

    for(int i = 0; i < BLOCK_NR; i++) {
        vector<Transaction> txs = generateTxs(users, 10000);
        vector<Transaction> rTx(TX_IN_BLOCK);
        cout << "Atsitiktinai parenkamos 100." << endl;
        for(int tx = 0; tx < TX_IN_BLOCK * 1.1; tx++) {
            try {
                int index = generateRandomInt(0, 10000);
                if(txs.at(index).doTx(users)) {
                    rTx.at(tx) = txs.at(index);
                }
            } catch (std::out_of_range e) {
                cout << "100 transakcijų pasiekta." << endl;
                break;
            }
        }
        rTx.shrink_to_fit();
        Block next(prevHash, "1.0", DIFFICULTY, rTx);
        cout << "Kasamas blokas..." << endl;
        next.mine();
        cout << endl;
        prevHash = next.getHash();
        blockChain.push_back(next);
    }

    printBlockChain(blockChain);
}