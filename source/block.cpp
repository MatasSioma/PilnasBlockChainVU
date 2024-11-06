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
    cout << "Paskutinės transakcijos ID: " << (this->transactions.end()-1)->getTransactionID() << endl;
    cout << endl;
}

// printTxs(2) - dvi pirmos, printTxs(-3) - trys paskutinės, printTxs(0) - visos
void Block::printTxs(int amount) {
    int totalTxs = this->transactions.size();

    if(amount == 0) {
        for(auto tx : this->transactions) {
            tx.print();
        }
        return;
    }

    if (abs(amount) > totalTxs) {
        cout << "Norimas transakcijų išspausdinimo kiekis viršija viso bloko transkacijų kiekį." << endl;
        return;
    }

    int start = 0;
    if(amount < 0) start = totalTxs + amount;
    for(int i = start; i < start + abs(amount); i++) {
        this->transactions.at(i).print();
    }
    cout << "Dar likę (nerodoma): " << totalTxs - abs(amount) << " transakcijų" << endl;
}

string Block::calcMerkleHash(vector<Transaction> &txs) {
    std::vector<bc::hash_digest> tx_hashes;

    for (auto &tx : txs) {
        bc::hash_digest tx_hash;
        bc::decode_hash(tx_hash, tx.getTransactionID());
        tx_hashes.push_back(tx_hash);
    }

    // Compute the Merkle root
    auto merkle_root = create_merkle(tx_hashes);

    // Convert the hash_digest to a hex string
    return bc::encode_base16(merkle_root);
}

string Block::getBlockString() {
    string output = "";
    output = output + this->previousHash + ctime(&this->timestamp) + this->version + this->merkleHash + to_string(this->difficulty);

    return output;
}

long Block::mine(long start, int maxTime) {
    long nonce = start;
    string target = "";
    for(int i = 0; i < this->difficulty; i++) target += '0';
    string block = this->getBlockString();
    string hash;

    Timer search;
    while(true) {
        string blockAndNonce = block + to_string(nonce);
        hash = bitsetToHexStr(hashStr(blockAndNonce, ""));
        // cout << nonce << ": "<< hash << endl;
        if (hash.substr(0, this->difficulty) == target) break;
        nonce++;
        if(search.elapsed() > maxTime) return nonce;
    }

    cout << "Blokas iškastas per " << search.elapsed() << "s." << endl;
    this->setHash(hash);
    this->setNonce(nonce);
    this->setMined(true);
    return -1;
}

bool Transaction::doTx(vector<User> &users) {
    User *senderPtr = nullptr;
    User *recipientPtr = nullptr;

    for (auto &user : users) {
        if (user.getPublicKey() == this->sender) {
            senderPtr = &user;
        } else if (user.getPublicKey() == this->receiver) {
            recipientPtr = &user;
        }
        
        if (senderPtr && recipientPtr) break;
    }

    if (senderPtr && recipientPtr && senderPtr->getBalance() >= this->amount) {
        senderPtr->setBalance(senderPtr->getBalance() - this->amount);
        recipientPtr->setBalance(recipientPtr->getBalance() + this->amount);
        return true;
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
    // cout << userCount << "-tojo duomenys:\n" << endl;
    // (users.end()-1)->print();

    return users;
}

vector<Transaction> generateTxs(vector<User> &users, int transactionCount) {
    vector<Transaction> txs;
    txs.reserve(transactionCount);
    for (int i = 0; i < transactionCount; i++) {
        User sender, receiver;
        do {
            sender = users.at(generateRandomInt(0, users.size() - 1));
            receiver = users.at(generateRandomInt(0, users.size() - 1));
        } while (sender.getPublicKey() == receiver.getPublicKey());

        // Generate transaction amount and add transaction to the list
        double amount = generateRandomDouble(0.10, sender.getBalance());
        if (!sender.getPublicKey().empty() && !receiver.getPublicKey().empty() && amount > 0) {
            Transaction newTx(sender.getPublicKey(), receiver.getPublicKey(), amount);
            txs.push_back(newTx);
        }
    }

    cout << "Sugeneruota " << txs.size() << " transakcijų." << endl;
    // cout << transactionCount << "-tos duomenys:\n" << endl;
    // (txs.end()-1)->print();

    return txs;
}

void printBlockChain(list<Block> &chain) {
    int i = 0;
    cout << endl << "Galutinė sugeneruota blokų grandinė: \n" << endl;
    for(auto block : chain) {
        cout << i << " Blokas:" << endl;
        cout << "hash: " << block.getHash() << endl;
        cout << "praito hash: " << block.getPreviousHash() << endl;
        cout << "txs: " << block.getTransactions().size() << " - " << block.getMerkleHash() << endl;
        cout << "nonce: " << block.getNonce() << endl;
        cout << endl;
        i++;
    }
}

void printBlockInChain(list<Block> &chain, int index) {
    index = abs(index);
    if(index >= chain.size()) {
        cout << "Indeksas " << index << " blokų grandinėje neegzistuoja." << endl;
        return;
    }
    cout << index + 1 << "-asis blokas grandinėje:" << endl;
    list<Block>::iterator block = chain.begin();
    advance(block, index);
    block->print();
}

void saveUsersToFile(vector<User> &users, string fname) {
    ofstream data(fname + ".txt");
    
    data << left << setw(5) << "#" 
        << setw(9) << "Vardas" 
        << setw(12) << "Balansas" 
        << setw(30) << "PK" << endl;


    data << string(90, '-') << endl;

    for (int i = 0; i < users.size(); i++) {
        data << left << setw(5) << i
            << setw(9) << users[i].getName()
            << setw(12) << fixed << setprecision(2) << users[i].getBalance()
            << setw(30) << users[i].getPublicKey() << endl;
    }

    data.close();
}