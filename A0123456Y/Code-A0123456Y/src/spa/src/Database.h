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

	// method to insert a procedure into the database
	static void insertProcedure(string procedureName);

    // method to insert a variable into the database
    static void insertVariable(string variableName);

    // method to insert read lines into the database
    static void insertRead(int lineNUM);

    // method to insert read lines into the database
    static void insertAssignment(int lineNUM);

    // method to insert read lines into the database
    static void insertPrint(int lineNUM);

    // method to insert read lines into the database
    static void insertStatement(int lineNUM);

    // method to insert read lines into the database
    static void insertConstants(int val);

	// method to get all the procedures from the database
	static void getProcedures(vector<string>& results);

    // method to get all the variables from the database
    static void getVariables(vector<string>& results);

    // method to get all the variables from the database
    static void getReads(vector<string>& results);

    // method to get all the variables from the database
    static void getAssignments(vector<string>& results);

    // method to get all the variables from the database
    static void getPrints(vector<string>& results);

    // method to get all the variables from the database
    static void getStatements(vector<string>& results);

    // method to get all the variables from the database
    static void getConstants(vector<string>& results);

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

