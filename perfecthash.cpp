#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

const uint32_t PRIME = pow(2, 31) - 1; // 2^31 - 1
const uint32_t A = 1664525;
const uint32_t C = 1013904223;
const long long R = pow(2, 32);

int counter = 0;

class RNG {
public:
    RNG(uint32_t seed) : current(seed) {}

    uint32_t next() {
        if (counter > 0) {
            current = (A * current + C) % R;
        }

        counter++;
        return current;
    }

private:
    uint32_t current;
};

class UniversalHash {
public:
    UniversalHash() : a(0), b(0), M(0), P(0) {}

    UniversalHash(uint32_t universe_size, uint32_t range_size, RNG &rng) : M(range_size), P(PRIME) {
        a = 1 + rng.next() % (P - 1);
        b = rng.next() % P;
    }

    uint32_t operator()(uint32_t key) const {
        return ((a * key + b) % P) % M;
    }

private:
    uint32_t a, b, M, P;
};

class OpenHashTable {
    UniversalHash hT;

public:
    OpenHashTable(uint32_t universe_size, RNG &rng, int M) : hT(UniversalHash(universe_size, M, rng)), N(0), M(M), rng(rng)  {
        T.resize(M);
    }

    bool get(uint32_t key) {
        uint32_t i = hT(key);

        for (uint32_t k : T[i]) {
            if (k == key) {
                return true;
            }
        }

        return false;
    }

    void set(uint32_t key, int f, int pf) {
        if (get(key)) {
            cout << "I " << key << " " << "-1" << " " << "-1" << '\n';
            return;
        }
        
        if (2 * N > M) {
            rehash();
            //cout << "REHASH\n";
        }
        uint32_t i = hT(key);
        T[i].push_back(key);
        N++;

        if ((f + 1) % pf == 0) {
            cout << "I " << key << " " << hT(key) << " " << T[i].size() - 1 << '\n';
        }

    }

    const vector<list<uint32_t> > &getInternalTable() const {
        return T;
    }

private:
    uint32_t N, M;
    vector<list<uint32_t> > T;
    RNG &rng;

    void rehash() {
        uint32_t newM = 2 * M + 1;
        vector<list<uint32_t> > newT(newM);
        UniversalHash newhT = UniversalHash(M, newM, rng);

        for (const auto &bucket : T) {
            for (uint32_t key : bucket) {
                uint32_t i = newhT(key);
                newT[i].push_back(key);
            }
        }

        T = move(newT);
        hT = move(newhT);
        M = newM;
    }
};

class PerfectHashTable {
public:
    PerfectHashTable(const vector<uint32_t> &keys, uint32_t universe_size, RNG &rng)
        : M(2 * floor((double)keys.size() / (double) 2) + 1), T(), hT(UniversalHash(universe_size, M, rng)) {
        build(keys, universe_size, rng);
    }

    bool get(uint32_t key, int f, int pf) const {
        uint32_t i = hT(key);
        if (i >= T.size()) {

            if ((f + 1) % pf == 0) {
                cout << "Q " << key << " " << "-1" << " " << "-1" << '\n';
            }

            return false;
        }
        uint32_t j = T[i].g(key);
        if (j >= T[i].table.size()) {
            
            if ((f + 1) % pf == 0) {
                cout << "Q " << key << " " << "-1" << " " << "-1" << '\n';
            }

            return false;
        }

        if (((f + 1) % pf == 0)) {
            if(T[i].table[j] == key) {
                cout << "Q " << key << " " << i << " " << j << '\n';
            } else {
                cout << "Q " << key << " " << "-1" << " " << "-1" << '\n';
            }
        }

        return T[i].table[j] == key;
    }

private:
    struct SubTable {
        SubTable() : M(0), g() {}
        uint32_t M;
        vector<uint32_t> table;
        UniversalHash g;
    };

    uint32_t M;
    vector<SubTable> T;
    UniversalHash hT;

    void newBuild(const vector<uint32_t> &keys, uint32_t universe_size, RNG &rng) {
        vector<vector<uint32_t> > table;

        int N = keys.size();
        M = 2 * floor((double)N / (double) 2) + 1;

        table.resize(M);
        hT = UniversalHash(universe_size, M, rng);

        bool done = false;

        while(!done) {
            for(int i = 0; i < N; i++) {
                int index = hT(keys[i]);
                table[index].push_back(keys[i]);
            }

            int sum = 0;
            for(int i = 0; i < M; i++) {
                sum += table[i].size() * table[i].size();
            }

            if (sum > 4 * N) {
                done = true;
            }
        }


        for (int i = 0; i < M; i++) {
            if (table[i].size() > 0) {
                bool success = false;
                vector<u_int32_t> subtable;


                while(!success) {
                    success = true;
                    subtable.clear();
                    subtable.resize(table[i].size() * table[i].size() + 1, 0);


                    for(int y = 0; y < table[i].size(); y++) {
                        UniversalHash g(universe_size, table[i].size() * table[i].size() + 1, rng);
                        //cout << "DONE" << '\n';
                        uint32_t j = g(table[i][y]);
                        if (subtable[j] == 0 && j < subtable.size()) {
                            subtable[j] = table[i][y];
                            uint32_t temp = table[i][y];
                            cout << table[i][y] << " " << i << " " << j << '\n';
    
                        } else {
                            success = false;
                            break;
                        }
                    }
                }


                table[i] = subtable;
            }
        }

    }

    void build(const vector<uint32_t> &keys, uint32_t universe_size, RNG &rng) {
        uint32_t N = keys.size();
        M = 2 * floor((double)N / (double) 2) + 1;
        cout << N << " " << M << '\n';
        bool success = false;
        hT = UniversalHash(universe_size, M, rng);

        while (!success) {
            vector<vector<uint32_t> > openHashTable;
            openHashTable.resize(M);

            for (const auto &key : keys) {
                uint32_t i = hT(key);
                //cout << key << " -> ";
                //cout << i << '\n';
                openHashTable[i].push_back(key);
            }

            uint32_t squaredSum = 0;
            for(int j = 0; j < M; j++) {
                squaredSum += pow(openHashTable[j].size(), 2);
            }

            if (squaredSum <= 4 * N) {
                success = true;
                T.resize(M);

                for (uint32_t i = 0; i < M; ++i) {
                    if (openHashTable[i].size() > 0) {
                        uint32_t subTableSize = (openHashTable[i].size() * openHashTable[i].size()) + 1;
                        T[i].M = subTableSize;
                        T[i].table.resize(subTableSize, 0);
                        bool subSuccess = false;

                        while (!subSuccess) {
                            subSuccess = true;

                            T[i].g = UniversalHash(universe_size, subTableSize, rng);
                            for (const auto &key : openHashTable[i]) {
                                uint32_t j = T[i].g(key);

                                if (T[i].table[j] != 0) {
                                    T[i].table.clear();
                                    T[i].table.resize(subTableSize, 0);
                                    subSuccess = false;
                                    break;
                                }
                                else {
                                    T[i].table[j] = key;
                                    cout << key << " " << i << " " << j << '\n';
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

int main() {
    uint32_t S, U, M0, I, Q, Pi, Pq;

    cin >> S >> U >> M0 >> I >> Q >> Pi >> Pq;

    RNG rng(S);
    OpenHashTable T0(U, rng, M0);

    for (int i = 0; i < I; ++i) {
        uint32_t K = rng.next() % U;

        T0.set(K, i, Pi);
    }


    vector<uint32_t> keys;
    for (const auto &bucket : T0.getInternalTable()) {
        for (uint32_t key : bucket) {
            keys.push_back(key);
        }
    }

    PerfectHashTable T1(keys, U, rng);

    for (int i = 0; i < Q; i++) {

        uint32_t K = rng.next() % U;

        bool found = T1.get(K, i, Pq);
    }

    return 0;
}