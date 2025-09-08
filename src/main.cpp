#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

struct Peg {
    char name;
    vector<int> disks;
};

void printState(const Peg &A, const Peg &B, const Peg &C, int n, int moveNo = 0, string moveInfo = "") {
    int width = n * 2 - 1;
    auto printDisk = [&](int size) {
        if (size == 0)
            cout << string((width - 1) / 2, ' ') << "|" << string((width - 1) / 2, ' ');
        else {
            int fill = size * 2 - 1;
            int pad = (width - fill) / 2;
            cout << string(pad, ' ') << string(fill, '=') << string(pad, ' ');
        }
    };
    
    cout << "\n";
    if (moveNo == 0) cout << "Initial State:\n";
    else cout << "Move " << moveNo << ": " << moveInfo << "\n";
    cout << string(width * 3 + 8, '-') << "\n";
    
    for (int level = n; level >= 1; --level) {
        for (auto peg : {A, B, C}) {
            int disk = 0;
            if ((int)peg.disks.size() >= level)
                disk = peg.disks[level - 1];
            printDisk(disk);
            cout << "   ";
        }
        cout << "\n";
    }
    
    cout << string(width * 3 + 8, '-') << "\n";
    cout << string((width - 1) / 2, ' ') << "A" 
         << string(width - 1, ' ') << "   "
         << string((width - 1) / 2, ' ') << "B" 
         << string(width - 1, ' ') << "   "
         << string((width - 1) / 2, ' ') << "C" << "\n\n";
}

void moveDisk(Peg &from, Peg &to) {
    if (!from.disks.empty()) {
        to.disks.push_back(from.disks.back());
        from.disks.pop_back();
    }
}

// Function to get reference to the correct peg
Peg& getPegRef(char name, Peg &A, Peg &B, Peg &C) {
    if (name == 'A') return A;
    if (name == 'B') return B;
    return C;
}

void hanoi(int n, char srcName, char auxName, char destName,
           Peg &A, Peg &B, Peg &C, int &moveCount, int totalDisks) {
    if (n == 0) return;
    
    // Recursive call: move n-1 disks from source to auxiliary
    hanoi(n - 1, srcName, destName, auxName, A, B, C, moveCount, totalDisks);
    
    // Move the bottom disk from source to destination
    Peg &src = getPegRef(srcName, A, B, C);
    Peg &dest = getPegRef(destName, A, B, C);
    
    moveDisk(src, dest);
    moveCount++;
    
    // Print current state
    printState(A, B, C, totalDisks, moveCount,
               string(1, srcName) + " -> " + string(1, destName));
    this_thread::sleep_for(chrono::milliseconds(500));
    
    // Recursive call: move n-1 disks from auxiliary to destination
    hanoi(n - 1, auxName, srcName, destName, A, B, C, moveCount, totalDisks);
}

int main() {
    int n;
    cout << "Enter number of disks: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Invalid input.\n";
        return 0;
    }
    
    Peg A{'A'}, B{'B'}, C{'C'};
    
    // Initialize peg A with disks (largest at bottom)
    for (int i = n; i >= 1; --i) {
        A.disks.push_back(i);
    }
    
    printState(A, B, C, n);
    
    int moveCount = 0;
    hanoi(n, 'A', 'B', 'C', A, B, C, moveCount, n);
    
    cout << "\nSolved in " << moveCount 
         << " moves. (Optimal = " << (1 << n) - 1 << ")\n";
    
    return 0;
}
