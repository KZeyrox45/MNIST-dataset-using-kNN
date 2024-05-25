# Processing MNIST Dataset and Implementing kNN Algorithm
The MNIST dataset is one of the most popular datasets commonly used for testing and evaluating machine learning and computer vision algorithms. The name "**_MNIST_**" stands for "**_Modified National Institute of Standards and Technology_**". This dataset contains a set of
images captured from handwritten samples of digits from 0 to 9, with each image having a size of 28x28 pixels. The mnist.csv files contained in both files "_kNN using List_" and "_kNN using kDTree_" are the same, where:
> Each row describes an image of a digit, encoded into 28x28 = 714 columns and one label column.
> The label column assigns a label to the digit represented by the image. For each image, the value range of the cells in this column is from 0 to 9.
> The remaining columns, labeled ixj where i, j are within the range [1,28], represent the coordinates of the pixel. For each image, the value range of the cells in this column is from 0 to 255 (the darkness of the pixel).
## Algorithm
```
1. Load the data.
2. Initialize the value of k.
3. To get the predicted class, iterate from 1 to the total number of training data points.
4. Calculate the distance between the test data and each row of the training data. Here, we will use the Euclidean distance as the method for measuring distance as it’s the most common method.
5. Sort the calculated distances in ascending order based on the distance value.
6. Take the top k rows from the sorted array.
7. Take the most common class of these rows.
8. Return the predicted class.
```
