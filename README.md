````markdown
# Case Técnico DTI Rubix: Gerenciador de Músicas

Este projeto é uma aplicação de console desenvolvida em C++ como parte do processo seletivo de estágio (Rubix) da DTI. A aplicação implementa as funcionalidades básicas de CRUD (Create, Read, Update, Delete) para gerenciar uma coleção de músicas, utilizando SQLite como banco de dados.

## 1. Visão Geral do Projeto

* **Linguagem Escolhida:** C++ (Padrão C++17). A linguagem foi escolhida por ser uma tecnologia robusta e orientada a objetos ao qual eu ja fui introduzida anteriormente.
* **Banco de Dados:** SQLite.
* **Recurso Escolhido:** Música.

### 1.1. Propriedades do Recurso (Música)

O recurso "Música" é gerenciado por uma única tabela no banco de dados (o usuário usou `music` no singular no `Database.cpp`). As propriedades definidas cumprem os requisitos de incluir campos obrigatórios, opcionais e tipos de dados variados (texto, numérico e data).

| Campo | Tipo de Dado (SQL) | Obrigatório? | Descrição |
| :--- | :--- | :--- | :--- |
| `id` | INTEGER | Sim (Automático) | Identificador único (Chave Primária). |
| `title` | TEXT | Sim | Título da música (Campo de texto). |
| `artist` | TEXT | Não | Nome do artista ou banda. |
| `album` | TEXT | Não | Álbum ao qual a música pertence. |
| `genre` | TEXT | Não | Gênero da música. |
| `release_year` | INTEGER | Não | Ano de lançamento (Campo numérico). |
| `registration_date` | DATE | Sim | Data em que o registro foi criado (Campo de data). |

## 2. Configuração do Ambiente e Dependências

A aplicação foi desenvolvida e testada em ambiente Linux (Ubuntu) e compilada utilizando `make`.

**Dependências Obrigatórias:**
* `g++` (ou `gcc`): Compilador C/C++.
* `make`: Ferramenta para automatizar a compilação.
* `git`: Para clonar o repositório.
* `libgtest-dev`: (Necessário para rodar os testes unitários).

Não é necessário instalar o SQLite, pois ele é "embutido" e seus arquivos-fonte (`sqlite3.c`, `sqlite3.h`) estão inclusos no projeto.

## 3. Compilação e Execução

Siga estes passos para compilar e iniciar a aplicação:

**1. Clonar o Repositório:**
```bash
git clone [https://github.com/analeopoldino/api-musica.git](https://github.com/analeopoldino/api-musica.git)
cd api-musica
````

**2. Compilar o Projeto:**
O projeto utiliza um `Makefile` para gerenciar a compilação.

```bash
make all
```

Isso criará um executável chamado `music_manager` na raiz do projeto.

**3. Executar a Aplicação:**

```bash
./music_manager
```

O script SQL para criação da tabela é executado automaticamente na inicialização.

## 4\. Guia de Uso (Funcionalidades)

A aplicação funciona através de um menu interativo.

  * **1. Cadastrar Música:**

      * Solicita ao usuário o título (obrigatório), artista, álbum, gênero e ano.
      * Realiza validações (ex: título não pode ser vazio).

  * **2. Listar Músicas:**

      * Exibe todas as músicas cadastradas no banco de dados.

  * **3. Buscar Música por ID:**

      * Busca exata pelo identificador único.

  * **4. Atualizar Música:**

      * Solicita o ID da música a ser atualizada.
      * Solicita que o usuário insira *todos* os novos dados (título, artista, álbum, etc.).

  * **5. Deletar Música:**

      * Solicita o ID da música a ser removida.
      * Pede confirmação (s/n) antes de deletar.

  * **0. Sair:**

      * Encerra a aplicação.

## 5\. Diferenciais Implementados

Além dos requisitos obrigatórios, os seguintes diferenciais opcionais foram implementados:

### 5.1. Testes Unitários

O projeto inclui um conjunto de testes unitários para a classe `Database`, garantindo a robustez e o correto funcionamento das operações de CRUD. Os testes foram desenvolvidos com a biblioteca Google Test (GTest).

**Como executar os testes:**

```bash
make test
```

### 5.2. Conteinerização da Aplicação (Docker)

Foi criado um `Dockerfile` que permite construir e executar a aplicação em um ambiente isolado e padronizado, facilitando a distribuição e a execução.

**Como construir e executar o contêiner:**

```bash
# 1. Construir a imagem
docker build -t music-manager .

# 2. Executar o contêiner
docker run -it music-manager
```

