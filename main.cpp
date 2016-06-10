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
void functionDeclaration(char fileContents[][1000], int rowIndex, int columnIndex, int lastLine, ofstream &outputPy, ofstream &outputTxt);
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
    outputTxt.open("outputtxt.txt");
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
        //Process functions
        functionDeclaration(fileContents, rowIndex, columnIndex, lastLine, outputPy, outputTxt);
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


void functionDeclaration(char fileContents[][1000], int rowIndex, int columnIndex,int lastLine, ofstream &outputPy, ofstream &outputTxt){
    bool isDone = false, foundFunction = false;;
    string functionDeclaration = "";
    int storedIndex = 0;
    string storedDeclaration;
    cout << "Function function run!" << endl;
    if((stringMatch(fileContents, rowIndex, columnIndex, "int ")) || (stringMatch(fileContents, rowIndex, columnIndex, "bool ")) || (stringMatch(fileContents, rowIndex, columnIndex, "char ")) || (stringMatch(fileContents, rowIndex, columnIndex, "void ")) || (stringMatch(fileContents, rowIndex, columnIndex, "string ")) || (stringMatch(fileContents, rowIndex, columnIndex, "double "))){
        cout << "possible function" << endl;
        /*while (fileContents[rowIndex][columnIndex] != ' '){
            cout << "Return type length: " << fileContents[rowIndex][columnIndex] << endl;
            columnIndex++;
        }
        columnIndex++;//Deals with the space after a return type
        */
        storedIndex = columnIndex; //storedIndex is to store the start of the potential function definition. columnIndex still is used to read values
        while (!isDone){
            cout << "Checking for declarations!" << endl;
            if (fileContents[rowIndex][columnIndex] == '('){//Looks for the bracket indicating this is a function and not a variable
                foundFunction = true;
               isDone = true;//Means once this code is done, the loop won't run again
               while (fileContents[rowIndex][storedIndex] != ';'){//Looks for the end of the declaration
                   functionDeclaration += (fileContents[rowIndex][storedIndex]);//Stores the declaration in an array
                   cout << fileContents[rowIndex][storedIndex] << endl;
                   cout << "Declaration?: " << functionDeclaration[storedIndex] << endl;
                   storedIndex++;
               }
            }
            else if (stringMatch(fileContents, rowIndex, columnIndex, "main")){//Ignore the likely "int main" line 
                isDone = true;
                rowIndex++;
                cout << "false positive main" << endl;
            }
            else if (fileContents[rowIndex][columnIndex] == '\n'){//Dealing with new lines in a function declaration
                rowIndex++;
                columnIndex = 0;
            }
            else if (fileContents[rowIndex][columnIndex] == ';') {//One possible end case if a variable is read into the function
                isDone = true;
                cout << "False positive variable" << endl;
            }
            else {
            functionDeclaration += (fileContents[rowIndex][storedIndex]);
            cout << "Printing stuff!: " << functionDeclaration[storedIndex] << endl;
            columnIndex++;
            storedIndex++;
            }
        }
        cout << "storedIndex value: " << storedIndex << endl;
        functionDeclaration[storedIndex] = '\0';
        cout << functionDeclaration[storedIndex-1] << endl;
        storedDeclaration = string(functionDeclaration);//Converts declaration to a string, in order to use stringMatch
        //Looking for the actual declaration next
        columnIndex = 0;//reset columnIndex
        cout << "declaration: " << storedDeclaration << endl;
        Sleep(1000);
        if (foundFunction){
            for (int y = rowIndex + 1; y != lastLine; y++){
                if (stringMatch(fileContents, y, 0, storedDeclaration)){
                    cout << "found it!" << endl;
                    fileOutput(storedDeclaration, outputPy, outputTxt);
                    fileOutput(':', outputPy, outputTxt);
                    while((fileContents[y][columnIndex] != '}') && (columnIndex < 1000)){
                        if (fileContents[y][columnIndex] == '\n'){
                            y++;
                            columnIndex = 0;
                            cout << "hey0" << endl;
                        } else {
                            fileOutput(fileContents[y][columnIndex], outputPy, outputTxt);
                            columnIndex++;
                            cout << "heyo" << endl;
                        }
                    }
                }
            }
        }
    }
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
