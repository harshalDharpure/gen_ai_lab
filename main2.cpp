#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cerr << "Cannot open input.txt" << endl;
        return 1;
    }

    string line;
    int N = 0, K = 0;
    vector<int> prompts;
    vector<vector<int>> graph;
    vector<int> indegree;

    while (getline(fin, line)) {
        stringstream ss(line);
        string token;
        ss >> token;

        if (token == "%" || token.empty()) continue;

        if (token == "N") {
            ss >> N;
        } else if (token == "K") {
            ss >> K;
        } else if (token == "A") {
            int id, prompt;
            ss >> id >> prompt;

            // Make sure vector is large enough (id starts from 1)
            int idx = id - 1;
            if (idx >= (int)prompts.size()) {
                prompts.resize(idx + 1);
                graph.resize(idx + 1);
                indegree.resize(idx + 1, 0);
            }

            prompts[idx] = prompt;

            // Read dependencies until 0
            int dep;
            while (ss >> dep) {
                if (dep == 0) break;
                int pre_idx = dep - 1;
                graph[pre_idx].push_back(idx);
                indegree[idx]++;
            }
        }
    }

    fin.close();

    if (N == 0 || K == 0 || prompts.empty()) {
        cerr << "Invalid input: missing N, K or assignments" << endl;
        return 1;
    }

    int total_capacity = N * K;
    int n = prompts.size();
    vector<int> remaining = prompts;  // copy

    queue<int> ready;
    vector<bool> processed(n, false);

    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            ready.push(i);
        }
    }

    int days = 0;
    int completed = 0;

    while (completed < n) {
        days++;
        int cap = total_capacity;
        vector<int> finished_today;

        // Work with current ready tasks (copy to avoid modifying while iterating)
        vector<int> available;
        set<int> seen;  // prevent duplicates this day

        queue<int> temp = ready;
        while (!temp.empty()) {
            int u = temp.front(); temp.pop();
            if (!processed[u] && remaining[u] > 0 && seen.find(u) == seen.end()) {
                available.push_back(u);
                seen.insert(u);
            }
        }

        // Prefer smaller tasks first
        sort(available.begin(), available.end(), [&](int a, int b) {
            return remaining[a] < remaining[b];
        });

        for (int u : available) {
            if (cap <= 0) break;
            if (remaining[u] <= 0) continue;

            int take = min(cap, remaining[u]);
            remaining[u] -= take;
            cap -= take;

            if (remaining[u] <= 0) {
                finished_today.push_back(u);
                completed++;
                processed[u] = true;
            }
        }

        // Unlock new tasks
        for (int u : finished_today) {
            for (int v : graph[u]) {
                indegree[v]--;
                if (indegree[v] == 0 && !processed[v]) {
                    ready.push(v);
                }
            }
        }

        // Clean up finished tasks from queue
        while (!ready.empty() && processed[ready.front()]) {
            ready.pop();
        }

        // Detect stuck situation
        if (finished_today.empty() && !available.empty()) {
            cerr << "Error: capacity too small to finish any remaining task" << endl;
            return 1;
        }
        if (finished_today.empty() && completed < n) {
            cerr << "Error: possible cycle or no progress" << endl;
            return 1;
        }
    }

    cout << days << endl;
    return 0;
}
