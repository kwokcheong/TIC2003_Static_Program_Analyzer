#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:
	// method to connect to the database and initialize tables in the database
	static void initialize();

	// method to close the database connection
	static void close();

	// method to insert
	static void insertProcedure(string procedureName);

    static void insertVariable(string variableName, string procedureName);

    static void insertRead(int lineNUM);

    static void insertAssignment(int lineNUM);

    static void insertPrint(int lineNUM);

    static void insertStatement(int currentLine, string statementType, int procedureId, int variableId, int constantId, string expressionValue, int parent_line_num);

    static void insertConstants(int lineNum, int val);

	// method to get
	static void getProcedures(vector<string>& results);

    static void getVariables(vector<string>& results);

    static void getReads(vector<string>& results);

    static void getAssignments(vector<string>& results);

    static void getPrints(vector<string>& results);

    static void getStatements(vector<string>& results);

    static void getConstants(vector<string>& results);

    static int getProcedureId(int &result, string procedureName);

    static int getVariableId(int &result, string variableName, string procedureName);

    static int getConstantId(int &result, int lineNum);

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

