# Code Cat Coffee

Projeto do grupo **HttpCats** para o primeiro semestre do curso de Sistemas de Informações da Uninove. Turma 6/2026

## HttpCats

| Nome                         | RA         |
|------------------------------|------------|
| Ana Santos Nascimento        | 3036101499 |
| Breno Fessel Da Paz          | 3026104340 |
| Gabrielli Minhano Barbosa    | 3026103592 |
| Rafael Baptista de Almeida   | 3026101346 |
| Rafaella Pinto Hahon         | 3026102787 |
| Victor Nunes Garcia          | 3026101023 |

## Estrutura do projeto

Temos 3 pastas, uma para cada tarefa designada:

- `banco-de-dados`: contendo o arquivo SQL e o arquivo SQLite com o banco de dados de exemplo.
- `programa-c`: programa de controle de pedidos usando o banco de dados em SQLite.
- `site-institucional`: site em HTML/CSS/JS sobre a Code Cat Coffee.

### Banco de Dados

Temos um arquivo compatível com MySQL e outro compatível com SQLite para criar as tabelas. Também temos um arquivo para inserções de dados de exemplo.

O DER está dentro da pasta com toda a estrutura das tabelas usadas no programa em C.

### Site Institucional

Temos o site institucional com 5 páginas, feito em HTML5 e CSS3. São elas:

- Home/Página Inicial: Introdução da Empresa e seu modelo de negócio
- Sobre Nós: Uma história da empresa e a missão dela
- Cardápio: Listagem de produtos e dos gatos disponíveis na Cafeteria. Somente para visualização.
- Avaliações: Página onde você pode avaliar a empresa e também ver as avaliações.
- Contato: Página com dados de contato da empresa e um formulário de contato onde você pode enviar mensagem.

### Programa em C

Temos um programa em C para coordenar e executar as atividades diárias do café. Através dele, podemos gerenciar cadastros de funcionários, clientes, mesas, gatos, entre outros.

Para poder executar o programa em sua máquina:

1. Abra o terminal do Power Shell ou Linha de Comando do Windows;
2. Navegue até a pasta raiz deste projeto;
3. Execute o seguinte comando: `.\bin\codecatcoffee-v$VERSAO.exe`, onde `$VERSAO` é a versão atual do programa;
4. Siga os passos para inicializar o banco de dados na máquina local;
5. Navegue pelo menu e bom trabalho!