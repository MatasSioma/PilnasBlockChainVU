# Centralizuota Blokų Grandinė
#### čia aprašyta v0.1 versija. Pakeitimai padaryti v0.2 yra apačioje (minimalūs).

Pagrindiniai komponentai yra `User` (vartotojas), `Transaction` (transakcija) ir `Block` (blokas). Viskas aprašoma `block.h` faile ir implementuojama `block.cpp` faile.

## Struktūra

- **User** klasė aprašo vartotoją su viešuoju raktu (angl. *public key*), balansu ir vardu.
- **Transaction** klasė aprašo transakciją, kurios metu tam tikra suma pervedama iš vieno vartotojo į kitą per viešąjį raktą.
- **Block** klasė aprašo bloką, kuriame yra transakcijos, ankstesnio bloko hash vertė, sudėtingumo lygis ir kita informacija.
- **Pagalbinės funkcijos** leidžia generuoti atsitiktinius vartotojus, transakcijas, išvedinėti klasių informaciją ir t.t..
- **`main()`** Naudoja visus minėtus komponentus ir generuoja, imituoja blokų grandinę.

> Vietose kada naudojamas hashavimas, naudojama mano maišos funkcija `hashStr` ir pagalbinė `bitsetToHex`.

## Komponentai

### `User` klasė

Ši klasė aprašo vartotojus, jų informaciją: vardą, viešąjį raktą ir balansą.

```cpp
User(); //Sukuria vartotoją su random vardu, PK, balansu
User(string &name, string &publicKey, double &balance);
/*
    geteriai, seteriai
    ...
*/
void print(); // Atspausdina vartotojo informacija
```

### `Transaction` klasė

Ši klasė aprašo transakciją: siuntėjo PK, gavėjo PK, sumą bei transakcijos ID (su'hash'uota kita info).

```cpp
Transaction(string sender, string receiver, double amount);
/*
    geteriai, seteriai
    ...
*/
bool doTx(vector<User> &users); // turint naudotojų masyvą, bando ivykdyti transakciją.
//Jei randamas gavėjas, siuntėjas ir siuntėjas turi pakankamai balanso, "transakcija įvygdoma" ir funckicija gražina - true. Visais kitai atvėjais - false.
void print(); // Atspausdina transakcijos informaciją

```

### `Block` klasė

Ši klasė aprašo bloką. Saugoma info:

```cpp
string hash;
string previousHash; // Praito hash
time_t timestamp;
string version;
string merkleHash;
long nonce = 0;
int difficulty = 0;
bool mined = false;
vector<Transaction> transactions;
```

Konstruktoriai, metodai:

```cpp
Block(vector<User> users); // Skirtas sukurti pradinį ("genesis") bloką, kuris jau "iškastas" ir pan.
Block(string previousHash, string version, int difficulty, vector<Transaction> transactions); // Skirta kurti sekantį bloką grandinėje 
/*
    geteriai, seteriai
    ...
*/
long mine(long start = 0, int maxTime = INT32_MAX); // Nuo 'start' parametro iteruoja nonce iki kol hash reikšmė turi nustatytą bloko 'difficulty' - 0 kiekį pradžioje.
// maxTime nusako maksimalų laiką iki kada funkcija gali "kasti"...
string getBlockString(); // Pagalbinis metodas leidžiatis visą bloko informaciją paversti į string tipą, kad galima būtų hash'uoti
string calcMerkleHash(vector<Transaction> &txs); // Suskaičiuoja sudėtinį transakcijų ID reiškmių hash'a
void print(); // atspauzdina visą bloko informaciją
void printTxs(int amount); // printTxs(2) - dvi pirmos transakcijos, printTxs(-3) - trys paskutinės, printTxs(0) - visos, naudojant Transaction::print()
```

### Pagalbinės funkcijos

```cpp
// Atsitiktinės reikšmės:
string generateRandomString(int length);
double generateRandomDouble(double low, double high);
int generateRandomInt(int low, int high);

//Generavimas:
vector<User> generateUsers(int userCount); //Naudoja default konstruktorių sugeneruoti 'userCount' skaičių vartotojų
vector<Transaction> generateTxs(vector<User> &users, int transactionCount); // Generuoja transakcijas pagal pateiktą vartotojų masyvą.
//Generuoja tokias kad siuntėjas ir gavėjas nesutampa, o transakcijos kiekis neviršija išrinkto siuntėjo balanso.

// Išvestis:
void saveUsersToFile(vector<User> &users, string fname); // Išsaugo vartotojus į tekstinį <fname>.txt failą
void printBlockChain(list<Block> &chain); // Atspausdina visą blokų grandinę
void printBlockInChain(list<Block> &chain, int index); // Atspausdina bloką esantį 'index' vietoje. Rodomas index + 1 - asis blokas grandinėje
```

### Pagrindinė generavimo funkcija `main()`

Blokų grandinės generavimas, panaudojant viršuje apibrėžtas funkcijas.

1. **Vartotojų generavimas ir išsaugojimas:**
   - Naudojant `generateUsers(USER_NR)`, sugeneruojamas nustatytas kiekis vartotojų.
   - Sugeneruoti vartotojai išsaugomi faile „usersStart“ su funkcija `saveUsersToFile`.

2. **Blokų grandinės pradžia (Genesis blokas):**
   - Sukuriamas pradinis blokas su visais sugeneruotais vartotojais (`Block genesis(users);`).
   - Genesis blokas pridedamas prie blokų grandinės.

3. **Blokų generavimas ir kasimas cikle:**
   - Kiekviename cikle vykdoma ši seka:
     - Sukuriama 10'000 atsitiktinių transakcijų (`generateTxs(users, 10'000)`).
     - Atsitiktinai pasirenkamą nustatytas kiekis transakcijų (`TX_IN_BLOCK`), kurios gali būti įtrauktos į bloką.
   - Pavykusios transakcijos pridedamos į naują bloką (`Block next`) su ankstesnio bloko hash’u, sudėtingumu ir t.t.

4. **Bloko kasimas ir pridėjimas prie grandinės:**
   - Blokas kasamas (`next.mine();`).
   - Po bloko kasimo hash’as saugomas ir naudojamas sekančiam blokui sujungti.
   - Iškastas blokas pridedamas prie blokų grandinės.

5. **Blokų grandinės ir galutinio vartotojų sąrašo išsaugojimas:**
   - Baigus blokų kūrimo ciklą, visa blokų grandinė išspausdinama naudojant `printBlockChain`.
   - išspausdinamas paskutinis blokas - visa jo saugoma informacija bei jo paskutinės dvi transakcijos (pridėta v0.2)
   - Galutinis vartotojų sąrašas išsaugomas faile „usersEnd“.

### v0.2

Pasiekti reikalavimus šiai versijai reikėjo labai nedaug.

Pakeitimai:
- calcMerkleHash() naudoja libbitcoin biblioteką per 3-iosios užduoties apraše esančia create_merkle() funkcija.
- main() sukuria nustatytą kiekį blokų variantų, kuriuos paeiliui bandoma kasti.\
Vis didinant leidžiama kasimo laiką. jei nei vienas iš variantų nebuvo iškasti. Kitas to pačio bloko kasimo ciklas prasideda nuo paskutinio patikrinto skaičiaus (nonce).
Pirmas iškastas blokas tampa priimtu sekančiu blokų grandinės bloku.
- Taip pat main() funkcijoje, kuomet parenkami atsitiktinės transakcijos, patikrinama ar sutampa jų maišos ir ID reikšmės.