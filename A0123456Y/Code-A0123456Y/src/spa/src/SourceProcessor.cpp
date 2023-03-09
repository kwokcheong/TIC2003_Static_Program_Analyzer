#include "SourceProcessor.h"
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <map>
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
    return false;
};

template<typename C, typename T>
bool contains(C&& c, T e) {
    return find(begin(c), end(c), e) != end(c);
};

bool procedureExists(string procedureName){
    int result = 0;
    Database::getProcedureId(result, procedureName);
    return result;
}

bool variableExists(string variableName){
    int result = 0;
    Database::getVariableId(result, variableName);
    return result;
}

bool constantExists(int lineNum){
    int result = 0;
    Database::getConstantId(result, lineNum);
    return result;
}

int getDatabaseId(int x, int lineNum, string procedureName, string variableName){
    int result;
    switch(x){
        case 0: // 0 : Procedure
            if(procedureExists(procedureName)) {
                Database::getProcedureId(result, procedureName);
                return result;
            } else {
                return -1;
            }
        case 1: // 1 : Variable
            if(variableExists(variableName)){
                Database::getVariableId(result, variableName);
                return result;
            } else {
                return -1;
            }
        case 2: // 2 : Constant
            if(constantExists(lineNum)){
                Database::getConstantId(result, lineNum);
                return result;
            } else {
                return -1;
            }
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
    vector<string> keywords { "while", "if"};
    vector<string> conditional { "!", "=", ">", "<", "%", "*", "+", "-"  };
    bool constantFlag = true;
    int constantValue = -1;
    int currentLine = 1;
    int parentLine = 0;
    stack<int> parentStack;
    stack<int> ifStatementStack;
    string procedureName, variableName, statementType, expressionValue  = "";

    // Note to self: I want to make this a switch case instead, but can do it next iteration
    for(int i = 0; i < tokens.size(); i++){
        string current_token = tokens.at(i);

        // This method adds for constants
        // Check if while, if (), block the ( x > 3 ) part
        if(contains(keywords, tokens.at(i))){
            statementType = tokens.at(i);
            variableName = "";
            constantValue = -1;
            //parentLine = currentLine;
            constantFlag = false;

            if(tokens.at(i+1) == "("){
                expressionValue = tokens.at(i+1);
                int flag = 1;
                int j = 1;
                while(flag > 0){
                    if(tokens.at(i+j+1) == "(") { flag++; }
                    if(tokens.at(i+j+1) == ")") { flag--; }
                    expressionValue += tokens.at(i+j+1);
                    j++;
                }
            }
        }

        if(tokens.at(i) == "else"){
            parentStack.push(ifStatementStack.top());
            ifStatementStack.pop();
        }

        if(tokens.at(i) == ";" || contains(keywords, tokens.at(i))){
            int procedure_id = getDatabaseId(0,currentLine, procedureName, variableName);
            int variable_id = (variableName != "") ? getDatabaseId(1,currentLine, procedureName, variableName) : -1;
            int constant_id = (constantValue > -1) ? getDatabaseId(2, currentLine, procedureName, variableName) : -1;
            parentLine = parentStack.size() > 0 ? parentStack.top() : 0;
            Database::insertStatement(currentLine, statementType, procedure_id, variable_id, constant_id, expressionValue, parentLine);
            if(contains(keywords, tokens.at(i))) {
                if(tokens.at(i) == "if"){
                    ifStatementStack.push(currentLine);
                }
                parentStack.push(currentLine);
            }
            currentLine += 1;
            statementType = "";
            expressionValue = "";
            constantValue = -1;
        }

        if(tokens.at(i) == "procedure"){
            statementType = "procedure";
            constantValue = -1;
            variableName = "";
            procedureName = tokens.at(i + 1);
            Database::insertProcedure(procedureName);
        }

        // This method adds for assignment and variables
        if(tokens.at(i) == "="){
            if(!contains(conditional, tokens.at(i-1)) && tokens.at(i+1) != "="){
                if(tokens.at(i+2) != ";") {
                    int j = 1;
                    while (tokens.at(i + j) != ";") {
                        expressionValue += tokens.at(i+j);
                        j++;
                    }
                }
                variableName = tokens.at(i-1);
                if(!variableExists(variableName)) { Database::insertVariable(variableName); };
                Database::insertAssignment(currentLine);
                statementType = "assign";
            }
        }

        if(tokens.at(i) == "read"){
            constantValue = -1;
            variableName = tokens.at(i + 1);
            statementType = tokens.at(i);

            if(!variableExists(variableName)) { Database::insertVariable(variableName); };
            Database::insertRead(currentLine);
        }

        // TODO: Note that for call, this will not work, need to update
        if(tokens.at(i) == "print"){
            constantValue = -1;
            statementType = tokens.at(i);
            variableName = tokens.at(i+1);
            if(!variableExists(variableName)) { Database::insertVariable(variableName); };
            Database::insertPrint(currentLine);
        }

        if(tokens.at(i) == "call"){
            constantValue = -1;
            variableName = "";
            statementType = tokens.at(i);
        }

        // once while() { , then unblock
        if ( tokens.at(i) == "{" ) {
            constantFlag = true;
        }

        if(!parentStack.empty()){
            if(tokens.at(i) == "}"){
                parentStack.pop();
            }
        }

        if(isNumber(tokens.at(i)) && constantFlag ) {
            constantValue = stoi(tokens.at(i));
            Database::insertConstants(currentLine, stoi(tokens.at(i)));
        }
    }
}
