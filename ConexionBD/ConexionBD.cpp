// ConexionBD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Conectado a la base de datos exitosamente." << endl;

        // Ejemplo de ejecución de una consulta
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // Imprimir encabezados de la tabla
        cout << "+---------------+----------------------------------------+-----------------+" << endl;
        cout << "| No. Empleado  | Nombre Completo                        | Directivo       |" << endl;
        cout << "+---------------+----------------------------------------+-----------------+" << endl;

        // Ejemplo de consulta SELECT
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Datos_Empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLCHAR name[50];
            SQLCHAR last_name[50];
            SQLCHAR slast_name[50];
            SQLCHAR directivo[50];
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, last_name, sizeof(last_name), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, slast_name, sizeof(slast_name), NULL);
                SQLGetData(hStmt, 10, SQL_C_CHAR, directivo, sizeof(directivo), NULL);

                // Concatenar nombre y apellido paterno
                string last_names = string((char*)last_name) + " " + string((char*)slast_name);
                string full_name = string((char*)name) + " " + string(last_names);
                string dir = string((char*)directivo);
                string respuesta;

                if (dir == "1") {
                    respuesta = "SI";
                }
                else 
                {
                    respuesta = "NO";
                }
               

                // Imprimir datos de la fila con alineación
                cout << "| " << setw(14) << left << num_empleado
                     << "| " << setw(39) << left << full_name 
                     << "| " << setw(15) << left << respuesta << " |" << endl;
            }
        }

        // Imprimir línea final de la tabla
        cout << "+---------------+----------------------------------------+-----------------+" << endl;

        // Liberar el manejador de conexión
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }
    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}



