#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

// “ип дл€ переход≥в у формат≥: ¬х≥дний символ 'a' -> наступн≥ стани {1, 2}
using TransitionMap = unordered_map<int, unordered_map<char, unordered_set<int>>>;

class FiniteAutomaton {
public:
    unordered_set<int> states;                 // стани
    unordered_set<char> alphabet;              // алфав≥т
    int startState;                            // початковий стан
    unordered_set<int> finalStates;            // ф≥нальн≥ стани
    TransitionMap trans;                       // переходи

    // «читуЇ автомат ≥з файлу
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return false;
        }

        int numAlphabet, numStates, numFinalStates;
        file >> numAlphabet >> numStates >> startState >> numFinalStates;

        for (int i = 0; i < numFinalStates; ++i) {
            int finalState;
            file >> finalState;
            finalStates.insert(finalState);
        }

        // «читуЇмо переходи
        int from, to;
        char symbol;
        while (file >> from >> symbol >> to) {
            alphabet.insert(symbol);
            states.insert(from);
            states.insert(to);
            trans[from][symbol].insert(to);
        }

        file.close();
        return true;
    }

    bool isWordAccepted(const string& w0) {
        int currentState = startState;

        // ќбробка w0, щоб знайти к≥нцевий стан п≥сл€ нього
        for (char symbol : w0) {
            if (trans[currentState].count(symbol) == 0) {
                return false; 
            }
            currentState = *trans[currentState][symbol].begin(); 
        }

        return finalStates.count(currentState) > 0;
    }
};

int main() {
    string filename, w0;
    cout << "Enter the filename with the automaton: ";
    cin >> filename;
    cout << "Enter the word w0: ";
    cin >> w0;

    FiniteAutomaton fa;
    if (!fa.loadFromFile(filename)) {
        cerr << "Error loading automaton from file." << endl;
        return 1;
    }

    if (fa.isWordAccepted(w0)) {
        cout << "The automaton accepts words of the form w = w1w0." << endl;
    }
    else {
        cout << "The automaton does not accept words of the form w = w1w0." << endl;
    }

    return 0;
}
