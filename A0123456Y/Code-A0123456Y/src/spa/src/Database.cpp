#include "Database.h"
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// INITIALIZE DB
void Database::initialize() {
    // open a database connection and store the pointer into dbConnection
    sqlite3_open("database.db", &dbConnection);

    vector<string> tables = { "procedures", "variables", "assignments", "constants", "statements", "prints", "reads"};

    for(auto & table : tables){
        string dropProcedureTableSQL = "DROP TABLE IF EXISTS " + table;
        sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);
    };

    // create a procedure table
    string createProcedureTableSQL = "CREATE TABLE procedures (id INTEGER PRIMARY KEY AUTOINCREMENT, procedure_name VARCHAR(256) NOT NULL);";
    sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a variable table
    string createVariableTableSQL = "CREATE TABLE variables (id INTEGER PRIMARY KEY AUTOINCREMENT, variable_name varchar(256) NOT NULL);";
    sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a constants table
    string createConstantsTableSQL = "CREATE TABLE constants (id INTEGER PRIMARY KEY AUTOINCREMENT, lineNum INTEGER, value varchar(256) NOT NULL);";
    sqlite3_exec(dbConnection, createConstantsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create an assignments table
    string createAssignmentsTableSQL = "CREATE TABLE assignments ( lineNUM INT );";
    sqlite3_exec(dbConnection, createAssignmentsTableSQL.c_str(), NULL, 0, &errorMessage);

    // create a statements table
    string createStatementsTableSQL = "CREATE TABLE statements (id INTEGER PRIMARY KEY, \n"
                                      "statement_type VARCHAR(256),\n"
                                      "procedure_id INT,\n"
                                      "variable_id INT,\n"
                                      "constant_id INT,\n"
                                      "expression VARCHAR(256), \n"
                                      "call_procedure_name VARCHAR(256),\n"
                                      "parent_line_num INT,\n"
                                      "FOREIGN KEY(procedure_id) REFERENCES procedures(id)\n"
                                      "FOREIGN KEY(variable_id) REFERENCES variables(id)\n"
                                      "FOREIGN KEY(constant_id) REFERENCES constants(id)\n"
                                      ");";
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

// CLOSE DB
void Database::close() {
    sqlite3_close(dbConnection);
}

// HELPER METHODS FOR DATA ABSTRACTION
template<typename C, typename T>
bool contains(C&& c, T e) {
    return find(begin(c), end(c), e) != end(c);
};

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int index_of_double_quotes(vector<string> vector) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i].find("\"") != string::npos) {
            return i;
        }
    }
    return -1; // Return -1 if no string contains double quotes
}

int index_of_underscore(vector<string> vector) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i] == "_") {
            return i;
        }
    }
    return -1; // Return -1 if string is not _
}

string remove_double_quotes(std::string str) {
    std::size_t pos = std::string::npos;
    while ((pos = str.find("\"")) != std::string::npos) {
        str.erase(pos, 1);
    }
    return str;
}


//INSERT DB
void Database::insertProcedure(string procedureName) {
    string insertProcedureSQL = "INSERT INTO procedures ('procedure_name') VALUES ('" + procedureName + "');";
    sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertVariable(string variableName) {
    string insertVariableSQL = "INSERT INTO variables ('variable_name') VALUES ('" + variableName + "');";
    sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertRead(int currentLine) {
    string insertReadSQL = "INSERT INTO reads ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertReadSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertAssignment(int currentLine) {
    string insertAssignmentsSQL = "INSERT INTO assignments ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertAssignmentsSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertPrint(int currentLine) {
    string insertPrintsSQL = "INSERT INTO prints ('lineNUM') VALUES ('" + to_string(currentLine) + "');";
    sqlite3_exec(dbConnection, insertPrintsSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertStatement(int currentLine, string statementType, int procedureId, int variableId, int constantId, string expressionValue, string call_procedure_name, int parent_line_num) {

    //string insertStatementSQL = "INSERT INTO statements ('statement_type','procedure_id', 'variable_id', 'constant_id', 'expression', 'parent_line_num) VALUES ('" + statementType + "', '" + to_string(procedureId) + "', '"+ to_string(variableId) + "', '" + to_string(constantId) + "', '" + expressionValue + "', '" + to_string(parent_line_num) + "' );";
    string insertStatementSQL = "INSERT INTO statements ('id', 'statement_type', 'procedure_id', 'variable_id', 'constant_id', 'expression', 'call_procedure_name', 'parent_line_num') "
                                "VALUES ('" + to_string(currentLine) + "', '" + statementType + "', '" + to_string(procedureId) + "', '" + to_string(variableId) + "', '" + to_string(constantId) + "', '" + expressionValue + "', '" + call_procedure_name + "', '"+ to_string(parent_line_num) + "')";

    sqlite3_exec(dbConnection, insertStatementSQL.c_str(), NULL, 0, &errorMessage);
}

void Database::insertConstants(int currentLine,int val) {
    string insertConstantsSQL = "INSERT INTO constants ('lineNum','value') VALUES ('" + to_string(currentLine) + "', '" + to_string(val) + "' );";
    sqlite3_exec(dbConnection, insertConstantsSQL.c_str(), NULL, 0, &errorMessage);
}

//GET DB
void Database::getProcedures(vector<string>& results){
    dbResults.clear();

    string getProceduresSQL = "SELECT procedure_name FROM procedures;";
    sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getVariables(vector<string>& results){
    dbResults.clear();

    string getVariablesSQL = "SELECT variable_name FROM variables;";
    sqlite3_exec(dbConnection, getVariablesSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getReads(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getReadsSQL = "SELECT id FROM statements WHERE statement_type = 'read';";
    sqlite3_exec(dbConnection, getReadsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getAssignments(vector<string>& results){
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getAssignmentsSQL = "SELECT id FROM statements WHERE statement_type = 'assign';";

    sqlite3_exec(dbConnection, getAssignmentsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getPrints(vector<string>& results){
    dbResults.clear();

    string getPrintsSQL = "SELECT id FROM statements WHERE statement_type = 'print';";
    sqlite3_exec(dbConnection, getPrintsSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

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

void Database::getConstants(vector<string> &results) {
    // clear the existing results
    dbResults.clear();

    // retrieve the procedures from the procedure table
    // The callback method is only used when there are results to be returned.
    string getConstantsSQL = "SELECT DISTINCT value FROM constants;";
    sqlite3_exec(dbConnection, getConstantsSQL.c_str(), callback, 0, &errorMessage);

    // postprocess the results from the database so that the output is just a vector of procedure names
    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getIfStatementIds(vector<string> &results) {
    dbResults.clear();

    string getIfSQL = "SELECT id FROM statements where statement_type = 'if';";
    sqlite3_exec(dbConnection, getIfSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getCallIds(vector<string> &results) {
    dbResults.clear();

    string getCallSQL = "SELECT id FROM statements where statement_type = 'call';";
    sqlite3_exec(dbConnection, getCallSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getWhileStatementIds(vector<string> &results) {
    dbResults.clear();

    string getIfSQL = "SELECT id FROM statements where statement_type = 'while';";
    sqlite3_exec(dbConnection, getIfSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        string result;
        result = dbRow.at(0);
        results.push_back(result);
    }
}

void Database::getProcedureId(int &result, string procedureName) {
    dbResults.clear();

    string getProcedureIdSQL = "SELECT id FROM procedures WHERE procedure_name = '" + procedureName + "';";
    sqlite3_exec(dbConnection, getProcedureIdSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        result = stoi(dbRow.at(0));
    }
}

void Database::getVariableId(int &result, string variableName) {
    dbResults.clear();

    string getVariableIdSQL = "SELECT id FROM variables WHERE variable_name = '" + variableName + "';";
    sqlite3_exec(dbConnection, getVariableIdSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        result = stoi(dbRow.at(0));
    }
}

void Database::getConstantId(int &result, int lineNum) {
    dbResults.clear();

    string getConstantIdSQL = "SELECT id FROM constants WHERE lineNum = '" + to_string(lineNum) + "';";
    sqlite3_exec(dbConnection, getConstantIdSQL.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        result = stoi(dbRow.at(0));
    }
}

void Database::getStatementsIfParent(vector<string> &results, map<string, string>& myMap){
    dbResults.clear();
    vector<string> keywords { "while", "if", "assign", "read", "print" };
    bool flag = 0;
    vector<string> keys;
    vector<int> id1;
    vector<int> id2;
    string getIds;

    // this gets the keys ie: while, if, assign, read, print : it will not store procedure if it sees it
    for (auto const& [key, val] : myMap) {
        if(contains(keywords, key)){
            keys.push_back(key);
        }
        if(key == "value"){
            flag = 1;
        }
    }

    if(keys.size() > 0) {
        // start of procedure
        if (flag) {
            dbResults.clear();
            getIds = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
            sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

            for (vector<string> dbRow: dbResults) {
                int id;
                id = stoi(dbRow.at(0));
                id1.push_back(id);
            }

            for (auto line: id1) {
                if (Database::parent(myMap["value"], line)) {
                    results.push_back(to_string(line));
                }
            }
        }
        //end of procedure

        // start of while if
        if (!flag) {
            if (keys.at(0) == "while" || keys.at(0) == "if") {
                dbResults.clear();
                getIds = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
                sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                for (vector<string> dbRow: dbResults) {
                    int id;
                    id = stoi(dbRow.at(0));
                    id1.push_back(id);
                }

                if (keys.size() > 1) {
                    dbResults.clear();
                    getIds = "SELECT id FROM statements where statement_type = '" + keys.at(1) + "';";
                    sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                    for (vector<string> dbRow: dbResults) {
                        int id;
                        id = stoi(dbRow.at(0));
                        id2.push_back(id);
                    }
                }
            } else {
                dbResults.clear();
                getIds = "SELECT id FROM statements where statement_type = '" + keys.at(1) + "';";
                sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                for (vector<string> dbRow: dbResults) {
                    int id;
                    id = stoi(dbRow.at(0));
                    id1.push_back(id);
                }

                if (keys.size() > 1) {
                    dbResults.clear();
                    getIds = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
                    sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                    for (vector<string> dbRow: dbResults) {
                        int id;
                        id = stoi(dbRow.at(0));
                        id2.push_back(id);
                    }
                }

                if (keys.size() > 1) {
                    for (auto it: id1) {
                        for (auto it2: id2) {
                            if (Database::parent(it, it2)) {
                                results.push_back(to_string(it2));
                            }
                        }
                    }
                } else {
                    for (auto it: id1) {
                        for (auto it2: id1) {
                            if (Database::parent(it, it2)) {
                                results.push_back(to_string(it2));
                            }
                        }
                    }
                }
            }
        }
        // end of while if
    }
};

void Database::getStatementsIfParentT(vector<string>& results, map<string, string>& myMap){
    dbResults.clear();
    vector<string> keywords { "while", "if", "assign", "read", "print" };
    bool flag = 0;
    vector<string> keys;
    vector<int> id1;
    vector<int> id2;
    string getIds;

    // this gets the keys ie: while, if, assign, read, print : it will not store procedure if it sees it
    for (auto const& [key, val] : myMap) {
        if(contains(keywords, key)){
            keys.push_back(key);
        }
        if(key == "value"){
            flag = 1;
        }
    }

    if(keys.size() > 0) {
        // start of procedure
        if (flag) {
            dbResults.clear();
            int procedure_id;
            Database::getProcedureId(procedure_id, myMap["value"]);
            dbResults.clear();

            string getStatements = "SELECT id FROM statements WHERE procedure_id = '" + to_string(procedure_id) + "' AND statement_type = '" + keys.at(0) + "';";
            sqlite3_exec(dbConnection, getStatements.c_str(), callback, 0, &errorMessage);

            for (vector<string> dbRow : dbResults) {
                results.push_back(dbRow.at(0));
            }
        }
        //end of procedure

        // start of while if
        if (!flag) {
            if (keys.at(0) == "while" || keys.at(0) == "if") {
                dbResults.clear();
                getIds = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
                sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                for (vector<string> dbRow: dbResults) {
                    int id;
                    id = stoi(dbRow.at(0));
                    id1.push_back(id);
                }

                if (keys.size() > 1) {
                    dbResults.clear();
                    getIds = "SELECT id FROM statements where statement_type = '" + keys.at(1) + "';";
                    sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                    for (vector<string> dbRow: dbResults) {
                        int id;
                        id = stoi(dbRow.at(0));
                        id2.push_back(id);
                    }
                }
            } else {
                dbResults.clear();
                getIds = "SELECT id FROM statements where statement_type = '" + keys.at(1) + "';";
                sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                for (vector<string> dbRow: dbResults) {
                    int id;
                    id = stoi(dbRow.at(0));
                    id1.push_back(id);
                }

                if (keys.size() > 1) {
                    dbResults.clear();
                    getIds = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
                    sqlite3_exec(dbConnection, getIds.c_str(), callback, 0, &errorMessage);

                    for (vector<string> dbRow: dbResults) {
                        int id;
                        id = stoi(dbRow.at(0));
                        id2.push_back(id);
                    }
                }

                if (keys.size() > 1) {
                    for (auto it: id1) {
                        for (auto it2: id2) {
                            if (Database::parentT(it, it2)) {
                                results.push_back(to_string(it2));
                            }
                        }
                    }
                } else {
                    for (auto it: id1) {
                        for (auto it2: id1) {
                            if (Database::parentT(it, it2)) {
                                results.push_back(to_string(it2));
                            }
                        }
                    }
                }
            }
        }
        // end of while if
    }
};

void Database::getStatementsIfUses(vector<string>& results, map<string, string>& myMap){
    dbResults.clear();
    vector<string> keywords { "while", "if", "assign", "read", "print" };
    vector<string> keys;
    vector<int> ids;

    // this gets the keys ie: while, if, assign, read, print : it will not store procedure if it sees it
    for (auto const& [key, val] : myMap) {
        if(contains(keywords, key)){
            keys.push_back(key);
        }
    }

    string getStatementIDs = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
    sqlite3_exec(dbConnection, getStatementIDs.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow: dbResults) {
        int id;
        id = stoi(dbRow.at(0));
        ids.push_back(id);
    }

    for (auto it : ids) {
        if(Database::uses(it, myMap["value"])){
            results.push_back(to_string(it));
        }
    }
};

void Database::getProcedureNamesIfUses(vector<string>& results, map<string, string>& myMap){
// procedure p; variable v;
    dbResults.clear();
    vector<string> procedure_names;
    Database::getProcedures(procedure_names);
    dbResults.clear();

    for(auto procedure_name : procedure_names){
        if(Database::uses(procedure_name, myMap["value"])){
            results.push_back(procedure_name);
        }
    }
};

void Database::getStatementsIfModifies(vector<string>& results, map<string, string>& myMap){
    dbResults.clear();
    vector<string> keywords { "while", "if", "assign", "read", "print" };
    vector<string> keys;
    vector<int> ids;

    // this gets the keys ie: while, if, assign, read, print : it will not store procedure if it sees it
    for (auto const& [key, val] : myMap) {
        if(contains(keywords, key)){
            keys.push_back(key);
        }
    }

    string getStatementIDs = "SELECT id FROM statements where statement_type = '" + keys.at(0) + "';";
    sqlite3_exec(dbConnection, getStatementIDs.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow: dbResults) {
        int id;
        id = stoi(dbRow.at(0));
        ids.push_back(id);
    }

    for (auto it : ids) {
        if(Database::modifies(it, myMap["value"])){
            results.push_back(to_string(it));
        }
    }
};

void Database::getProcedureNamesIfModifies(vector<string>& results, map<string, string>& myMap){
// procedure p; variable v;
    dbResults.clear();
    vector<string> procedure_names;
    Database::getProcedures(procedure_names);
    dbResults.clear();

    for(auto procedure_name : procedure_names){
        if(Database::modifies(procedure_name, myMap["value"])){
            results.push_back(procedure_name);
        }
    }
};

void Database::getProcedureNamesIfCalls(vector<string>& results, map<string, string>& myMap){
    // This should return the procedure name if it calls the procedure
    dbResults.clear();
    // if there is no quote  " ", assume it is open variable.
    // map should contain 2 procedure.
    vector<string> procedure_names;
    Database::getProcedures(procedure_names);
    dbResults.clear();

    vector<std::string> value = split(myMap["value"], ',');
    int index = index_of_double_quotes(value);
    int index_2 = -1;
    if(index == -1){
        index_2 = index_of_underscore(value);
    }


    // this will get all call statements
    string getCallStatements = "SELECT DISTINCT procedure_id, call_procedure_name FROM statements WHERE statement_type = \"call\"";
    sqlite3_exec(dbConnection, getCallStatements.c_str(), callback, 0, &errorMessage);
    vector<vector<string>> callResults = dbResults;
    string result = "";
    for(vector<string> row : callResults){
        result = "";
        if(index != -1){
            string compare_value = remove_double_quotes(value.at(index));
            if(index == 1){
                // 2. calls(P, "Compute_centroid")
                if (Database::calls(procedure_names.at(stoi(row.at(0)) - 1), compare_value)){
                    result += procedure_names.at(stoi(row.at(0)) - 1);
                    results.push_back(result);
                }
            }
            if(index == 0){
                // 3. calls("Compute_centroid" , P)
                int id = 0;
                Database::getProcedureId(id, compare_value);
                if(stoi(row.at(0)) == id) {
                    results.push_back(row.at(1));
                }
            }
        } else if(index_2 != -1){
            if(index_2 == 1){
                result += procedure_names.at(stoi(row.at(0)) - 1);
                results.push_back(result);
            }
            // calls(_,P)
            if(index_2 == 0){
                result += row.at(1);
                results.push_back(result);
            }
        }
        else {
            // 1: calls(P,Q)
            result += procedure_names.at(stoi(row.at(0)) - 1) + " " + row.at(1);
            results.push_back(result);
        }
    }
};

void Database::getProcedureNamesIfCallsT(vector<string>& results, map<string, string>& myMap){
    // This should return the procedure name if it transitively or normally calls the procedure
    dbResults.clear();
    // if there is no quote  " ", assume it is open variable.
    // map should contain 2 procedure.
    vector<string> procedure_names;
    Database::getProcedures(procedure_names);
    dbResults.clear();

    vector<std::string> value = split(myMap["value"], ',');
    stack<string> s;
    int index = index_of_double_quotes(value);
    int index_2 = -1;
    if(index == -1){
        index_2 = index_of_underscore(value);
    }


    // this will get all call statements
    string getCallStatements = "SELECT DISTINCT procedure_id, call_procedure_name FROM statements WHERE statement_type = \"call\"";
    sqlite3_exec(dbConnection, getCallStatements.c_str(), callback, 0, &errorMessage);
    vector<vector<string>> callResults = dbResults;
    string result = "";
    for(vector<string> row : callResults){
        result = "";
        if(index != -1){
            string compare_value = remove_double_quotes(value.at(index));
            if(index == 1){
                // 2. Calls*(p, "readPoint") => computeCentroid, main
                if (Database::calls(procedure_names.at(stoi(row.at(0)) - 1), compare_value)){
                    result += procedure_names.at(stoi(row.at(0)) - 1);
                    results.push_back(result);
                }
            }
            if(index == 0){
                // 3. Calls*("main", p) => computeCentroid, printResults, readPoint

            }
        } else if(index_2 != -1){
            if(index_2 == 1){
            }
            // calls(_,P)
            if(index_2 == 0){
            }
        }
        else {
            // 1: calls(P,Q)
        }
    }
};

// DB ABSTRACTION METHODS
bool Database::parent(int lineNum1, int lineNum2){
    dbResults.clear();
    int parent_line_num = 0;

    string getParentLineNum = "SELECT parent_line_num FROM statements WHERE id = '" + to_string(lineNum2) + "';";
    sqlite3_exec(dbConnection, getParentLineNum.c_str(), callback, 0, &errorMessage);

    for (vector<string> dbRow : dbResults) {
        parent_line_num = stoi(dbRow.at(0));
    }
    if(lineNum1 == parent_line_num){
        return 1;
    }
    return 0;
}
bool Database::parent(string procedureName, int lineNum){
    dbResults.clear();
    int parent_line = 0;
    int procedure_id = 0;
    Database::getProcedureId(procedure_id, procedureName);
    dbResults.clear();

    string getParentLineNum = "SELECT id FROM statements WHERE procedure_id = '" + to_string(procedure_id) + "' AND parent_line_num = '" + to_string(parent_line) + "';";
    sqlite3_exec(dbConnection, getParentLineNum.c_str(), callback, 0, &errorMessage);

    for(int i = 0; i<dbResults.size(); i++){
        if(stoi(dbResults.at(i).at(0)) == lineNum){
            return 1;
        }
    }
    return 0;
}

// transitivity PARENT*
bool Database::parentT(int lineNum1, int lineNum2){
    dbResults.clear();
    vector<string> keywords { "while", "if"};
    // early break cases
    // lineNum1 should be smaller than lineNum2
    if(lineNum1 >= lineNum2 ){ return 0; }

    // get statement between lineNum1 and lineNum2
    string getStatementRange = "SELECT * FROM statements WHERE id BETWEEN '" + to_string(lineNum1) + "' AND '" + to_string(lineNum2) + "';";
    sqlite3_exec(dbConnection, getStatementRange.c_str(), callback, 0, &errorMessage);

    // break if lineNum1 is not a container
    if(!contains(keywords, dbResults.at(0).at(1))) { return 0; };

    // start transitivity
    // as long as there is no 0 between 3 and 7, not inclusive, return true
    for (int i = 1; i < dbResults.size(); i++)  {
        if(stoi(dbResults.at(i).at(6)) == 0){ return 0; };
    }

    return 1;
}

//USES base function to receive lineNum : ASSIGN, PRINT, CONTAINER
bool Database::uses(int lineNum, string variableName) {
    dbResults.clear();
    // Receive the lineNum and determine the `statement_type`
    // print and assign only require 1 row
    // Container (WHILE and IF) will require all parent and transitivity
    string getStatementRow = "SELECT * FROM statements WHERE id = '" + to_string(lineNum) + "';";
    sqlite3_exec(dbConnection, getStatementRow.c_str(), callback, 0, &errorMessage);

    Tokenizer tk;
    vector<string> tokens, output;
    for (vector<string> dbRow : dbResults) {
        string row;
        for(auto it : dbRow){
            row = it;
            output.push_back(row);
        }
    }
    int target_variable_id = 0;
    Database::getVariableId(target_variable_id, variableName);
    if(output.at(1) == "print"){
        if(stoi(output.at(3)) == target_variable_id){
            return 1;
        }
    }

    if(output.at(1) == "assign"){
        tk.tokenize(output.at(5), tokens);
        if(contains(tokens, variableName)){
            return 1;
        }
    }

    if(output.at(1) == "if" || output.at(1) == "while"){
        tk.tokenize(output.at(5), tokens);
        if(contains(tokens, variableName)){
            return 1;
        } else {
            dbResults.clear();
            string getStatementsSQL = "SELECT * FROM statements;";
            sqlite3_exec(dbConnection, getStatementsSQL.c_str(), callback, 0, &errorMessage);

            for (int i = lineNum; i < dbResults.size(); i++) {
                if(stoi(dbResults.at(i).at(6)) >= lineNum){
                    if(dbResults.at(i).at(1) == "read" || dbResults.at(i).at(1) == "print"){
                        if(stoi(dbResults.at(i).at(3)) == target_variable_id){
                            return 1;
                        }
                    }
                    tokens.clear();
                    if(dbResults.at(i).at(5) != ""){
                        tk.tokenize(dbResults.at(i).at(5), tokens);
                        if(contains(tokens, variableName)){
                            return 1;
                        }
                    }
                } else {
                    break;
                }
            }
        }
    }

    return 0;
}

//USES Function overloading to allow for different parameter data type : PROCEDURES
bool Database::uses(string procedureName, string variableName) {
    dbResults.clear();
    Tokenizer tk;
    vector<string> tokens;
    int id = 0;
    Database::getProcedureId(id, procedureName);
    dbResults.clear();

    int target_variable_id = 0;
    Database::getVariableId(target_variable_id, variableName);
    dbResults.clear();

    string getStatementIds = "SELECT * FROM statements WHERE procedure_id = '" + to_string(id) + "';";
    sqlite3_exec(dbConnection, getStatementIds.c_str(), callback, 0, &errorMessage);

    for(vector<string> row : dbResults){
        if(row.at(1) == "print"){
            if(stoi(row.at(3)) == target_variable_id){
                return 1;
            }
        }

        if(row.at(5) != ""){
            tk.tokenize(row.at(5), tokens);
            if(contains(tokens, variableName)){
                return 1;
            }
        }
    }
    return 0;
}

bool Database::modifies(int lineNum, string variableName) {
    dbResults.clear();

    string getStatementRow = "SELECT * FROM statements WHERE id = '" + to_string(lineNum) + "';";
    sqlite3_exec(dbConnection, getStatementRow.c_str(), callback, 0, &errorMessage);

    Tokenizer tk;
    vector<string> tokens, output;
    for (vector<string> dbRow : dbResults) {
        string row;
        for(auto it : dbRow){
            row = it;
            output.push_back(row);
        }
    }
    int target_variable_id = 0;
    Database::getVariableId(target_variable_id, variableName);
    dbResults.clear();

    if(output.at(1) == "read" || output.at(1) == "assign"){
        if(stoi(output.at(3)) == target_variable_id){
            return 1;
        }
    }

    if(output.at(1) == "if" || output.at(1) == "while"){
        dbResults.clear();
        string getStatementsSQL = "SELECT * FROM statements;";
        sqlite3_exec(dbConnection, getStatementsSQL.c_str(), callback, 0, &errorMessage);

        for (int i = lineNum; i < dbResults.size(); i++) {
            if(stoi(dbResults.at(i).at(6)) >= lineNum) {
                if (dbResults.at(i).at(1) == "read" || dbResults.at(i).at(1) == "assign") {
                    if (stoi(dbResults.at(i).at(3)) == target_variable_id) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
bool Database::modifies(string procedureName, string variableName) {
    dbResults.clear();
    Tokenizer tk;
    vector<string> tokens;
    int id = 0;
    Database::getProcedureId(id, procedureName);
    dbResults.clear();

    int target_variable_id = 0;
    Database::getVariableId(target_variable_id, variableName);
    dbResults.clear();

    string getStatementIds = "SELECT * FROM statements WHERE procedure_id = '" + to_string(id) + "';";
    sqlite3_exec(dbConnection, getStatementIds.c_str(), callback, 0, &errorMessage);

    for(vector<string> row : dbResults){
        if(row.at(1) == "read" || row.at(1) == "assign"){
            if(stoi(row.at(3)) == target_variable_id){
                return 1;
            }
        }
    }
    return 0;
}

bool Database::calls(string procedureName1, string procedureName2){
    // This method's purpose is only to return true or false if the provided P1 calls P2, both must be a name!
    // P1 should be the one calling P2.
    dbResults.clear();
    int id = 0;
    Database::getProcedureId(id, procedureName1);
    dbResults.clear();

    string getCallStatements = "SELECT * FROM statements WHERE statement_type = \"call\" and procedure_id = '" + to_string(id) + "';";
    sqlite3_exec(dbConnection, getCallStatements.c_str(), callback, 0, &errorMessage);

    for(vector<string> row : dbResults){
        cout << "COMPARE THIS: " << row.at(6) << endl;
        if(row.at(6) == procedureName2){
            return 1;
        }
    }

    return 0;
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
