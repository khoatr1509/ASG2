#include <math.h>

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

template <class K, class V>
class BKUTree {
   public:
    class AVLTree;

    class SplayTree;

    class Entry {
       public:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) {
        }
    };

   private:
    AVLTree *avl;
    SplayTree *splay;
    queue<K> keys;
    int maxNumOfKeys;

   public:
    BKUTree(int maxNumOfKeys = 5) {
        this->maxNumOfKeys = maxNumOfKeys;
        avl = new AVLTree();
        splay = new SplayTree();
    }

    ~BKUTree() {
        this->clear();
    }

    void add(K key, V value) {
        Entry *entry = new Entry(key, value);
        typename SplayTree::Node *splayNode = splay->add(entry);
        typename AVLTree::Node *avlNode = avl->add(entry);
        avlNode->corr = splayNode;
        splayNode->corr = avlNode;
        if ((int)keys.size() == maxNumOfKeys) {
            keys.pop();
            keys.push(key);
        } else
            keys.push(key);
    }

    void remove(K key) {
        splay->removeNode_NOTDELETE(splay->root, key);
        avl->remove(key);
        queue<V> newQueue;
        while (!keys.empty()) {
            if (keys.front() != key) {
                newQueue.push(keys.front());
            }
            keys.pop();
        }
        keys = newQueue;
    }

    V search(K key, vector<K> &traversedList) {
        V retV;
        bool didSplay = false;
        bool didHaveV = false;
        queue<K> newQ = keys;
        if (splay->root->entry->key == key) return splay->root->entry->value;
        while (!newQ.empty()) {
            if (newQ.front() == key) {
                searchSplay(splay->root, key, traversedList, didSplay, didHaveV, retV);
                if ((int)keys.size() == maxNumOfKeys) {
                    keys.pop();
                    keys.push(key);
                } else
                    keys.push(key);
                return retV;
            }
            newQ.pop();
        }
        typename AVLTree::Node *AVLnode = splay->root->corr;
        searchAVL(AVLnode, key, didHaveV, retV, traversedList);
        if (didHaveV) {
            bool didSplay = false;
            bool didHaveV = false;
            this->splay->searchNode(splay->root, key, didHaveV, retV, didSplay);
            if ((int)keys.size() == maxNumOfKeys) {
                keys.pop();
                keys.push(key);
            } else
                keys.push(key);
            return retV;
        } else {
            searchAVLfromRoot(avl->root, key, didHaveV, retV, traversedList, AVLnode);
            if (didHaveV) {
                bool didSplay = false;
                bool didHaveV = false;
                this->splay->searchNode(splay->root, key, didHaveV, retV, didSplay);
                if ((int)keys.size() == maxNumOfKeys) {
                    keys.pop();
                    keys.push(key);
                } else
                    keys.push(key);
                return retV;
            } else
                throw string("Not found");
        }
    }

    void
    searchAVLfromRoot(typename AVLTree::Node *p, K &key, bool &didHaveV, V &retV, vector<K> &traversedList, typename AVLTree::Node *&AVLnode) {
        if (p) {
            if (p == AVLnode) {
                throw string("Not found");
            }
            if (p->entry->key < key) {
                traversedList.push_back(p->entry->key);
                searchAVL(p->right, key, didHaveV, retV, traversedList);
            } else if (p->entry->key > key) {
                traversedList.push_back(p->entry->key);
                searchAVL(p->left, key, didHaveV, retV, traversedList);
            } else {
                retV = p->entry->value;
                didHaveV = true;
                return;
            }
        }
    }

    void searchAVL(typename AVLTree::Node *p, K &key, bool &didHaveV, V &retV, vector<K> &traversedList) {
        if (p) {
            if (p->entry->key < key) {
                traversedList.push_back(p->entry->key);
                searchAVL(p->right, key, didHaveV, retV, traversedList);
            } else if (p->entry->key > key) {
                traversedList.push_back(p->entry->key);
                searchAVL(p->left, key, didHaveV, retV, traversedList);
            } else {
                retV = p->entry->value;
                didHaveV = true;

                return;
            }
        }
    }

    void
    searchSplay(typename SplayTree::Node *&p, K key, vector<K> &traversedList, bool &didSplay, bool &didHaveV, V &retV) {
        if (p) {
            if (p->entry->key > key) {
                traversedList.push_back(p->entry->key);
                searchSplay(p->left, key, traversedList, didSplay, didHaveV, retV);
            } else if (p->entry->key < key) {
                traversedList.push_back(p->entry->key);
                searchSplay(p->right, key, traversedList, didSplay, didHaveV, retV);
            } else {
                didHaveV = true;
                retV = p->entry->value;
                return;
            }
            if (!didSplay && didHaveV) {
                p = splay->SplayNode(p, key, didSplay);
                return;
            }
        }
    }

    void traverseNLROnAVL(void (*func)(K key, V value)) {
        avl->traverseNLR(func);
    }

    void traverseNLROnSplay(void (*func)(K key, V value)) {
        splay->traverseNLR(func);
    }

    void clear() {
        delete avl;
        delete splay;
        this->avl = new AVLTree();
        this->splay = new SplayTree();
        this->keys = {};
    }

    class SplayTree {
       public:
        friend class AVLTree;

        friend class BKUTree;

       public:
        class Node {
            friend class SplayTree;

            friend class BKUTree;

            Entry *entry;
            Node *left;
            Node *right;
            typename AVLTree::Node *corr;

            Node(Entry *entry = NULL, Node *left = NULL, Node *right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }
        };

       public:
        int getHeightRec(Node *node) {
            if (node == NULL)
                return 0;
            int lh = this->getHeightRec(node->left);
            int rh = this->getHeightRec(node->right);
            return (lh > rh ? lh : rh) + 1;
        }

        int getHeight() {
            return this->getHeightRec(this->root);
        }

        void printNSpace(int n) {
            for (int i = 0; i < n - 1; i++)
                cout << " ";
        }

        void printInteger(int &n) {
            cout << n << " ";
        }

        void printTreeStructure() {
            int height = this->getHeight();
            if (this->root == NULL) {
                cout << "NULL\n";
                return;
            }
            queue<Node *> q;
            q.push(root);
            Node *temp;
            int count = 0;
            int maxNode = 1;
            int level = 0;
            int space = pow(2, height);
            printNSpace(space / 2);
            while (!q.empty()) {
                temp = q.front();
                q.pop();
                if (temp == NULL) {
                    cout << " ";
                    q.push(NULL);
                    q.push(NULL);
                } else {
                    cout << temp->entry->key;
                    q.push(temp->left);
                    q.push(temp->right);
                }
                printNSpace(space);
                count++;
                if (count == maxNode) {
                    cout << endl;
                    count = 0;
                    maxNode *= 2;
                    level++;
                    space /= 2;
                    printNSpace(space / 2);
                }
                if (level == height)
                    return;
            }
        }

       public:
        Node *root;

        SplayTree() : root(NULL){};

        ~SplayTree() {
            this->clear();
        };

        /////////////////////////////////
        Node *RotateRight(Node *X) {
            Node *Y = X->left;
            X->left = Y->right;
            Y->right = X;
            return Y;
        }

        Node *RotateLeft(Node *Y) {
            Node *X = Y->right;
            Y->right = X->left;
            X->left = Y;
            return X;
        }

        Node *SplayNode(Node *p, K v, bool &didSplay) {
            if (root->entry->value == v) {
                didSplay = true;
                return p;
            }
            if (this->root->left) {
                if (this->root->left->entry->key == v) {
                    p = RotateRight(this->root);
                    didSplay = true;
                    return p;
                }
            }
            if (this->root->right) {
                if (this->root->right->entry->key == v) {
                    p = RotateLeft(this->root);
                    didSplay = true;
                    return p;
                }
            }

            // TH ROOT -> LEFT->LEFT/RIGHR
            if (p->left) {
                if (p->left->left) {
                    if (p->left->left->entry->key == v) {
                        p = RotateRight(p);
                        p = RotateRight(p);
                        didSplay = true;

                        return p;
                    }
                    //LEFT RIGHT
                }
                if (p->left->right) {
                    if (p->left->right->entry->key == v) {
                        p->left = RotateLeft(p->left);
                        p = RotateRight(p);
                        didSplay = true;

                        return p;
                    }
                }
                //TH ROOT_> RIGHT->LEFT/RIGHR
            }
            if (p->right) {
                if (p->right->right) {
                    if (p->right->right->entry->key == v) {
                        p = RotateLeft(p);
                        p = RotateLeft(p);
                        didSplay = true;

                        return p;
                    }
                }
                if (p->right->left) {
                    if (p->right->left->entry->key == v) {
                        p->right = RotateRight(p->right);
                        p = RotateLeft(p);
                        didSplay = true;

                        return p;
                    }
                }
            }

            return p;
        }

        /////////////////////////////ADD///////////////////////////////////////////////////////////////////
        void addNode(Node *&root, Entry *entry, Node *&getNode) {
            if (root == NULL) {
                root = new Node(entry);
                getNode = root;
                search(entry->key);
            } else {
                if (entry->key == root->entry->key)
                    throw string("Duplicate key");
                else if (entry->key < root->entry->key) {
                    addNode(root->left, entry, getNode);
                } else
                    addNode(root->right, entry, getNode);
            }
        }

        Node *add(K key, V value) {
            Entry *newEntry = new Entry(key, value);
            return add(newEntry);
        }

        Node *add(Entry *entry) {
            Node *newNode = new Node();
            this->addNode(this->root, entry, newNode);
            return newNode;
        }

        ////////////////REMOVE///////////////////////////////
        Node *findMax(Node *p) {
            while (p->right) {
                p = p->right;
            }
            return p;
        }

        //KHONG DELETE ROOT
        void removeNode_NOTDELETE(Node *&p, K value) {
            if (p == NULL)
                return;
            else {
                if (search(value)) {
                    Node *TreeLeft = this->root->left;
                    Node *TreeRight = this->root->right;
                    if (TreeLeft == NULL && TreeRight != NULL) {
                        this->root = TreeRight;
                    } else if (TreeRight == NULL && TreeLeft != NULL) {
                        this->root = TreeLeft;
                    } else {
                        //TIM vaLUE MAx Va RETURN
                        Node *maxNode = findMax(TreeLeft);
                        this->root = TreeLeft;
                        this->search(maxNode->entry->key);
                        this->root->right = TreeRight;
                    }
                }
            }
        }

        //DELETE ROOT
        void remove(K value) {
            if (root == NULL)
                return;
            else {
                if (search(value)) {
                    Node *p = this->root;
                    Node *TreeLeft = this->root->left;
                    Node *TreeRight = this->root->right;
                    if (TreeLeft == NULL && TreeRight != NULL) {
                        this->root = TreeRight;
                        delete p;

                    } else if (TreeRight == NULL && TreeLeft != NULL) {
                        this->root = TreeLeft;
                        delete p;

                    } else {
                        Node *maxNode = findMax(TreeLeft);
                        this->root = TreeLeft;
                        search(maxNode->entry->key);
                        this->root->right = TreeRight;
                        delete p;
                    }
                }
            }
        }

        //////////////////////SEARCH///////////////////////////////////////////////////////////////////////////////
        void searchNode(Node *&p, K &k, bool &didhaveV, V &v, bool &didSplay) {
            if (p) {
                if (p->entry->key > k) {
                    searchNode(p->left, k, didhaveV, v, didSplay);
                } else if (p->entry->key < k) {
                    searchNode(p->right, k, didhaveV, v, didSplay);
                } else {
                    v = p->entry->value;
                    didhaveV = true;
                    return;
                }
                if (didhaveV && !didSplay) {
                    p = SplayNode(p, k, didSplay);
                    return;
                }
            } else
                return;
        }

        V search(K key) {
            bool didSplay = false;  // DA SPLAY CHUA
            bool didhaveV = false;  //kiemtra viec SPLAY 1 lan
            V Nodevalue = 0;
            searchNode(root, key, didhaveV, Nodevalue, didSplay);
            if (!didhaveV) throw("Not found");
            while (this->root->entry->key != key) {
                didhaveV = false;
                didSplay = false;
                this->searchNode(this->root, key, didhaveV, Nodevalue, didSplay);
            }
            return Nodevalue;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////TRAVERSE//////////////////////////////
        void Traverse(void (*func)(K key, V value), Node *p) {
            if (p) {
                func(p->entry->key, p->entry->value);
                Traverse(func, p->left);
                Traverse(func, p->right);
            }
        }

        void traverseNLR(void (*func)(K key, V value)) {
            this->Traverse(func, this->root);
        }

        ///////////////////////////////CLEAR////////////////////////////////////////////////////////////////////
        void clearSplay(Node *p) {
            if (p) {
                clearSplay(p->right);
                clearSplay(p->left);
                delete p;
            } else
                return;
        }

        void clear() {
            clearSplay(this->root);
        }
    };

    class AVLTree {
       public:
        class Node {
            friend class AVLTree;

            friend class BKUTree;

            Entry *entry;
            Node *left;
            Node *right;
            int balance;
            int height;
            typename SplayTree::Node *corr;

            Node(Entry *entry = NULL, Node *left = NULL, Node *right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->height = 0;
                this->balance = 0;
                this->corr = NULL;
            }
        };

       public:
        Node *root;

        AVLTree() : root(NULL){};

        ~AVLTree() {
            this->clear();
        };

        ///////////////////////////////////////////ROTATE//////////////////////////////////////////////////////////////
        Node *LeftRotate(Node *X) {
            Node *Y = X->right;
            X->right = Y->left;
            Y->left = X;
            X->height = findHeight(X);
            Y->height = findHeight(Y);
            X->balance = findBalance(X);
            Y->balance = findBalance(Y);
            return Y;
        }

        Node *RightRotate(Node *Y) {
            Node *X = Y->left;
            Y->left = X->right;
            X->right = Y;
            Y->height = findHeight(Y);
            X->height = findHeight(X);
            Y->balance = findBalance(Y);
            X->balance = findBalance(X);
            return X;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////ADD///////////////////////////////////////////////////////////////////////////////////////

        int Max(int a, int b) {
            if (a <= b)
                return b;
            return a;
        }

        int findBalance(Node *p) {
            if (p == NULL) return 0;
            return getHeight(p->left) - getHeight(p->right);
        }

        int findHeight(Node *p) {
            if (p == NULL) return 0;
            return Max(getHeight(p->left), getHeight(p->right)) + 1;
        }

        int getHeight(Node *p) {
            if (p == NULL) return 0;
            return p->height;
        }

        void insert(Node *&p, Entry *E, Node *&getNode) {
            if (p == NULL) {
                p = new Node(E);
                getNode = p;
            } else {
                if (p->entry->key > E->key)
                    insert(p->left, E, getNode);
                else if (p->entry->key < E->key)
                    insert(p->right, E, getNode);
                else
                    throw runtime_error("Duplicate key");
            }
            p->height = findHeight(p);
            p->balance = findBalance(p);
            if (findBalance(p) > 1 && findBalance(p->left) >= 0) {
                p = RightRotate(p);
                return;
            }
            if (findBalance(p) > 1 && findBalance(p->left) < 0) {
                p->left = LeftRotate(p->left);
                p = RightRotate(p);
                return;
            }
            if (findBalance(p) < -1 && findBalance(p->right) <= 0) {
                p = LeftRotate(p);
                return;
            }
            if (findBalance(p) < -1 && findBalance(p->right) > 0) {
                p->right = RightRotate(p->right);
                p = LeftRotate(p);
                return;
            }
        }

        void add(K key, V value) {
            Entry *newEntry = new Entry(key, value);
            add(newEntry);
        }

        Node *add(Entry *entry) {
            Node *newNode = new Node();
            this->insert(this->root, entry, newNode);
            return newNode;
        }

        ///////////////////////////////////////////////REMOVE///////////////////////////////
        void removeNode(Node *&p, K value) {
            if (p == NULL)
                return;
            if (value < p->entry->key)
                removeNode(p->left, value);
            else if (value > p->entry->key)
                removeNode(p->right, value);
            else {
                // NO CHILD
                if (p->left == NULL && p->right == NULL) {
                    delete p;
                    p = NULL;
                }  // 1 CHILD
                else if ((p->left == NULL && p->right != NULL)) {
                    Node *temp = p;
                    p = p->right;
                    delete temp;

                } else if (p->right == NULL && p->left != NULL) {
                    Node *temp = p;
                    p = p->left;
                    delete temp;
                } else {  //both node right and left

                    Node *temp = p->left;
                    while (temp->right) {
                        temp = temp->right;
                    }
                    p->entry->key = temp->entry->key;
                    p->entry->value = temp->entry->value;
                    p->corr = temp->corr;
                    p->corr->corr = p;
                    removeNode(p->left, temp->entry->key);
                }
            }
            if (p) {
                p->height = findHeight(p);
                p->balance = findBalance(p);
                if (findBalance(p) > 1 && findBalance(p->left) >= 0) {
                    p = RightRotate(p);
                    return;
                }
                if (findBalance(p) > 1 && findBalance(p->left) < 0) {
                    p->left = LeftRotate(p->left);
                    p = RightRotate(p);
                    return;
                }
                if (findBalance(p) < -1 && findBalance(p->right) <= 0) {
                    p = LeftRotate(p);
                    return;
                }
                if (findBalance(p) < -1 && findBalance(p->right) > 0) {
                    p->right = RightRotate(p->right);
                    p = LeftRotate(p);
                    return;
                }
            }
        }

        void remove(K key) {
            removeNode(this->root, key);
        }

        /////////////////////////////////////////////////////////////////////
        V search(K key) {
            V value = V();
            bool didHaveValue = false;
            searchNode(this->root, key, didHaveValue, value);
            return didHaveValue ? value : throw runtime_error("Not found");
        }

        void searchNode(Node *p, K &key, bool &didHaveV, V &retV) {
            if (p) {
                if (p->entry->key > key) {
                    searchNode(p->left, key, didHaveV, retV);
                } else if (p->entry->key < key) {
                    searchNode(p->right, key, didHaveV, retV);
                } else {
                    retV = p->entry->value;
                    didHaveV = true;
                    return;
                }
            }
        }

        /////////////////////////////TRAVERSE//////////////////////////////
        void Traverse(void (*func)(K key, V value), Node *p) {
            if (p) {
                func(p->entry->key, p->entry->value);
                Traverse(func, p->left);
                Traverse(func, p->right);
            }
        }

        void traverseNLR(void (*func)(K key, V value)) {
            this->Traverse(func, this->root);
        }

        /////////////////////////////////CLEAR//////////////////////////////////////
        void
        clearNode(Node *p) {
            if (p) {
                clearNode(p->right);
                clearNode(p->left);
                delete p;
            } else
                return;
        }

        void clear() {
            clearNode(root);
        }
        ////////////////////////////////////////////////////////////
        void printNSpace(int n) {
            for (int i = 0; i < n - 1; i++)
                cout << " ";
        }

        void printInteger(int &n) {
            cout << n << " ";
        }
        void printTreeStructure() {
            int height = this->getHeight(this->root);
            if (this->root == NULL) {
                cout << "NULL\n";
                return;
            }
            queue<Node *> q;
            q.push(root);
            Node *temp;
            int count = 0;
            int maxNode = 1;
            int level = 0;
            int space = pow(2, height);
            printNSpace(space / 2);
            while (!q.empty()) {
                temp = q.front();
                q.pop();
                if (temp == NULL) {
                    cout << " ";
                    q.push(NULL);
                    q.push(NULL);
                } else {
                    cout << temp->entry->key;
                    q.push(temp->left);
                    q.push(temp->right);
                }
                printNSpace(space);
                count++;
                if (count == maxNode) {
                    cout << endl;
                    count = 0;
                    maxNode *= 2;
                    level++;
                    space /= 2;
                    printNSpace(space / 2);
                }
                if (level == height)
                    return;
            }
        }
    };
};

// void printKey(int key, int value) {
//     cout << key << endl;
// }
// int main() {
//     BKUTree<int, int>::AVLTree *tree = new BKUTree<int, int>::AVLTree();
//     int keys[] = {1, 3, 5, 7, 9, 2, 4, 11, 14};
//     for (int i = 0; i < 9; i++) tree->add(keys[i], keys[i] + 1);
//     tree->printTreeStructure();
//     tree->remove(11);
//     // tree->remove(1);
//     // tree->add(30, 4);
//     // tree->remove(3);
//     // tree->add(25, 4);
//     // tree->add(3, 4);
//     // tree->add(16, 4);
//     // tree->add(15, 4);
//     tree->printTreeStructure();
//     // cout << endl
//     //      << tree->search(15);
//     // tree->remove(16);

//     // tree->traverseNLROnAVL(printKey);
// }