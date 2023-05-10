#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <queue>
#include <climits>

using namespace std;

typedef unsigned long long ull;

int opCounter = 0;

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

class SegTree {
private:
    int N, op;
    vector<ull> tree;

    void build(vector<ull> &v, int n, int l, int r) {
        if (l == r) {
            tree[n] = v[l];
            return;
        }

        build(v, 2 * n, l, (l + r) / 2);
        build(v, 2 * n + 1, ((l + r) / 2) + 1, r);

        tree[n] = perform_operation(tree[2 * n], tree[2 * n + 1]);
    }

    ull perform_operation(ull a, ull b) {
        switch (op) {
            case 1:
                return a + b;
            case 2:
                return min(a, b);
            case 3:
                return max(a, b);
        }
    }

    ull default_value() {
        switch (op) {
            case 1:
                return 0;
            case 2:
                return ULLONG_MAX;
            case 3:
                return 0;
        }
    }

    ull rawQuery(int lI, int rI, int n, int l, int r) {
        if (lI > r || rI < l) {
            return default_value();
        }
        if (lI <= l && r <= rI) return tree[n];

        return perform_operation(rawQuery(lI, rI, 2 * n, l, (l + r) / 2), rawQuery(lI, rI, 2 * n + 1, ((l + r) / 2) + 1, r));
    }

    void rawUpdate(int pos, ull val, int n, int l, int r) {
        if (l == r) {
            tree[n] = val;
            return;
        }

        if (pos <= (l + r) / 2)
            rawUpdate(pos, val, 2 * n, l, (l + r) / 2);
        else
            rawUpdate(pos, val, 2 * n + 1, ((l + r) / 2) + 1, r);

        tree[n] = perform_operation(tree[2 * n], tree[2 * n + 1]);
    }

public:
    SegTree(int n, int op, vector<ull> &v) {
        this->N = n;
        this->op = op;

        tree.resize(4 * N);
        build(v, 1, 0, n - 1);
    }

    ull query(int l, int r) {
        return rawQuery(l, r, 1, 0, N - 1);
    }

    void update(ull val, int pos) {
        rawUpdate(pos, val, 1, 0, N - 1);
    }
};

void process_case(ull S, ull N, const string& f, ull W, ull Q, ull U, ull P) {
    int op;
    if (f == "SUM") op = 1;
    if (f == "MIN") op = 2;
    if (f == "MAX") op = 3;

    counter = 0;
    RNG rng(S);

    vector<ull> arr(N);

    for (int i = 0; i < N; i++) {
        arr[i] = rng.next() % (4 * N);
    }

    SegTree segtree(N, op, arr);
    cout << "caso " << opCounter++ << '\n';

    for (ull a = 0; a < W; a++) {
        ull x = rng.next();

        if (x % (Q + U) < Q) {
            ull l = rng.next() % N;
            ull r = l + (rng.next() % (N - l));

            if (!(a % P)){
                cout << segtree.query(l, r) << '\n';
            } 

        } else {
            ull i = rng.next() % N, v = rng.next() % (4 * N);
            segtree.update(v, i);
            if (!(a % P)){
                cout << segtree.query(i, N) << '\n';
            } 
        }
    }
    cout << '\n';
}

int main() {
    ull S;

    while (cin >> S) {
        ull N, W, Q, U, P;
        string f;

        cin >> N >> f >> W >> Q >> U >> P;

        process_case(S, N, f, W, Q, U, P);
    }

    return 0;
}