//
// Created by lahmer on 23/08/17.
//

#include "DataBaseUtils.h"

static int dummyCallback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}


DataBaseUtils::DataBaseUtils(const std::string &path) {
    m_path = path;
    int code = sqlite3_open(m_path.c_str(), &m_db);
    if( code ) {
        std::cout<<"Can't open database: "<<sqlite3_errmsg(m_db)<<std::endl;
        std::terminate();
    }
}


void DataBaseUtils::executeQuery(const char *query) {
    char *errorMessage = 0;

    int code = sqlite3_exec(m_db,
                            query,
                            dummyCallback, 0, &errorMessage);


    if( code != SQLITE_OK ) {
        std::cout<<"SQL error: \n"<<errorMessage<<std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(m_db);
        std::terminate();
    } else {
        std::cout<<"Operation done successfully"<<std::endl;
    }
}


DataBaseUtils::~DataBaseUtils() {
    sqlite3_close(m_db);
}
