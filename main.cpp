#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    ifstream input;
    input.open("input.txt");
    ofstream output;
    output.open("output.txt");
    char newLine[100];
    int loop = 0;
    while(input.get(newLine[loop])){
        cout << newLine[loop];
        loop++;
    }
    if(newLine[0] == 'H' and newLine[1] == 'i'){
        output << "Hello";
    }
    return 0;
}
