// ConexionBD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>

using namespace std;

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Allocate an environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Allocate a connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Connect to the database
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Connected to database successfully." << endl;

        // Example of executing a query
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Example SELECT query
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Datos_Empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            SQLCHAR name[255];
            SQLCHAR last_name[255];
            int ID;
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &ID, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, last_name, sizeof(name), NULL);
               // SQLGetData(hStmt, 2, SQL_C_LONG, &age, 0, NULL);
                cout << "ID: " << ID << ", Name: " << name << ", Lastname: " << last_name << endl;
            }
        }

        // Free statement handle
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    else {
        cout << "Failed to connect to database." << endl;
    }

    // Disconnect and free handles
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
