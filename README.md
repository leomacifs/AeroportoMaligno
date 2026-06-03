# ANAC - Sistema de Gerenciamento de Malha Aérea

Projeto desenvolvido para a disciplina de **Estrutura e Recuperação de Dados II** da **PUC-Campinas**, utilizando **Grafos implementados com Matriz Esparsa de Adjacência** para representar aeroportos e voos da malha aérea brasileira.

## Objetivo

O sistema simula uma solução para a ANAC (Agência Nacional de Aviação Civil), permitindo o gerenciamento de aeroportos e voos através de uma estrutura de dados baseada em grafos.

Cada aeroporto é representado por um vértice do grafo e cada voo por uma aresta direcionada entre dois aeroportos.

---

## Funcionalidades

### Cadastro de Aeroportos

Permite cadastrar novos aeroportos informando:

* Código IATA (ex.: GRU, GIG, CNF)
* Nome da cidade

### Cadastro de Voos

Permite cadastrar voos entre aeroportos existentes informando:

* Aeroporto de origem
* Aeroporto de destino
* Número do voo

### Remoção de Voos

Remove um voo através do número identificador.

### Listagem de Voos

Exibe todos os voos que partem de um aeroporto específico, mostrando:

* Número do voo
* Cidade de destino

### Busca de Trajetos

Permite encontrar caminhos entre dois aeroportos, incluindo:

* Voos diretos
* Rotas com conexões intermediárias

### Listagem de Aeroportos

Exibe todos os aeroportos atualmente cadastrados no sistema.

---

## Estruturas de Dados Utilizadas

### Grafo

O sistema utiliza um grafo direcionado onde:

* Vértices representam aeroportos
* Arestas representam voos

### Matriz Esparsa

A matriz de adjacência foi implementada de forma esparsa através de listas encadeadas, armazenando apenas conexões existentes, reduzindo o consumo de memória.

### Vetor Dinâmico

Os aeroportos são armazenados em um vetor dinâmico que cresce automaticamente quando necessário.

---

## Organização do Projeto


.
├── main.c
├── grafo_aeroportos.c
├── grafo_aeroportos.h
├── matriz_esparsa.c
├── matriz_esparsa.h
└── Makefile


### Arquivos

#### main.c

Responsável pela interface de menu e interação com o usuário.

#### grafo_aeroportos.h / grafo_aeroportos.c

Implementação das operações relacionadas ao grafo:

* Cadastro de aeroportos
* Cadastro de voos
* Remoção de voos
* Busca de aeroportos
* Busca de trajetos
* Listagens

#### matriz_esparsa.h / matriz_esparsa.c

Implementação genérica da matriz esparsa:

* Inserção
* Remoção
* Busca
* Expansão dinâmica
* Destruição da estrutura

#### Makefile

Automatiza a compilação do projeto.

---

## Compilação

### Utilizando Make

```bash
make
```

### Compilação Manual

```bash
gcc -Wall -Wextra -std=c11 -g \
main.c grafo_aeroportos.c matriz_esparsa.c \
-o anac
```

---

## Execução

Linux:

```bash
./anac
```

Ou utilizando o Makefile:

```bash
make run
```

---

## Menu Principal


1. Cadastrar aeroporto
2. Cadastrar voo
3. Remover voo
4. Listar voos de um aeroporto
5. Listar trajetos entre aeroportos
6. Listar todos os aeroportos
0. Sair


---

## Dados de Teste

Ao iniciar o programa são carregados aeroportos de exemplo baseados no enunciado:

* BSB — Brasília
* CNF — Belo Horizonte
* GIG — Rio de Janeiro
* SSA — Salvador
* GRU — São Paulo

Esses dados facilitam os testes iniciais do sistema.

---

## Tratamento de Erros

O sistema realiza diversas validações, incluindo:

* Aeroporto já cadastrado
* Aeroporto inexistente
* Origem e destino iguais
* Número de voo inválido
* Remoção de voo inexistente
* Falhas de alocação de memória

Mensagens de sucesso e erro são exibidas para orientar o usuário.

---

## Conceitos Aplicados

* Grafos direcionados
* Matriz de adjacência esparsa
* Listas encadeadas
* Vetores dinâmicos
* Alocação dinâmica de memória
* Modularização em C
* Busca de caminhos em grafos

---

## Disciplina

Estrutura e Recuperação de Dados II

Pontifícia Universidade Católica de Campinas (PUC-Campinas)

---

## Autores

Guillermo Neto, Leonardo Maciel, Nicollas Fabbio

---

## Licença

Projeto desenvolvido exclusivamente para fins acadêmicos e educacionais.

---

# Diário de Desenvolvimento Individual

## Guillermo Neto

### 14/05/2025

**Horário:** 08:00 às 10:00

* Estudo do enunciado do projeto.
* Planejamento inicial da estrutura do grafo.
* Definição das estruturas para armazenamento dos aeroportos e voos.
* Discussão sobre a utilização de matriz esparsa para representar a malha aérea.

### 16/05/2025

**Horário:** 09:00 às 12:00

* Implementação da estrutura de aeroportos.
* Desenvolvimento das funções de cadastro e busca de aeroportos.
* Testes iniciais de inserção e validação de códigos duplicados.

### 20/05/2025

**Horário:** 13:30 às 15:30

* Implementação da funcionalidade de listagem dos aeroportos cadastrados.
* Correção de inconsistências identificadas durante os testes.

### 27/05/2025

**Horário:** 14:00 às 16:00

* Auxílio nos testes integrados do sistema.
* Revisão das mensagens de erro e validações de entrada.

---

## Leonardo Maciel

### 15/05/2025

**Horário:** 09:30 às 11:00

* Criação da estrutura principal do projeto.
* Configuração dos arquivos de cabeçalho (.h) e implementação (.c).
* Organização da modularização do sistema.

### 18/05/2025

**Horário:** 13:30 às 16:30

* Implementação da matriz esparsa de adjacência.
* Desenvolvimento das funções de inserção, remoção e busca na matriz.
* Testes de alocação dinâmica de memória.

### 22/05/2025

**Horário:** 09:00 às 12:00

* Implementação do cadastro de voos.
* Integração entre aeroportos e matriz esparsa.
* Verificação de consistência entre origem e destino.

### 29/05/2025

**Horário:** 15:30 às 17:00

* Correções gerais do sistema.
* Revisão do código.
* Ajustes de desempenho e tratamento de erros.
* Preparação do projeto para apresentação.

---

## Nicollas Fabbio

### 17/05/2025

**Horário:** 09:00 às 11:00

* Desenvolvimento do menu principal.
* Implementação da interface textual do programa.
* Criação da navegação entre as funcionalidades.

### 21/05/2025

**Horário:** 14:00 às 16:00

* Implementação da funcionalidade de remoção de voos.
* Desenvolvimento das validações de entrada para operações do usuário.

### 24/05/2025

**Horário:** 09:30 às 12:00

* Implementação da listagem de voos por aeroporto.
* Exibição dos números de voo e respectivos destinos.

### 28/05/2025

**Horário:** 14:00 às 16:30

* Implementação da busca de trajetos entre aeroportos.
* Testes de caminhos diretos e indiretos.
* Auxílio na validação final do projeto.

---

## Resumo da Divisão de Responsabilidades

| Integrante      | Principais Responsabilidades                                          |
| --------------- | --------------------------------------------------------------------- |
| Guillermo Neto  | Estruturas de aeroportos, cadastro, busca e listagem de aeroportos    |
| Leonardo Maciel | Matriz esparsa, integração do grafo, cadastro de voos e revisão geral |
| Nicollas Fabbio | Interface do sistema, remoção de voos, listagens e busca de trajetos  |


