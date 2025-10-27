#include "Database.hpp"
#include <vector>
#include <iostream>
#include <fstream>  
#include <sstream>  
#include <stdexcept>

//Construtor - abre a conexão com o banco de dados
Database::Database(const std::string& db_file) : db(nullptr) {
    int rc = sqlite3_open(db_file.c_str(), &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir banco de dados: %s\n", sqlite3_errmsg(db));
        throw std::runtime_error("Falha ao abrir banco");
    } else {
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    }
}; 

//Destrutor - fecha a conexão com o banco de dados
Database::~Database(){
    if(db){
        sqlite3_close(db);
    }
};

//Callback para execuções de comandos SQL
int Database::callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
};

//Inicializar tabelas
bool Database::initializeTables(){
    std::ifstream schemaFile("data/MusicsDatabase.sql");
    if (!schemaFile.is_open()) {
        throw std::runtime_error("Could not open SQL file");
    }

    std::stringstream buffer;
    buffer << schemaFile.rdbuf();
    std::string sql = buffer.str();

    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error("Failed to initialize tables");
    }

    return true;
}; 

//Cadastrar nova musica
bool Database::registerMusic(const Music& music){
    if (music.title.empty()) {
        throw std::invalid_argument("Title cannot be empty");
    }

    sqlite3_stmt* stmt;
    std::string sql = "INSERT INTO music (title, artist, album, genre, release_year, registration_date) VALUES (?, ?, ?, ?, ?, ?);";    
    
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(stmt, 1, music.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, music.artist.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, music.album.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, music.genre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, music.release_year);
    sqlite3_bind_text(stmt, 6, music.registration_date.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;   
    }

    sqlite3_finalize(stmt);
    return true;
}

//Listar todas as musicas
std::vector<Music> Database::listAllMusics(){
    std::vector<Music> musics;
    std::string sql = "SELECT id, title, artist, album, genre, release_year, registration_date FROM music;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return musics;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        Music music;
        music.id = sqlite3_column_int(stmt, 0);
        music.title = (const char*)sqlite3_column_text(stmt, 1);
        music.artist = (const char*)sqlite3_column_text(stmt, 2);
        music.album = (const char*)sqlite3_column_text(stmt, 3);
        music.genre = (const char*)sqlite3_column_text(stmt, 4);
        music.release_year = sqlite3_column_int(stmt, 5);
        music.registration_date = (const char*)sqlite3_column_text(stmt, 6);
        musics.push_back(music);
    }

    sqlite3_finalize(stmt);
    return musics;
};

//Buscar musica por ID
Music Database::searchById(int id){
    Music music;
    std::string sql = "SELECT id, title, artist, album, genre, release_year, registration_date FROM music WHERE id = ?;";
    sqlite3_stmt* stmt;

    music.id = 0; 

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return music;
    }

    sqlite3_bind_int(stmt, 1, id);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        music.id = sqlite3_column_int(stmt, 0);
        music.title = (const char*)sqlite3_column_text(stmt, 1);
        music.artist = (const char*)sqlite3_column_text(stmt, 2);
        music.album = (const char*)sqlite3_column_text(stmt, 3);
        music.genre = (const char*)sqlite3_column_text(stmt, 4);
        music.release_year = sqlite3_column_int(stmt, 5);
        music.registration_date = (const char*)sqlite3_column_text(stmt, 6);;
    }

    sqlite3_finalize(stmt);
    return music;
};

//Atualizar musica
bool Database::updateMusic(int id, const Music& music){
    sqlite3_stmt* stmt;
    std::string sql = "UPDATE music SET title = ?, artist = ?, album = ?, genre = ?, release_year = ? WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, music.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, music.artist.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, music.album.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, music.genre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, music.release_year);
    sqlite3_bind_int(stmt, 6, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, " Failed to update data: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return (sqlite3_changes(db) > 0);
};

//Deletar musica
bool Database::deleteMusic(int id){
    sqlite3_stmt* stmt;
    std::string sql = "DELETE FROM music WHERE id = ?;";

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar statement (DELETE): %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erro ao executar step (DELETE): %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return (sqlite3_changes(db) > 0);
};

