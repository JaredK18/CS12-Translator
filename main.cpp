/* 
 * File:   main.cpp
 * Author: Jared
 *
 * Created on May 10, 2016, 1:30 PM
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace std;

/*
 * 
 */
void functionDeclaration(char fileContents[][1000], int rowIndex, int columnIndex, int lastLine);
bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm);
int variableDeclaration(char fileContents[][1000], int rowIndex, int columnIndex);
void consoleOutput(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt);
void ifStatement(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt);
template <typename genericOutput> void fileOutput(genericOutput output, ofstream &outputPy, ofstream &outputTxt){
    outputPy << output;
    outputTxt << output;
}

int main(int argc, char** argv){
    ofstream outputPy;
    outputPy.open("outputpy.py");
    ofstream outputTxt;
    outputTxt.open("outputog.txt");
    ifstream input;
    input.open("input.txt");
    char fileContents[1000][1000];
    int rowIndex = 0;
    int columnIndex = 0;
    int lastChar = 0;
    int lastLine = 0;
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
        //Process if/else/else if
        ifStatement(fileContents, rowIndex, columnIndex, outputPy, outputTxt);
        if(fileContents[rowIndex][columnIndex] == '}'){
            columnIndex++;
            doPrint = false;
        }
        if(fileContents[rowIndex][columnIndex] == '{'){
            columnIndex++;
            fileOutput(":", outputPy, outputTxt);
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
    input.close();
    rowIndex = 0;
    columnIndex = 0;
    functionDeclaration(fileContents, rowIndex, columnIndex, lastLine);
    return 0;
}


void functionDeclaration(char fileContents[][1000], int rowIndex, int columnIndex,int lastLine){
    ofstream outputPy;
    outputPy.open("outputpy.py");
    ofstream outputTxt;
    outputTxt.open("outputtxt.txt");
    ifstream input;
    input.open("outputog.txt");
    bool isDone = false, foundFunction = false;;
    string functionDeclaration = "";
    int storedIndex = 0;//storedIndex is to store the start of the potential function definition. columnIndex still is used to read values
    int lastChar = 0;
    lastLine = 0;
    string storedDeclaration;
    while(input.get(fileContents[lastLine][lastChar])){
        cout << fileContents[lastLine][lastChar];
        if(fileContents[lastLine][lastChar] == '\n'){
            lastChar = -1;
            lastLine++;
        }
        lastChar++;
    }
    //cout << "Function function run!" << endl;
    /*while (fileContents[rowIndex][columnIndex] != ' '){
        cout << "Return type length: " << fileContents[rowIndex][columnIndex] << endl;
        columnIndex++;
    }
    columnIndex++;//Deals with the space after a return type
    */
    
    while((rowIndex != lastLine) or (columnIndex <= lastChar)){
        storedIndex = 0;
        columnIndex = 0;
        cout << endl;
        Sleep(1000);
        while (!isDone){
            if (fileContents[rowIndex][columnIndex] == '('){//Looks for the bracket indicating this is a function and not a variable
               foundFunction = true;
               isDone = true;//Means once this code is done, the loop won't run again
               while (fileContents[rowIndex][storedIndex] != '\n'){//Looks for the end of the declaration
                   cout << fileContents[rowIndex][storedIndex];
                   functionDeclaration += (fileContents[rowIndex][storedIndex]);//Stores the declaration in an array
                   storedIndex++;
               }
               if(fileContents[rowIndex][storedIndex - 1] == ':'){
                   foundFunction = false;
               }
            }
            else if(fileContents[rowIndex][columnIndex] == '\n'){
                rowIndex++;
                columnIndex = 0;
            }
            else {
            functionDeclaration += (fileContents[rowIndex][columnIndex]);
            cout << fileContents[rowIndex][columnIndex];
            columnIndex++;
            }
        }
        
        functionDeclaration[storedIndex] = '\0';
        storedDeclaration = string(functionDeclaration);//Converts declaration to a string, in order to use stringMatch
        //Looking for the actual declaration next
        columnIndex = 0;//reset columnIndex
        if (foundFunction){
            fileOutput(storedDeclaration, outputPy, outputTxt);
            fileOutput(':', outputPy, outputTxt);
            for(rowIndex = 0; rowIndex < lastLine; rowIndex++){
                if ((stringMatch(fileContents, rowIndex, 0, storedDeclaration)) and (stringMatch(fileContents, storedDeclaration.length(), 0, ":"))){
                    rowIndex++;
                    while(fileContents[rowIndex][columnIndex] != '}'){
                        fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
                        if (fileContents[rowIndex][columnIndex] == '\n'){
                            rowIndex++;
                            columnIndex = 0;
                        } else {
                            columnIndex++;
                        }
                    }
                }
            }
        }
        fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
        while(fileContents[rowIndex][columnIndex] == '\n'){
            rowIndex++;
            columnIndex = 0;
        }
        rowIndex++;
    }
    outputPy.close();
    outputTxt.close();
    input.close();
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
    else if(stringMatch(fileContents, rowIndex, columnIndex, "void ")){
        return 5;
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
