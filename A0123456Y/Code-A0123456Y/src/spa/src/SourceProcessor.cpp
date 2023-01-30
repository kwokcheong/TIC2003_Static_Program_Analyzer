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

    for(int i = 0; i < tokens.size(); i++){
        if(tokens.at(i) == "procedure"){
            string procedureName = tokens.at(i + 1);
            // insert the procedure into the database
            Database::insertProcedure(procedureName);
        }
        if(tokens.at(i) == "="){
            string variableName = tokens.at(i-1);
            // insert the variable into the database
            Database::insertVariable(variableName);
        }
        if(tokens.at(i) == "read"){
            string variableName = tokens.at(i + 1);
            // insert the variable into the database
            Database::insertVariable(variableName);
        }

    }
}