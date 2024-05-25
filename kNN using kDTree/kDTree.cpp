#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

void merge(vector<vector<int>>& points, int axis, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<vector<int>> leftSub(n1), rightSub(n2);
    for(int i = 0; i < n1; i++)
        leftSub[i] = points[left+i];
    for(int j = 0; j < n2; j++)
        rightSub[j] = points[mid+j+1];
    int i = 0, j = 0, k = left;
    while(i < n1 && j < n2) {
        if(leftSub[i][axis] == rightSub[j][axis]) {
            if(leftSub[i][axis+1] < rightSub[j][axis+1]) {
                points[k] = leftSub[i];
                i++;
            } else {
                points[k] = rightSub[j];
                j++;
            }
        } else {
            if(leftSub[i][axis] < rightSub[j][axis]) {
                points[k] = leftSub[i];
                i++;
            } else {
                points[k] = rightSub[j];
                j++;
            }
        }
        k++;
    }
    while(i < n1) {
        points[k] = leftSub[i];
        i++; k++;
    }
    while(j < n2) {
        points[k] = rightSub[j];
        j++; k++;
    }
}

void mergeSort(vector<vector<int>> &points, int axis, int left, int right) {
    if(left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(points, axis, left, mid);
        mergeSort(points, axis, mid + 1, right);
        merge(points, axis, left, mid, right);
    }
}

double distance(const vector<int>& a, const vector<int>& b) {
    double sum = 0;
    for(size_t i = 0; i < a.size(); ++i) {
        sum += pow(a[i] - b[i], 2);
    }
    return sqrt(sum);
}

// Supportive methods
void kDTree::inorderTraversal(kDTreeNode *node, bool &first) const {
    if(node) {
        inorderTraversal(node->left, first);
        if(!first) cout << " ";
        else first = false;
        string s = "(";
        for(size_t i = 0; i < node->data.size() - 1; i++) {
            s += to_string(node->data[i]) + ", ";
        }
        s += to_string(node->data[node->data.size()-1]) + ")";
        cout << s;
        inorderTraversal(node->right, first);
    }
}

void kDTree::preorderTraversal(kDTreeNode *node, bool &first) const {
    if(node) {
        if(!first) cout << " ";
        else first = false;
        string s = "(";
        for(size_t i = 0; i < node->data.size() - 1; i++) {
            s += to_string(node->data[i]) + ", ";
        }
        s += to_string(node->data[node->data.size()-1]) + ")";
        cout << s;
        preorderTraversal(node->left, first);
        preorderTraversal(node->right, first);
    }
}

void kDTree::postorderTraversal(kDTreeNode* node, bool &first) const {
    if(node) {
        postorderTraversal(node->left, first);
        postorderTraversal(node->right, first);
        if(!first) cout << " ";
        else first = false;
        string s = "(";
        for(size_t i = 0; i < node->data.size() - 1; i++) {
            s += to_string(node->data[i]) + ", ";
        }
        s += to_string(node->data[node->data.size()-1]) + ")";
        cout << s;
    }
}

int kDTree::height(kDTreeNode *node) const {
    if(!node) return 0;
    return max(height(node->left), height(node->right)) + 1;
}

int kDTree::nodeCount(kDTreeNode *node) const {
    if(!node) return 0;
    return nodeCount(node->left) + nodeCount(node->right) + 1;
}

int kDTree::leafCount(kDTreeNode *node) const {
    if(!node) return 0;
    return leafCount(node->left) + leafCount(node->right) + ((node->left || node->right) ? 0 : 1);
}

void kDTree::clear(kDTreeNode *node) {
    if(node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void kDTree::insert(kDTreeNode *&node, const vector<int> &point, int depth) {
    if(!node) {
        node = new kDTreeNode(point);
        return;
    }
    int cd = depth % k;
    if(point[cd] < node->data[cd]) {
        insert(node->left, point, depth + 1);
    } else {
        insert(node->right, point, depth + 1);
    }
}

void kDTree::copyPoint(vector<int> &p1, vector<int> &p2) {
    for(int i = 0; i < k; i++) {
        p1[i] = p2[i];
    }
}

bool kDTree::search(kDTreeNode *node, const vector<int> &point, int depth) {
    if(!node) return false;
    if(node->data == point) return true;
    int cd = depth % k;
    if(point[cd] < node->data[cd]) {
        return search(node->left, point, depth + 1);
    } else {
        return search(node->right, point, depth + 1);
    }
}

void kDTree::nearestNeighbourRec(kDTreeNode* node, const vector<int> &target, kDTreeNode *&best, double &dist) {
    if(!node) return;
    double newDist = distance(target, node->data);
    if(newDist < dist) {
        dist = newDist;
        best = node;
    }
    int axis = node->data.size() - 1;
    int split = node->data[axis];
    int targetAxis = target[axis];
    kDTreeNode *nearNode, *farNode;
    if(targetAxis <= split) {
        nearNode = node->left;
        farNode = node->right;
    } else {
        nearNode = node->right;
        farNode = node->left;
    }
    nearestNeighbourRec(nearNode, target, best, dist);
    if(farNode && dist > distance(target, farNode->data)) {
        double r = dist;
        double d = targetAxis - split;
        if(d * d <= r * r) {
            nearestNeighbourRec(farNode, target, best, dist);
        }
    }
}

void kDTree::kNearestNeighbourHelper(kDTreeNode* node, const vector<int>& target, kNN_Struct* bestK, int k, int depth) {
    int axis = depth % k;
    double dist = distance(target, node->data);
    int i;
    for(i = 0; i < k; i++) {
        if(dist <= bestK[i].dist) {
            break;
        }
    }
    if(i < k) {
        for(int j = k - 1; j > i; j--) {
            bestK[j] = bestK[j-1];
        }
        bestK[i].dist = dist;
        bestK[i].node = node;
    }
    int split = node->data[axis];
    if(target[axis] <= split) {
        if(node->left) {
            kNearestNeighbourHelper(node->left, target, bestK, k, depth + 1);
        }
        if(node->right && abs(target[axis] - split) < bestK[k-1].dist) {
            kNearestNeighbourHelper(node->right, target, bestK, k, depth + 1);
        }
    } else {
        if(node->right) {
            kNearestNeighbourHelper(node->right, target, bestK, k, depth + 1);
        }
        if(node->left && abs(target[axis] - split) < bestK[k-1].dist) {
            kNearestNeighbourHelper(node->left, target, bestK, k, depth + 1);
        }
    }
}

// Main methods
void kDTree::inorderTraversal() const {
    if(root) {
        bool first = true;
        inorderTraversal(root, first);
    }
}

void kDTree::preorderTraversal() const {
    if(root) {
        bool first = true;
        preorderTraversal(root, first);
    }
}

void kDTree::postorderTraversal() const {
    if(root) {
        bool first = true;
        postorderTraversal(root, first);
    }
}

int kDTree::height() const {
    if(!root) return 0;
    return max(height(root->left), height(root->right)) + 1;
}

int kDTree::nodeCount() const {
    return nodeCount(root);
}

int kDTree::leafCount() const {
    return leafCount(root);
}

void kDTree::clear() {
    clear(root);
    root = nullptr;
}

void kDTree::insert(const vector<int> &point) {
    insert(root, point, 0);
}

void kDTree::remove(const vector<int> &point) {
    root = remove(root, point, 0);
}

bool kDTree::search(const vector<int> &point) {
    return search(root, point, 0);
}

void kDTree::buildTree(const vector<vector<int>> &pointList) {
    vector<vector<int>> points = pointList;
    root = buildTreeRec(points, 0);
}

void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best) {
    double dist = MAX_VAL;
    nearestNeighbourRec(root, target, best, dist);
}

void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode*> &bestList) {
    kNN_Struct bestK[k];
    for(int i = 0; i < k; ++i) {
        bestK[i].dist = MAX_VAL;
        bestK[i].node = nullptr;
    }
    kNearestNeighbourHelper(root, target, bestK, k, 0);
    for(int i = 0; i < k; ++i) {
        bestList.push_back(bestK[i].node);
    }
}

void kNN::fit(Dataset &X_train, Dataset &y_train) {
    this->X_train = X_train;
    this->y_train = y_train;
    vector<vector<int>> train_data;
    int nRows, nCols;
    X_train.getShape(nRows, nCols);
    for(int i = 0; i < nRows; i++) {
        vector<int> point;
        for(int j = 0; j < nCols; j++) {
            auto x_it = X_train.data.begin();
            advance(x_it, i);
            auto subIt = x_it->begin();
            advance(subIt, j);
            point.push_back(*subIt);
        }
        auto y_it = y_train.data.begin();
        advance(y_it, i);
        point.push_back(*y_it->begin());
        train_data.push_back(point);
    }
    this->kdtree = kDTree(nCols);
    kdtree.buildTree(train_data);
}

Dataset kNN::predict(Dataset &X_test) {
    Dataset y_pred;
    int nRows, nCols;
    X_test.getShape(nRows, nCols);
    if(nRows == 0 || nCols == 0){
        return y_pred;
    }
    y_pred.columnName = {"label"};
    for(int i = 0; i < nRows; i++) {
        vector<int> point;
        for (int j = 0; j < nCols; j++) {
            auto x_it = X_test.data.begin();
            advance(x_it, i);
            auto sub_x_it = x_it->begin();
            advance(sub_x_it, j);
            point.push_back(*sub_x_it);
        }
        vector<kDTreeNode*> bestList;
        this->kdtree.kNearestNeighbour(point, k, bestList);
        int labelCounts[10] = {0};
        for (auto &node: bestList) {
            int label = node->data.back();
            labelCounts[label]++;
        }

        int maxCount = labelCounts[0];
        int predLabel = 0;
        for(int j = 1; j < 10; j++) {
            if(labelCounts[j] > maxCount) {
                maxCount = labelCounts[j];
                predLabel = j;
            }
        }
        y_pred.data.push_back({predLabel});
    }
    return y_pred;
}

double kNN::score(const Dataset &y_test, const Dataset &y_pred) {
    int correct = 0;
    for(int i = 0; i < (int)y_test.data.size(); i++) {
        auto test_it = y_test.data.begin(), pred_it = y_pred.data.begin();
        advance(test_it, i);
        advance(pred_it, i);
        if((*test_it).front() == (*pred_it).front()) {
            correct++;
        }
    }
    return correct*1.0 / y_test.data.size();
}
