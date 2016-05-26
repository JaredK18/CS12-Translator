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
bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm);
int variableDeclaration(char fileContents[][1000], int rowIndex, int columnIndex);
void consoleOutput(char fileContents[][1000], int& rowIndex, int& columnIndex, ofstream &outputPy, ofstream &outputTxt);
template <typename genericOutput> void fileOutput(genericOutput output, ofstream &outputPy, ofstream &outputTxt){
    outputPy << output;
    outputTxt << output;
}
//void pyFileCreator(char )
//void print(string maybe);

int main(int argc, char** argv){
    ofstream outputPy;
    outputPy.open("outputpy.py", ios::app);
    ofstream outputTxt;
    outputTxt.open("outputtxt.txt", ios::app);
    ifstream input;
    input.open("input.txt");
    /*ofstream outputPy;
    outputPy.open("outputpy.py");
    ofstream outputTxt;
    outputTxt.open("outputtxt.txt");*/
    char fileContents[1000][1000];
    int rowIndex = 0;
    int columnIndex = 0;
    int lastChar = 0;
    int lastLine = 0;
    int tabs = 0;
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
            /*columnIndex = columnIndex + 4;
            fileOutput("print (");
            while(fileContents[rowIndex][columnIndex] == ' ' or fileContents[rowIndex][columnIndex] == '<'){
                columnIndex++;
            }
            while(fileContents[rowIndex][columnIndex] != ';'){
                if(fileContents[rowIndex][columnIndex] == '"'){
                    inQuotes = true;
                }
                while(inQuotes){
                    fileOutput(fileContents[rowIndex][columnIndex]);
                    columnIndex++;
                    if(fileContents[rowIndex][columnIndex] == '"'){
                        inQuotes = false;
                    }
                }
                if(stringMatch(fileContents, rowIndex, columnIndex, "<<")){
                    columnIndex = columnIndex + 2;
                    fileOutput(fileContents[rowIndex][columnIndex]);
                }
                else if(fileContents[rowIndex][columnIndex] == ' '){
                    columnIndex++;
                }
                else if(fileContents[rowIndex][columnIndex] == '\n'){                    
                    fileOutput(")\n");
                    rowIndex++;
                    columnIndex = 0;    
                    fileOutput("print (");
                }
                else{
                    fileOutput(fileContents[rowIndex][columnIndex]);
                    columnIndex++;
                }
            }
            columnIndex++;
            fileOutput(")");*/
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "if(")){
            fileOutput("if ", outputPy, outputTxt);
            columnIndex = columnIndex + 3;
            tabs++;
            while(fileContents[rowIndex][columnIndex] != ')'){
                fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
                columnIndex++;
            }
            columnIndex = columnIndex + 2;
            fileOutput(":", outputPy, outputTxt);
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "else if(")){
            fileOutput("elif ", outputPy, outputTxt);
            columnIndex = columnIndex + 8;
            tabs++;
            while(fileContents[rowIndex][columnIndex] != ')'){
                fileOutput(fileContents[rowIndex][columnIndex], outputPy, outputTxt);
                columnIndex++;
            }
            columnIndex = columnIndex + 2;
            fileOutput(":", outputPy, outputTxt);
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "else{")){
            fileOutput("else:", outputPy, outputTxt);
            columnIndex = columnIndex + 5;
            tabs++;
        }
        else if(fileContents[rowIndex][columnIndex] == '}'){
            columnIndex++;
            doPrint = false;
            tabs--;
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
}
