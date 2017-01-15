# SGBD-UFF
(Este trabalho ganhou o premio de melhor trabalho 2 em BD 2016.2)
Implementação de um SGBD com as funçoes "Create Table", "Insert" e "Select".
Ele funciona lendo um arquivo .sql escrito pelo usuario. 
A sintaxe que deve ser usada é:

  CREATE TABLE nomeTabela(
  atr1 TIPO(INTEGER ou STRING) [NN] [KEY] [ORD],
  atr2 TIPO(INTEGER ou STRING) [NN] [KEY] [ORD],
  ...)
  
  INSERT
  INTO nomeTabela
  VALUES ("atr1","atr2","atr3" ou NULO se o atributo aceitar,...);
  
  SELECT atr1,atr2,...
  FROM nomeTabela [JOIN nomeTabela[2] ON atrX=atrY]
  WHERE atr(=,>,<,>=,<=,><)valor;
  
O SGBD impede que você cometa erros como colocar nulo em atributos nao nulos, ter dois atributos chave na tabela ou dois atributos que ordenem a tabela.
Foi utilizado C.
