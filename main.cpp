/* 
 * File:   main.cpp
 * Author: Jared
 *
 * Created on May 10, 2016, 1:30 PM
 */

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

/*
 * 
 * bool beforeCode = true;
    while(line[location] != '\n'){
        
    }
 */
bool stringMatch(char fileContents[], int startPosition, string searchTerm);

int main(int argc, char** argv){
    ifstream input;
    input.open("input.txt");
    ofstream output;
    output.open("outputpy.py");
    ofstream output1;
    output1.open("outputtxt.txt");
    char fileContents[10000];
    int loop = 0;
    int end = 0;
    while(input.get(fileContents[end])){
        cout << fileContents[end];
        end++;
    }
    //this puts one line of code into the array newLine
    do{
        if(stringMatch(fileContents, loop, "#include ")){
            while(fileContents[loop] != '\n'){
                loop++;
            }
            while(fileContents[loop] == '\n'){
                loop++;
            }
        }
        if(stringMatch(fileContents, loop, "int ")){
            loop = loop + 4;
        }
        else if(stringMatch(fileContents, loop, "bool ")){
            loop = loop + 5;
        }
        else if(stringMatch(fileContents, loop, "char ")){
            loop = loop + 5;
        }
        else if(stringMatch(fileContents, loop, "string ")){
            loop = loop + 7;
        }
        else if(stringMatch(fileContents, loop, "double ")){
            loop = loop + 7;
        }
        else if(fileContents[loop] == ';'){
            //output << endl;
            loop++;
        }
        output << fileContents[loop];
        output1 << fileContents[loop];
        loop++;
    }while(loop != end);
    input.close();
    output.close();
    output1.close();
    return 0;
}


bool stringMatch(char fileContents[], int startPosition, string searchTerm){
    bool match = true;
    for(int i = startPosition; match and i < (searchTerm.length() + startPosition); i++){
        if(fileContents[i] != searchTerm.at(i-startPosition)){
            match = false;
        }
    }
    return match;
}