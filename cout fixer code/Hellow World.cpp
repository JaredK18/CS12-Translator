/* To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: shawn
 *
 * Created on February 8, 2016, 1:57 PM
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/*
 *
 */
int main(int argc, char** argv) {
	//Variables
	ifstream inStream;
	ofstream outStream;
	string output;
	char nextChar;
	vector<char> codeLine;
	bool EndOfVector = false;

	//Main I/O
	inStream.open("BrokenFile.txt");
	if (inStream.fail()){
		cout << "File couldn't be opened. " << endl;
		return 0;
	}

	while (inStream >> nextChar){
		//Find code to fix
		if ((nextChar == 'c') && ((inStream.peek()) == 'o')){ //Deals with cout case
			while (nextChar != ';'){ //Until the end of the line of code
				codeLine.push_back(nextChar); //Enter the code into a vector
				cout << nextChar;
				inStream >> nextChar;
			}
			codeLine.push_back(';');
			//Code Fixer

			for (int i = 0; i <= abs(codeLine.size()); i ++){
				if (codeLine[i] == '>'){
					codeLine[i] = '<';
				}
			}
			for (int y = 0; y <= abs(codeLine.size()); y ++){
				cout << "Made it here!5 And y is: "<< y << endl;
				cout << abs(codeLine.size()) << endl;
				if (codeLine[y] == '<'){
					codeLine.insert((codeLine.begin() + y), ' ');
					codeLine.insert((codeLine.begin() + y + 3), ' ');
					y = y + 2;
				}
			}
			codeLine.push_back('\n');
		}
		else if ((nextChar == 'c') && ((inStream.peek()) == 'i')){ //Deals with cin case
			cout << "Made it here!2" << endl;
			while (nextChar != ';'){ //Until the end of the line of code
				cout << "Made it here!3" << endl;
				codeLine.push_back(nextChar); //Enter the code into a vector
				cout << nextChar;
				inStream >> nextChar;
			}
			codeLine.push_back(';');
			//Code Fixer

			for (int i = 0; i <= abs(codeLine.size()); i ++){
				cout << "Made it here!4" << endl;
				if (codeLine[i] == '>'){
					codeLine[i] = '<';
				}
			}
			for (int y = 0; y <= abs(codeLine.size()); y ++){
				cout << "Made it here!5 And y is: "<< y << endl;
				cout << abs(codeLine.size()) << endl;
				if (codeLine[y] == '<'){
					cout << "Made it here!6" << endl;
					codeLine.insert((codeLine.begin() + y), ' ');
					codeLine.insert((codeLine.begin() + y + 3), ' ');
					y = y + 2;
				}
			}
			codeLine.push_back('\n');
		}
		else {
			cout << nextChar;

		}

	}
	cout << endl;

	//Create an output file
	outStream.open("FixedFile.txt");
	for (int j = 0; j <= abs(codeLine.size()); j ++){
		outStream << codeLine[j];
		cout << codeLine[j];
	}

	cout << "File Fixed.";

	//Close files
	inStream.close();
	outStream.close();
    return 0;
}

