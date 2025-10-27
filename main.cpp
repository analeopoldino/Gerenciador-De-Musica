#include <iostream>
#include <string>
#include <vector>
#include <limits>    
#include <chrono>    
#include <ctime>    

#include "Database.hpp"
#include "Music.hpp"

//Função auxiliar para data
std::string getFormattedCurrentDate() {
    auto nowTime = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(nowTime);
    std::tm tm_local;
    #ifdef _WIN32
        localtime_s(&tm_local, &currentTime); // Windows
    #else
        localtime_r(&currentTime, &tm_local); // POSIX (Linux/macOS)
    #endif
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm_local);

    return std::string(buffer);
}

//Função auxiliar para limpar a tela 
void clearScreen() {
   #if defined(_WIN32) || defined(_WIN64)
    system("cls");
    #else 
        system("clear");
    #endif
}

//Função auxiliar para limpar o buffer de entrada
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Função auxiliar para pausar a execução até o usuário pressionar Enter
void pauseToContinues() {
    std::cout << "\n\nPressione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cin.get();
}

// Menu para cadastrar nova musica
// Menu para cadastrar nova musica
void menuRegisterMusic(Database& db) {
    Music newMusic;
    
    std::cout << "\n--- Cadastrar Nova Musica ---\n";
    clearInputBuffer();

    std::cout << "Digite o titulo (obrigatorio): ";
    std::getline(std::cin, newMusic.title);

    std::cout << "Digite o artista (opcional): ";
    std::getline(std::cin, newMusic.artist);

    std::cout << "Digite o album (opcional): ";
    std::getline(std::cin, newMusic.album);

    // --- CORREÇÃO DO BUG AQUI ---
    std::cout << "Digite o genero (opcional): ";
    std::getline(std::cin, newMusic.genre); // <-- Estava 'newMusic.album'

    std::cout << "Digite o ano de lancamento (opcional, 0 se nao souber): ";
    while (!(std::cin >> newMusic.release_year)) {
        std::cout << "Erro: Digite um numero valido.\n";
        std::cin.clear();
        clearInputBuffer();
    }

    newMusic.registration_date = getFormattedCurrentDate();

    try {
        if (db.registerMusic(newMusic)) {
            std::cout << "\nMusica cadastrada com sucesso!\n"; 
        } else {
            std::cout << "\nErro desconhecido ao cadastrar musica.\n";
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "\nErro de validacao: " << e.what() << std::endl;
    }
}


// Menu para listar todas as musicas
void menuListAllMusics(Database& db) {
    std::cout << "\n--- Lista de Musicas ---\n";
    std::vector<Music> musics = db.listAllMusics();

    if (musics.empty()) {
        std::cout << "Nenhuma musica cadastrada.\n";
        return;
    }

    for (const auto& music : musics) {
        std::cout << "ID: " << music.id << "\n"
                  << "  Titulo: " << music.title << "\n"
                  << "  Artista: " << (music.artist.empty() ? "(N/A)" : music.artist) << "\n"
                  << "  Album: " << (music.album.empty() ? "(N/A)" : music.album) << "\n"
                  << "  Genero: " << (music.genre.empty() ? "(N/A)" : music.genre) << "\n"
                  << "  Ano: " << (music.release_year == 0 ? "(N/A)" : std::to_string(music.release_year)) << "\n"
                  << "  Cadastrada em: " << music.registration_date << "\n"
                  << "---------------------------\n";
    }
}

// Menu para buscar musica por ID
void menuSearchById(Database& db) {
    int id;
    std::cout << "\n--- Buscar Musica por ID ---\n";
    std::cout << "Digite o ID da musica: ";
    while (!(std::cin >> id)) {
        std::cout << "Erro: Digite um numero de ID valido.\n";
        std::cin.clear();
        clearInputBuffer();
    }

    Music music = db.searchById(id);

    if (music.id == 0) { 
        std::cout << "Erro: Musica com ID " << id << " nao encontrada.\n";
    } else {
        std::cout << "Musica encontrada:\n"
                  << "  Titulo: " << music.title << "\n"
                  << "  Artista: " << music.artist << "\n"
                  << "  Album: " << music.album << "\n"
                  << "  Genero: " << music.genre << "\n"
                  << "  Ano: " << music.release_year << "\n"
                  << "  Cadastrada em: " << music.registration_date << "\n";
    }
}

// Menu para atualizar musica
void menuUpdateMusic(Database& db) {
    int id;
    std::cout << "\n--- Atualizar Musica ---\n";
    std::cout << "Digite o ID da musica que deseja atualizar: ";
    while (!(std::cin >> id)) {
        std::cout << "Erro: Digite um numero de ID valido.\n";
        std::cin.clear();
        clearInputBuffer();
    }

    Music musicToUpdate = db.searchById(id);
    if (musicToUpdate.id == 0) {
        std::cout << "Erro: Musica com ID " << id << " nao encontrada.\n";
        return;
    }

    std::cout << "Musica encontrada: " << musicToUpdate.title << "\n";
    std::cout << "Insira os *novos* dados:\n"; 

    Music newMusicData;
    clearInputBuffer();

    while (true) {
        std::cout << "Novo titulo (obrigatorio): ";
        std::getline(std::cin, newMusicData.title);
        if (!newMusicData.title.empty()) break;
        std::cout << "Erro: O titulo nao pode ser vazio.\n";
    }
    
    std::cout << "Novo artista (opcional): ";
    std::getline(std::cin, newMusicData.artist);
    
    std::cout << "Novo album (opcional): ";
    std::getline(std::cin, newMusicData.album);

    std::cout << "Novo genero (opcional): ";
    std::getline(std::cin, newMusicData.genre);
    
    std::cout << "Novo ano (opcional, 0 se nao souber): ";
    while (!(std::cin >> newMusicData.release_year)) {
        std::cout << "Erro: Digite um numero valido.\n";
        std::cin.clear();
        clearInputBuffer();
    }

    if (db.updateMusic(id, newMusicData)) {
        std::cout << "Musica atualizada com sucesso!\n";
    } else {
        std::cout << "Erro ao atualizar musica (ID " << id << " nao encontrado).\n";
    }
}

// Menu para deletar musica
void menuDeleteMusic(Database& db) {
    int id;
    std::cout << "\n--- Deletar Musica ---\n";
    std::cout << "Digite o ID da musica que deseja deletar: ";
    while (!(std::cin >> id)) {
        std::cout << "Erro: Digite um numero de ID valido.\n";
        std::cin.clear();
        clearInputBuffer();
    }

    std::cout << "Tem certeza que deseja deletar a musica com ID " << id << "? (s/n): ";
    char confirm;
    std::cin >> confirm;

    if (confirm == 's' || confirm == 'S') {
        if (db.deleteMusic(id)) {
            std::cout << "Musica deletada com sucesso!\n";
        } else {
            std::cout << "Erro: Musica com ID " << id << " nao foi encontrada.\n";
        }
    } else {
        std::cout << "Operacao cancelada.\n";
    }
}

// Função para exibir o menu principal
void showMenu() {
    std::cout << "\n===== Sistema de Gerenciamento de Musicas =====\n"
              << "1. Cadastrar Musica\n"
              << "2. Listar Musicas\n"
              << "3. Buscar Musica por ID\n"
              << "4. Atualizar Musica\n"
              << "5. Deletar Musica\n"
              << "0. Sair\n"
              << "===============================================\n"
              << "Escolha uma opcao: ";
}

// Função principal
int main() {
    Database db("musics.db");
    try {
        db.initializeTables(); 
    } catch (const std::exception& e) {
        std::cerr << "Erro critico ao inicializar o banco de dados: " << e.what() << std::endl;
        return 1; 
    }

    int choice = -1;

    while (choice != 0) {
        clearScreen();

        showMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "Opcao invalida. Por favor, digite um numero.\n";
            std::cin.clear();
            clearInputBuffer();
            choice = -1;
            pauseToContinues();
            continue;
        }

        switch (choice) {
            case 1:
                menuRegisterMusic(db);
                break;
            case 2:
                menuListAllMusics(db);
                break;
            case 3:
                menuSearchById(db);
                break;
            case 4:
                menuUpdateMusic(db);
                break;
            case 5:
                menuDeleteMusic(db);
                break;
            case 0:
                std::cout << "Saindo do sistema...\n";
                break;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
        }
        
        if(choice != 0){
            pauseToContinues();
        }
    }

    return 0;
}