#include "SourceProcessor.h"
#include <iostream>

using namespace std;

// method for processing the source program

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

    int currentLine = 0;
    for(int i = 0; i < tokens.size(); i++){
        if(tokens.at(i) == "{" || tokens.at(i) == ";"){
            currentLine += 1;
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
    }
}