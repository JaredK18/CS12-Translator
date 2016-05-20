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
 * redooooo inputs into 4/2th demetion arrays so cades willz works
 */
bool stringMatch(char fileContents[][1000], int lineIndex, int startPosition, string searchTerm);
//void pyFileCreator(char )
//void print(string maybe);

int main(int argc, char** argv){
    ifstream input;
    input.open("input.txt");
    ofstream outputPy;
    outputPy.open("outputpy.py");
    ofstream outputTxt;
    outputTxt.open("outputtxt.txt");
    ofstream ogOutput;
    ogOutput.open("outputog.txt");
    char fileContents[1000][1000];
    int rowIndex = 0;
    int columnIndex = 0;
    int lastChar = 0;
    int lastLine = 0;
    bool doPrint = true;
    bool end = true;
    bool end2 = true;
    while(input.get(fileContents[lastLine][lastChar])){
        ogOutput << fileContents[lastLine][lastChar];
        cout << fileContents[lastLine][lastChar];
        if(fileContents[lastLine][lastChar] == '\n'){            
            lastChar = -1;
            lastLine++;
        }
        lastChar++;
        //cout << lastChar << endl;
    }
    //this puts one line of code into the array newLine
    while((rowIndex != lastLine)/*end*/ or (columnIndex <= lastChar)){
        if(stringMatch(fileContents, rowIndex, columnIndex, "#include ")){
            rowIndex++;
            columnIndex = 0;
            doPrint = false;
        }
        //Process variable declarations
        if(stringMatch(fileContents, rowIndex, columnIndex, "int ")){
            columnIndex = columnIndex + 4;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "bool ")){
            columnIndex = columnIndex + 5;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "char ")){
            columnIndex = columnIndex + 5;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "string ")){
            columnIndex = columnIndex + 7;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "double ")){
            columnIndex = columnIndex + 7;
        }
        else if(fileContents[rowIndex][columnIndex] == ';'){
            columnIndex++;
        }
        else if(stringMatch(fileContents, rowIndex, columnIndex, "cout")){
            columnIndex = columnIndex + 4;
            outputPy << "print (";
            outputTxt << "print (";
            while(fileContents[rowIndex][columnIndex] == ' ' or fileContents[rowIndex][columnIndex] == '<'){
                columnIndex++;
            }
            while(fileContents[rowIndex][columnIndex] != ';'){
                cout << "test loop 2\n" << columnIndex << endl;
                Sleep(50);
                if(stringMatch(fileContents, rowIndex, columnIndex, "<<")){
                    columnIndex = columnIndex + 2;
                    outputPy << fileContents[rowIndex][columnIndex];
                    outputTxt << fileContents[rowIndex][columnIndex];
                    cout << "test loop 77\n";
                }
                else if(fileContents[rowIndex][columnIndex] == ' '){
                    columnIndex++;
                    cout << "test loop 88\n";
                }
                //might not want this
                else if(fileContents[rowIndex][columnIndex] == '\n'){                    
                    outputPy << ")\n";
                    cout << "test loop 99\n";
                    outputTxt << ")\n";   
                    rowIndex++;
                    columnIndex = 0;    
                    outputPy << "print (";
                    outputTxt << "print (";
                }
                else{
                    cout << "test loop 00\n";
                    outputPy << fileContents[rowIndex][columnIndex];
                    outputTxt << fileContents[rowIndex][columnIndex];
                    columnIndex++;
                }
            }
            columnIndex++;
            outputPy << ")";
            outputTxt << ")";
        }
        if(doPrint){
            outputPy << fileContents[rowIndex][columnIndex];
            outputTxt << fileContents[rowIndex][columnIndex];
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
    input.close();
    outputPy.close();
    outputTxt.close();
    ogOutput.close();
    return 0;
}


bool stringMatch(char fileContents[][1000], int lineIndex, int startPosition, string searchTerm){
    bool match = true;
    for(int i = startPosition; match and i < (searchTerm.length() + startPosition); i++){
        if(fileContents[lineIndex][i] != searchTerm.at(i-startPosition)){
            match = false;
        }
    }
    return match;
}
