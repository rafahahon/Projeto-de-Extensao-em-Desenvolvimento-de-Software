INSERT INTO cliente (nome, github_user, email) VALUES 
-- Lista de Clientes
('João Silva', 'joaosilva_dev', 'joao.silva@codecoffee.com'),
('Maria Santos', 'msantos_coder', 'maria.santos@codecoffee.com'),
('Pedro Oliveira', 'pedro_oliveira99', 'pedro.oliveira@codecoffee.com'),
('Ana Costa', 'ana_costa_dev', 'ana.costa@codecoffee.com'),
('Carlos Lima', 'carloslima_tech', 'carlos.lima@codecoffee.com'),
('Beatriz Souza', 'bia_souza', 'beatriz.souza@codecoffee.com'),
('Lucas Ferreira', 'lucas_f_dev', 'lucas.ferreira@codecoffee.com'),
('Fernanda Lima', 'nanda_lima', 'fernanda.lima@codecoffee.com'),
('Ricardo Alves', 'ricardo_alves_cod', 'ricardo.alves@codecoffee.com'),
('Juliana Mendes', 'ju_mendes_dev', 'juliana.mendes@codecoffee.com'),
('Gabriel Rocha', 'gabriel_rocha_tech', 'gabriel.rocha@codecoffee.com'),
('Camila Dias', 'camila_dias_dev', 'camila.dias@codecoffee.com'),
('Thiago Pires', 'thiago_pires_js', 'thiago.pires@codecoffee.com'),
('Larissa Gomes', 'lari_gomes_dev', 'larissa.gomes@codecoffee.com'),
('Rafael Costa', 'rafa_costa_coder', 'rafael.costa@codecoffee.com');

INSERT INTO funcionario (nome, cargo) VALUES 
-- Lista de Funcionários
('Lara Silva', 'Gerente de Loja'),
('Bruno Alves', 'Barista'),
('Carla Dias', 'Atendente'),
('Daniel Santos', 'Cuidador de Gatos'),
('Elisa Farias', 'Barista'),
('Fábio Souza', 'Auxiliar de Limpeza'),
('Giulia Mendes', 'Atendente'),
('Henrique Dias', 'Cuidador de Gatos'),
('Igor Costa', 'Barista'),
('Júlia Santos', 'Responsável pelo Adoção'),
('Kevin Oliveira', 'Auxiliar de Cozinha'),
('Luana Rocha', 'Atendente'),
('Marco Pires', 'Cuidador de Gatos'),
('Nádia Gomes', 'Auxiliar de Limpeza'),
('Otávio Lima', 'Gerente de Operações'),
('Paula Andrade', 'Cuidador de Gatos'),
('Ricardo Neves', 'Barista'),
('Sofia Martins', 'Atendente'),
('Tiago Oliveira', 'Auxiliar de Limpeza'),
('Vanessa Souza', 'Gerente de Loja');

INSERT INTO playground (nome_setor, capacidade_gatos, capacidade_clientes) VALUES 
-- Lista de Playground
('Zona de Programação Felina', 10, 5);

INSERT INTO gato (nome, personalidade, status_gato, id_playground) VALUES 
-- Lista de Gatos
('Python', 'Simpático', 'Disponível', 1),
('Java', 'Misterioso', 'Disponível', 1),
('Ruby', 'Elegante', 'Descansando', 1),
('Swift', 'Magestoso', 'Disponível', 1),
('Kotlin', 'Carismática', 'Disponível', 1),
('TypeScript', 'Sofisticado', 'Veterinário', 1),
('Rust', 'Explorador e destemido', 'Disponível', 1),
('Go', 'Ágil e muito brincalhão', 'Disponível', 1),
('PHP', 'Amigável, mas um pouco desastrado', 'Descansando', 1),
('C#', 'Elegante e organizado', 'Disponível', 1),
('Dart', 'Muito veloz e sociável', 'Disponível', 1);

INSERT INTO produto (nome, descricao, preco, quantidade_estoque, id_categoria) VALUES 
-- CAFÉS (Categoria: Bebida)
('Latte Art', 'Café com leite vaporizado e desenho', 15.70, 50, '3'),
('Canela Café', 'Café com um toque aromático de canela', 15.00, 30, '3'),
('Flat White', 'Café com leite cremoso', 9.00, 40, '3'),
('Macchiato', 'Café expresso com uma mancha de espuma', 8.50, 35, '3'),
('Espresso', 'Café concentrado de alta qualidade', 7.00, 100, '3'),
('Shot', 'Dose única de puro café', 10.50, 100, '3'),
('Capuccino', 'Café com leite e cacau em pó', 8.00, 45, '3'),

-- SALGADOS E DOCES (Categoria: Salgados e Doces)
('Queijo', 'Pão de queijo artesanal', 7.00, 80, '2'),
('Muffins', 'Muffin de frutas vermelhas', 8.50, 20, '1'),
('Quiche', 'Quiche de alho poró', 10.00, 15, '2'),
('Brownie', 'Brownie de chocolate belga', 9.00, 25, '1'),
('Coxinha', 'Coxinha de frango tradicional', 10.00, 60, '2'),
('Cheesecake', 'Cheesecake com calda de frutas', 12.00, 10, '1'),

-- BEBIDAS GELADAS (Categoria: Bebida)
('Americano', 'Café diluído com gelo', 10.00, 40, '3'),
('Pink', 'Bebida refrescante de morango', 10.50, 20, '3'),
('Frappé', 'Café batido com gelo e chantilly', 16.00, 30, '3'),
('Soda', 'Soda italiana artesanal', 15.00, 25, '3'),
('Hibisco', 'Chá de hibisco gelado', 14.50, 20, '3'),
('Cold Brew', 'Café extraído a frio', 15.00, 30, '3'),

-- PETISCO (Categoria: Petisco)
('Sachê de Frango', 'Petisco premium para gatos', 4.00, 200, '4'),
('Sachê de Salmão', 'Petisco nutritivo para gatos', 4.50, 200, '4'),
('Sachê de Carne', 'Petisco saboroso para gatos', 4.00, 150, '4');

INSERT INTO reserva_mesa (id_cliente, id_mesa, inicio_reserva, fim_reserva) VALUES 
-- Lista de Reservas de mesas
(1, 1, '2026-04-26 14:00:00', '2026-04-26 15:30:00'), 
(2, 2, '2026-04-26 15:00:00', '2026-04-26 16:30:00'),
(3, 3, '2026-04-26 16:00:00', '2026-04-26 17:30:00'), 
(4, 4, '2026-04-26 17:00:00', '2026-04-26 18:30:00'),
(5, 5, '2026-04-26 18:00:00', '2026-04-26 19:30:00'), 
(6, 1, '2026-04-27 10:00:00', '2026-04-27 11:30:00'),
(7, 2, '2026-04-27 11:00:00', '2026-04-27 12:30:00'), 
(8, 3, '2026-04-27 12:00:00', '2026-04-27 13:30:00'),
(9, 4, '2026-04-27 13:00:00', '2026-04-27 14:30:00'), 
(10, 5, '2026-04-27 14:00:00', '2026-04-27 15:30:00'),
(11, 1, '2026-04-28 09:00:00', '2026-04-28 10:30:00'), 
(12, 2, '2026-04-28 10:00:00', '2026-04-28 11:30:00'),
(13, 3, '2026-04-28 11:00:00', '2026-04-28 12:30:00'), 
(14, 4, '2026-04-28 12:00:00', '2026-04-28 13:30:00'),
(15, 5, '2026-04-28 13:00:00', '2026-04-28 14:30:00');

INSERT INTO sessoes_playground (id_cliente, id_playground, minutos_permanencia, valor_pago) VALUES 
-- Lista de Sessões no Playground
(1, 1, 30, 15.00), 
(2, 1, 60, 25.00), 
(3, 1, 45, 20.00), 
(4, 1, 30, 15.00), 
(5, 1, 60, 25.00),
(6, 1, 45, 20.00), 
(7, 1, 30, 15.00), 
(8, 1, 60, 25.00), 
(9, 1, 45, 20.00), 
(10, 1, 30, 15.00),
(11, 1, 60, 25.00), 
(12, 1, 45, 20.00), 
(13, 1, 30, 15.00), 
(14, 1, 60, 25.00), 
(15, 1, 45, 20.00);

INSERT INTO pedido (id_cliente, id_fun, valor_total_pedido, id_pagamento) VALUES 
-- Lista de Pedidos feitos pelos clientes
(1,1,24.50,'1'),
(2,2,28.50,'3'),
(3,3,15.00,'4'),
(4,4,30.00,'1'),
(5,5,12.00,'1'),
(6,6,45.00,'2'),
(7,7,20.00,'1'),
(8,8,18.00,'3'),
(9,9,25.00,'4'),
(10,10,35.00,'1'),
(11,11,10.00,'2'),
(12,12,22.00,'1'),
(13,13,40.00,'3'),
(14,14,15.00,'4'),
(15,15,20.00,'1');

INSERT INTO itens_pedido (id_pedido, id_produto, quantidade_pedida, preco_unitario) VALUES 
-- Lista de itens em pedidos de clientes
(1, 1, 1, 12.50), 
(1, 8, 1, 6.00), 
(1, 20, 2, 4.00),
(2, 16, 1, 15.00), 
(2, 9, 1, 8.50),
(3, 5, 2, 7.00), 
(3, 11, 1, 9.00),
(4, 17, 1, 10.00), 
(4, 12, 1, 7.00),
(5, 8, 2, 6.00), 
(5, 21, 1, 4.50),
(6, 11, 2, 9.00), 
(6, 13, 1, 14.00),
(7, 2, 1, 10.00), 
(7, 18, 1, 13.00),
(8, 9, 2, 8.50), 
(8, 22, 1, 4.00),
(9, 10, 1, 12.00), 
(9, 3, 1, 11.00),
(10, 18, 1, 13.00), 
(10, 14, 1, 9.00),
(11, 6, 2, 5.00), 
(11, 15, 1, 12.00),
(12, 12, 2, 7.00), 
(12, 20, 1, 4.00),
(13, 19, 1, 9.50), 
(13, 13, 1, 14.00),
(14, 13, 1, 14.00), 
(14, 4, 1, 9.50),
(15, 20, 2, 4.00), 
(15, 7, 1, 11.50);

INSERT INTO categoria_produto (nome_categoria) VALUES 
-- Lista de categoria de produto
('Doce'),
('Salgado'),
('Bebida'),
('Petisco');


INSERT INTO forma_pagamento (nome_pagamento) VALUES
-- Lista de formas de pagamento
('PIX'),
('Cartão Débito'),
('Cartão Crédito'),
('Dinheiro');
