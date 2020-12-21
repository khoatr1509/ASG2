#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "BKUTRee.cpp"  //Replace test.cpp with correct file

using namespace std;

ofstream outFile("master_out.txt");

template <typename K, typename V>
void printNode(K key, V value) {
    outFile << key << ' ' << value << '\n';
}

template <typename T>
void printPath(vector<T> vec) {
    for (T e : vec) {
        outFile << e << ' ';
    }
    outFile << '\n';
}

int main() {
    ifstream inFile1("init.txt");
    ifstream inFile2("misc.txt");

    BKUTree<int, int>::AVLTree AVLtree;
    BKUTree<int, int>::SplayTree Splaytree;
    BKUTree<int, int> BKUtree(20);

    vector<int> E;
    for (string line; getline(inFile1, line);) {
        E.push_back(stoi(line));
    }

    vector<int> M;
    for (string line; getline(inFile2, line);) {
        M.push_back(stoi(line));
    }

    inFile1.close();
    inFile2.close();

    //=========================================================================================================
    cout << "[+] Testing AVL tree: Phase 1-3.\n";
    outFile << "=================================== PHASE 1 ===================================\n";

    for (int e : E) {
        AVLtree.add(e, e + e % 13);
    }

    for (int m : M) {
        AVLtree.remove(m);
    }

    AVLtree.traverseNLR(&printNode<int, int>);

    outFile << "=================================== PHASE 2 ===================================\n";

    AVLtree.clear();

    for (int e : E) {
        AVLtree.add(e, e + e % 7);
    }

    for (int m : M) {
        outFile << AVLtree.search(m) << '\n';
    }

    AVLtree.traverseNLR(&printNode<int, int>);

    outFile << "=================================== PHASE 3 ===================================\n";

    for (int m : M) {
        AVLtree.remove(m);
    }

    int catches = 0;
    for (int m : M) {
        try {
            AVLtree.search(m);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    catches = 0;
    for (int m : M) {
        try {
            AVLtree.remove(m);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    catches = 0;
    for (int e : E) {
        try {
            AVLtree.add(e, e + e % 91);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == E.size() - M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    AVLtree.traverseNLR(&printNode<int, int>);

    //=========================================================================================================
    cout << "[+] Testing SplayTree: Phase 4-6.\n";
    outFile << "=================================== PHASE 4 ===================================\n";

    for (int e : E) {
        Splaytree.add(e, e + e % 13);
    }

    for (int m : M) {
        Splaytree.remove(m);
    }

    Splaytree.traverseNLR(&printNode<int, int>);

    outFile << "=================================== PHASE 5 ===================================\n";

    Splaytree.clear();

    for (int e : E) {
        Splaytree.add(e, e + e % 7);
    }

    for (int m : M) {
        outFile << Splaytree.search(m) << '\n';
    }

    Splaytree.traverseNLR(&printNode<int, int>);

    outFile << "=================================== PHASE 6 ===================================\n";

    for (int m : M) {
        Splaytree.remove(m);
    }

    catches = 0;
    for (int m : M) {
        try {
            Splaytree.search(m);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    catches = 0;
    for (int m : M) {
        try {
            Splaytree.remove(m);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    catches = 0;
    for (int e : E) {
        try {
            Splaytree.add(e, e + e % 91);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == E.size() - M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    Splaytree.traverseNLR(&printNode<int, int>);

    //=========================================================================================================
    cout << "[+] Testing BKUTree: Phase 7-9.\n";
    outFile << "=================================== PHASE 7 ===================================\n";

    for (int e : E) {
        BKUtree.add(e, e + e % 46);
    }

    for (int m : M) {
        BKUtree.remove(m);
    }

    BKUtree.traverseNLROnAVL(&printNode<int, int>);
    BKUtree.traverseNLROnSplay(&printNode<int, int>);

    outFile << "=================================== PHASE 8 ===================================\n";

    BKUtree.clear();

    for (int e : E) {
        BKUtree.add(e, e + e % 7);
    }

    for (int m : M) {
        vector<int> path;
        outFile << BKUtree.search(m, path) << '\n';
        printPath<int>(path);
    }

    BKUtree.traverseNLROnAVL(&printNode<int, int>);
    BKUtree.traverseNLROnSplay(&printNode<int, int>);

    outFile << "=================================== PHASE 9 ===================================\n";

    for (int m : M) {
        BKUtree.remove(m);
    }

    catches = 0;
    for (int m : M) {
        try {
            vector<int> path;
            BKUtree.search(m, path);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    catches = 0;
    for (int m : M) {
        try {
            BKUtree.remove(m);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    catches = 0;
    for (int e : E) {
        try {
            BKUtree.add(e, e + e % 74);
        } catch (const char *c) {
            catches++;
        }
    }

    if (catches == E.size() - M.size()) {
        outFile << "Exceptions handled correcly !\n";
    } else {
        outFile << "Exceptions handled incorrectly !!!\n";
    }

    BKUtree.traverseNLROnAVL(&printNode<int, int>);
    BKUtree.traverseNLROnSplay(&printNode<int, int>);

    outFile.close();
}