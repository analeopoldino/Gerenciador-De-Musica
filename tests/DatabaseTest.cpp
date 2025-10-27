#include "gtest/gtest.h"
#include "Database.hpp"
#include "Music.hpp"

class DatabaseTestFixture : public ::testing::Test {
protected:
    Database* db;

    void SetUp() override {
        db = new Database("test.db");
        db->initializeTables();
    }

    void TearDown() override {
        delete db;
        remove("test.db"); 
    }
};

//Teste 1: Cadastro básico 
TEST_F(DatabaseTestFixture, RegisterValidMusic ) {
    Music m;
    m.title = "Bohemian Rhapsody";
    m.artist = "Queen";
    m.album = "A Night at the Opera";
    m.genre = "Rock";
    m.release_year = 1975;
    m.registration_date = "2025-10-27";

    bool ok = db->registerMusic(m);
    ASSERT_TRUE(ok);

    auto all = db->listAllMusics();
    ASSERT_EQ(all.size(), 1);
    EXPECT_EQ(all[0].title, "Bohemian Rhapsody");
}

//Teste 2: Cadastro inválido que corresponde ao título estar vazio
TEST_F(DatabaseTestFixture, RegisterUntitledMusic) {
    Music m;
    m.title = "";
    m.artist = "Queen";
    m.registration_date = "2025-10-27";

    EXPECT_THROW(db->registerMusic(m), std::invalid_argument);
}

//Teste 3: Buscar por ID existente
TEST_F(DatabaseTestFixture, SearchByExistingId) {
    Music m;
    m.title = "Imagine";
    m.artist = "John Lennon";
    m.registration_date = "2025-10-27";
    db->registerMusic(m);

    Music result = db->searchById(1);

    ASSERT_EQ(result.id, 1);
    ASSERT_EQ(result.title, "Imagine");
    ASSERT_EQ(result.artist, "John Lennon");
}

//Teste 4: Buscar ID inexistente
TEST_F(DatabaseTestFixture, SearchForNonExistentId) {
    Music result = db->searchById(999);
    EXPECT_EQ(result.id, 0);
    EXPECT_TRUE(result.title.empty());
}

//Teste 5: Atualizar música existente
TEST_F(DatabaseTestFixture, UpdateExistingMusic) {
    Music m;
    m.title = "Blinding Lights";
    m.artist = "The Weeknd";
    m.registration_date = "2025-10-27";
    db->registerMusic(m);

    Music newMusic;
    newMusic.title = "Hotel California";
    newMusic.artist = "Eagles";
    newMusic.album = "Hotel California";
    newMusic.genre = "Rock";
    newMusic.release_year = 2025;

    bool ok = db->updateMusic(1, newMusic);
    ASSERT_TRUE(ok);

    Music atualizado = db->searchById(1);
    EXPECT_EQ(atualizado.title, "Smells Like Teen Spirit");
    EXPECT_EQ(atualizado.artist, "Nirvana");
    EXPECT_EQ(atualizado.album, "Nevermind");
}

//Teste 6: Atualizar música inexistente
TEST_F(DatabaseTestFixture, UpdateNonExistentMusic) {
    Music newMusic;
    newMusic.title = "Despacito";
    newMusic.artist = "Luis Fonsi ft. Daddy Yankee";
    bool ok = db->updateMusic(999, newMusic);
    EXPECT_FALSE(ok);
}

//Teste 7: Deletar música existente
TEST_F(DatabaseTestFixture, DeleteExistingMusic) {
    Music m;
    m.title = "Shape of You";
    db->registerMusic(m);

    bool deleted = db->deleteMusic(1);
    ASSERT_TRUE(deleted);

    Music result = db->searchById(1);
    EXPECT_TRUE(result.title.empty());
}

//Teste 8: Deletar música inexistente
TEST_F(DatabaseTestFixture, DeleteNonExistingMusic) {
    bool deleted = db->deleteMusic(123);
    EXPECT_FALSE(deleted);
}

//Teste 9: Listar múltiplas músicas
TEST_F(DatabaseTestFixture, MultipleMusicsList) {
    Music m1;
    m1.title = "Somebody That I Used to Know";
    m1.artist = "Gotye ft. Kimbra";
    m1.album = "Making Mirrors";
    m1.genre = "Indie Pop";
    m1.release_year = 2011;
    m1.registration_date = "2025-10-27";

    Music m2;
    m2.title = "Sunflower";
    m2.artist = "Post Malone & Swae Lee";
    m2.album = "Spider-Man: Into the Spider-Verse (Soundtrack)";
    m2.genre = "Hip Hop";
    m2.release_year = 2018;
    m2.registration_date = "2025-10-27";

    Music m3;
    m3.title = "Wonderwall";
    m3.artist = "Oasis";
    m3.album = "(What's the Story) Morning Glory?";
    m3.genre = "Britpop";
    m3.release_year = 1995;
    m3.registration_date = "2025-10-27";

    db->registerMusic(m1);
    db->registerMusic(m2);
    db->registerMusic(m3);

    auto todas = db->listAllMusics();
    ASSERT_EQ(todas.size(), 3);
    EXPECT_EQ(todas[1].title, "Song2");
}

//Teste 10: Campos opcionais vazios
TEST_F(DatabaseTestFixture, RegisterWithEmptyOptionalFields) {
    Music m;
    m.title = "Thriller";
    m.artist = "";
    m.album = "";
    m.genre = "";
    m.release_year = 0;
    m.registration_date = "2025-10-27";

    ASSERT_TRUE(db->registerMusic(m));

    Music r = db->searchById(1);
    EXPECT_EQ(r.title, "Thriller");
    EXPECT_TRUE(r.artist.empty());
    EXPECT_TRUE(r.genre.empty());
}
