void printKey(int key, int value) {
    cout << key << endl;
}
int main() {
    BKUTree<int, int>::AVLTree *tree = new BKUTree<int, int>::AVLTree();
    int keys[] = {1, 3, 5, 7, 9, 2, 4, 11, 14};
    for (int i = 0; i < 9; i++) tree->add(keys[i], keys[i] + 1);
    // tree->printTreeStructure();
    tree->remove(5);
    tree->remove(1);
    tree->add(30, 4);
    tree->remove(3);
    tree->add(25, 4);
    tree->add(3, 4);
    tree->add(16, 4);
    tree->add(15, 4);
    tree->printTreeStructure();
    cout << endl
         << tree->search(15);

    // tree->traverseNLROnAVL(printKey);
}