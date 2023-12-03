#include "main.h"

// Huffman tree node abstract base class
class HuffNode {
   public:
    virtual ~HuffNode(){};      // Base destructor
    virtual int weight() = 0;   // Return frequency
    virtual bool isLeaf() = 0;  // Determine type
    virtual HuffNode* left() const { return NULL; }
    virtual void setLeft(HuffNode* b){};
    virtual HuffNode* right() const { return NULL; };
    virtual void setRight(HuffNode* b){};
    virtual void update() { return; }
    virtual HuffNode* parent() const { return NULL; }
    virtual void setParent(HuffNode* b){};
    virtual char val() { return 0; }
};

// Leaf node subclass
class LeafNode : public HuffNode {
   private:
    char it;  // Value
    int wgt;  // Weight
   public:
    LeafNode(const char& val, int freq)  // Constructor
    {
        it = val;
        wgt = freq;
    }
    int weight() { return wgt; }
    char val() { return it; }
    bool isLeaf() { return true; }
};

// Internal node subclass
class IntlNode : public HuffNode {
   private:
    HuffNode* pa;
    HuffNode* lc;  // Left child
    HuffNode* rc;  // Right child
    int wgt;       // Subtree weight
   public:
    IntlNode(HuffNode* l, HuffNode* r, HuffNode* p) {
        wgt = l->weight() + r->weight();
        lc = l;
        rc = r;
        pa = p;
    }
    ~IntlNode() {
        // delete lc;
        // delete rc;
    }
    int weight() { return wgt; }
    bool isLeaf() { return false; }
    HuffNode* left() const { return lc; }
    void setLeft(HuffNode* b) {
        lc = b;
        lc->setParent(this);
    }
    HuffNode* right() const { return rc; }
    void setRight(HuffNode* b) {
        rc = b;
        rc->setParent(this);
    }
    void update() { wgt = lc->weight() + rc->weight(); }
    HuffNode* parent() const { return pa; }
    void setParent(HuffNode* b) { pa = b; }
};

class HuffTree {
   private:
    HuffNode* Root;

   public:
    HuffTree(char& val, int freq) { Root = new LeafNode(val, freq); }
    HuffTree(HuffTree* l, HuffTree* r) { Root = new IntlNode(l->root(), r->root(), this->root()); }
    void removeHuffTree(HuffNode* node) {
        if (node) {
            removeHuffTree(node->left());
            removeHuffTree(node->right());
            delete node;
        }
    }
    ~HuffTree() {
        // removeHuffTree(Root);
        // Root = nullptr;
    }  // Destructor

    HuffNode* root() { return Root; }  // Get root
    void setRoot(HuffNode* node) { Root = node; }
    int weight() { return Root->weight(); }  // Root weight
    int getHight(HuffNode* root) {
        if (root == nullptr) {
            return 0;
        }
        return max(getHight(root->left()), getHight(root->right())) + 1;
    }

    int getBalance(HuffNode* root) {
        return (root == nullptr) ? 0 : (getHight(root->left()) - getHight(root->right()));
    }

    HuffNode* rotateLeft(HuffNode* root) {
        HuffNode* temp = root->right();
        root->setRight(temp->left());
        temp->setLeft(root);
        root->update();
        temp->update();
        return temp;
    }

    HuffNode* rotateRight(HuffNode* root) {
        HuffNode* temp = root->left();
        root->setLeft(temp->right());
        temp->setRight(root);
        root->update();
        temp->update();
        return temp;
    }

    void printHuffmanTree(HuffNode* root, int indent = 0) {
        if (root == nullptr) {
            return;
        }

        if (root->isLeaf()) {
            LeafNode* leaf = static_cast<LeafNode*>(root);
            cout << string(indent, ' ') << "Leaf: " << leaf->val() << " (" << leaf->weight() << ")" << endl;
        } else {
            IntlNode* intl = static_cast<IntlNode*>(root);
            cout << string(indent, ' ') << "Internal Node: " << intl->weight() << endl;
            cout << string(indent, ' ') << "├─ Left: " << endl;
            printHuffmanTree(intl->left(), indent + 4);
            cout << string(indent, ' ') << "└─ Right: " << endl;
            printHuffmanTree(intl->right(), indent + 4);
        }
    }

    void IntlNodeToStack(HuffNode* root, stack<HuffNode*>& st) {
        if (root == nullptr) return;
        queue<HuffNode*> q;
        q.push(root);
        while (!q.empty()) {
            HuffNode* node = q.front();
            q.pop();
            if (!node->isLeaf() && node != this->Root) st.push(node);
            if (node->left() != nullptr) q.push(node->left());
            if (node->right() != nullptr) q.push(node->right());
        }
    }

    HuffNode* checkRotate(HuffNode* root) {
        if (root->isLeaf()) return root;
        if (getBalance(root) > 1) {
            if (getBalance(root->left()) >= 0) {
                root = rotateRight(root);
            } else {
                root->setLeft(rotateLeft(root->left()));
                root = rotateRight(root);
            }
        } else if (getBalance(root) < -1) {
            if (getBalance(root->right()) <= 0) {
                root = rotateLeft(root);
            } else {
                root->setRight(rotateRight(root->right()));
                root = rotateLeft(root);
            }
        }
        root->setLeft(checkRotate(root->left()));
        root->setRight(checkRotate(root->right()));
        return root;
    }

    void getEncodeList(HuffNode* root, string str, vector<pair<char, string>>& list) {
        if (root == nullptr) return;
        if (root->isLeaf()) {
            list.push_back(make_pair(root->val(), str));
        }
        getEncodeList(root->left(), str + "0", list);
        getEncodeList(root->right(), str + "1", list);
    }
};

class customer {
   public:
    int Result;
    HuffTree* tree;
    customer* left;
    customer* right;
    customer() : Result(0), tree(nullptr), left(nullptr), right(nullptr){};
    ~customer() {
        tree->removeHuffTree(tree->root());
        delete tree;
        tree = nullptr;
    }
};

class hashBST {
   public:
    class BSTTree;

   private:
    int size;
    vector<BSTTree*> table;

   public:
    hashBST(int num) : size(num) {
        table.resize(size + 1);
    }
    ~hashBST() {
        for (int i = 0; i < table.size(); i++) {
            if (table[i] != nullptr) {
                delete table[i];
                table[i] = nullptr;
            }
        }
    }

    vector<BSTTree*> getTable() {
        return table;
    }

    void insert(customer* cus) {
        int id = cus->Result % size + 1;
        if (table[id] == nullptr) {
            table[id] = new BSTTree;
        }
        table[id]->root = table[id]->insert(table[id]->root, cus);
        table[id]->q.push(cus);
    }

    void remove(int id) {  // TODO: ???????
        if (table[id]->root == nullptr) return;
        customer* cus = table[id]->q.front();
        table[id]->root = table[id]->remove(table[id]->root, cus);
        table[id]->q.pop();
    }

    vector<int> postorder(int id) {
        vector<int> list;
        stack<customer*> st;
        BSTTree* tree = table[id];
        if (tree == nullptr) return list;
        st.push(tree->root);
        while (!st.empty()) {
            customer* node = st.top();
            st.pop();
            list.push_back(node->Result);
            if (node->left != nullptr) st.push(node->left);
            if (node->right != nullptr) st.push(node->right);
        }
        return list;
    }

    void printTreeHelper(customer* root, const string& prefix, bool isLeft) {
        if (root == nullptr) return;

        cout << prefix;
        if (isLeft) {
            cout << "├─ Left: ";
        } else {
            cout << "└─ Right: ";
        }
        cout << root->Result << endl;

        if (root->left != nullptr) {
            printTreeHelper(root->left, prefix + "│  ", true);
        }

        if (root->right != nullptr) {
            printTreeHelper(root->right, prefix + "   ", false);
        }
    }

    void printTree(int id) {
        BSTTree* tree = table[id];
        if (tree == nullptr) return;
        printTreeHelper(tree->root, "", true);
    }

   public:
    class BSTTree {
       public:
        customer* root;
        queue<customer*> q;

       public:
        BSTTree() : root(nullptr) {}
        ~BSTTree() {
            removeTree(root);
            root = nullptr;
        }
        void removeTree(customer* root) {
            if (root == nullptr) return;
            removeTree(root->left);
            removeTree(root->right);
            delete root;
        }
        customer* insert(customer* root, customer* cus) {
            if (root == nullptr) return cus;
            if (cus->Result < root->Result) {
                root->left = insert(root->left, cus);
            } else if (cus->Result >= root->Result) {
                root->right = insert(root->right, cus);
            }
            q.push(cus);
            return root;
        }
        customer* remove(customer* root, customer* cus) {
            if (root == nullptr) return root;
            if (cus->Result < root->Result) {
                root->left = remove(root->left, cus);
            } else if (cus->Result > root->Result) {
                root->right = remove(root->right, cus);
            } else {
                if (root->left == nullptr) {
                    customer* temp = root->right;
                    delete root;
                    return temp;
                } else if (root->right == nullptr) {
                    customer* temp = root->left;
                    delete root;
                    return temp;
                }
                customer* temp = root->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                root->Result = temp->Result;
                root->right = remove(root->right, temp);
            }
            return root;
        }
        customer* remove(customer* root, int Result) {
            if (root == nullptr) return root;
            if (Result < root->Result) {
                root->left = remove(root->left, Result);
            } else if (Result > root->Result) {
                root->right = remove(root->right, Result);
            } else {
                if (root->left == nullptr) {
                    customer* temp = root->right;
                    delete root;
                    return temp;
                } else if (root->right == nullptr) {
                    customer* temp = root->left;
                    delete root;
                    return temp;
                }
                customer* temp = root->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                root->Result = temp->Result;
                root->right = remove(root->right, temp->Result);
            }
            return root;
        }
        customer* search(customer* root, int Result) {  // FIXME: ??????????
            if (root == nullptr || root->Result == Result) return root;
            if (root->Result < Result) return search(root->right, Result);
            return search(root->left, Result);
        }
    };
};

class minHeap {
   public:
    class area;

   private:
    int size;
    int time;
    vector<area*> table;

   public:
    minHeap(int num) : size(num), time(0) {
        table.resize(num + 1, nullptr);
    };

    ~minHeap() {
        for (int i = 0; i < table.size(); i++) {
            if (table[i] != nullptr) {
                delete table[i];
                table[i] = nullptr;
            }
        }
    }

    void reheapup(int id) {
        if (id == 1) return;
        int parent = id / 2;
        if (table[parent] == nullptr || table[id]->num < table[parent]->num) {
            swap(table[id], table[parent]);
            reheapup(parent);
        }
    }

    void reheapdown(int id) {
        int left = id * 2;
        int right = id * 2 + 1;
        if (left > size) return;
        int min = left;
        if (right <= size && (table[right]->num < table[left]->num || (table[right]->num == table[left]->num && table[right]->time < table[left]->time))) {
            min = right;
        }
        swap(table[id], table[min]);
        reheapdown(min);
    }

    void insert(customer* cus) {
        int id = cus->Result % size + 1;
        if (table[id] == nullptr) {
            table[id] = new area(id, 1);
            table[id]->q.push(cus);
        } else {
            table[id]->q.push(cus);
            table[id]->num++;
        }
        table[id]->time = time++;
        reheapup(id);
    }

    void remove(int id) {
        if (table[id] == nullptr) return;
        table[id]->q.pop();
        table[id]->num--;
        if (table[id]->num == 0) {
            delete table[id];
            table[id] = nullptr;
            return;
        }
        table[id]->time = time++;
        reheapdown(id);
    }

    vector<area*> findMin(int num) {
        area* min = table[1];
        vector<area*> list;
        if (min == nullptr) return list;
        if (num > size) return table;
        for (int i = 0; i < num; i++) {
            
        }
        return list;
    }

   public:
    class area {
       public:
        int label;
        int num;
        int time;
        queue<customer*> q;
        area(int label, int num) : label(label), num(num) {}
        ~area() {
            while (!q.empty()) {
                customer* cus = q.front();
                q.pop();
                delete cus;
            }
        }
    };
};

class restaurant {
   private:
    int maxsize;
    hashBST* gojo;
    minHeap* sukuna;

   public:
    restaurant() : maxsize(0) {}
    ~restaurant() {
        delete gojo;
        delete sukuna;
    }
    void LAPSE(string name);
    void KOKUSEN();
    void KEITEIKEN(int num);
    void HAND();
    void LIMITLESS(int num);
    void CLEAVE(int num);
    void setMAXSIZE(int num) {
        maxsize = num;
        gojo = new hashBST(maxsize);
        sukuna = new minHeap(maxsize);
    }
    char encodeCaesar(char c, int shift) {
        if (isalpha(c)) {
            if (isupper(c)) {
                c = (c - 'A' + shift) % 26 + 'A';
            } else {
                c = (c - 'a' + shift) % 26 + 'a';
            }
        }
        return c;
    }
    int bin2dec(string str) {
        int num = 0;
        for (int i = 0; i < str.length(); i++) {
            num = (num << 1) + (str[i] - '0');
        }
        return num;
    }

    int permutePostOrder(vector<int>& list, int l, int r) {
        int count = 1;
        if (r - l < 3) return 1;
        int root = list[r];
        int i = l;
        for (; i < r; i++)
            if (list[i] > root) break;

        if (i == r) count = 2 * permutePostOrder(list, l, r - 1);
        else if (i == l) count = 2 * permutePostOrder(list, l + 1, r);
        else count = 2 * permutePostOrder(list, l, i - 1) * permutePostOrder(list, i, r - 1);
        return count;
    }

   public:
    struct compare {
        bool operator()(HuffTree* l, HuffTree* r) {
            return (l->weight() > r->weight());
        }
    };

    struct letter {
        char orig = 0;
        char encodeCaesar = 0;
        int freq = 0;
    };
};

void restaurant::LAPSE(string name) {
    int length = name.length();
    string encode = "";
    string encodeBin = "";
    vector<letter> listChr;
    for (char c : name) {
        bool found = false;
        for (auto& chr : listChr) {
            if (chr.orig == c) {
                chr.freq++;
                found = true;
                break;
            }
        }
        if (!found) {
            letter temp;
            temp.orig = c;
            temp.freq = 1;
            listChr.push_back(temp);
        }
    }
    if (listChr.size() < 3) return;
    customer* cus = new customer;
    // for (int i = 0; i < listChr.size() - 1; i++) {
    //     for (int j = 0; j < listChr.size() - i - 1; j++) {
    //         if (listChr[j].freq > listChr[j + 1].freq) {
    //             swap(listChr[j], listChr[j + 1]);
    //         }
    //     }
    // }
    for (auto& chr : listChr) {
        chr.encodeCaesar = encodeCaesar(chr.orig, chr.freq);
    }
    for (auto& c : name) {
        for (auto& chr : listChr) {
            if (chr.orig == c) {
                encode += chr.encodeCaesar;
                break;
            }
        }
    }
    for (int i = 0; i < listChr.size() - 1; i++) {
        for (int j = i + 1; j < listChr.size(); j++) {
            if (listChr[i].encodeCaesar == listChr[j].encodeCaesar) {
                listChr[i].freq += listChr[j].freq;
                listChr.erase(listChr.begin() + j);
                j--;
            }
        }
    }
    for (int i = 0; i < listChr.size() - 1; i++) {
        for (int j = 0; j < listChr.size() - i - 1; j++) {
            if ((listChr[j].freq > listChr[j + 1].freq) || (listChr[j].freq == listChr[j + 1].freq && listChr[j].encodeCaesar > listChr[j + 1].encodeCaesar)) {
                swap(listChr[j], listChr[j + 1]);
            }
        }
    }
    priority_queue<HuffTree*, vector<HuffTree*>, compare> pq;
    for (int i = 0; i < listChr.size(); i++) {
        pq.push(new HuffTree(listChr[i].encodeCaesar, listChr[i].freq));
    }
    HuffTree *temp1, *temp2, *tree;
    while (pq.size() > 1) {
        temp1 = pq.top();
        pq.pop();
        temp2 = pq.top();
        pq.pop();
        tree = new HuffTree(temp1, temp2);
        tree->setRoot(tree->checkRotate(tree->root()));
        pq.push(tree);

        delete temp1;
        delete temp2;
    }
    tree = pq.top();
    pq.pop();
    cus->tree = tree;
    // print Huffman tree
    tree->printHuffmanTree(tree->root());
    vector<pair<char, string>> list;
    tree->getEncodeList(tree->root(), "", list);

    for (int i = length - 1; i >= 0 && i >= length - 10; i--) {
        for (auto& chr : list) {
            if (chr.first == encode[i]) {
                encodeBin = chr.second + encodeBin;
                break;
            }
        }
    }
    length = encodeBin.length();
    encodeBin = (length > 10) ? encodeBin.substr(length - 10) : encodeBin;
    cus->Result = bin2dec(encodeBin);
    cout << cus->Result << endl;
    (cus->Result % 2) ? gojo->insert(cus) : sukuna->insert(cus);
}

void restaurant::KOKUSEN() {
    vector<int> list;
    for (int i = 1; i <= maxsize; i++) {
        list = gojo->postorder(i);
        int size = list.size();
        if (!size) continue;
        int numPermute = permutePostOrder(list, 0, size - 1) % maxsize;
        for (int j = 0; j < numPermute; j++) {
            gojo->remove(i);
        }
    }
}

void restaurant::KEITEIKEN(int num) {
}

void restaurant::HAND() {
}

void restaurant::LIMITLESS(int num) {
}

void restaurant::CLEAVE(int num) {
}

void simulate(string filename) {
    restaurant* res = new restaurant;
    ifstream ss(filename);
    string str, name, maxsize, num;
    while (ss >> str) {
        if (str == "MAXSIZE") {
            ss >> maxsize;
            res->setMAXSIZE(stoi(maxsize));
        } else if (str == "LAPSE") {
            ss >> name;
            res->LAPSE(name);
        } else if (str == "KOKUSEN") {
            res->KOKUSEN();
        } else if (str == "KEITEIKEN") {
            ss >> num;
            res->KEITEIKEN(stoi(num));
        } else if (str == "HAND") {
            res->HAND();
        } else if (str == "LIMITLESS") {
            ss >> num;
            res->LIMITLESS(stoi(num));
        } else if (str == "CLEAVE") {
            ss >> num;
            res->CLEAVE(stoi(num));
        } else {
            break;
            ;
        }
    }
    ss.close();
    delete res;
    return;
}