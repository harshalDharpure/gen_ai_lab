#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Task {
    int id, val;
    vector<int> reqs;
};

// Global variables
int N = 0, K = 0, M = 0;
int total_tasks = 0;
long long sol_count = 0;
vector<Task> tasks;
vector<bool> done; // Dynamic vector to prevent memory errors
vector<vector<vector<int>>> res;

// Function to read input safely line-by-line
void load_data(string fname) {
    ifstream fin(fname);
    if (!fin) {
        cout << "Error opening file." << endl;
        exit(1);
    }

    string line;
    while (getline(fin, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        // Skip comments (lines starting with %)
        size_t start = line.find_first_not_of(" \t");
        if (start == string::npos || line[start] == '%') continue;

        stringstream ss(line);
        char type;
        ss >> type;

        if (type == 'N') {
            ss >> N;
        } else if (type == 'K') {
            ss >> K;
        } else if (type == 'A') {
            Task t;
            ss >> t.id >> t.val;
            int d;
            while (ss >> d && d != 0) {
                t.reqs.push_back(d);
            }
            tasks.push_back(t);
            total_tasks++;
        }
    }
    fin.close();
}

// Check if a task is valid to take
bool check(int idx, int rem) {
    if (done[idx] || tasks[idx].val > rem) return false;
    
    // Check dependencies
    for (int r : tasks[idx].reqs) {
        bool met = false;
        for (int i = 0; i < tasks.size(); ++i) {
            if (tasks[i].id == r && done[i]) {
                met = true; break;
            }
        }
        if (!met) return false;
    }
    return true;
}

// Print the final schedule
void print_sol() {
    cout << "--------------------------------------------------" << endl;
    cout << "Schedule #" << sol_count + 1 << endl;
    for (int i = 0; i < res.size(); ++i) {
        cout << "Day " << i + 1 << ":" << endl;
        for (int j = 0; j < N; ++j) {
            cout << "  Student " << j + 1 << ": ";
            if (res[i][j].empty()) {
                cout << "(Free)";
            } else {
                for (int id : res[i][j]) cout << "A" << id << " ";
            }
            cout << endl;
        }
    }
    cout << "--------------------------------------------------" << endl;
}

// Recursive solver (Backtracking)
void dfs(int d, int s, int cap, int idx, int cnt) {
    // Base case: All tasks assigned
    if (cnt == total_tasks) {
        print_sol();
        sol_count++;
        return;
    }

    if (d > M) return; // Exceeded max days

    // Move to next day if all students are done for current day
    if (s == N) {
        res.push_back(vector<vector<int>>(N));
        dfs(d + 1, 0, K, 0, cnt);
        res.pop_back();
        return;
    }

    // Ensure the schedule vector has space for the current day
    if (res.size() < d) res.push_back(vector<vector<int>>(N));

    // Try assigning tasks to current student
    for (int i = idx; i < tasks.size(); ++i) {
        if (check(i, cap)) {
            done[i] = true;
            res[d-1][s].push_back(tasks[i].id);

            // Recurse: same day, same student, reduced capacity
            dfs(d, s, cap - tasks[i].val, i + 1, cnt + 1);

            res[d-1][s].pop_back();
            done[i] = false;
        }
    }

    // Finished trying tasks for this student, move to next student
    dfs(d, s + 1, K, 0, cnt);

    // Cleanup: if we created a day frame here and are backtracking, remove it
    if (s == 0 && res.size() == d) res.pop_back();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input_file> <days>" << endl;
        return 1;
    }

    load_data(argv[1]);
    M = stoi(argv[2]);

    // Safety check for empty input or bad parsing
    if (N == 0) {
        cout << "Error: Invalid input file or N not found." << endl;
        return 1;
    }

    // Resize done vector based on actual tasks read
    done.resize(tasks.size(), false);

    dfs(1, 0, K, 0, 0);

    cout << "Total no of schedule: " << sol_count << endl;

    return 0;
}
