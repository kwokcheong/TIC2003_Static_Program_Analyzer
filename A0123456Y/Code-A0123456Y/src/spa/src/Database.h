#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include <iostream>
#include <stack>
#include <algorithm>
#include "Tokenizer.h"
#include <map>

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
    // INITIALIZE DB
    static void initialize();

    // CLOSE DB
    static void close();

    // INSERT DB
    static void insertProcedure(string procedureName);

    static void insertVariable(string variableName);

    static void insertRead(int lineNUM);

    static void insertAssignment(int lineNUM);

    static void insertPrint(int lineNUM);

    static void insertStatement(int currentLine, string statementType, int procedureId, int variableId, int constantId, string expressionValue, string call_procedure_name, int parent_line_num);

    static void insertConstants(int lineNum, int val);

    // GET DB
    static void getProcedures(vector<string>& results);

    static void getVariables(vector<string>& results);

    static void getReads(vector<string>& results);

    static void getAssignments(vector<string>& results);

    static void getPrints(vector<string>& results);

    static void getStatements(vector<string>& results);

    static void getConstants(vector<string>& results);

    static void getIfStatementIds(vector<string> &results);

    static void getWhileStatementIds(vector<string> &results);

    static void getCallIds(vector<string> &results);

    static void getProcedureId(int &result, string procedureName);

    static void getVariableId(int &result, string variableName);

    static void getConstantId(int &result, int lineNum);

    // DB ABSTRACTION

    static void getStatementsIfParent(vector<string>& results, map<string, string>& myMap);

    static void getStatementsIfParentT(vector<string>& results, map<string, string>& myMap);

    static void getStatementsIfUses(vector<string>& results, map<string, string>& myMap);

    static void getProcedureNamesIfUses(vector<string>& results, map<string, string>& myMap);

    static void getStatementsIfModifies(vector<string>& results, map<string, string>& myMap);

    static void getProcedureNamesIfModifies(vector<string>& results, map<string, string>& myMap);

    static bool parent(int lineNum1, int lineNum2);

    static bool parent(string procedureName, int lineNum2);

    static bool parentT(int lineNum1, int lineNum2);

    static bool uses(int lineNum, string variableName);

    static bool uses(string procedureName, string variableName);

    static bool modifies(int lineNum, string variableName);

    static bool modifies(string procedureName, string variableName);

    static bool calls(string procedureName1, string procedureName2);

    //static bool callsT();

private:
    // the connection pointer to the database
    static sqlite3* dbConnection;
    // a vector containing the results from the database
    static vector<vector<string>> dbResults;
    // the error message from the database
    static char* errorMessage;
    // callback method to put one row of results from the database into the dbResults vector
    // This method is called each time a row of results is returned from the database
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
};