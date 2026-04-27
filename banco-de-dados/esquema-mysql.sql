/**
 *              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Clara Oliveira da Silva     | 3026104548 |
 * | Ana Santos Nascimento           | 3036101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Pedro Junior da Silva Soares    | 3026104213 |
 * | Rafael Baptista de Almeida      | 3026103546 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Banco de dados em MySQL para o controle de pedidos, reservas, etc.
 */
CREATE DATABASE IF NOT EXISTS code_cat_coffee;
USE code_cat_coffee;

-- 1. CLIENTE
CREATE TABLE cliente (
    id_cliente INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(100) NOT NULL,
    github_user VARCHAR(50),
    email VARCHAR(100) UNIQUE
);

-- 2. FUNCIONÁRIO
CREATE TABLE funcionario (
    id_fun INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(100) NOT NULL,
    cargo VARCHAR(100) NOT NULL
);

-- 3. MESA (Fundamental para o Coworking)
CREATE TABLE mesa (
    id_mesa INT PRIMARY KEY AUTO_INCREMENT,
    numero_mesa INT NOT NULL UNIQUE,
    tomada INT, 
    extra_monitor TINYINT(1) DEFAULT 0
);

-- 4. PLAYGROUND
CREATE TABLE playground (
    id_playground INT PRIMARY KEY AUTO_INCREMENT,
    nome_setor VARCHAR(50) NOT NULL, 
    capacidade_gatos INT,
    capacidade_clientes INT
);

-- 5. GATO
CREATE TABLE gato (
    id_gato INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(50) NOT NULL,
    personalidade VARCHAR(100),
    status_gato ENUM('Disponível', 'Descansando', 'Veterinário') DEFAULT 'Disponível', 
    id_playground INT,
    FOREIGN KEY (id_playground) REFERENCES Playground(id_playground)
);

-- 6. PRODUTO
CREATE TABLE produto (
    id_produto INT PRIMARY KEY AUTO_INCREMENT,
    nome VARCHAR(100) NOT NULL,
    descricao TEXT, 
    preco DECIMAL(10,2) NOT NULL,
    quantidade_estoque INT DEFAULT 0,
    id_categoria INT,
    FOREIGN KEY (id_categoria) REFERENCES categoria_produto(id_categoria)
);

-- 7. RESERVA DE MESA
CREATE TABLE reserva_Mesa (
    id_reserva INT PRIMARY KEY AUTO_INCREMENT,
    id_cliente INT,
    id_mesa INT,
    inicio_reserva DATETIME,
    fim_reserva DATETIME,
    FOREIGN KEY (id_cliente) REFERENCES Cliente(id_cliente),
    FOREIGN KEY (id_mesa) REFERENCES Mesa(id_mesa)
);

-- 8. SESSÕES DE PLAYGROUND
CREATE TABLE sessoes_playground (
    id_sessao INT PRIMARY KEY AUTO_INCREMENT,
    id_cliente INT,
    id_playground INT,
    hora_entrada DATETIME DEFAULT CURRENT_TIMESTAMP,
    minutos_permanencia INT,
    valor_pago DECIMAL(10,2),
    FOREIGN KEY (id_cliente) REFERENCES Cliente(id_cliente),
    FOREIGN KEY (id_playground) REFERENCES Playground(id_playground)
);

-- 9. PEDIDO (Cabeçalho)
CREATE TABLE pedido (
    id_pedido INT PRIMARY KEY AUTO_INCREMENT,
    id_cliente INT,
    id_fun INT, 
    valor_total_pedido DECIMAL(10,2),
    id_pagamento INT,
    data_pedido DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_cliente) REFERENCES cliente(id_cliente),
    FOREIGN KEY (id_fun) REFERENCES funcionario(id_fun),
    FOREIGN KEY (id_pagamento) REFERENCES forma_pagamento(id_pagamento)
);

-- 10. ITENS_PEDIDO (Detalhes)
CREATE TABLE itens_pedido (
    id_item INT PRIMARY KEY AUTO_INCREMENT,
    id_pedido INT,
    id_produto INT,
    quantidade_pedida INT NOT NULL,
    preco_unitario DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (id_pedido) REFERENCES Pedido(id_pedido),
    FOREIGN KEY (id_produto) REFERENCES Produto(id_produto)
);

-- 11. CATEGORIA_PRODUTO (Identificar e individualizar cada produto)
CREATE TABLE categoria_produto (
    id_categoria INT PRIMARY KEY AUTO_INCREMENT,
    nome_categoria VARCHAR(50) NOT NULL UNIQUE
);

-- 12. FORMA_PAGAMENTO (Categorizar forma de pagamento dos pedidos)
CREATE TABLE forma_pagamento (
    id_pagamento INT PRIMARY KEY AUTO_INCREMENT,
    nome_pagamento VARCHAR(50) NOT NULL UNIQUE
);
