#include <iostream>
#include <string>
#include <iomanip> 
#include "../blockchainHash/hash.h"

using namespace std;

int main() {
    string input = "labas";
    printHex(hashStr(input, ""));
}
