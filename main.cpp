#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Improved hash function to reduce collisions
int HashString(const string& value) {
    int hashCode = 0;
    int prime = 31; // Use a prime multiplier for better distribution
    for (char character : value) {
        hashCode = hashCode * prime + character;
    }
    return (int)(hashCode & 0x7fffffff); // Ensure positive value
}

int main() {
    unordered_map<int, vector<string>> passwordTable; // Hash table with collision handling
    ifstream passwordFile("rockyou.txt"); // Replace with actual file path

    if (!passwordFile) {
        cerr << "Error: Password file not found!" << endl;
        return 1;
    }

    string password;
    int collisionCount = 0;
    cout << "Reading passwords and hashing...\n";
    
    while (getline(passwordFile, password)) {
        int hashValue = HashString(password);
        
        // Check if this hash value already exists (collision detection)
        if (passwordTable.find(hashValue) != passwordTable.end()) {
            collisionCount++;
        }
        
        passwordTable[hashValue].push_back(password); // Store in vector to handle collisions
    }
    
    passwordFile.close();

    cout << "Hashing complete.\n";
    cout << "Total collisions detected: " << collisionCount << endl;

    cout << "Testing first 10 password hashes:\n";
    int count = 0;
    for (const auto& entry : passwordTable) {
        if (count++ >= 10) break;
        cout << "Hash: " << entry.first << " -> Passwords: ";
        for (const string& pass : entry.second) {
            cout << pass << " ";
        }
        cout << endl;
    }

    int userHash;
    while (true) {
        cout << "\nEnter hashed value to find original passwords (or -1 to exit): ";
        cin >> userHash;

        if (userHash == -1) break;

        if (passwordTable.find(userHash) != passwordTable.end()) {
            cout << "Possible original passwords:\n";
            for (const string& pass : passwordTable[userHash]) {
                cout << " - " << pass << endl;
            }
        } else {
            cout << "Password not found in database." << endl;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
