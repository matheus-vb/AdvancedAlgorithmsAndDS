#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

struct SccComparator {
    bool operator()(const vector<int>& a, const vector<int>& b) const {
        return a[0] < b[0];
    }
};

void firstDFS(int u, const vector<vector<int> >& adjList, vector<bool>& visited, stack<int>& stack) {
    visited[u] = true;
    for (int v : adjList[u]) {
        if (!visited[v]) {
            firstDFS(v, adjList, visited, stack);
        }
    }
    stack.push(u);
}

void secondDFS(int u, const vector<vector<int> >& reverse_adjList, vector<bool>& visited, vector<int>& scc) {
    visited[u] = true;
    scc.push_back(u);
    for (int v : reverse_adjList[u]) {
        if (!visited[v]) {
            secondDFS(v, reverse_adjList, visited, scc);
        }
    }
}

vector<vector<int> > updateRevList(vector<vector<int> > list, int i, vector<vector<int> > revList) {
    for(int j = 0; j < i; j++){
        for (int v : list[j]) {
            revList[v].push_back(j);
        }
    }

    return revList;
}

vector<vector<int> > kosaraju(const vector<vector<int> >& adjList) {
    int n = adjList.size();
    vector<bool> visited(n, false);
    stack<int> stack;

    for (int u = 0; u < n; u++) {
        if (!visited[u]) {
            firstDFS(u, adjList, visited, stack);
        }
    }

    vector<vector<int> > reverse_adjList(n);

    reverse_adjList = updateRevList(adjList, n, reverse_adjList);

    visited.assign(n, false);

    vector<vector<int> > sccs;

    while (!stack.empty()) {
        int u = stack.top();
        stack.pop();
        if (!visited[u]) {
            vector<int> scc;
            secondDFS(u, reverse_adjList, visited, scc);
            sort(scc.begin(), scc.end());
            sccs.push_back(scc);
        }
    }

    sort(sccs.begin(), sccs.end(), SccComparator());

    return sccs;
}

void solve(int C) {

    while (C) {
        int nodeCount;
        
        cin >> nodeCount;

        vector<vector<int> > adjList(nodeCount);

        for (int u = 0; u < nodeCount; u++) {
            uint32_t node, out_degree;
            char checker;
            
            cin >> node >> out_degree >> checker;

            while (out_degree--) {
                int successors;
                cin >> successors;
                adjList[u].push_back(successors);
            }
        }
        vector<vector<int> > sccs = kosaraju(adjList);
        cout << sccs.size() << '\n';
        for (const vector<int>& scc : sccs) {
            for(int j=0; j < scc.size(); j++){
                if(j == 0) cout << scc[0];
                else cout << " " << scc[j];
            }
            cout << '\n';
        }

        C--;

        cout << '\n';
    }
}

int main() {
    int C;

    cin >> C;

    solve(C);

    return 0;
}