#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>

using namespace std;

struct Task {
    string id;
    int burst, remaining, pos;
    vector<string> memoryBlocks;
};

class Cache {
private:
    int capacity;
    deque<string> blocks;
public:
    Cache(int cap) { capacity = cap; }
    
    bool contains(string block) {
        auto it = find(blocks.begin(), blocks.end(), block);
        if(it != blocks.end()) {
            blocks.erase(it);
            blocks.push_back(block);
            return true;
        }
        return false;
    }
    
    void insert(string block) {
        if(contains(block)) return; 
        if((int)blocks.size() >= capacity) blocks.pop_front();
        blocks.push_back(block);
    }
    
    void print(string name) {
        cout << name << ": [";
        for(int i = 0; i < (int)blocks.size(); i++) {
            cout << blocks[i] << (i != (int)blocks.size() - 1 ? ", " : "");
        }
        cout << "]";
    }
};

class Scheduler {
public:
    void run(vector<Task>& tasks, int quantum) {
        Cache L1(32), L2(128), L3(512);
        int cycle = 0, ramAccesses = 0;
        queue<int> q;
        
        for(int i = 0; i < (int)tasks.size(); i++) q.push(i);

        while(!q.empty()) {
            int idx = q.front();
            q.pop();
            Task &curr = tasks[idx];
            int run_time = min(quantum, curr.remaining);

            for(int i = 0; i < run_time; i++) {
                cout << "\nRunning : " << curr.id << "\n";
                if(curr.memoryBlocks.empty()) {
                    cycle++;
                    cout << "Cycle " << cycle << "\nNo memory requests. Executing instructions...\n";
                } else {
                    string block = curr.memoryBlocks[curr.pos % curr.memoryBlocks.size()];
                    curr.pos++;
                    cout << "Requesting : " << block << "\n";

                    if(L1.contains(block)) {
                        cycle += 4;
                        cout << "L1 HIT (4 cycles) -> Cycle: " << cycle << "\n";
                    } else if(L2.contains(block)) {
                        cycle += 12;
                        cout << "L2 HIT (12 cycles) -> Cycle: " << cycle << "\n";
                        L1.insert(block);
                    } else if(L3.contains(block)) {
                        cycle += 40;
                        cout << "L3 HIT (40 cycles) -> Cycle: " << cycle << "\n";
                        L1.insert(block);
                    } else {
                        cycle += 200;
                        cout << "MISS -> RAM ACCESS (200 cycles) -> Cycle: " << cycle << "\n";
                        ramAccesses++;
                        L3.insert(block);
                        L2.insert(block);
                        L1.insert(block);
                    }
                    L1.print("L1"); cout << "\n";
                    L2.print("L2"); cout << "\n";
                    L3.print("L3"); cout << "\n";
                }
                curr.remaining--;
                if(curr.remaining == 0) break;
            }
            if(curr.remaining > 0) q.push(idx);
        }

        cout << "\n======================\nFinal Results\n======================\n";
        cout << "Total Cycles : " << cycle << "\nTasks Completed : " << tasks.size() << "\n";
        cout << "Scheduler : Round Robin\nQuantum : " << quantum << "\n";
        cout << "Bonus Policy : LRU Cache\n";
        cout << "RAM Accesses : " << ramAccesses << "\n";
    }
};

int main() {
    ifstream file("input_task2.txt");
    if(!file) {
        cout << "Input file not found\n";
        return 0;
    }

    vector<Task> tasks;
    string line;
    while(getline(file, line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        Task t;
        string temp;
        ss >> temp >> t.id >> temp >> t.burst;
        t.remaining = t.burst;
        t.pos = 0;
        ss >> temp;
        while(ss >> temp) t.memoryBlocks.push_back(temp);
        tasks.push_back(t);
    }

    Scheduler sched;
    sched.run(tasks, 3);

    return 0;
}