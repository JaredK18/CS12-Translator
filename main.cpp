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
int main(int argc, char** argv) {
    ifstream input;
    input.open("input.txt");
    ofstream output;
    output.open("outputpy.py");
    ofstream output1;
    output1.open("outputtxt.txt");
    char newLine[10000];
    int loop = 0;
    int end = 0;
    while(input.get(newLine[end])){
        cout << newLine[end];
        end++;
    }
    //this puts one line of code into the array newLine
    do{
        if(newLine[loop] == 'i' and newLine[loop+1] == 'n' and newLine[loop+2] == 't' and newLine[loop+3] == ' '){
            loop = loop + 4;
        }
        else if(newLine[loop] == 'b' and newLine[loop+1] == 'o' and newLine[loop+2] == 'o' and newLine[loop+3] == 'l' and newLine[loop+4] == ' '){
            loop = loop + 5;
        }
        else if(newLine[loop] == 'c' and newLine[loop+1] == 'h' and newLine[loop+2] == 'a' and newLine[loop+3] == 'r' and newLine[loop+4] == ' '){
            loop = loop + 5;
        }
        else if(newLine[loop] == 's' and newLine[loop+1] == 't' and newLine[loop+2] == 'r' and newLine[loop+3] == 'i' and newLine[loop+4] == 'n' and newLine[loop+5] == 'g' and newLine[loop+6] == ' '){
            loop = loop + 7;
        }
        else if(newLine[loop] == 'd' and newLine[loop+1] == 'o' and newLine[loop+2] == 'u' and newLine[loop+3] == 'b' and newLine[loop+4] == 'l' and newLine[loop+5] == 'e' and newLine[loop+6] == ' '){
            loop = loop + 7;
        }
        else if(newLine[loop] == ';'){
            //output << endl;
            loop++;
        }
        output << newLine[loop];
        output1 << newLine[loop];
        loop++;
    }while(loop != end);
    input.close();
    output.close();
    output1.close();
    return 0;
}

