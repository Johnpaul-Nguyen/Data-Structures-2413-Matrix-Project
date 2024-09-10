A rectangular array of numbers is called a matrix. Sometimes, a matrix is referred to as a table,
2-dimensional array, or array of arrays. Consider the following matrix below. It has 5 rows (denoted n) and 8
columns (denoted m). As you can see the row and column numbers start with a 0. Many entries might be
zero in matrix computations, especially with large matrices. Storing all elements, including zeros, would be
inefficient regarding memory and computational resources. The Sparse Matrix Representation (SMR)
efficiently stores and operates on matrices where most the elements are zero. Instead of storing every
element, we only store the non-zero elements along with their positions in the matrix, which reduces the
amount of memory used and can make certain operations more efficient. This project will help you learn
how to implement and work with this data structure using object-oriented principles by encapsulating it
within a class and relevant operations.

This project focuses on manipulating and storing matrices using a specific data structure called Sparse Matrix
Representation (SMR). We will encapsulate the SMR within a class, including a set of methods designed to
operate on matrices stored in this format.
