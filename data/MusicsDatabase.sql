/* Tabela Musica - Recurso escolhido para a aplicação*/

CREATE TABLE IF NOT EXISTS music(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    artist TEXT,
    album TEXT,
    genre TEXT,
    release_year INTEGER,
    registration_date DATE NOT NULL
)