#pragma once
#include "sqlite3.h"
#include "Music.hpp"
#include <vector>

class Database {
private:
    //Ponteiro para o banco de dados SQLite
    sqlite3* db;

    //Callback para execuções de comandos SQL
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);

public:
    //Construtor e Destrutor
    Database(const std::string& db_file); 
    ~Database();

    //Inicializar tabelas
    bool initializeTables(); 

    //CRUD - Create, Read, Update, Delete
    bool registerMusic(const Music& music);
    std::vector<Music> listAllMusics();
    Music searchById(int id);
    bool updateMusic(int id, const Music& music);
    bool deleteMusic(int id); 
};