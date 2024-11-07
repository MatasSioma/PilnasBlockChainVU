#include "../headers/block.h"

using namespace std;

#define USER_NR 1000
#define TX_IN_BLOCK 100
#define BLOCK_NR 3
#define DIFFICULTY 3
#define BLOCK_OPTION_COUNT 5

int main() {
    vector<User> users = generateUsers(USER_NR);
    saveUsersToFile(users, "usersStart");
    list<Block> blockChain;
    Block genesis(users);
    blockChain.push_back(genesis);
    string prevHash = "0";

    for(int i = 0; i < BLOCK_NR; i++) {
        vector<Transaction> txs = generateTxs(users, 10000);

        vector<vector<User>> accounts;
        accounts.reserve(BLOCK_OPTION_COUNT); 
        for(int n = 0; n < BLOCK_OPTION_COUNT; n++)
            accounts.push_back(users);

        std::vector<int> indices(10000);
        std::iota(indices.begin(), indices.end(), 0);
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);

        vector<Block> blockOptions;
        blockOptions.reserve(BLOCK_OPTION_COUNT);

        for(int n = 0; n < BLOCK_OPTION_COUNT; n++) {
            vector<Transaction> rTx;
            rTx.reserve(TX_IN_BLOCK);
            cout << n + 1 << " bloko variantui - atsitiktinai parenkamos +-" << TX_IN_BLOCK << endl;

            for(int i = 0; i < TX_IN_BLOCK*1.2+(TX_IN_BLOCK/USER_NR); i++) {
                int index = indices[i];
                Transaction tx = txs.at(index);
                if(tx.doTx(accounts.at(n))) {
                    rTx.push_back(tx);
                    //Transakcijos maišos reikšmės tikrinimas
                    string hashInput = tx.getSender() + tx.getReceiver() + to_string(tx.getAmount());
                    if(bitsetToHexStr(hashStr(hashInput, "")) != tx.getTransactionID())
                        cout << "Transackicjos informacijos maišos reikšmė nesutampa su transakcijos ID!" << endl;
                }
            }

            rTx.shrink_to_fit();
            Block next(prevHash, "1.0", DIFFICULTY, rTx);
            blockOptions.push_back(next);
        }
        
        int n = 0, maxTime = 1;
        vector<long> lastTry(BLOCK_OPTION_COUNT, 0);
        while(true) {
            if(n >= BLOCK_OPTION_COUNT) {
                n = 0;
                maxTime *= 2;
                cout << "\nDidinamas kasimo laikas.\n" << endl;
                continue;
            }
            cout << "Kasamas "<< n + 1 << "-as bloko variantas..." << endl;
            long res = blockOptions.at(n).mine(lastTry.at(n), maxTime);
            if(res != -1) {
                cout << "Blokas neiškastas per " << maxTime << "s" << endl;
                lastTry.at(n) = res;
                n++;
            } else {
                prevHash = blockOptions.at(n).getHash();
                blockChain.push_back(blockOptions.at(n));
                users = accounts.at(n);
                break;
            }
        }
        cout << endl;

    }

    printBlockChain(blockChain);
    saveUsersToFile(users, "usersEnd");

    cout << "\nPaskutinis blokas detaliai:" << endl;
    int lastIndex = blockChain.size() - 1 ;
    printBlockInChain(blockChain, lastIndex);

    list<Block>::iterator block = blockChain.begin();
    advance(block, lastIndex);
    cout << "Dvi paskutinės bloko transakcijos:" << endl;
    block->printTxs(-2);


    /*
        išvesti n-taji bloką:
        printBlockInChain(blockChain, 2) // išves 3-itaji (index = 2) bloką su Block objekto .print() metodu

        išvesti bloko transakciją/as:
        Block block.printTxs(kiekis)

        Kiekviename bloke yra transakcijų masyvas (vektorius), kur pasirinkę transakciją n galime ją išvesti su:
        Block block.getTransactions().at(n).print()

    */
}
