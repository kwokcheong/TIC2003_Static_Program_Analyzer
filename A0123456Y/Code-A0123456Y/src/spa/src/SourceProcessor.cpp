#include "SourceProcessor.h"
#include <iostream>
#include <ctype.h>
#include <string>
#include <algorithm>
using namespace std;

// method for processing the source program
bool helper(string text){
    try {
        if(stoi(text) >= 0){
            if(text[0] == '0' && text.length() > 1){
                return false;
            } else {
                return true;
            }
        }
    } catch(...) {
        return false;
    }
};

template<typename C, typename T>
bool contains(C&& c, T e) {
    return std::find(std::begin(c), std::end(c), e) != std::end(c);
};

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);
    vector<string> keywords { "while", "if", "for"};
    vector<string> conditional { "!", "=", ">", "<"};
    int currentLine = 1;
    // line num
    for(int i = 0; i < tokens.size(); i++){
        string current_token = tokens.at(i);
        if(tokens.at(i) == ";" || contains(keywords, tokens.at(i))){
            currentLine += 1;
            Database::insertStatement(currentLine - 1);
        }
        // procedure
        if(tokens.at(i) == "procedure"){
            string procedureName = tokens.at(i + 1);
            // insert the procedure into the database
            Database::insertProcedure(procedureName);
        }
        // assignment and variables
        if(tokens.at(i) == "="){
            if(!contains(conditional, tokens.at(i-1)) && tokens.at(i+1) != "="){
                string variableName = tokens.at(i-1);
                Database::insertVariable(variableName);
                Database::insertAssignment(currentLine);
            }
        }
        // read
        if(tokens.at(i) == "read"){
            string variableName = tokens.at(i + 1);
            // insert the variable into the database
            Database::insertVariable(variableName);
            Database::insertRead(currentLine);
        }
        if(tokens.at(i) == "print"){
            Database::insertPrint(currentLine);
        }
        // This method catches for constants
        if(helper(tokens.at(i))){
            Database::insertConstants(stoi(tokens.at(i)));
        }
    }
}
