#include "kNN.hpp"

void tc1(){
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    dataset.printHead(10, 10);
    cout << endl;
    dataset.printTail(10, 10);
    cout << endl;
    int nRows, nCols;
    dataset.getShape(nRows, nCols);
    cout << "Shape: " << nRows << "x" << nCols << endl;
}

void tc2(){
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    List<int>* row = dataset.getData()->get(0);
    row->insert(0, 1);
    row->print();
}

void tc3(){
    Dataset dataset;
    dataset.loadFromCSV("mnist.csv");
    int nRows, nCols;
    dataset.getShape(nRows, nCols);
    cout << "Shape: " << nRows << "x" << nCols << endl;

    kNN knn;
    Dataset X_train, X_test, y_train, y_test;
    Dataset feature = dataset.extract(0, -1, 1, -1);
    Dataset label = dataset.extract(0, -1, 0, 0);
    train_test_split(feature, label, 0.2, X_train, X_test, y_train, y_test);

    knn.fit(X_train, y_train);
    Dataset y_pred = knn.predict(X_test);
    double accuracy = knn.score(y_test, y_pred);
    cout << "Accuracy: " << accuracy << endl;
}

int main() {
    tc1();
    return 0;
}