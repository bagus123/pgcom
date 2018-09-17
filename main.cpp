/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Hp
 *
 * Created on September 17, 2018, 2:40 PM
 */

#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

pqxx::result query(char *argv[]) {

    std::string host = argv[1];
    std::string db = argv[2];
    std::string user = argv[3];
    std::string pass = argv[4];
    std::string query = argv[5];


    pqxx::connection c{"dbname = "+db+" user = "+user+" password = "+pass+" host = "+host+" port = 5432"};
    pqxx::work txn{c};
    pqxx::result r = txn.exec(query);
    bool printHeader = true;
    for (auto row = r.begin(); row != r.end(); row++) {
        if (printHeader) {
            for (auto field = row.begin(); field != row.end(); field++) {
                if (printHeader) {
                    std::cout << field->name() << '|';

                }
            }
            std::cout << std::endl;
        }
        for (auto field = row.begin(); field != row.end(); field++) {
            std::cout << field->c_str() << '|';
        }

        printHeader = false;
        std::cout << std::endl;

    }
    txn.commit();
    return r;
}

int main(int, char *argv[]) {
    try {
        pqxx::result r = query(argv);
    } catch (const pqxx::sql_error &e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
        return 2;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}