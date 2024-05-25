#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

template <typename T>
void DLinkedList<T>::push_back(T value) {
    DNode<T>* newNode = new DNode<T>(value);
    if(!head) head = tail = newNode;
    else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template <typename T>
void DLinkedList<T>::push_front(T value) {
    DNode<T>* newNode = new DNode<T>(value);
    newNode->next = head;
    if(head) head->prev = newNode;
    head = newNode;
    if(!tail) tail = newNode;
    size++;
}

template <typename T>
void DLinkedList<T>::insert(int index, T value) {
    if(index < 0 || index > size) {
        return;
    }
    if(index == 0) push_front(value);
    else if(index == size) push_back(value);
    else {
        DNode<T>* newNode = new DNode<T>(value);
        DNode<T>* curr = head;
        for(int i = 0; i < index - 1; i++) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        newNode->prev = curr;
        curr->next->prev = newNode;
        curr->next = newNode;
        size++;
    }
}

template <typename T>
void DLinkedList<T>::remove(int index) {
    if(index < 0 || index >= size) {
        return;
    }
    DNode<T>* temp;
    if(index == 0) {
        temp = head;
        head = head->next;
        if(head) head->prev = nullptr;
    } else {
        DNode<T>* curr = head;
        for(int i = 0; i < index - 1; i++) {
            curr = curr->next;
        }
        temp = curr->next;
        curr->next = temp->next;
        if(curr->next) curr->next->prev = curr;
        else tail = curr;
    }
    delete temp;
    size--;
}

template <typename T>
T& DLinkedList<T>::get(int index) const {
    if(index < 0 || index >= size) {
        throw std::out_of_range("get(): Out of range");
    }
    DNode<T>* curr = head;
    for(int i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->data;
}

template <typename T>
void DLinkedList<T>::clear() {
    while(head != nullptr) {
        DNode<T>* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    size = 0;
}

template <typename T>
void DLinkedList<T>::print() const {
    if(size == 0) return;
    DNode<T>* curr = head;
    for(int i = 0; i < size; i++) {
        if(i == size - 1) {
            cout << curr->data << " " << endl;
        } else {
            cout << curr->data << " ";
        }
        curr = curr->next;
    }
}

template <typename T>
void DLinkedList<T>::reverse() {
    if(!head) return;
    DNode<T>* temp = nullptr;
    DNode<T>* curr = head;
    while(curr != nullptr) {
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev;
    }
    if(temp != nullptr) {
        tail = head;
        head = temp->prev;
    }
}

Dataset::Dataset() {
    data = new DLinkedList<List<int>*>();
    firstRow = new DLinkedList<string>();
}

Dataset::Dataset(const Dataset& other) {
    data = new DLinkedList<List<int>*>();
    for(int i = 0; i < other.data->length(); i++) {
        List<int>* row = new DLinkedList<int>();
        for(int j = 0; j < other.data->get(i)->length(); j++) {
            row->push_back(other.data->get(i)->get(j));
        }
        data->push_back(row);
    }
    firstRow = new DLinkedList<string>();
    for(int i = 0; i < other.firstRow->length(); i++) {
        firstRow->push_back(other.firstRow->get(i));
    }
}

Dataset& Dataset::operator=(const Dataset& other) {
    if(this != &other) {
        clear();
        data = new DLinkedList<List<int>*>();
        for(int i = 0; i < other.data->length(); i++) {
            List<int>* row = new DLinkedList<int>();
            for(int j = 0; j < other.data->get(i)->length(); j++) {
                row->push_back(other.data->get(i)->get(j));
            }
            data->push_back(row);
        }
        firstRow = new DLinkedList<string>();
        for(int i = 0; i < other.firstRow->length(); i++) {
            firstRow->push_back(other.firstRow->get(i));
        }
    }
    return *this;
}

void Dataset::clear() {
    for(int i = 0; i < data->length(); i++) {
        delete data->get(i);
    }
    data->clear();
    firstRow->clear();
}

bool Dataset::loadFromCSV(const char* fileName) {
    ifstream file(fileName);
    if(!file.is_open()) return false;
    string line;
    if(getline(file, line)) {
        stringstream ss(line);
        string cell;
        while(getline(ss, cell, ',')) {
            firstRow->push_back(cell);
        }
    }
    while(getline(file, line)) {
        stringstream ss(line);
        string cell;
        List<int>* row = new DLinkedList<int>();
        while(getline(ss, cell, ',')) {
            row->push_back(stoi(cell));
        }
        data->push_back(row);
    }
    file.close();
    return true;
}

void Dataset::printHead(int nRows, int nCols) const {
    if(nRows < 0 || nCols < 0) return;
    for(int i = 0; i < min(nCols, firstRow->length()); i++) {
        if(i == min(nCols, firstRow->length()) - 1) {
            cout << firstRow->get(i);
        } else cout << firstRow->get(i) << " ";
    }
    cout << endl;
    List<int>* row;
    for(int i = 0; i < min(nRows, data->length()); i++) {
        row = data->get(i);
        for(int j = 0; j < min(nCols, row->length()); j++) {
            if(j == min(nCols, row->length()) - 1) {
                cout << row->get(j);
            } else cout << row->get(j) << " ";
        }
        if(i != min(nRows, data->length()) - 1) cout << endl;
    }
}

void Dataset::printTail(int nRows, int nCols) const {
    if(nRows < 0 || nCols < 0) return;
    for(int i = max(0, firstRow->length() - nCols); i < firstRow->length(); i++) {
        if(i == firstRow->length() - 1) {
            cout << firstRow->get(i);
        } else cout << firstRow->get(i) << " ";
    }
    cout << endl;
    List<int>* row;
    for(int i = max(0, data->length() - nRows); i < data->length(); i++) {
        row = data->get(i);
        for(int j = max(0, row->length() - nCols); j < row->length(); j++) {
            if(j == row->length() - 1) {
                cout << row->get(j);
            } else cout << row->get(j) << " ";
        }
        if(i != data->length() - 1) cout << endl;
    }
}

void Dataset::getShape(int& nRows, int& nCols) const {
    nRows = data->length();
    int count = 0;
    List<int>* row = data->get(0);
    for(int i = 0; i < row->length(); i++) count++;
    nCols = count;
}

void Dataset::columns() const {
    for(int i = 0; i < firstRow->length() - 1; i++) {
        cout << firstRow->get(i) << " ";
    }
    cout << firstRow->get(firstRow->length() - 1);
}

bool Dataset::drop(int axis, int index, std::string columnName) {
    if(axis != 0 && axis != 1) return false;
    if(axis == 0) {
        if(index < 0 || index >= data->length()) {
            return false;
        }
        data->remove(index);
    } else {
        if(columnName == "") {
            return false;
        }
        int colIndex = -1;
        for(int i = 0; i < firstRow->length(); i++) {
            if(columnName == firstRow->get(i)) {
                colIndex = i;
                break;
            }
        }
        if(colIndex == -1) return false;
        firstRow->remove(colIndex);
        for(int i = 0; i < data->length(); i++) {
            data->get(i)->remove(colIndex);
        }
    }
    return true;
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const {
    Dataset subset;
    if(startRow < 0) startRow = 0;
    if(endRow == -1 || endRow >= data->length()) {
        endRow = data->length() - 1;
    }
    if(startCol < 0) startCol = 0;
    if(endCol == -1 || endCol >= data->get(0)->length()) {
        endCol = data->get(0)->length() - 1;
    }
    for(int i = startRow; i <= endRow; i++) {
        List<int>* row = new DLinkedList<int>();
        for(int j = startCol; j <= endCol; j++) {
            row->push_back(data->get(i)->get(j));
        }
        subset.data->push_back(row);
    }
    for(int i = startCol; i <= endCol; i++) {
        subset.firstRow->push_back(firstRow->get(i));
    }
    return subset;
}

void kNN::fit(const Dataset& X_train, const Dataset& y_train) {
    this->X_train = X_train;
    this->y_train = y_train;
}

/* --------------- Calculate Euclidean distance ------------------ */
double euclidean_distance(const List<int>& a, const List<int>& b) {
    double sum = 0;
    for(int i = 0; i < a.length(); i++) {
        sum += pow(a.get(i) - b.get(i), 2);
    }
    return sqrt(sum);
}
/* --------------------------------------------------------------- */

/* ------------------ Sort List of distance ---------------------- */
void swap(double& a, double& b) {
    double temp = a;
    a = b;
    b = temp;
}

int partition(DLinkedList<double>& distances, DLinkedList<int>& labels, int low, int high) {
    double pivot = distances.get(high);
    int i = (low - 1);
    for(int j = low; j <= high - 1; j++) {
        if(distances.get(j) < pivot) {
            i++;
            swap(distances.get(i), distances.get(j));
            swap(labels.get(i), labels.get(j));
        }
    }
    swap(distances.get(i + 1), distances.get(high));
    swap(labels.get(i + 1), labels.get(high));
    return (i + 1);
}

void quickSort(DLinkedList<double>& distances, DLinkedList<int>& labels, int low, int high) {
    if(low < high) {
        int pi = partition(distances, labels, low, high);
        quickSort(distances, labels, low, pi - 1);
        quickSort(distances, labels, pi + 1, high);
    }
}

void sortDistances(DLinkedList<double>& distances, DLinkedList<int>& labels) {
    quickSort(distances, labels, 0, distances.length() - 1);
}
/* ----------------------- Quick Sort ---------------------------- */

Dataset kNN::predict(const Dataset& X_test) {
    Dataset X_test_subset = X_test.extract(0, X_test.length() - 1);
    Dataset y_pred;
    if(X_train.length() == 0 || y_train.length() == 0) return y_pred;
    y_pred.getFirstRow()->push_back("label");
    for(int i = 0; i < X_test_subset.length(); i++) {
        DLinkedList<double> distances;
        DLinkedList<int> labels;
        for(int j = 0; j < X_train.length(); j++) {
            double distance = euclidean_distance(*X_test_subset.get(i), *X_train.get(j));
            distances.push_back(distance);
            labels.push_back(y_train.get(j)->get(0));
        }
        sortDistances(distances, labels);
        DLinkedList<int> counts;
        for(int j = 0; j < 10; j++) {
            counts.push_back(0);
        }
        for(int j = 0; j < k; j++) {
            if(j >= labels.length()) break;
            counts.get(labels.get(j))++;
        }
        int max_count = 0;
        int pred = -1;
        for(int j = 0; j < counts.length(); j++) {
            if(counts.get(j) > max_count) {
                max_count = counts.get(j);
                pred = j;
            }
        }
        List<int>* row = new DLinkedList<int>();
        row->push_back(pred);
        y_pred.getData()->push_back(row);
    }
    return y_pred;
}

double kNN::score(const Dataset& y_test, const Dataset& y_pred) {
    int correct = 0;
    for(int i = 0; i < y_test.length(); i++) {
        if(y_test.get(i)->get(0) == y_pred.get(i)->get(0)) {
            correct++;
        }
    }
    return static_cast<double>(correct) / y_test.length();
}

void train_test_split(Dataset& X, Dataset& y, double test_size, Dataset& X_train,
Dataset& X_test, Dataset& y_train, Dataset& y_test) {
    int nRows, nCols;
    X.getShape(nRows, nCols);

    if(test_size == 1) return;

    // Get number of rows for training set
    int train_size = static_cast<int>(nRows * (1 - test_size));

    // Split X into X_train and X_test
    X_train = X.extract(0, train_size - 1);
    X_test = X.extract(train_size, nRows - 1);

    // Split y into y_train and y_test
    y_train = y.extract(0, train_size - 1);
    y_test = y.extract(train_size, nRows - 1);
}
