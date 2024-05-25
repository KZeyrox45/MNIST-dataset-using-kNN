#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

const double MAX_VAL = 1e20;

void merge(vector<vector<int>>& points, int axis, int left, int mid, int right);
void mergeSort(vector<vector<int>> &points, int axis, int left, int right);
double distance(const vector<int>& a, const vector<int>& b);

struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }

    friend ostream &operator<<(ostream &os, const kDTreeNode &node)
    {
        os << "(";
        for (size_t i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;

    kDTreeNode *clone(kDTreeNode *node) {
        if(!node) return nullptr;
        kDTreeNode *newNode = new kDTreeNode(node->data, clone(node->left), clone(node->right));
        return newNode;
    }

    void inorderTraversal(kDTreeNode *node, bool &first) const;
    void preorderTraversal(kDTreeNode *node, bool &first) const;
    void postorderTraversal(kDTreeNode* node, bool &first) const;
    int height(kDTreeNode *node) const;
    int nodeCount(kDTreeNode *node) const;
    int leafCount(kDTreeNode *node) const;
    void clear(kDTreeNode *node);

    void insert(kDTreeNode *&node, const vector<int> &point, int depth);
    void copyPoint(vector<int> &p1, vector<int> &p2);

    kDTreeNode *findMinNode(kDTreeNode *node, int axis, int targetAxis) {
        int cd = targetAxis % k;
        if(!node) return nullptr;
        if(axis == cd) {
            if(!node->left) return node;
            return findMinNode(node->left, axis, cd);
        }
        kDTreeNode *leftMin = findMinNode(node->left, axis, cd);
        kDTreeNode *rightMin = findMinNode(node->right, axis, cd);
        kDTreeNode *minNode = node;
        if(leftMin && leftMin->data[axis] < minNode->data[axis]) {
            minNode = leftMin;
        }
        if(rightMin && rightMin->data[axis] < minNode->data[axis]) {
            minNode = rightMin;
        }
        return minNode;
    }

    kDTreeNode* getRoot() {
        return this->root;
    }

    kDTreeNode *findSuccessor(kDTreeNode *node, int axis, int targetAxis) {
        int cd = targetAxis % k;
        if(!node) return nullptr;
        if(axis == cd){
            return findMinNode(node->right, axis, cd);
        }
        kDTreeNode *leftMin = findMinNode(node->left, axis, cd);
        kDTreeNode *rightMin = findMinNode(node->right, axis, cd);
        kDTreeNode *minNode = node;
        if (leftMin && leftMin->data[axis] < minNode->data[axis]) {
            minNode = leftMin;
        }
        if (rightMin && rightMin->data[axis] < minNode->data[axis]) {
            minNode = rightMin;
        }
        return minNode;
    }

    kDTreeNode* remove(kDTreeNode *node, const vector<int> point, int depth) {
        if(!node || !search(point)) return nullptr;
        int cd = depth % k;
        if(point[cd] < node->data[cd]) {
            node->left = remove(node->left, point, depth + 1);
        } else if(point[cd] > node->data[cd]) {
            node->right = remove(node->right, point, depth + 1);
        } else {
            if(!node->left && !node->right){
                delete node;
                return nullptr;
            }
            if(node->right) {
                kDTreeNode *temp = findSuccessor(node->right, height(node->right), cd);
                copyPoint(node->data, temp->data);
                node->right = remove(node->right, temp->data, height(node->right));
            } else {
                kDTreeNode *temp = findSuccessor(node->left, height(node->left), cd);
                copyPoint(node->data, temp->data);
                node->left = remove(node->left, temp->data, height(node->left));
            }
        }
        return node;
    }

    bool search(kDTreeNode *node, const vector<int> &point, int depth);

    kDTreeNode *buildTreeRec(vector<vector<int>> &points, int depth) {
        if(points.empty()) return nullptr;
        int axis = depth % k;
        int med = points.size() / 2;
        if(points.size() % 2 == 0) med--;
        mergeSort(points, axis, 0, points.size() - 1);
        kDTreeNode *node = new kDTreeNode(points[med]);
        vector<vector<int>> leftPoints(points.begin(), points.begin() + med);
        vector<vector<int>> rightPoints(points.begin() + med + 1, points.end());
        node->left = buildTreeRec(leftPoints, depth + 1);
        node->right = buildTreeRec(rightPoints, depth + 1);
        return node;
    }

    struct kNN_Struct {
        double dist;
        kDTreeNode* node;
    };

    void nearestNeighbourRec(kDTreeNode* node, const vector<int> &target, kDTreeNode *&best, double &dist);
    void kNearestNeighbourHelper(kDTreeNode* node, const vector<int>& target, kNN_Struct* bestK, int k, int depth);

public:
    kDTree(int k = 2) : k(k), root(nullptr) {}
    ~kDTree() { clear(); }

    const kDTree &operator=(const kDTree &other) {
        if(this != &other) {
            clear();
            k = other.k;
            root = clone(other.root);
        }
        return *this;
    }

    kDTree(const kDTree &other) : k(other.k), root(clone(other.root)) {}

    void inorderTraversal() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    int height() const;
    int nodeCount() const;
    int leafCount() const;
    void clear();

    void insert(const vector<int> &point);

    void remove(const vector<int> &point);
    bool search(const vector<int> &point);
    void buildTree(const vector<vector<int>> &pointList);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);
};

class kNN
{
private:
    int k;
    Dataset X_train;
    Dataset y_train;
    kDTree kdtree;

public:
    kNN(int k = 5) : k(k) {}
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);

    kDTree getKDTree() {return kdtree;}
};

// Please add more or modify as needed
