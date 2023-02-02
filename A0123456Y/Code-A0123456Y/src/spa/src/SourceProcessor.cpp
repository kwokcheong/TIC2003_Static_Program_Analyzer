#include "SourceProcessor.h"
#include <iostream>
#include <ctype.h>
#include <string>
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

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

    int currentLine = 0;
    for(int i = 0; i < tokens.size(); i++){
        string current_token = tokens.at(i);
        if(tokens.at(i) == "{" || tokens.at(i) == ";"){
            currentLine ++;
        }
        if(tokens.at(i) == "procedure"){
            string procedureName = tokens.at(i + 1);
            // insert the procedure into the database
            Database::insertProcedure(procedureName);
        }
        if(tokens.at(i) == "="){
            string variableName = tokens.at(i-1);
            // insert the variable into the database
            Database::insertVariable(variableName);
            Database::insertAssignment(currentLine);
            Database::insertStatement(currentLine);
        }
        if(tokens.at(i) == "read"){
            string variableName = tokens.at(i + 1);
            // insert the variable into the database
            Database::insertVariable(variableName);
            Database::insertRead(currentLine);
            Database::insertStatement(currentLine);
        }
        if(tokens.at(i) == "print"){
            Database::insertPrint(currentLine);
            Database::insertStatement(currentLine);
        }
        // This method catches for constants
        if(helper(tokens.at(i))){
            Database::insertConstants(stoi(tokens.at(i)));
        }
    }
}
