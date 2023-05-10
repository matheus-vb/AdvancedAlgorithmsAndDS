#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned long long ull;

int counter;

const uint32_t PRIME = pow(2, 31) - 1; // 2^31 - 1
const uint32_t A = 1664525;
const uint32_t C = 1013904223;
const long long R = pow(2, 32);

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

struct Node{
    ull val;
    int height;
    vector <Node*> next;
};

class SkipList{
    private:
        Node head;

        int randomHeight(RNG &rng){
            int l = 1;
            while(rng.next() % 100 < 50 && l <= head.height) l++;
            return l;
        }


    public:
        SkipList(ull L){
            head.height = L;
            head.next.assign(L, nullptr);
        }

        bool insert(ull val, RNG &rng) {
            vector<Node*> p = parents(val);

            if (p[0]->next[0] != nullptr && p[0]->next[0]->val == val) {
                return false;
            }

            ull h = randomHeight(rng);

            if (h > head.height) {
                head.next.resize(h, nullptr);
                head.height = h;
                p.resize(h, &head);
            }

            Node* n = new Node();

            n->height = h;
            n->next.assign(h, nullptr);
            n->val = val;

            for (ull i = 0; i < h; i++) {
                n->next[i] = p[i]->next[i];
                p[i]->next[i] = n;
            }

            return true;
        }

        bool remove(ull val){
            vector <Node *> p = parents(val);

            if(p[0]->next[0] != nullptr && p[0]->next[0]->val == val){
                Node *n = p[0]->next[0];
                
                for(int i = 0; i < n->height; i++){
                    p[i]->next[i] = n->next[i];
                }

                while(head.height > 1 && head.next.back() == nullptr){
                    head.height--;
                    head.next.pop_back();
                }

                return true;
            }

            return false;
        }

         Node* search(ull val){
            counter = 1;
            vector <Node *> p = parents(val);

            if(p[0]->next[0] != nullptr && p[0]->next[0]->val == val){
                cout << "F " << counter << " " << p[0]->next[0]->height << '\n';
                return p[0]->next[0];
            }
            else{
                cout << "F " << counter << " " << 0 << '\n';
                return nullptr;
            }
        }

        vector <Node *> parents(ull val){
            vector <Node *> p(head.height, nullptr);
            Node *cur = &head;
            for(int i = head.height - 1; i >= 0; i--){
                while(cur->next[i] != nullptr && cur->next[i]->val < val){
                    cur = cur->next[i];
                    counter++;
                }

                p[i] = cur;
            }

            return p;
        }
};

int main(){
    ull S, U, B, N, F, I, D, P;

    cin >> S >> U >> B >> N >> F >> I >> D >> P;
    
    RNG rng(S);
    SkipList sl(1);

    for(int i = 0; i < B; i++){
        ull x = rng.next() % U;
        sl.insert(x, rng);
    }

    for(int i = 0; i < N; i++){
        int x = rng.next() % (F+I+D);

        if(x < F){
            x = rng.next() % U;
            if (!(i % P)) {
                sl.search(x);
            }
        }

        else if(F <= x && x < F + I){
            x = rng.next() % U;
            if (!(i % P)) {
                std::cout << "I " << sl.insert(x, rng) << '\n';
            } else {
                sl.insert(x, rng);
            }
            
        }

        else{
            x = rng.next() % U;
            if (!(i% P)) {
                std::cout << "D " << sl.remove(x) << '\n';
            } else {
                sl.remove(x);
            }
        }
    }

    return 0;
}