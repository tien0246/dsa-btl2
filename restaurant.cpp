#include "main.h"
#define delete(x) \
    delete x;     \
    x = nullptr;

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
            delete (node);
        }
    }
    ~HuffTree() {}  // Destructor

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
        cout << "Rotate left" << endl;
        HuffNode* temp = root->right();
        root->setRight(temp->left());
        temp->setLeft(root);
        // root->update();  // No need
        // temp->update();
        return temp;
    }

    HuffNode* rotateRight(HuffNode* root) {
        cout << "Rotate right\n";
        HuffNode* temp = root->left();
        root->setLeft(temp->right());
        temp->setRight(root);
        // root->update();  // No need
        // temp->update();
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
            cout << string(indent, ' ') << "Internal Node: " << intl->weight() << " (Height: " << getHight(intl) << ")" << endl;
            cout << string(indent, ' ') << "├─ Left:" << endl;
            printHuffmanTree(intl->left(), indent + 4);
            cout << string(indent, ' ') << "└─ Right:" << endl;
            printHuffmanTree(intl->right(), indent + 4);
        }
    }

    void getInorderTree(HuffNode* root, string& result) {
        if (root == nullptr) return;
        getInorderTree(root->left(), result);
        if (root->isLeaf()) {
            result += root->val();
            result += "\n";
        } else {
            result += to_string(root->weight());
            result += "\n";
        }
        getInorderTree(root->right(), result);
    }

    HuffNode* checkRotate(HuffNode* root, bool& isRotate, bool& unreal) {
        if (unreal) return root;
        if (root->isLeaf()) return root;
        if (getBalance(root) > 1) {
            if (getBalance(root->left()) >= 0) {
                root = rotateRight(root);
            } else {
                root->setLeft(rotateLeft(root->left()));
                root = rotateRight(root);
            }
            if (root->isLeaf() && (root->right() != nullptr || root->left() != nullptr)) unreal = true;
            isRotate = true;
            return root;
        } else if (getBalance(root) < -1) {
            if (getBalance(root->right()) <= 0) {
                root = rotateLeft(root);
            } else {
                root->setRight(rotateRight(root->right()));
                root = rotateLeft(root);
            }
            if (root->isLeaf() && (root->right() != nullptr || root->left() != nullptr)) unreal = true;
            isRotate = true;
            return root;
        }
        if (!isRotate) root->setLeft(checkRotate(root->left(), isRotate, unreal));
        if (!isRotate) root->setRight(checkRotate(root->right(), isRotate, unreal));
        return root;
    }

    bool rotateTree() {
        bool unreal = false;
        for (int i = 0; i < 3; i++) {
            bool isRotate = false;
            // printHuffmanTree(Root);
            Root = checkRotate(Root, isRotate, unreal);
            if (unreal || !isRotate) return unreal;
        }
        return unreal;
    }

    void getEncodeList(HuffNode* root, string str, unordered_map<char, string>& encodingMap) {
        if (root == nullptr) return;

        if (root->isLeaf()) {
            encodingMap[root->val()] = str;
        }
        getEncodeList(root->left(), str + "0", encodingMap);
        getEncodeList(root->right(), str + "1", encodingMap);
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
        delete (tree);
    }
};

class hashBST {
   public:
    struct BSTNode {
        int result;
        BSTNode *left, *right;
    };
    class BSTTree;

   private:
    int size;
    vector<BSTTree*> table;

   public:
    hashBST(int num) : size(num) {
        table = vector<BSTTree*>(size + 1, nullptr);
    }
    ~hashBST() {
        for (unsigned int i = 0; i < table.size(); i++) {
            if (table[i] != nullptr) {
                delete (table[i]);
            }
        }
    }

    void insert(customer* cus) {
        int id = cus->Result % size + 1;
        if (table[id] == nullptr) {
            table[id] = new BSTTree;
        }
        table[id]->insert(cus);
    }

    void remove(int id, int n) {  // TODO: ???????
        if (table[id]->root == nullptr) return;
        table[id]->remove(n);
        if (table[id]->root == nullptr) {
            delete (table[id]);
        }
    }

    vector<int> postorder(int id) {
        vector<int> list;
        stack<BSTNode*> st;
        BSTTree* tree = table[id];
        if (tree == nullptr) return list;
        st.push(tree->root);
        while (!st.empty()) {
            BSTNode* node = st.top();
            st.pop();
            list.push_back(node->result);
            if (node->left != nullptr) st.push(node->left);
            if (node->right != nullptr) st.push(node->right);
        }
        return list;
    }

    void printTree(int id) {
        BSTTree* tree = table[id];
        if (tree == nullptr) return;
        printTree(tree->root, "", true);
    }

    void printTree(BSTNode* node, const string& prefix, bool isLeft) {
        if (node == nullptr) return;

        cout << prefix;
        cout << (isLeft ? "├── " : "└── ");
        cout << node->result << endl;

        string childPrefix = prefix + (isLeft ? "│   " : "    ");
        printTree(node->left, childPrefix, true);
        printTree(node->right, childPrefix, false);
    }

    void printInorder(BSTNode* root) {
        if (root == nullptr) return;
        printInorder(root->left);
        cout << root->result << "\n";
        printInorder(root->right);
    }

    void printInorder(int id) {
        BSTTree* tree = table[id];
        if (tree == nullptr) return;
        printInorder(tree->root);
    }

   public:
    class BSTTree {
       public:
        BSTNode* root;
        queue<customer*> q;

       public:
        BSTTree() : root(nullptr) {}
        ~BSTTree() {
            removeTree(root);
            while (!q.empty()) {
                customer* cus = q.front();
                q.pop();
                delete (cus);
            }
        }
        void removeTree(BSTNode*& root) {
            if (root == nullptr) return;
            removeTree(root->left);
            removeTree(root->right);
            delete (root);
        }
        BSTNode* insert(BSTNode* root, int result) {
            if (root == nullptr) return new BSTNode{result, nullptr, nullptr};
            if (result < root->result) {
                root->left = insert(root->left, result);
            } else {
                root->right = insert(root->right, result);
            }
            return root;
        }
        void insert(customer* cus) {
            root = insert(root, cus->Result);
            q.push(cus);
        }
        void remove(unsigned int n) {
            if (n >= q.size()) {
                removeTree(root);
                cout << root << endl;
                while (!q.empty()) {
                    customer* cus = q.front();
                    q.pop();
                    delete (cus);
                }
                return;
            }
            while (n--) {
                customer* cus = q.front();
                q.pop();
                root = remove(root, cus->Result);
                delete (cus);
            }
        }
        BSTNode* remove(BSTNode* root, int result) {
            if (root == nullptr) return root;
            if (result < root->result) {
                root->left = remove(root->left, result);
            } else if (result > root->result) {
                root->right = remove(root->right, result);
            } else {
                if (root->left == nullptr) {
                    BSTNode* temp = root->right;
                    delete (root);
                    return temp;
                } else if (root->right == nullptr) {
                    BSTNode* temp = root->left;
                    delete (root);
                    return temp;
                }
                BSTNode* temp = root->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                root->result = temp->result;
                root->right = remove(root->right, temp->result);
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
    int capacity;
    unsigned int time;
    int size;
    vector<area*> table;

   public:
    minHeap(int num) : capacity(num), time(0), size(0) {
        table = vector<area*>(capacity + 1, nullptr);
    };

    ~minHeap() {
        for (int i = 1; i <= size; i++) {
            if (table[i] != nullptr) {
                delete (table[i]);
            }
        }
    }

    void printHeap() {
        for (int i = 1; i <= size; i++) {
            cout << table[i]->label << " " << table[i]->num << " " << table[i]->time << endl;
        }
    }

    void reheapup(int i) {
        if (i == 1) return;
        int parent = i / 2;
        if (table[i]->num < table[parent]->num) {
            swap(table[i], table[parent]);
            reheapup(parent);
        }
    }

    void reheapdown(int i) {
        int left = i * 2;
        int right = i * 2 + 1;
        if (left > size) return;
        int min = left;
        if (right <= size && (table[right]->num < table[left]->num || (table[right]->num == table[left]->num && table[right]->time < table[left]->time))) {
            min = right;
        }
        if (table[i]->num < table[min]->num || (table[i]->num == table[min]->num && table[i]->time < table[min]->time)) return;
        swap(table[i], table[min]);
        reheapdown(min);
    }

    void insert(customer* cus) {
        int id = cus->Result % capacity + 1;
        int i = search(id);
        if (i != -1) {
            table[i]->num++;
            table[i]->time = time++;
            table[i]->q.push(cus);
            reheapdown(i);
            return;
        }
        size++;
        table[size] = new area(id, 1);
        table[size]->q.push(cus);
        table[size]->time = time++;
        reheapup(size);
    }

    int search(int lable) {
        for (int i = 1; i <= size; i++) {
            if (table[i]->label == lable) {
                return i;
            }
        }
        return -1;
    }

    void remove(area* area) {
        int i = search(area->label);
        for (int j = 0; j < area->num; j++) {
            customer* cus = area->q.front();
            area->q.pop();
            cout << cus->Result << "-" << area->label << endl;
            delete (cus);
        }
        table[i] = table[size];
        table[size] = area;
        delete (table[size]);
        size--;
        reheapdown(i);
        return;
    }

    void remove(area* area, int n) {
        if (area->num < n) {
            remove(area);
            return;
        }
        int i = search(area->label);
        area->num -= n;
        area->time = time++;
        while (n--) {
            customer* cus = area->q.front();
            area->q.pop();
            cout << cus->Result << "-" << area->label << endl;
            delete (cus);
        }
        reheapup(i);
    }

    vector<area*> findMin(int n) {
        vector<area*> list;
        vector<tempArea> temp;
        for (int i = 1; i <= size; i++) {
            temp.push_back({table[i], table[i]->num, table[i]->time});
        }
        sort(temp.begin(), temp.end(), [](const tempArea& a, const tempArea& b) {
            return (a.num < b.num) || ((a.num == b.num) && (a.time < b.time));
        });
        // for (int i = 0; i < temp.size() - 1; i++) {
        //     for (int j = 0; j < temp.size() - i - 1; j++) {
        //         if ((temp[j].num > temp[j + 1].num) || (temp[j].num == temp[j + 1].num && temp[j].time > temp[j + 1].time)) {
        //             swap(temp[j], temp[j + 1]);
        //         }
        //     }
        // }
        for (int i = 0; i < min(n, size); i++) {
            list.push_back(temp[i].areaPtr);
        }
        return list;
    }

    void remove(int n) {
        vector<area*> list = findMin(n);
        for (auto& area : list) {
            remove(area, n);
        }
    }

    void printPreorder(int i, int n) {
        if (i > size) return;
        table[i]->printQueue(n);
        printPreorder(i * 2, n);
        printPreorder(i * 2 + 1, n);
    }

    void printPreorder(int n) {
        printPreorder(1, n);
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
                delete (cus);
            }
        }

        void printQueueHelper(queue<customer*> q, int n) {
            if (q.empty() || n == 0) return;
            customer* cus = q.front();
            q.pop();
            printQueueHelper(q, n - 1);
            cout << label << "-" << cus->Result << "\n";
        }

        void printQueue(int n) {
            queue<customer*> temp = q;
            printQueueHelper(temp, n);
        }
    };
    struct tempArea {
        area* areaPtr;
        int num;
        int time;
    };
};

class restaurant {
   private:
    int maxsize;
    hashBST* gojo;
    minHeap* sukuna;
    string lastCustomer;

   public:
    struct compare {
        bool operator()(HuffTree* l, HuffTree* r) {
            if (l->weight() != r->weight()) {
                return (l->weight() > r->weight());
            }
            return l > r;
        }
    };

    struct letter {
        char encodeCaesar = 0;
        int freq = 0;
    };

   public:
    restaurant() : maxsize(0), gojo(nullptr), sukuna(nullptr), lastCustomer("") {}
    ~restaurant() {
        delete (gojo);
        delete (sukuna);
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
        for (unsigned int i = 0; i < str.length(); i++) {
            num = (num << 1) + (str[i] - '0');
        }
        return num;
    }

    void calculateFact(int fact[], int N) {
        fact[0] = 1;
        for (long long int i = 1; i < N; i++) {
            fact[i] = fact[i - 1] * i;
        }
    }

    int nCr(int fact[], int N, int R) {
        if (R > N) return 0;
        int res = fact[N] / fact[R];
        res /= fact[N - R];
        return res;
    }

    int countWays(vector<int>& arr, int fact[]) {
        int N = arr.size();
        if (N <= 2) return 1;
        vector<int> leftSubTree;
        vector<int> rightSubTree;
        int root = arr[0];
        for (int i = 1; i < N; i++) {
            if (arr[i] < root) leftSubTree.push_back(arr[i]);
            else rightSubTree.push_back(arr[i]);
        }
        int N1 = leftSubTree.size();
        // int N2 = rightSubTree.size();
        int countLeft = countWays(leftSubTree, fact);
        int countRight = countWays(rightSubTree, fact);
        return nCr(fact, N - 1, N1) * countLeft * countRight;
    }

    int permutePostOrder(vector<int>& list, int size) {
        int fact[size];
        calculateFact(fact, size);
        reverse(list.begin(), list.end());
        return countWays(list, fact);
    }
};

void restaurant::LAPSE(string name) {
    int length = name.length();
    string encode = "";
    string encodeBin = "";
    vector<letter> listChr;
    unordered_map<char, letter> charFrequency;

    for (char c : name) {
        charFrequency[c].freq++;
    }
    for (auto& entry : charFrequency) {
        // letter chr;
        // chr.orig = entry.first;
        // chr.freq = entry.second;
        entry.second.encodeCaesar = encodeCaesar(entry.first, entry.second.freq);
        // listChr.push_back(chr);
    }

    // for (char c : name) {
    //     bool found = false;
    //     for (auto& chr : listChr) {
    //         if (chr.orig == c) {
    //             chr.freq++;
    //             found = true;
    //             break;
    //         }
    //     }
    //     if (!found) {
    //         letter temp;
    //         temp.orig = c;
    //         temp.freq = 1;
    //         listChr.push_back(temp);
    //     }
    // }
    if (charFrequency.size() < 3) return;
    customer* cus = new customer;
    // for (int i = 0; i < listChr.size() - 1; i++) {
    //     for (int j = 0; j < listChr.size() - i - 1; j++) {
    //         if (listChr[j].freq > listChr[j + 1].freq) {
    //             swap(listChr[j], listChr[j + 1]);
    //         }
    //     }
    // }
    // for (auto& chr : listChr) {

    // }
    for (auto& c : name) {
        // for (auto& chr : listChr) {
        //     if (chr.orig == c) {
        //         encode += chr.encodeCaesar;
        //         break;
        //     }
        // }
        encode += charFrequency[c].encodeCaesar;
    }
    for (auto& entry : charFrequency) {
        listChr.push_back(entry.second);
    }

    for (unsigned int i = 0; i < listChr.size() - 1; i++) {
        for (unsigned int j = i + 1; j < listChr.size(); j++) {
            if (listChr[i].encodeCaesar == listChr[j].encodeCaesar) {
                listChr[i].freq += listChr[j].freq;
                listChr.erase(listChr.begin() + j);
                j--;
            }
        }
    }
    std::sort(listChr.begin(), listChr.end(), [](const letter& a, const letter& b) {
        if (a.freq != b.freq) {
            return a.freq < b.freq;
        } else if ((a.encodeCaesar < 'a' && b.encodeCaesar < 'a') || (a.encodeCaesar >= 'a' && b.encodeCaesar >= 'a')) {
            return a.encodeCaesar < b.encodeCaesar;
        }
        return a.encodeCaesar > b.encodeCaesar;
    });
    priority_queue<HuffTree*, vector<HuffTree*>, compare> pq;
    for (auto& chr : listChr) {
        // cout << chr.encodeCaesar << " " << chr.freq << endl;
        pq.push(new HuffTree(chr.encodeCaesar, chr.freq));
    }
    bool unreal = false;
    int i = 0;
    HuffTree *temp1, *temp2, *tree;
    while (pq.size() > 1) {
        temp1 = pq.top();
        pq.pop();
        temp2 = pq.top();
        pq.pop();
        tree = new HuffTree(temp1, temp2);

        cout << "Iteration " << i++ << ":" << endl;
        cout << "sub tree 1--------------------------------------------------" << endl;
        temp1->printHuffmanTree(temp1->root());
        cout << "sub tree 2--------------------------------------------------" << endl;
        temp2->printHuffmanTree(temp2->root());
        unreal = tree->rotateTree();
        cout << "new tree----------------------------------------------------" << endl;
        tree->printHuffmanTree(tree->root());
        cout << "------------------------------------------------------------" << endl << endl;

        pq.push(tree);
        delete (temp1);
        delete (temp2);
    }
    tree = pq.top();
    pq.pop();
    cus->tree = tree;
    if (unreal) {
        delete (cus);
        return;
    }
    lastCustomer = "";
    tree->getInorderTree(tree->root(), lastCustomer);
    // print Huffman tree
    // tree->printHuffmanTree(tree->root());
    unordered_map<char, string> list;
    tree->getEncodeList(tree->root(), "", list);
    for (int i = length - 1; i >= 0 && encodeBin.length() < 10; i--) {
        char character = encode[i];
        auto it = list.find(character);
        encodeBin = it->second + encodeBin;
    }
    encodeBin = (encodeBin.length() > 10) ? encodeBin.substr(encodeBin.length() - 10, 10) : encodeBin;
    reverse(encodeBin.begin(), encodeBin.end());
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
        int numPermute = permutePostOrder(list, size) % maxsize;

        cout << "Hoan vi " << numPermute << ":\n";

        gojo->remove(i, numPermute);
    }
}

void restaurant::KEITEIKEN(int num) {
    sukuna->remove(num);
}

void restaurant::HAND() {
    cout << lastCustomer;
}

void restaurant::LIMITLESS(int num) {
    gojo->printInorder(num);
}

void restaurant::CLEAVE(int num) {
    sukuna->printPreorder(num);
}

void simulate(string filename) {
    restaurant* res = new restaurant;
    ifstream ss(filename);
    string str, name, maxsize, num;
    while (ss >> str) {
        cout << str << endl;
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
        } else {
            ss >> num;
            res->CLEAVE(stoi(num));
        }
    }
    ss.close();
    delete (res);
    return;
}