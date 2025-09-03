#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>

using namespace std;

class graph {
    int edgeCount;
    unordered_map<string, vector<pair<string, int>>> mapRomania;

public:
    graph() : edgeCount(0) {}

    void add(string s1, string s2, int weight) {
        mapRomania[s1].push_back(make_pair(s2, weight));
        mapRomania[s2].push_back(make_pair(s1, weight));
        edgeCount++;
    }

    void bfs(string start, string end, vector<string>& result) {
        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;
        queue<string> q;

        visited[start] = true;
        q.push(start);

        bool found = false;

        while (!q.empty()) {
            string frontValue = q.front();
            q.pop();

            if (frontValue == end) {
                found = true;
                break;
            }

            for (const auto& neighboor : mapRomania[frontValue]) {
                if (!visited[neighboor.first]) {
                    visited[neighboor.first] = true;
                    parent[neighboor.first] = frontValue;

                    q.push(neighboor.first);
                }
            }
        }

        if (!found) return;

        // reconstruct path
        string curr = end;
        stack<string> path;
        while (curr != start) {
            path.push(curr);
            curr = parent[curr];
        }
        path.push(start);

        while (!path.empty()) {
            result.push_back(path.top());
            path.pop();
        }
    }

    void dfs(string start, string end, vector<string>& result) {
        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;
        stack<string> stk;

        visited[start] = true;
        stk.push(start);

        bool found = false;

        while(!stk.empty()) {
            string topValue = stk.top();
            stk.pop();

            if (topValue == end) {
                found = true;
                break;
            }

            for (const auto& neighboor : mapRomania[topValue]) {
                if (!visited[neighboor.first]) {
                    parent[neighboor.first] = topValue;
                    visited[neighboor.first] = true;
                    
                    stk.push(neighboor.first);
                }
            }
        }

        if (!found) return;

        // reconstruct path
        stack<string> path;
        string curr = end;

        while (curr != start) {
            path.push(curr);
            curr = parent[curr];
        }

        while (!path.empty()) {
            result.push_back(path.top());
            path.pop();
        }
    }

    void ucs(string start, string end, vector<string> &result, int &totalcost) { // Uniform Cost Search (Dijkstra)
        unordered_map<string, int> cost;
        unordered_map<string, string> parent;
        
        typedef pair<int, string> PQNode; // (cost, node)
        priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

        cost[start] = 0;
        pq.push({0, start});

        bool found = false;

        while (!pq.empty()) {
            auto [currCost, currNode] = pq.top();
            pq.pop();

            if (currNode == end) {
                found = true;
                break;
            }

            for (const auto& neighbor : mapRomania[currNode]) {
                int newCost = currCost + neighbor.second;
                if (!cost.count(neighbor.first) || newCost < cost[neighbor.first]) {  // is visited and lowest cost
                    cost[neighbor.first] = newCost;
                    parent[neighbor.first] = currNode;
                    pq.push({newCost, neighbor.first});
                }
            }
        }

        if (!found) return;

        // reconstruct path
        string curr = end;
        stack<string> path;
        while (curr != start) {
            path.push(curr);
            curr = parent[curr];
        }
        path.push(start);

        while (!path.empty()) {
            result.push_back(path.top());
            path.pop();
        }
        totalcost = cost[end];
    }
};

int main() {
    int n; 
    cin >> n;
    
    graph g;
    for (int i = 0; i < n; i++) {
        string s1, s2;
        int w;
    
        cin >> s1 >> s2 >> w;
        g.add(s1, s2, w);
    }

    string start, end;
    cin >> start >> end; 

    vector<string> result_bfs;
    g.bfs(start, end, result_bfs);

    cout << "bfs output:\n";
    if (result_bfs.empty()) {
        cout << "No path found\n";
    } else {
        for (auto it : result_bfs) {
            cout << it << ' ';
        }
        cout << endl;
    }

    vector<string> result_dfs;
    g.dfs(start, end, result_dfs);

    cout << "\ndfs output:\n";
    if (result_dfs.empty()) {
        cout << "No path found\n";
    } else {
        for (auto it : result_dfs) {
            cout << it << ' ';
        }
        cout << endl;
    }

    vector<string> result_ucs;
    int cost = 0;
    g.ucs(start, end, result_ucs, cost);

    cout << "\nucs output: \n";
    if (result_ucs.empty()) {
        cout << "No path found\n";
    } else {
        for (auto it : result_ucs) {
            cout << it;
            (it == end) ? cout << "\nTotal cost: " << cost : cout << ' ';
        }
        cout << endl;
    }
}

/**
23
Arad Zerind 75
Arad Sibiu 140
Arad Timisoara 118
Zerind Oradea 71
Oradea Sibiu 151
Timisoara Lugoj 111
Lugoj Mehadia 70
Mehadia Dobreta 75
Dobreta Craiova 120
Craiova Rimnicu_Vilcea 146
Rimnicu_Vilcea Sibiu 80
Craiova Pitesti 138
Rimnicu_Vilcea Pitesti 97
Sibiu Fagaras 99
Fagaras Bucharest 211
Pitesti Bucharest 101
Bucharest Giurgiu 90
Bucharest Urziceni 85
Urziceni Hirsova 98
Hirsova Eforie 86
Urziceni Vaslui 142
Vaslui Lasi 92
Lasi Neamt 87
Arad Bucharest

 */

/*
Output:
bfs output:
Arad Sibiu Fagaras Bucharest 

dfs output:
Timisoara Lugoj Mehadia Dobreta Craiova Pitesti Bucharest 

ucs output: 
Arad Sibiu Rimnicu_Vilcea Pitesti Bucharest
Total cost: 418

*/

