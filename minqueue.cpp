#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned long long ull;

ull S, B, N, P;

class MinimumQueue {
public:
    MinimumQueue() : currentIndex(0) {}

    void push(ull x) {
        queue.push_back(x);

        while (!minQueue.empty() && x <= minQueue.back().first) {
            minQueue.pop_back();
        }
        minQueue.emplace_back(x, queue.size() - 1);
    }

    void pop() {
        if (currentIndex == queue.size() - 1) return;

        if (!minQueue.empty() && minQueue.front().second == currentIndex) {
            minQueue.erase(minQueue.begin());
        }

        currentIndex++;
    }

    ull minCount() { 
        return minQueue.size();
    }

    long long minPosition() {
        return minQueue[0].second - currentIndex;
    }

private:
    vector<ull> queue;
    vector<pair<ull, ull> > minQueue;
    ull currentIndex;
};

int main() {
    const ull A = 1664525;
    const ull C = 1013904223;
    const ull R = static_cast<ull>(pow(2, 32));

    MinimumQueue minQueue;

    cin >> S >> B >> N >> P;

    for (int i = 0; i < B; i++) {
        minQueue.push(S);
        S = (A * S + C) % R;
    }

    for (int i = 0; i < N; i++) {
        if (S % 10 < P) {
            S = (A * S + C) % R;
            minQueue.push(S);
        } else {
            minQueue.pop();
        }

        cout << minQueue.minCount() << " " << minQueue.minPosition() << endl;
        S = (A * S + C) % R;
    }

    return 0;
}
