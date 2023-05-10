#include <iostream>
#include <cmath>

using namespace std;

typedef unsigned long long ull;

int queryCounter = 0;

const uint32_t PRIME = pow(2, 31) - 1; // 2^31 - 1
const uint32_t A = 1664525;
const uint32_t C = 1013904223;
const ull R = pow(2, 32);

int counter = 0;

class RNG {
public:
    RNG(uint32_t S) : current(S) {}

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

class SparseTable
{
public:
    int op;

    void preprocess(int ops, long long *array, int len){

        double prelog = log2(len);

        logN = (int)floor(prelog) + 1;
        table = new ull*[logN];

        for (int i = 0; i < logN; i++){
            table[i] = new ull[len];
        }
        op = ops;

        starter = 0;

        if(op == 0) starter = 0;
        if(op == 1) starter = numeric_limits<ull>::max();
        if(op == 2) starter = numeric_limits<ull>::min();;

        N = len;

        for (int j = 0; j < len; j++){
            table[0][j] = array[j];
        }

        build();
    }

    void build(){
        int k = 1;
        for (int i = 1; i < logN; i++){
            for (int j = 0; j < N; j++){
                if (j + k >= N){
                    table[i][j] = table[i - 1][j];
                }
                else{
                    switch (op){
                    case 0:
                        table[i][j] = table[i - 1][j] + table[i - 1][j + k];
                        break;
                    case 1:
                        table[i][j] = min(table[i - 1][j], table[i - 1][j + k]);
                        break;
                    case 2:
                        table[i][j] = max(table[i - 1][j], table[i - 1][j + k]);
                        break;
                    default:
                        return;
                        break;
                    }
                }
            }
            k *= 2;
        }
    }

    void update(long long newValue, int indexAux){
        int index = indexAux;
        table[0][index] = newValue;

        int k = 1;

        for (int i = 1; i < logN; i++){
            int lAux = (int)max(0, index - (2 * k) + 1);

            for (int l = lAux; l <= index; l++){
                if (l + k >= N){
                    table[i][l] = table[i - 1][l];
                }
                else{
                    switch (op){
                    case 0:
                        table[i][l] = table[i - 1][l] + table[i - 1][l + k];
                        break;

                    case 1:
                        table[i][l] = min(table[i - 1][l], table[i - 1][l + k]);
                        break;

                    case 2:
                        table[i][l] = max(table[i - 1][l], table[i - 1][l + k]);
                        break;

                    default:
                        return;
                        break;
                    }
                }
            }
            k *= 2;
        }
    }

    long long query(long long left, long long right){
        ull result = starter;

        int leftAux = (int)left;
        int rightCursor = (int)right;

        while (leftAux < rightCursor){
            double indexAux = log2(rightCursor - leftAux);
            int index = (int)indexAux;

            switch (op){
            case 0:
                result = result + table[index][leftAux];
                break;

            case 1:
                result = min(result, table[index][leftAux]);
                break;

            case 2:
                result = max(result, table[index][leftAux]);
                break;

            default:
                return 1;
                break;
            }
            leftAux += pow(2, index);
        }
        return result;
    }
private:
    ull **table;
    int logN;
    ull starter;
    int N;
};

void solve() {
    ull S, N;
    string op;
    int O, Q, U;

    while (cin >> S){

        cin >> N >> op >> O >> Q >> U;

        counter = 0;
        RNG rng(S);

        SparseTable sparseTable;

        if (op == "SUM") sparseTable.op = 0;
        if (op == "MIN") sparseTable.op = 1;
        if (op == "MAX") sparseTable.op = 2;

        int m = 4 * N;
        long long *array = new long long[N];
        array[0] = S % m;

        rng.next();

        for (int i = 1; i < N; i++){
            array[i] = rng.next() % m;
        }

        sparseTable.preprocess(sparseTable.op, array, N);

        cout << "caso " << queryCounter << endl;
        for (int i = 0; i < O; i++){
            ull curr = rng.next();
            if(curr % (Q + U) < Q) {
                ull leftMax = rng.next() % N;
                ull cursor = rng.next();
                ull rightCursor = cursor % (N - leftMax);
                ull rightMax = leftMax + 1 + rightCursor;

                cout << sparseTable.query(leftMax, rightMax) << '\n';
            } else {
                ull updateIndex = rng.next() % N;
                ull updateValue = rng.next() % m;

                sparseTable.update(updateValue, updateIndex);

                cout << sparseTable.query(updateIndex, N) << '\n';
            }
        }
        queryCounter++;
        std::cout << '\n';
    }
}

int main(){
    solve();

    return 0;
}