//
// Created by lahmer on 23/08/17.
//

#ifndef SCENARIO_DATABASEUTILS_H
#define SCENARIO_DATABASEUTILS_H

#include <sqlite3.h>
#include <iostream>
#include <map>


class DataBaseUtils {

private:
    sqlite3 *m_db;
    std::string m_path;
    std::map<std::string, std::string> m_values;

public:
    DataBaseUtils(const std::string& path);

    void executeQuery(const char* query);

    ~DataBaseUtils();

    void addCouple(std::string column, std::string text){
        m_values[column] = "'" + text + "'";
    }

    void addCouple(std::string column, double value){
        std::cout<<" adding values "<<column<<"    "<<value<<std::endl;
        m_values[column] = std::to_string(value);
    }


    void addCouple(std::string& column, uint64_t& value){
        m_values[column] = std::to_string(value);
    }

    void addCouple(std::string& column, uint32_t& value){
        m_values[column] = std::to_string(value);
    }


    std::string constructInsertQuery(){
        std::string first = " INSERT INTO results (";
        std::string second= " VALUES (";

        for(auto it = m_values.begin(); it != m_values.end(); it++){
            first += it->first;
            second += it->second;
            it++;
            if( it != m_values.end()){
                first  += ",";
                second += ",";
            }
            it--;
        }
        first += ") ";
        second += ") ";
        return first + second;

    }
};


#endif //SCENARIO_DATABASEUTILS_H
