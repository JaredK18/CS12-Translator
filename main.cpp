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
void consoleOutput(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt);
void ifStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt);
void caseStatement(char fileContents[][1000], int &rowIndex, int &columnIndex, ofstream &outputPy, ofstream &outputTxt, int switchVariableRow, int switchVariableColumn);
void whileLoopStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt);
template <typename genericOutput> void fileOutput(genericOutput output, ofstream &outputPy, ofstream &outputTxt){
    outputPy << output;
    outputTxt << output;
}

int main(int argc, char** argv){
    ofstream outputPy;
    outputPy.open("outputpy.py");
    ofstream outputTxt;
    outputTxt.open("outputtxt.txt");
    ifstream input;
    input.open("input.txt");
    char fileContents[1000][1000];
    int rowIndex = 0;
    int columnIndex = 0;
    int lastChar = 0;
    int lastLine = 0;
    int switchVariableColumn = 0;
    int switchVariableRow = 0;
    bool beforeCode = false;
    bool doPrint = true;
    while(input.get(fileContents[lastLine][lastChar])){
        cout << fileContents[lastLine][lastChar];
        if(fileContents[lastLine][lastChar] == '\n'){
            lastChar = -1;
            lastLine++;
        }
        lastChar++;
    }
    input.close();
    //this puts one line of code into the array newLine
    while((rowIndex != lastLine) or (columnIndex <= lastChar)){
		//Process includes
        if(stringMatch(fileContents, rowIndex, columnIndex, "#include ")){
            rowIndex++;
            columnIndex = 0;
            doPrint = false;
        }
        //Process variable declarations
        columnIndex = columnIndex + variableDeclaration(fileContents, rowIndex, columnIndex);
        if(fileContents[rowIndex][columnIndex] == ';'){
            columnIndex++;
        }
	//Process outputs to the console
        else if(stringMatch(fileContents, rowIndex, columnIndex, "cout")){
            consoleOutput(fileContents, rowIndex, columnIndex, outputPy, outputTxt);
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "switch (")){
            columnIndex = columnIndex + 8;
            switchVariableRow = rowIndex;
            switchVariableColumn = columnIndex;
            rowIndex++;
            columnIndex = 0;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "switch(")){
            columnIndex = columnIndex + 7;
            switchVariableRow = rowIndex;
            switchVariableColumn = columnIndex;
            rowIndex++;
            columnIndex = 0;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "case ")){
            columnIndex = columnIndex + 5;
            caseStatement(fileContents, rowIndex, columnIndex, outputPy, outputTxt, switchVariableRow, switchVariableColumn);
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "while(")){
            fileOutput("while ");
            columnIndex = columnIndex + 6;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "default:")){
            columnIndex = 0;
            rowIndex++;
            fileOutput("else:\n", outputPy, outputTxt);
        }
        //Process if/else/else if
        ifStatement(fileContents, rowIndex, columnIndex, outputPy, outputTxt);
        if(fileContents[rowIndex][columnIndex] == '}'){
            columnIndex++;
            doPrint = false;
        }
        if(doPrint){
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        }
        while(fileContents[rowIndex][columnIndex] == '\n'){
            rowIndex++;
            columnIndex = 0;
            doPrint = false;
        }
        if(doPrint){
            columnIndex++;
        }
        doPrint = true;
    }
    outputPy.close();
    outputTxt.close();
    return 0;
}


bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm){
    bool match = true;
    for(int i = startPosition; match and i < (searchTerm.length() + startPosition); i++){
        if(fileContents[rowIndex][i] != searchTerm.at(i-startPosition)){
            match = false;
        }
    }
    return match;
}

int variableDeclaration(char fileContents[][1000], int rowIndex, int columnIndex){
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

void consoleOutput(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt){
    bool inQuotes = false;
    columnIndex = columnIndex + 4;
    fileOutput("print (", outputPy, outputTxt);
    while(fileContents[rowIndex][columnIndex] == ' ' or fileContents[rowIndex][columnIndex] == '<'){
        columnIndex++;
    }
    while(fileContents[rowIndex][columnIndex] != ';'){
        if(fileContents[rowIndex][columnIndex] == '"'){
            inQuotes = true;
        }
        while(inQuotes){
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
            columnIndex++;
            if(fileContents[rowIndex][columnIndex] == '"'){
                inQuotes = false;
            }
        }
        if(stringMatch(fileContents, rowIndex, columnIndex, "<<")){
            columnIndex = columnIndex + 2;
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        }
        else if(fileContents[rowIndex][columnIndex] == ' '){
            columnIndex++;
        }
        else if(fileContents[rowIndex][columnIndex] == '\n'){                    
            fileOutput(")\n", outputPy, outputTxt);
            rowIndex++;
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

void ifStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt){
    if(stringMatch(fileContents, rowIndex, columnIndex, "if(")){
        fileOutput("if ", outputPy, outputTxt);
        columnIndex = columnIndex + 3;
        while(fileContents[rowIndex][columnIndex] != ')'){
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
            columnIndex++;
        }
        columnIndex = columnIndex + 2;
        fileOutput(":", outputPy, outputTxt);
        return;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "else if(")){
        fileOutput("elif ", outputPy, outputTxt);
        columnIndex = columnIndex + 8;
        while(fileContents[rowIndex][columnIndex] != ')'){
            fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
            columnIndex++;
        }
        columnIndex = columnIndex + 2;
        fileOutput(":", outputPy, outputTxt);
        return;
    }
    else if(stringMatch(fileContents, rowIndex, columnIndex, "else{")){
        fileOutput("else:", outputPy, outputTxt);
        columnIndex = columnIndex + 5;
        return;
    }
}

void caseStatement(char fileContents[][1000], int &rowIndex, int &columnIndex, ofstream &outputPy, ofstream &outputTxt, int switchVariableRow, int switchVariableColumn){
    int i = 0;
    fileOutput("if ", outputPy, outputTxt);
    while(fileContents[switchVariableRow][switchVariableColumn+i] != ')'){
        fileOutput(fileContents[switchVariableRow][switchVariableColumn+i], outputPy, outputTxt);
        i++;
    }
    fileOutput(" == ", outputPy, outputTxt);
    while(fileContents[rowIndex][columnIndex] != ':'){
        fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        columnIndex++;
    }
    columnIndex++;
    return;
}

void whileLoopStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt){
    
}
