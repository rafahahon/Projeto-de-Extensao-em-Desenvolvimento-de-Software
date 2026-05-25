/**
 *              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Santos Nascimento           | 3036101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Rafael Baptista de Almeida      | 3026101346 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Banco de dados para o controle de pedidos, reservas, etc. com base no MySQL e adaptado para as restrições do SQLite.
 */

PRAGMA
foreign_keys = ON;

-- CLIENTE
CREATE TABLE cliente
(
    id_cliente  INTEGER PRIMARY KEY,
    nome        TEXT NOT NULL,
    github_user TEXT,
    email       TEXT UNIQUE
);

-- FUNCIONÁRIO
CREATE TABLE funcionario
(
    id_fun INTEGER PRIMARY KEY,
    nome   TEXT NOT NULL,
    cargo  TEXT NOT NULL
);

-- MESA (Fundamental para o Coworking)
CREATE TABLE mesa
(
    id_mesa       INTEGER PRIMARY KEY,
    numero_mesa   INTEGER NOT NULL UNIQUE,
    tomada        INTEGER,
    extra_monitor NUMERIC DEFAULT 0
);

-- RESERVA DE MESA
CREATE TABLE reserva_mesa
(
    id_reserva     INTEGER PRIMARY KEY,
    id_cliente     INTEGER,
    id_mesa        INTEGER,
    inicio_reserva INTEGER,
    fim_reserva    INTEGER,
    FOREIGN KEY (id_cliente) REFERENCES cliente (id_cliente),
    FOREIGN KEY (id_mesa) REFERENCES mesa (id_mesa)
);

-- PLAYGROUND
CREATE TABLE playground
(
    id_playground       INTEGER PRIMARY KEY,
    nome_setor          TEXT NOT NULL,
    capacidade_gatos    INTEGER,
    capacidade_clientes INTEGER
);

-- GATO
CREATE TABLE gato
(
    id_gato       INTEGER PRIMARY KEY,
    nome          TEXT NOT NULL,
    personalidade TEXT,
    status_gato   INTEGER DEFAULT 1, -- 1-Disponível, 2-Descansando, 3-Veterinário
    id_playground INTEGER,
    FOREIGN KEY (id_playground) REFERENCES playground (id_playground)
);

-- SESSOES DE PLAYGROUND PRA BRINCAR COM OS GATINHOS
CREATE TABLE sessoes_playground
(
    id_sessao           INTEGER PRIMARY KEY,
    id_cliente          INTEGER,
    id_playground       INTEGER,
    hora_entrada        INTEGER DEFAULT CURRENT_TIMESTAMP,
    minutos_permanencia INTEGER,
    valor_pago          NUMERIC,
    FOREIGN KEY (id_cliente) REFERENCES cliente (id_cliente),
    FOREIGN KEY (id_playground) REFERENCES playground (id_playground)
);

-- FORMAS DE PAGAMENTO DO PEDIDO
CREATE TABLE forma_pagamento
(
    id_forma_pagto INTEGER PRIMARY KEY,
    nome           TEXT NOT NULL
);

-- PEDIDO (Cabeçalho)
CREATE TABLE pedido
(
    id_pedido          INTEGER PRIMARY KEY,
    id_cliente         INTEGER,
    id_fun             INTEGER NOT NULL,
    id_forma_pagto     INTEGER NOT NULL,
    valor_total_pedido NUMERIC,
    data_pedido        INTEGER DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_cliente) REFERENCES cliente (id_cliente),
    FOREIGN KEY (id_fun) REFERENCES funcionario (id_fun),
    FOREIGN KEY (id_forma_pagto) REFERENCES forma_pagamento (id_forma_pagto)
);

-- CATEGORIAS DOS PRODUTOS
CREATE TABLE categoria_produto
(
    id_cat_produto INTEGER PRIMARY KEY,
    nome           TEXT NOT NULL
);

-- PRODUTO
CREATE TABLE produto
(
    id_produto         INTEGER PRIMARY KEY,
    nome               TEXT    NOT NULL,
    descricao          TEXT,
    preco              NUMERIC NOT NULL,
    quantidade_estoque INTEGER DEFAULT 0,
    id_cat_produto     INTEGER NOT NULL,
    FOREIGN KEY (id_cat_produto) REFERENCES categoria_produto (id_cat_produto)
);

-- ITENS_PEDIDO (Detalhes)
CREATE TABLE itens_pedido
(
    id_item           INTEGER PRIMARY KEY,
    id_pedido         INTEGER,
    id_produto        INTEGER,
    quantidade_pedida INTEGER NOT NULL,
    preco_unitario    NUMERIC NOT NULL,
    FOREIGN KEY (id_pedido) REFERENCES pedido (id_pedido),
    FOREIGN KEY (id_produto) REFERENCES produto (id_produto)
);
