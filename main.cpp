#include <cstdlib>
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace std;

/*
 * 
 */
bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm);
int variableDeclaration(char fileContents[][1000], int rowIndex, int columnIndex);
void consoleOutput(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt, int& tabs);
void ifStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt, int& tabs);
void caseStatement(char fileContents[][1000], int &rowIndex, int &columnIndex, ofstream &outputPy, ofstream &outputTxt, int switchVariableRow, int switchVariableColumn, int& tabs, bool& firstCase);
void tabPrinter(char fileContents[][1000], int& rowIndex, int &columnIndex, int& tabs, ofstream &outputPy, ofstream &outputTxt);
template <typename genericOutput> void fileOutput(genericOutput output, ofstream &outputPy, ofstream &outputTxt){
    outputPy << output;
    outputTxt << output;
    cout << output;
}

int main(int argc, char** argv){
    //open input and output files
    ofstream outputPy;
    outputPy.open("outputpy.py");
    ofstream outputTxt;
    outputTxt.open("outputtxt.txt");
    ifstream input;
    input.open("input.txt");
    //this array holds the input file
    char fileContents[1000][1000];
    //the current row number in the file contents array
    int rowIndex = 0;
    //the current column number in the file contents array
    int columnIndex = 0;
    //the value of the last column in the last row
    int lastChar = 0;
    //the value of the last row
    int lastLine = 0;
    // the current number of tabs needed after a new line
    int tabs = 0;
    //the row an column for the location of the variable being used in a switch
    int switchVariableColumn = 0;
    int switchVariableRow = 0;
    //used to know whether to output if or elif
    bool firstCase = true;
    //if true the current character prints
    bool doPrint = true;
    //inputs the input file into the array
    while(input.get(fileContents[lastLine][lastChar])){
        if(fileContents[lastLine][lastChar] == '\n'){
            lastChar = -1;
            lastLine++;
        }
        lastChar++;
    }
    input.close();
    while((rowIndex != lastLine) or (columnIndex <= lastChar)){ //will end after reaching the end of the file
	// remove "#include" statements
        if(stringMatch(fileContents, rowIndex, columnIndex, "#include ")){
            tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            doPrint = false;
        }
        //Process variable declarations
        columnIndex = columnIndex + variableDeclaration(fileContents, rowIndex, columnIndex);
        //skips semi colons
        if(fileContents[rowIndex][columnIndex] == ';'){
            columnIndex++;
        }
	//Process outputs to the console
        else if(stringMatch(fileContents, rowIndex, columnIndex, "cout")){
            consoleOutput(fileContents, rowIndex, columnIndex, outputPy, outputTxt, tabs);
        }
        //processes switches
        else if(stringMatch(fileContents, rowIndex, columnIndex, "switch (")){
            columnIndex = columnIndex + 8;
            switchVariableRow = rowIndex;
            switchVariableColumn = columnIndex;
            tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            columnIndex = 0;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "switch(")){
            columnIndex = columnIndex + 7;
            switchVariableRow = rowIndex;
            switchVariableColumn = columnIndex;
            tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            columnIndex = 0;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "case ")){
            columnIndex = columnIndex + 5;
            caseStatement(fileContents, rowIndex, columnIndex, outputPy, outputTxt, switchVariableRow, switchVariableColumn, tabs, firstCase);
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "default:")){
            columnIndex = 0;
            tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            fileOutput("else:\n", outputPy, outputTxt);
            firstCase = true;
        }
        //processes while loops
        else if(stringMatch(fileContents, rowIndex, columnIndex, "while(")){
            fileOutput("while(", outputPy, outputTxt);
            columnIndex = columnIndex + 6;
            while(fileContents[rowIndex][columnIndex] != ')'){
                fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
                columnIndex++;
            }
            fileOutput("):", outputPy, outputTxt);
            columnIndex = columnIndex + 1;
        }
        //Process if/else/else if
        ifStatement(fileContents, rowIndex, columnIndex, outputPy, outputTxt, tabs);
        // processes end lines and ends of "code chunks"
        if(fileContents[rowIndex][columnIndex] == '}'){
            tabs--;
            columnIndex++;
            if(fileContents[rowIndex][columnIndex] == '\n'){
                tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            }
            doPrint = false;
        }
        //processes starts of code chunks
        if(fileContents[rowIndex][columnIndex] == '{'){
            tabs++;
            columnIndex++;
            doPrint = false;
        }
        //prints the current char if doPrint is true
        if(doPrint){
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        }
        //skips multiple empty lines
        while(fileContents[rowIndex][columnIndex] == '\n'){
            tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            doPrint = false;
        }
        if(doPrint){
            columnIndex++;
        }
        doPrint = true;
    }
    //close files
    outputPy.close();
    outputTxt.close();
    return 0;
}


bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm){
    bool match = true;
    //steps throught the array character by character and sets match to false if a character doesnt match the inputed search term
    for(int i = startPosition; match and i < (searchTerm.length() + startPosition); i++){
        if(fileContents[rowIndex][i] != searchTerm.at(i-startPosition)){ //true when there is no match
            match = false;
        }
    }
    return match;
}

int variableDeclaration(char fileContents[][1000], int rowIndex, int columnIndex){
    //looks for the basic variable types and skips past them because python doesn't use types in declarations
    if(stringMatch(fileContents, rowIndex, columnIndex, "int ")){
        return 4;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "bool ")){
        return 5;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "char ")){
        return 5;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "string ")){
        return 7;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "double ")){
        return 7;
    }
    else{
        return 0;
    }
}

void consoleOutput(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt, int& tabs){
    bool inQuotes = false; //bool to ensure our code doesnt alter teh code in quotes
    columnIndex = columnIndex + 4; //skipes the "cout"
    fileOutput("print (", outputPy, outputTxt); //outputs the console output format for python
    //skips any spaces and "<"
    while(fileContents[rowIndex][columnIndex] == ' ' or fileContents[rowIndex][columnIndex] == '<'){
        columnIndex++;
    }
    //runs untill reaching the end of the cout statement
    while(fileContents[rowIndex][columnIndex] != ';'){
        if(fileContents[rowIndex][columnIndex] == '"'){
            inQuotes = true;
        }
        while(inQuotes){ //outputs char for char untill it is out of quotes
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
            columnIndex++;
            if(fileContents[rowIndex][columnIndex] == '"'){
                inQuotes = false;
            }
        }
        if(stringMatch(fileContents, rowIndex, columnIndex, "<<")){ //skips "<<" because it is not part of python
            columnIndex = columnIndex + 2;
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        }
        else if(fileContents[rowIndex][columnIndex] == ' '){ // skips empty spaces
            columnIndex++;
        }
        else if(fileContents[rowIndex][columnIndex] == '\n'){ //if a cout statement is cut into two lines it makes a separate print statement for each line
            fileOutput(")\n", outputPy, outputTxt);
            tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            columnIndex = 0;    
            fileOutput("print (", outputPy, outputTxt);
        }
        else{
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
            columnIndex++;
        }
    }
    columnIndex++;
    fileOutput(")", outputPy, outputTxt);
    return;
}

void ifStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt, int& tabs){
    if(stringMatch(fileContents, rowIndex, columnIndex, "if(")){ // when an if is found it copies the input untill the "{" where it replaces it with a colon
        fileOutput("if(", outputPy, outputTxt);
        columnIndex = columnIndex + 3;
        do{
            columnIndex++;
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        }while(fileContents[rowIndex][columnIndex] != ')');
        fileOutput(":", outputPy, outputTxt);
        columnIndex = columnIndex + 1;
        return;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "else if(")){ // does the same as if but python uses "elif" instead of "else if"
        fileOutput("elif ", outputPy, outputTxt);
        columnIndex = columnIndex + 8;
        do{
            columnIndex++;
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        }while(fileContents[rowIndex][columnIndex] != ')');
        fileOutput(":", outputPy, outputTxt);
        columnIndex = columnIndex + 1;
        return;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "else{")){ //changes "else{" to "else:"
        fileOutput("else:", outputPy, outputTxt);
        columnIndex = columnIndex + 5;
        return;
    }
}

void caseStatement(char fileContents[][1000], int &rowIndex, int &columnIndex, ofstream &outputPy, ofstream &outputTxt, int switchVariableRow, int switchVariableColumn, int& tabs, bool& firstCase){
    //python does not have switches so it changes it to if/elif statements
    int i = 0;
    if(firstCase){
        fileOutput("if ", outputPy, outputTxt);
        firstCase = false;
    }
    else{
        fileOutput("elif ", outputPy, outputTxt); 
    }
    while(fileContents[switchVariableRow][switchVariableColumn+i] != ')'){ //prints the variable being used for the switch 
        fileOutput(fileContents[switchVariableRow][switchVariableColumn+i], outputPy, outputTxt);
        i++;
    }
    fileOutput(" == ", outputPy, outputTxt);
    while(fileContents[rowIndex][columnIndex] != ':'){
        fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        columnIndex++;
    }
    fileOutput(":", outputPy, outputTxt); 
    columnIndex++;
    return;
}
void tabPrinter(char fileContents[][1000], int& rowIndex, int &columnIndex, int& tabs, ofstream &outputPy, ofstream &outputTxt){
    rowIndex++;
    columnIndex = 0;
    while(fileContents[rowIndex][columnIndex] == ' ' or fileContents[rowIndex][columnIndex] == '\t' or fileContents[rowIndex][columnIndex] == '\n' or fileContents[rowIndex][columnIndex] == '}'){
        if(fileContents[rowIndex][columnIndex] == '}'){
            tabs--;
            columnIndex++;
            if(fileContents[rowIndex][columnIndex] == '\n'){
                tabPrinter(fileContents, rowIndex, columnIndex, tabs, outputPy, outputTxt);
            }
        }
        if(fileContents[rowIndex][columnIndex] == '\n'){
            rowIndex++;
            columnIndex = -1;
        }
        columnIndex++;
    }
    for(int i = 1; i <= tabs; i++){
        fileOutput("\t", outputPy, outputTxt);
    }
    return;
}
