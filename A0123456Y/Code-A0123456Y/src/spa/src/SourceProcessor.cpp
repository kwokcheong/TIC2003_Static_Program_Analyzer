#include "SourceProcessor.h"
#include <iostream>
#include <ctype.h>
#include <string>
#include <algorithm>
using namespace std;

bool isNumber(string text){
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
    return find(begin(c), end(c), e) != end(c);
};

int getDatabaseId(int x, int lineNum, string procedureName, string variableName){
    int result;
    switch(x){
        case 0: // 0 : Procedure
            result = Database::getProcedureId(result, procedureName);
            return result;
        case 1: // 1 : Variable
            result = Database::getVariableId(result, variableName, procedureName);
            cout << "THIS IS THE RESULT FROM getProcedureId QUERY: " << result << endl;
            return result;
        case 2: // 2 : Constant
            result = Database::getConstantId(result, lineNum);
            return result;
        default: return -1;
    }
}

void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);

    // Initialize standard variables
    vector<string> keywords { "while", "if", "for"};
    vector<string> conditional { "!", "=", ">", "<", "%", "*", "+","-"};
    vector<string> arithmetics{ "%", "*", "+","-" };
    bool constantFlag = true;
    int constantValue = -1;
    int currentLine = 1;
    string procedureName, variableName, statementType, expressionValue  = "";

    // Note to self: I want to make this a switch case instead, but can do it next iteration
    for(int i = 0; i < tokens.size(); i++){
        string current_token = tokens.at(i);

        // This method counts the line num
        if(tokens.at(i) == ";" || contains(keywords, tokens.at(i))){
            currentLine += 1;
            int procedure_id = getDatabaseId(0,currentLine, procedureName, variableName);
            int variable_id = (variableName != "") ? getDatabaseId(1,currentLine, procedureName, variableName) : -1;
            int constant_id = (constantValue > -1) ? getDatabaseId(2, currentLine, procedureName, variableName) : -1;
            Database::insertStatement(currentLine - 1, statementType, procedure_id, variable_id, constant_id, expressionValue, 0); // convert this to a temporary create statement
        }

        // This method adds for procedure
        if(tokens.at(i) == "procedure"){
            constantValue = -1;
            variableName = "";
            procedureName = tokens.at(i + 1);
            // insert the procedure into the database
            Database::insertProcedure(procedureName);
        }

        // This method adds for assignment and variables
        if(tokens.at(i) == "="){
            if(!contains(conditional, tokens.at(i-1)) && tokens.at(i+1) != "="){
                variableName = tokens.at(i-1);
                Database::insertVariable(variableName, procedureName);
                Database::insertAssignment(currentLine);
            }
        }

        // This method adds for read
        if(tokens.at(i) == "read"){
            constantValue = -1;
            variableName = tokens.at(i + 1);
            // insert the variable into the database
            Database::insertVariable(variableName, procedureName);
            Database::insertRead(currentLine);
        }

        if(tokens.at(i) == "print"){
            Database::insertPrint(currentLine);
        }

        // This method adds for constants
        // Check if while, for, if (), block the ( x > 3 ) part
        if(contains(keywords, tokens.at(i))){
            variableName = "";
            statementType = tokens.at(i);
            constantFlag = false;
        }
        // once while() { , then unblock
        if ( tokens.at(i) == "{" ) {
            constantFlag = true;
        }
        // helper checks if the token is an integer.
        // but note that it does not cater for ( x = x + 1 )
        if(isNumber(tokens.at(i)) && constantFlag ) {
            constantValue = stoi(tokens.at(i));
            Database::insertConstants(currentLine, stoi(tokens.at(i)));
        }
    }
}
