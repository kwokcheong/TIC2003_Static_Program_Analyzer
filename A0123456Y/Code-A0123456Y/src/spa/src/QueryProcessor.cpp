#include "QueryProcessor.h"
#include "Tokenizer.h"
#include <iostream>
#include <map>
#include <cctype>

using namespace std;

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

// helper method to evaluate a query
template<typename C, typename T>
bool contains(C&& c, T e) {
    return find(begin(c), end(c), e) != end(c);
};

string toLower(std::string str) {
    for (auto& c : str) {
        c = std::tolower(c);
    }
    return str;
}

void addToMap(map<string, string>& myMap, vector<string> synonymLine){
    string name;
    for(int i=1; i<synonymLine.size(); i++){
        name += synonymLine.at(i);
    }
    myMap.insert(pair<string, string>(synonymLine.at(0), name));
}

void retrieveDatabase(string command, string synonym, map<string, string>& myMap, vector<string>& databaseResults ){
    if(command == "parentT") {
        Database::getStatementsIfParentT(databaseResults, myMap);
    }

    if(command == "parent") {
        Database::getStatementsIfParent(databaseResults, myMap);
    }

    if(command == "uses") {
        // assumes that you want procedures
        if(synonym == "procedure"){
            Database::getProcedureNamesIfUses(databaseResults, myMap);
        }else {
            Database::getStatementsIfUses(databaseResults, myMap);
        }
    }

    if(command == "modifies") {
        // assumes that you want procedures
        if(synonym == "procedure"){
            Database::getProcedureNamesIfModifies(databaseResults, myMap);
        }else {
            Database::getStatementsIfModifies(databaseResults, myMap);
        }
    }

    if(command == "calls") {
        Database::getProcedureNamesIfCalls(databaseResults, myMap);
    }

    if(command == "callsT") {
        Database::getProcedureNamesIfCallsT(databaseResults, myMap);
    }

    if(command == "all"){
        if(synonym == "procedure"){ Database::getProcedures(databaseResults); }
        if(synonym == "variable"){ Database::getVariables(databaseResults);}
        if(synonym == "read"){ Database::getReads(databaseResults); }
        if(synonym == "assign"){ Database::getAssignments(databaseResults); }
        if(synonym == "print"){ Database::getPrints(databaseResults); }
        if(synonym == "stmt"){ Database::getStatements(databaseResults); }
        if(synonym == "constant"){ Database::getConstants(databaseResults); }
        if(synonym == "if"){ Database::getIfStatementIds(databaseResults); }
        if(synonym == "while"){ Database::getWhileStatementIds(databaseResults); }
        if(synonym == "call"){ Database::getCallIds(databaseResults); }
    }
}

void QueryProcessor::evaluate(string query, vector<string>& output) {
    // clear the output vector
    output.clear();

    // tokenize the query
    Tokenizer tk;
    vector<string> tokens;
    map<string, string> myMap;
    tk.tokenize(query, tokens);

    // check what type of synonym is being declared
    string synonymType = tokens.at(0);

    // create a vector for storing the results from database
    vector<string> databaseResults;
    vector<string> synonymLine;
    vector<string> skip = { ";", "such", "that"};
    int selectIndex = 0;

    // create the base map body for calling database get request
    for(int i = 0; i<tokens.size(); i++){
        synonymLine.push_back(tokens[i]);
        if(tokens[i+1] == ";"){ // this will add all the synonyms
            i++;
            addToMap(myMap, synonymLine);
            synonymLine.clear();
        }

        if(tokens.at(i) == "Select" || tokens.at(i) == "select"){ // this will add the select statement
            synonymLine.clear();
            synonymLine.push_back(tokens.at(i));
            synonymLine.push_back(tokens.at(i+1));
            addToMap(myMap, synonymLine);
            synonymLine.clear();
            selectIndex = i;
        }

    }

    // evaluate select statement
    string command = "all";
    for(int i = selectIndex; i<tokens.size(); i++){
        //iteration 2 remains simple. To handle for only 1 abstraction.
        string value = "";
        if(tokens.at(i) == "that"){
            if(tokens.at(i+1) == "Parent"){
                if(tokens.at(i+2) == "*"){
                    command = "parentT";
                    if(tokens.at(i+4) == "\""){
                        synonymLine.clear();
                        synonymLine.push_back("value");
                        synonymLine.push_back(tokens.at(i+5));
                        addToMap(myMap, synonymLine);
                        synonymLine.clear();
                    }
                } else {
                    command = "parent";
                    if(tokens.at(i+3) == "\""){
                        synonymLine.clear();
                        synonymLine.push_back("value");
                        synonymLine.push_back(tokens.at(i+5));
                        addToMap(myMap, synonymLine);
                        synonymLine.clear();
                    }
                }
            }
            if(tokens.at(i+1) == "Uses"){
                command = "uses";
                synonymLine.clear();
                synonymLine.push_back("value");
                synonymLine.push_back(tokens.at(i+6));
                addToMap(myMap, synonymLine);
                synonymLine.clear();
            }
            if(tokens.at(i+1) == "Modifies"){
                command = "modifies";
                synonymLine.clear();
                synonymLine.push_back("value");
                synonymLine.push_back(tokens.at(i+6));
                addToMap(myMap, synonymLine);
                synonymLine.clear();
            }
            if(tokens.at(i+1) == "Calls"){
                if(tokens.at(i+2) == "*"){
                    command = "callsT";
                    synonymLine.clear();
                    for(int j = 3; tokens.at(i + 1 + j) != ")" ; j++){
                        value += tokens.at(i + 1 + j);
                    }
                    synonymLine.push_back("value");
                    synonymLine.push_back(value);
                    addToMap(myMap, synonymLine);
                    synonymLine.clear();
                }else {
                    command = "calls";
                    synonymLine.clear();
                    for(int j = 2; tokens.at(i + 1 + j) != ")" ; j++){
                        value += tokens.at(i + 1 + j);
                    }
                    synonymLine.push_back("value");
                    synonymLine.push_back(value);
                    addToMap(myMap, synonymLine);
                    synonymLine.clear();
                }
            }
        }
        if(tokens.at(i) == "Pattern"){
            // do something
        }
    }

    // call the method in database to retrieve the results
    retrieveDatabase(command, synonymType, myMap, databaseResults);

    //for (const auto& [key, value] : myMap) {
    //    std::cout << "Key: " << key << ", Value: " << value << std::endl;
    //}

    // post process the results to fill in the output vector
    for (string databaseResult : databaseResults) {
        output.push_back(databaseResult);
    }
}