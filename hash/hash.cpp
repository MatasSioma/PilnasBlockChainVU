#include <iostream>
#include <iomanip> 
#include <string>
#include <limits>
#include <bitset>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>

#include "hash.h"

using namespace std;

void printHex(vector<bitset<8>> input) {
    cout << "Hex reprezentacija:" << endl << "0x";
    for(const auto& byte : input) cout << hex << setw(2) << setfill('0') << byte.to_ulong();
    cout << endl;
}

void printBin(vector<bitset<8>> input) {
    cout << "Dvejatainė reprezentacija:" << endl;
    for(const auto& byte : input) cout << byte.to_string() << "'";
    cout << endl;
}

string bitsetToHexStr(vector<bitset<8>> input) {
    stringstream in;
    string out;
    for (auto B : input) {
        in << hex << setw(2) << setfill('0') << B.to_ulong();
    }
    getline(in, out);
    return out;
}

void pasirinktiEiga(string msg, int* option, int max) {
    while (true) {
        try {
            cout << msg;
            cin >> *option;
            if(!cin.good() || *option < 1 || *option > max) {
                throw invalid_argument("Netinkama įvestis. Įveskite skaičių nuo 1 iki " + to_string(max));
            }
            break;
        } catch(invalid_argument &e) {
            cerr << e.what() << endl;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getInputString() {
    int option;
    string inputText;

    pasirinktiEiga("Pasirinkite įvesties tipą.\n1 - Tekstas komandinėje eilutėje,\n2 - Duomenys iš failo\n", &option, 2);
    if(option == 1) {
        cout << "Jūsų tekstas: " << endl;
        cin >> inputText;

        return inputText;
    } else {
        string inFileName;
        ifstream is;
        
        while(true) {
            try {
                cout << "Įveskite failo pavadinimą: ";
                cin >> inFileName;
                is.open(inFileName);
                if (is.fail()) throw invalid_argument("Failas nerastas.");
                break;
            } catch (exception &e) {
                cerr << e.what() << endl;
            }
        }

        stringstream buffer;
        string result;
        string line;

        option = 0;
        cout << "kiek eilučių nuskaityti? (0 - visas): ";
        cin >> option;
        cout << endl;

        if (option == 0) {
            buffer << is.rdbuf();
            is.close();
            return buffer.str();
        } else {
            for (int i = 0; i < option; ++i) {
                if (getline(is, line)) {
                    result += line + "\n";
                } else break;
            }
            is.close();
            return result;
        }
    }
}

string getSalt() {
    string salt; 
    cout << "Įveskite \"druską\" (ENTER - be druskos): ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    getline(cin, salt);
    cout << endl;

    return salt;
}

string getKonstitucija(int lineNum) {
    ifstream is;
    string line, result;
    is.open("duomenys/konstitucija.txt");

    for (int i = 0; i < lineNum; ++i) {
        if (getline(is, line)) {
            result += line + "\n";
        } else break;
    }
    is.close();
    return result;

}

void readInput(vector<bitset<8>> &arr, string inputText, unsigned int &k) {
    if(inputText == "") {
        vector<bitset<8>> empty(32);
        arr = empty;
        return;
    }
    for(char c : inputText) {
        bitset<8> b((unsigned char)c);
        arr.push_back(b);
        k += b.test(0) + b.test(1)*21 + b.test(2)*32 + b.test(3)*43 + b.test(4)*54 + b.test(5)*65 + b.test(6)*76 + b.test(7)*87;
    }
    arr.shrink_to_fit();
}

bitset<8> SBoxTransformation(bitset<8> &input) {
    return ((input << 2) ^ (input >> 3)) ^ ((input << 4) & (input >> 2));
}

vector<bitset<8>> TransformTo256(vector<bitset<8>> arr) {
    if(arr.size() % 32 != 0) {
        int i = 0;
        while (arr.size() % 32 != 0) {
            arr.push_back(SBoxTransformation(arr.at(i).flip()));
            i++;
        }
    }

    if(arr.size() == 32) {
        return arr;
    }

    int arrsize = arr.size()/2;

    vector<bitset<8>> newArr;
    newArr.reserve(arrsize);
    for (int i = 0; i < arrsize; i++) {
        auto leftValue = arr.at(i).to_ulong();
        auto rightValue = arr.at(i + arrsize).to_ulong();
        newArr.push_back(bitset<8>(((leftValue ^ rightValue) + ((leftValue << 1) | (rightValue >> 1))) % 256));
    }
    // // arba
    // vector<bitset<8>> newArr(arrsize);
    // std::vector<bitset<8>> firstHalf(arr.begin(), arr.begin() + arrsize);
    // std::vector<bitset<8>> secondHalf(arr.begin() + arrsize, arr.end());
    // newArr = joinTwoArr(firstHalf, secondHalf);

    return TransformTo256(newArr);
}

//Mazas pokytis reiskia daug (avalanche effect)
unsigned int magnify(vector<bitset<8>> &arr) {
    unsigned int k = 0;
    for (auto it = arr.begin(); it != arr.end(); it++) {
        int index = distance(arr.begin(), it);
        *it = ((*it).to_ulong() * (*it).to_ulong() + index) % 256;
        k += it->count();

        auto oppositeIt = (arr.end() - (index + 1));
        *oppositeIt = SBoxTransformation((*oppositeIt ^ *it).flip());
        k += oppositeIt->count();
    }
    return k;
}

vector<bitset<8>> joinTwoArr(vector<bitset<8>> &arr, vector<bitset<8>> &arr2) {
    if(arr.size() != arr2.size()) {
        throw invalid_argument("Masyvų dydis nevienodas.");
    }
    vector<bitset<8>> output(arr.size());
    for(int i = 0; i < arr.size(); i++) {
        auto leftValue = arr[i].to_ulong();
        auto rightValue = arr2[i].to_ulong();
        output.at(i) = bitset<8>(((leftValue ^ rightValue) + ((rightValue << 3) | (rightValue >> 5)) + 31) % 256);
        output.at(i) = SBoxTransformation(output.at(i));
    }
    return output;
}

vector<bitset<8>> hashStr(string &userInputStr, string salt) {
    vector<bitset<8>> randomStr, userInput, output;
    unsigned int k = 0;

    readInput(userInput, userInputStr, k);
    k += magnify(userInput);
    userInput = TransformTo256(userInput);
    k = k * 2 * 3 % 193;
    readInput(randomStr, Strs[k], k);

    if(!salt.empty()) {
        vector<bitset<8>> saltBits;
        readInput(saltBits, salt, k);
        saltBits = TransformTo256(saltBits);
        randomStr = joinTwoArr(saltBits, randomStr);
    }

    output = joinTwoArr(userInput, randomStr);

    return output;
}