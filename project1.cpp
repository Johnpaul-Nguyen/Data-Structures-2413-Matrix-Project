#include <iostream>
using namespace std;

class SparseRow{
    protected:
        int row; // Row#
        int col; // Column#
        int value; // We will assume that all our values will be integers
    public:
        SparseRow(); // Default constructor; row = -1; col = -1; value = 0
        SparseRow(int rowNum, int colNum, int val); // Constructor with parameters
        void display(); // print Row#, Colum#, value
        friend ostream& operator<< (ostream& s, const SparseRow);
        // Other methods that are necessary such as get and set
        int getRow(); 
        int getCol(); 
        int getValue();
        void setRow(int r);
        void setCol(int c);
        void setValue(int v);
};

/* 
*  LLM: When asking ChatGPT on how to improve the constructor (initially reading the file inputs in contructor instead of in the main function), 
*  ChatGPT responded in an improve way to clean up the constructor that I have not seen before. Constructing the constructor in this way improves readability and
*  sets variables with no need to assign them within curly brakets
*/
SparseRow::SparseRow() : row(-1), col(-1), value(0){} // Default constructor
SparseRow::SparseRow(int rowNum, int colNum, int val) : row(rowNum), col (colNum), value(val){}
int SparseRow::getRow() { return row; }
int SparseRow::getCol() { return col; }
int SparseRow::getValue() { return value; }
void SparseRow::setRow(int r) { row = r; }
void SparseRow::setCol(int c){ col = c; }
void SparseRow::setValue(int v){ value = v; }

/* 
*  LLM: I asked ChatGPT how would I implement a method using an ostream& operator<< method. I was not sure how to use an ostream operator<< method and
*  to my surprise, it looked a lot easier than expected. 
*/
ostream& operator<< (ostream& s, const SparseRow sr){ 
    s << sr.row << ", " << sr.col << ", " << sr.value;
    return s;
}

void SparseRow::display(){
    cout << "Rows: " << row << " Columns: " << col << " Value: " << value << endl;
}

class SparseMatrix{
    protected:
        int numRows; // Number of rows of the original matrix
        int numCols; // Number of columns of the orginal matrix
        int commonValue; // Read from input
        int sparseValue;
        SparseRow* myMatrixArr; // Array
        int size; // Used for Insert method
    public:
        SparseMatrix();
        SparseMatrix(int n, int m, int cv, int nonCV);
        
        SparseMatrix* Transpose();
        SparseMatrix* Multiply(SparseMatrix& M);
        SparseMatrix* Add (SparseMatrix& M);
        friend ostream& operator<< (ostream& s, const SparseMatrix& sm);
        void displayMatrix(); // Display the matrix in its original format
        
        // Other methods that are necessary such as get and set
        void Insert(int row, int col, int value);
        int getNumRow();
        int getNumCol();
        int getCV();
        int getSV();
        int getValueAt(int row, int col);
        void setNumRow(int r);
        void setNumCol(int c);
        void setCV(int cv);
        void setSV(int sv);

        // Destructor
        ~SparseMatrix();
};

SparseMatrix::SparseMatrix() : numRows(0), numCols(0), commonValue(0), sparseValue(0), size(0){}
SparseMatrix::SparseMatrix(int n, int m, int cv, int sv) : numRows(n), numCols(m), commonValue(cv), sparseValue(sv), size(0) {
    myMatrixArr = new SparseRow[sparseValue]; // Initialize array dynamically
}

/* 
*  LLM: Asked ChatGPT to help me explain why initializing myMatrixArr was yielding me unexpecting, large values in indexes that were not touched
*  when first initialized (happened in the early parts of developing this project). ChatGPT gave me a suggestion to create myMatrixArr with the number of 
*  sparseValues in the matrix and insert only when an input that was read in was not equal to commonValue. This function serves to insert values into myMatrixArr
*  and increase its size as values are inserted. If or when values exceed the size of myMatrixArr, else is called to help debug the code. This function helped 
*  improve the Transpose, Addition, and Multiply functions, as well as reading in the matrices. 
*/
// This function's purpose is to insert non-common values into the sparse matrix at a specified row and column
void SparseMatrix::Insert(int row, int col, int value){
    if (size < sparseValue){
        myMatrixArr[size] = SparseRow(row, col, value);
        size++;
    }
    else{
        cout << "Exceeded SparseValue limit - Insert function" << endl; // Debug Line for when size exceed myMatrixArr's length
    }
}
int SparseMatrix::getNumRow(){ return numRows; }
int SparseMatrix::getNumCol(){ return numCols; }
int SparseMatrix::getCV(){ return commonValue; }
int SparseMatrix::getSV(){ return sparseValue; }

/* 
*  LLM: The getValueAt function proved Very helpful in the making of the Addition and Multiply functions. I asked Chat to help me with the Multiply function 
*  (at the time, I was using a for loop to iterate through and index of row and col to find if there was a value in a certain place), and Chat produced code that
*  did not seem to help me until I saw the getValueAt function be called. I thought making this function would be of great use and it was. This function was made
*  after I first finished the Addition function and was then working on the Multiply method, so I later went back and used the getValueAt function to inprove
*  the Addition and Multiply functions, as well as the displayMatrix function.
*/
// This function's purpose is to retrieve the value at a specific column and row
int SparseMatrix::getValueAt(int rowInput, int colInput){
    for (int i = 0; i < sparseValue; i++){
        if (myMatrixArr[i].getRow() == rowInput && myMatrixArr[i].getCol() == colInput){
            return myMatrixArr[i].getValue();
        }
    }
    return commonValue; // If row and col inputs do not match, return commonValue
}
void SparseMatrix::setNumRow(int r){ numRows = r; }
void SparseMatrix::setNumCol(int c){ numCols = c; }
void SparseMatrix::setCV(int cv){ commonValue = cv; }
void SparseMatrix::setSV(int sv){ sparseValue = sv; }

// This function's purpose is to switch the row and column
SparseMatrix* SparseMatrix::Transpose(){
    // Create a new matrix to return
    SparseMatrix* transposedMatrix = new SparseMatrix(numRows, numCols, commonValue, sparseValue);    

    for (int i = 0; i < sparseValue; i++){
        // Invert values of myMatrix and insert into transposed matrix
        transposedMatrix->Insert(myMatrixArr[i].getCol(), myMatrixArr[i].getRow(), myMatrixArr[i].getValue());
    }

    return transposedMatrix;
}

/* 
*  LLM: Asked Chat to help me figure out why the Multiply function was not working in early developments of the Multiply function. 
*  Many problems that were arising were based on incorrect use of double for loops to multiply matrices. Chat had provided an code 
*  fragement that did not work later. I asked Chat again that I believed the second half of the Multiply function then was not working.
*  That is when I came across the getValueAt function. Instead of trying to iterate through the sparse matrix and keeping track of which
*  column and row i was on for both matrices and the resulting matrix, I took ideas of previous Chat code fragements and implemented
*  the multiply function from there.
*/
// This function's purpose is to multiply two matrices together
SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M){
    // Multiplying matrices together has a big prerequisite. The number of columns of the first matrix must equal the number of rows of the second marix
    // If this prerequisite is not met, multiplying matrices is not possible
    if (numCols != M.getNumRow()){
        cout << "Matrix multiplication is not possible" << endl;
        return nullptr; // return null pointer
    }
    else{
        // Count non-zero entries in the resulting matrix
        int count = 0;
        for (int i = 0; i < sparseValue; i++){
            for (int k = 0; k < M.sparseValue; k++){
                if (myMatrixArr[i].getCol() == M.myMatrixArr[k].getRow()){
                    count++;
                }
            }
        }

        // Initialize resulting matrix
        SparseMatrix* result = new SparseMatrix(numRows, M.getNumCol(), commonValue, count);

        int i = 0;
        int inputValue = 0;
        int colIndex1 = 0;
        int rowIndex1 = 0;
        int colIndex2 = 0;
        int rowIndex2 = 0;

        while (i <= result->sparseValue){
            // Goes through the steps of multiplying matrices
            inputValue += getValueAt(rowIndex1, colIndex1) * M.getValueAt(rowIndex2, colIndex2);

            if (colIndex1 >= (numCols - 1) && rowIndex2 >= (M.numRows - 1)){
                if (inputValue != commonValue){
                    result->Insert(rowIndex1, colIndex2, inputValue);
                    i++;

                    inputValue = 0; // reset for next iteration
                }
                
                // Reset for colIndex for first matrix and rowIndex for second matrix
                colIndex1 = 0;
                rowIndex2 = 0;

                // Increment to move to the next colIndex for scond matrix
                colIndex2++;
                if (colIndex2 == M.numCols){
                    // Increment first matrix rowIndex if colIndex2 is equal to numCols in the second matrix
                    rowIndex1++;
                }

                //reset inputValue for next iteration
                inputValue = 0;
            }
            else{
                colIndex1++;
                rowIndex2++;
            }
            
            // Check if gone beyond the bounds of the second matrix's columns
            if (colIndex2 >= M.getNumCol()) {
                colIndex2 = 0;  // Reset column index in the second matrix
            }

            // Check if gone beyond the bounds of the first matrix's rows
            if (rowIndex1 >= numRows) {
                break;  // End the loop if we've finished processing all rows
            }   
        }
        return result;
    }
}

// This function's purpose is to add two matrices together
SparseMatrix* SparseMatrix::Add(SparseMatrix& M){
    // Prerequisite of adding matrices is both matrices must be of the same size, meaning both row and column are the same
    if (numRows != M.getNumRow() || numCols != M.getNumCol()){
        cout << "Matrix addition is not possible" << endl;
        return nullptr; // Return nullptr if both matrice's row and column are not equal
    }

    // Figure out how many sparse values are in the resulting matrix
    int count = 0;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            // Using getValueAt function to retrieve values
            int val1 = getValueAt(row, col);
            int val2 = M.getValueAt(row, col);
            if (val1 != commonValue || val2 != M.getCV()) {
                count++;
            }
        }
    }
    
    // Creates result matrix
    SparseMatrix* result = new SparseMatrix(numRows, numCols, commonValue, count);
    
    // Nested for loops work in this case as addition of matrices is adding the values in the same row and column
    int row = 0, col = 0;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            int sum = getValueAt(row, col) + M.getValueAt(row, col);

            // Only store non-common values in the result
            if (sum != commonValue) {
                result->Insert(row, col, sum);
            }
        }
    }

    return result;
}

// LLM: Similar to SparseRow. I asked Chat how to implement an ostream& operator<< function to display a matrix and this is what Chat returned
ostream& operator<< (ostream& s, const SparseMatrix& sm){
    for (int i = 0; i < sm.sparseValue; i++){
        s << sm.myMatrixArr[i] << endl;
    }
    
    if (sm.sparseValue == 0){
        s << "Matrix has no sparse values" << endl;
    }
    return s;
}

void SparseMatrix::displayMatrix(){
    int index = 0;
    for (int i = 0; i < numRows; i++){
        for (int k = 0; k < numCols; k++){
            // Went back to change previous method of displaying to use getValueAt, became much simplier
            cout << getValueAt(i, k) << " ";
        }
        cout << endl;
    }
}

// Destructor to free dynamically allocated memory
SparseMatrix::~SparseMatrix(){
    delete[] myMatrixArr;
}

// Function to read in file
void readFileInput(SparseMatrix* matrix);

int main(){
    int n, m, cv, sv;
    SparseMatrix* temp;

    // Read inputs for first matrix
    cin >> n >> m >> cv >> sv;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, sv);
    readFileInput(firstOne);

    // Read inputs for second matrix
    cin >> n >> m >> cv >> sv;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, sv);
    readFileInput(secondOne);
    
    // Display first matrix in sparse matrix format
    cout << "First one in sparse matrix format" << endl;
    cout << (*firstOne);
    
    // Transposing and displaying first matrix
    cout << "After transpose" << endl;
    SparseMatrix* transposedFirstOne = firstOne->Transpose();
    cout << (*transposedFirstOne);
    
    // Display first matrix in original format
    cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();

    // Display second matrx in sparse matrix format
    cout << "Second one in sparse matrix format" << endl;
    cout << (*secondOne);
    
    // Transposing and displaying second matrix
    cout << "After transpose" << endl;
    SparseMatrix* transposedSecondOne = secondOne->Transpose();
    cout << (*transposedSecondOne);
    
    // Display second matrix in original format
    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();

    // Matrix Addition
    cout << "Matrix Addition Result" << endl;
    temp = firstOne->Add(*secondOne);
    if (temp != nullptr){
        (*temp).displayMatrix();
    }
    
    // Matrix Multiplication
    cout << "Matrix Multiplication Result" << endl;
    temp = (firstOne->Multiply(*secondOne));
    if (temp != nullptr){
        (*temp).displayMatrix();
    }

    // Delete to free memory
    delete temp;
    delete firstOne;
    delete secondOne;

    // End of main
    return 0;
}

// Reading in the matrix in the file
void readFileInput(SparseMatrix* matrix){
    int input;
    int k = 0;
    int row = 0;
    int col = 0;

    for (int i = 0; i < matrix->getNumRow() * matrix->getNumCol(); i++){
        cin >> input; // Iterates through the inputs for the matrix
        if (input != matrix->getCV()){
            if (k < matrix->getSV()){
                int r = row;
                int c = col;

                matrix->Insert(r, c, input);
                k++;
            }
            else{
                cout << "Exceeded SparseValue limit main function" << endl; // Debug line
            }
        }

        if (col == matrix->getNumCol() - 1){
            col = 0;
            row++;
        }
        else{
            col++;
        }
        
    }
}

/* 
*  When Debugging the program, I used cout to output values to debug (debugging in VSC did not like me). Many couts for debugging have been deleted
*  for output readibility and code readibility. 
*
*  Many of the problems I had was not iterating through the correct row/column in certian circumstances (Multiply function). Through lots of brain power,
*  I realized that I could use methods that Chat had helped me make for other functions could help me in the function I was working on. I implemented and 
*  debugging, changing a few things at a time (such as == to >=, changing row to col, for loops to while loops, etc). Other issues that occured are 
*  incorrect values and infinite loops. Again, many of these fixes were thanks to Chat providing functions that resolved many of my problems and 
*  through lots debugging cout lines. 
*
*  I tested the program using the different inputs given in the canvas page for Project 1. I was only testing the program with text1.txt up until towards 
*  the end. I tried testing other text inputs and found that I was reading in some of the inputs wrong. Values would be going to different rows and columns
*  and I checked the readFileInput function to realize that what I had would only work with matrices where the row and the column were equal. I changed the
*  function to iterate through the row and column of the matrix and that fixed reading in the inputs of the file and revealed more problems in the code that
*  I went back to fix (most problems that happend after that was not iterating through the correct row/column). 
*  
*  I also decided to test what would happen if we had a matrix with only 0's in all indeces with a matrix with minimal sparse values and another test with
*  both matrices with 0 sparse values. The display for the matrices with 0 sparse values did not output. I changed that to output "Matrix has no sparse value"
*  instead of outputting nothing. 
* 
*  I asked Chat to help me brainstorm some other tests to do and came up with invalid inputs. The fix would be interesting since the inputs for this project
*  is redirected inputs. No fix was implemented, but it was though of. 
*  
*/