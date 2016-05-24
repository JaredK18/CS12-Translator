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
bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm);
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
	int tabs = 0;
	bool beforeCode = false;
    bool doPrint = true;
	bool inQuotes = false;
    while(input.get(fileContents[lastLine][lastChar])){
        ogOutput << fileContents[lastLine][lastChar];
        cout << fileContents[lastLine][lastChar];
        if(fileContents[lastLine][lastChar] == '\n'){            
            lastChar = -1;
            lastLine++;
        }
        lastChar++;
    }
    //this puts one line of code into the array newLine
    while((rowIndex != lastLine) or (columnIndex <= lastChar)){
		//Process includes
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
		//Process outputs to the console
        else if(stringMatch(fileContents, rowIndex, columnIndex, "cout")){
            columnIndex = columnIndex + 4;
            outputPy << "print (";
            outputTxt << "print (";
            while(fileContents[rowIndex][columnIndex] == ' ' or fileContents[rowIndex][columnIndex] == '<'){
                columnIndex++;
            }
            while(fileContents[rowIndex][columnIndex] != ';'){
                Sleep(50);
				if(fileContents[rowIndex][columnIndex] == '"'){
					inQuotes = true;
				}
				while(inQuotes){
					outputPy << fileContents[rowIndex][columnIndex];
                    outputTxt << fileContents[rowIndex][columnIndex];
                    if(fileContents[rowIndex][columnIndex] == '\\'){
						columnIndex++;
						outputPy << fileContents[rowIndex][columnIndex];
						outputTxt << fileContents[rowIndex][columnIndex];
					}
					else if(fileContents[rowIndex][columnIndex] == '"'){
						inQuotes = false;
					}
					columnIndex++;
                }
                if(stringMatch(fileContents, rowIndex, columnIndex, "<<")){
                    columnIndex = columnIndex + 2;
                    outputPy << fileContents[rowIndex][columnIndex];
                    outputTxt << fileContents[rowIndex][columnIndex];
                }
                else if(fileContents[rowIndex][columnIndex] == ' '){
                    columnIndex++;
                }
                else if(fileContents[rowIndex][columnIndex] == '\n'){                    
                    outputPy << ")\n";
                    outputTxt << ")\n";   
                    rowIndex++;
                    columnIndex = 0;    
                    outputPy << "print (";
                    outputTxt << "print (";
                }
                else{
                    outputPy << fileContents[rowIndex][columnIndex];
                    outputTxt << fileContents[rowIndex][columnIndex];
                    columnIndex++;
                }
            }
            columnIndex++;
            outputPy << ")";
            outputTxt << ")";
        }
		else if(stringMatch(fileContents, rowIndex, columnIndex, "if("){
			outputPy << "if ";
            outputTxt << "if ";
			tabs++;
			while(fileContents[rowIndex][columnIndex] != ')'){
				outputPy << fileContents[rowIndex][columnIndex];
                outputTxt << fileContents[rowIndex][columnIndex];
                columnIndex++;
			}
			outputPy << ":\n";
            outputTxt << ":\n";
		}
		else if(stringMatch(fileContents, rowIndex, columnIndex, "else if("){
			outputPy << "elif ";
            outputTxt << "elif ";
			tabs++;
			while(fileContents[rowIndex][columnIndex] != ')'){
				outputPy << fileContents[rowIndex][columnIndex];
                outputTxt << fileContents[rowIndex][columnIndex];
                columnIndex++;
			}
			outputPy << ":\n";
            outputTxt << ":\n";
		}
		else if(stringMatch(fileContents, rowIndex, columnIndex, "else("){
			outputPy << "else:\n";
            outputTxt << "else:\n";
			tabs++;
		}
		else if(fileContents[rowIndex][columnIndex] == '}'){
			columnIndex++;
			outputPy << endl;
            outputTxt << endl;
			doPrint = false;
			tabs--;
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


bool stringMatch(char fileContents[][1000], int rowIndex, int startPosition, string searchTerm){
    bool match = true;
    for(int i = startPosition; match and i < (searchTerm.length() + startPosition); i++){
        if(fileContents[rowIndex][i] != searchTerm.at(i-startPosition)){
            match = false;
        }
    }
    return match;
}