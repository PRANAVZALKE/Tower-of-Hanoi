/*
 * Tower of Hanoi Visualizer
 * 
 * This program implements the classic Tower of Hanoi puzzle with
 * real-time ASCII visualization. It uses recursive algorithm to
 * solve the puzzle and displays each move step by step.
 * 
 * Author: [Pranav A Zalke]
 * Date: [09/08/2025]
 * Language: C++
 */

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <iomanip>

using namespace std;

// Structure to represent a peg in the Tower of Hanoi
struct Peg {
    char name;              // Name of the peg (A, B, or C)
    vector<int> disks;      // Stack of disks (represented by their sizes)
    
    // Constructor
    Peg(char n) : name(n) {}
    Peg() : name(' ') {}
};

/*
 * Function to print the current state of all three pegs
 * Parameters:
 *   A, B, C - The three pegs
 *   n - Total number of disks
 *   moveNo - Current move number (0 for initial state)
 *   moveInfo - Description of the current move
 */
void printState(const Peg &A, const Peg &B, const Peg &C, int n, 
                int moveNo = 0, string moveInfo = "") {
    
    int width = n * 2 - 1;  // Width needed for the largest disk
    
    // Lambda function to print a single disk or empty space
    auto printDisk = [&](int size) {
        if (size == 0) {
            // Print empty space with center rod
            cout << string((width - 1) / 2, ' ') << "|" << string((width - 1) / 2, ' ');
        } else {
            // Print disk with '=' characters
            int fill = size * 2 - 1;
            int pad = (width - fill) / 2;
            cout << string(pad, ' ') << string(fill, '=') << string(pad, ' ');
        }
    };
    
    // Print header
    cout << "\n";
    if (moveNo == 0) {
        cout << "🎯 Initial State:" << "\n";
    } else {
        cout << "📋 Move " << moveNo << ": " << moveInfo << "\n";
    }
    
    // Print top border
    cout << string(width * 3 + 8, '-') << "\n";
    
    // Print each level from top to bottom
    for (int level = n; level >= 1; --level) {
        for (auto peg : {A, B, C}) {
            int disk = 0;
            // Check if this peg has a disk at this level
            if ((int)peg.disks.size() >= level) {
                disk = peg.disks[level - 1];
            }
            printDisk(disk);
            cout << "   ";  // Space between pegs
        }
        cout << "\n";
    }
    
    // Print bottom border
    cout << string(width * 3 + 8, '-') << "\n";
    
    // Print peg labels
    cout << string((width - 1) / 2, ' ') << "A" 
         << string(width - 1, ' ') << "   "
         << string((width - 1) / 2, ' ') << "B" 
         << string(width - 1, ' ') << "   "
         << string((width - 1) / 2, ' ') << "C" << "\n\n";
}

/*
 * Function to move a disk from one peg to another
 * Parameters:
 *   from - Source peg
 *   to - Destination peg
 */
void moveDisk(Peg &from, Peg &to) {
    if (!from.disks.empty()) {
        to.disks.push_back(from.disks.back());
        from.disks.pop_back();
    }
}

/*
 * Helper function to get reference to the correct peg by name
 * Parameters:
 *   name - Name of the peg ('A', 'B', or 'C')
 *   A, B, C - References to the three pegs
 * Returns:
 *   Reference to the requested peg
 */
Peg& getPegRef(char name, Peg &A, Peg &B, Peg &C) {
    switch(name) {
        case 'A': return A;
        case 'B': return B;
        case 'C': return C;
        default: return A;  // Default case (should never happen)
    }
}

/*
 * Recursive function to solve Tower of Hanoi
 * Parameters:
 *   n - Number of disks to move
 *   srcName - Source peg name
 *   auxName - Auxiliary peg name
 *   destName - Destination peg name
 *   A, B, C - References to the three pegs
 *   moveCount - Reference to move counter
 *   totalDisks - Total number of disks (for display)
 */
void hanoi(int n, char srcName, char auxName, char destName,
           Peg &A, Peg &B, Peg &C, int &moveCount, int totalDisks) {
    
    // Base case: no disks to move
    if (n == 0) return;
    
    // Step 1: Move n-1 disks from source to auxiliary peg
    hanoi(n - 1, srcName, destName, auxName, A, B, C, moveCount, totalDisks);
    
    // Step 2: Move the bottom disk from source to destination
    Peg &src = getPegRef(srcName, A, B, C);
    Peg &dest = getPegRef(destName, A, B, C);
    
    moveDisk(src, dest);
    moveCount++;
    
    // Display the current state
    printState(A, B, C, totalDisks, moveCount,
               string(1, srcName) + " -> " + string(1, destName));
    
    // Add delay for visualization
    this_thread::sleep_for(chrono::milliseconds(800));
    
    // Step 3: Move n-1 disks from auxiliary to destination peg
    hanoi(n - 1, auxName, srcName, destName, A, B, C, moveCount, totalDisks);
}

/*
 * Function to display welcome message and instructions
 */
void displayWelcome() {
    cout << "\n";
    cout << "🏗️  ===================================== 🏗️\n";
    cout << "🎯        TOWER OF HANOI VISUALIZER       🎯\n";
    cout << "🏗️  ===================================== 🏗️\n\n";
    
    cout << "📋 Rules:\n";
    cout << "   • Move all disks from peg A to peg C\n";
    cout << "   • Only one disk can be moved at a time\n";
    cout << "   • A larger disk cannot be placed on a smaller disk\n\n";
    
    cout << "💡 Recommended: Start with 3-5 disks for best experience\n";
    cout << "⚠️  Warning: Each additional disk doubles the number of moves!\n\n";
}

/*
 * Function to get valid input from user
 */
int getValidInput() {
    int n;
    while (true) {
        cout << "🔢 Enter number of disks (1-8): ";
        cin >> n;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "❌ Invalid input! Please enter a number.\n\n";
            continue;
        }
        
        if (n <= 0) {
            cout << "❌ Number of disks must be positive!\n\n";
            continue;
        }
        
        if (n > 8) {
            cout << "⚠️  More than 8 disks will take a very long time!\n";
            cout << "   Are you sure? This will require " << (1 << n) - 1 << " moves!\n";
            cout << "   Continue anyway? (y/n): ";
            char choice;
            cin >> choice;
            if (choice != 'y' && choice != 'Y') {
                cout << "\n";
                continue;
            }
        }
        
        return n;
    }
}

/*
 * Main function
 */
int main() {
    // Display welcome message
    displayWelcome();
    
    // Get number of disks from user
    int n = getValidInput();
    
    // Initialize the three pegs
    Peg A('A'), B('B'), C('C');
    
    // Place all disks on peg A (largest at bottom)
    cout << "\n🔄 Setting up the puzzle...\n";
    for (int i = n; i >= 1; --i) {
        A.disks.push_back(i);
    }
    
    // Display initial state
    printState(A, B, C, n);
    
    // Wait for user to be ready
    cout << "⏳ Press Enter to start solving...";
    cin.ignore();
    cin.get();
    
    cout << "\n🚀 Starting to solve the puzzle...\n";
    
    // Solve the puzzle
    int moveCount = 0;
    auto startTime = chrono::high_resolution_clock::now();
    
    hanoi(n, 'A', 'B', 'C', A, B, C, moveCount, n);
    
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    
    // Display results
    cout << "🎉 ===================================== 🎉\n";
    cout << "🏆           PUZZLE SOLVED!              🏆\n";
    cout << "🎉 ===================================== 🎉\n\n";
    
    cout << "📊 Statistics:\n";
    cout << "   • Total moves: " << moveCount << "\n";
    cout << "   • Optimal moves: " << (1 << n) - 1 << "\n";
    cout << "   • Efficiency: " << (moveCount == (1 << n) - 1 ? "Perfect! ✅" : "Suboptimal ⚠️") << "\n";
    cout << "   • Time taken: " << duration.count() / 1000.0 << " seconds\n";
    cout << "   • Disks: " << n << "\n\n";
    
    if (moveCount == (1 << n) - 1) {
        cout << "🌟 Congratulations! The puzzle was solved optimally! 🌟\n";
    }
    
    cout << "Thank you for using Tower of Hanoi Visualizer! 👋\n\n";
    
    return 0;
}
