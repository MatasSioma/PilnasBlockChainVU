#include "../headers/block.h"

using namespace std;

int main() {
    int usersToGen = 1000;
    vector<User> users(usersToGen);

    for (auto it = users.begin(); it != users.end(); it++) {
        User gen;
        *it = gen;
        gen.printUser();
    }

    cout << users.size() << endl;
}