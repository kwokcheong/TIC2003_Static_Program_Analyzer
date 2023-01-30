#include "Database.h"
#include <iostream>

using namespace std;

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
	// open a database connection and store the pointer into dbConnection
	sqlite3_open("database.db", &dbConnection);

    vector<string> tables = { "procedures", "variables", "assignments", "constants", "statements", "prints", "reads"};

    for(auto & table : tables){
        string dropProcedureTableSQL = "DROP TABLE IF EXISTS " + table;
        sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);
    };

    // create a procedure table
	string createProcedureTableSQL = "CREATE TABLE procedures ( procedureName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a variable table
    string createVariableTableSQL = "CREATE TABLE variables ( variableName VARCHAR(255) PRIMARY KEY );";
    sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a constants table
    string createConstantsTableSQL = "CREATE TABLE constants ( lineNUM INT );";
    sqlite3_exec(dbConnection, createConstantsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create an assignments table
    string createAssignmentsTableSQL = "CREATE TABLE assignments ( lineNUM INT );";
    sqlite3_exec(dbConnection, createAssignmentsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a statements table
    string createStatementsTableSQL = "CREATE TABLE statements ( lineNUM INT );";
    sqlite3_exec(dbConnection, createStatementsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a prints table
    string createPrintsTableSQL = "CREATE TABLE prints ( lineNUM INT );";
    sqlite3_exec(dbConnection, createPrintsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a reads table
    string createReadsTableSQL = "CREATE TABLE reads ( lineNUM INT );";
    sqlite3_exec(dbConnection, createReadsTableSQL.c_str(), NULL, 0, &errorMessage);

    // initialize the result vector

	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

//INSERTS

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName) {
	string insertProcedureSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
	sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a variable into the database
void Database::insertVariable(string variableName) {
    string insertVariableSQL = "INSERT INTO variables ('variableName') VALUES ('" + variableName + "');";
    sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a reads into the database
void Database::insertRead(int currentLine) {
    string insertReadSQL = "INSERT INTO reads ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertReadSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a reads into the database
void Database::insertAssignment(int currentLine) {
    string insertAssignmentsSQL = "INSERT INTO assignments ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertAssignmentsSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a reads into the database
void Database::insertPrint(int currentLine) {
    string insertPrintsSQL = "INSERT INTO prints ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertPrintsSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a reads into the database
void Database::insertStatement(int currentLine) {
    string insertStatementSQL = "INSERT INTO statements ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertStatementSQL.c_str(), NULL, 0, &errorMessage);
}

//GETS

// method to get all the procedures from the database
void Database::getProcedures(vector<string>& results){
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getProceduresSQL = "SELECT * FROM procedures;";
	sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the procedures from the database
void Database::getVariables(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getVariablesSQL = "SELECT * FROM variables;";
    sqlite3_exec(dbConnection, getVariablesSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// method to get all the procedures from the database
void Database::getReads(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getReadsSQL = "SELECT * FROM reads;";
    sqlite3_exec(dbConnection, getReadsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// method to get all the procedures from the database
void Database::getAssignments(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getAssignmentsSQL = "SELECT * FROM assignments;";
    sqlite3_exec(dbConnection, getAssignmentsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// method to get all the procedures from the database
void Database::getPrints(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getPrintsSQL = "SELECT * FROM prints;";
    sqlite3_exec(dbConnection, getPrintsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// method to get all the procedures from the database
void Database::getStatements(vector<string> &results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getStatementsSQL = "SELECT * FROM statements;";
    sqlite3_exec(dbConnection, getStatementsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
	NotUsed = 0;
	vector<string> dbRow;

	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	for (int i = 0; i < argc; i++) {
		dbRow.push_back(argv[i]);
	}

	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);

	return 0;
}
