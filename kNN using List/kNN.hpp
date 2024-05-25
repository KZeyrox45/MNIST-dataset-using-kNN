#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

template <typename T>
class List {
public:
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T& get(int index) const = 0;
    virtual int length() const = 0 ;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;
};

template <typename T>
struct DNode {
    T data;
    DNode* next;
    DNode* prev;
    DNode(T val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DLinkedList : public List<T> {
private:
    DNode<T>* head;
    DNode<T>* tail;
    int size;
public:
    DLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~DLinkedList() {clear();}
    void push_back(T value);
    void push_front(T value);
    void insert(int index, T value);
    void remove(int index);
    T& get(int index) const;
    int length() const {return size;}
    void clear();
    void print() const;
    void reverse();
};

class Dataset {
private:
    List<List<int>*>* data;
    List<string>* firstRow;
    void clear();
    //You may need to define more
public:
    Dataset();
    ~Dataset() {clear();}
    Dataset(const Dataset& other);
    Dataset& operator=(const Dataset& other);
    bool loadFromCSV(const char* fileName);
    void printHead(int nRows = 5, int nCols = 5) const;
    void printTail(int nRows = 5, int nCols = 5) const;
    void getShape(int& nRows, int& nCols) const;
    void columns() const;
    bool drop(int axis = 0, int index = 0, std::string columns = "");
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const;
    List<int>* get(int index) const {return data->get(index);}
    int length() const {return data->length();}
    List<List<int>*>* getData() const {return data;}
    List<string>* getFirstRow() const {return firstRow;}
};

// kNN functions helper
double euclidean_distance(const List<int>& a, const List<int>& b);
void swap(double& a, double& b);
int partition(DLinkedList<double>& distances, DLinkedList<int>& labels, int low, int high);
void quickSort(DLinkedList<double>& distances, DLinkedList<int>& labels, int low, int high);
void sortDistances(DLinkedList<double>& distances, DLinkedList<int>& labels);

class kNN {
private:
    int k;
    Dataset X_train;
    Dataset y_train;
    //You may need to define more
public:
    kNN(int k = 5) : k(k) {}
    void fit(const Dataset& X_train, const Dataset& y_train);
    Dataset predict(const Dataset& X_test);
    double score(const Dataset& y_test, const Dataset& y_pred);
};

void train_test_split(Dataset& X, Dataset& y, double test_size, Dataset& X_train,
                      Dataset& X_test, Dataset& y_train, Dataset& y_test);

// Please add more or modify as needed
