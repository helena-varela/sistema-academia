# Sistema de Gestão de uma Academia
Esse projeto foi feito para a matéria de **Linguagem de Programação I**.

## 🗂️ Arquitetura do Projeto
A arquitetura do projeto foi modularizada para separar as definições de tipos, as implementações lógicas e os arquivos gerados no ciclo de compilação:

```plaintext
/SISTEMA-ACADEMIA
├───/bin                         # Arquivos gerados na compilação
├───/data                        # Arquivos de persistência de dados
├───/include                     # Arquivos de cabeçalho (.hpp)
├───/src                         # Implementação das classes (.cpp)
├───Makefile                     # Automação do processo de compilação
├───.gitignore                   # Arquivos e diretórios ignorados pelo Git
└───README.md                    # Documentação principal do projeto
```

### 1. sistema-academia/ (Raiz): 

Diretório principal do projeto. Contém os arquivos de configuração global como o Makefile , .gitignore e a documentação principal (README.md)

### 2. bin/ (Binários): 

Armazena todos os arquivos de objeto intermediários (.o) e o arquivo executável final do sistema. Esta pasta é gerada automaticamente pelo Makefile e não deve ser enviada para o repositório Git (deve estar no .gitignore).

### 3. data/ (Persistência): 

Destinada aos arquivos de texto (.txt) utilizados para salvar e carregar os dados cadastrais do sistema entre as execuções da aplicação.

### 4. include/ (Cabeçalhos / Headers): 

Contém os arquivos de extensão .hpp. É onde ficam as declarações das 9 classes , atributos, protótipos de métodos e as definições de exceções personalizadas. Nenhuma lógica de código deve ser escrita aqui.

### 5. src/ (Fontes / Source): 

Contém os arquivos de extensão .cpp com a implementação real da lógica declarada nos headers, além do arquivo main.cpp , que gerencia o menu de interação via terminal.


## ⚙️ Como Compilar

### make:

**O que faz:** Compila de forma incremental todo o projeto.

**Como funciona:** O make varre a pasta src/, identifica quais arquivos .cpp foram criados ou modificados, gera seus respectivos arquivos de objeto .o dentro de bin/ e faz a linkagem final para construir o executável. Se nenhum arquivo foi modificado desde a última compilação, ele avisa que o alvo já está atualizado, poupando tempo.

### make run

**O que faz:** Compila o projeto (caso haja alguma alteração pendente) e inicia a aplicação imediatamente.

**Como funciona:** Ele dispara o processo padrão do make e, logo em seguida, executa o arquivo binário gerado, abrindo o menu interativo no terminal. É o comando ideal para testar o código rapidamente durante o desenvolvimento.  

### make clean

**O que faz:** Limpa o ambiente de desenvolvimento deletando os arquivos gerados pela compilação.

**Como funciona:** Ele remove completamente o diretório bin/ e tudo o que está dentro dele. É útil usar este comando antes de criar arquivos compactados (.zip) para entrega  ou para forçar uma compilação do zero quando ocorrer algum erro estranho de cache de arquivos.  

## 📑 Diagrama de Classes


```mermaid
classDiagram
    direction OA
    
    %% --- RELAÇÕES ---
    Pessoa <|-- Cliente
    Pessoa <|-- Instrutor
    Plano <|-- PlanoMensal
    Plano <|-- PlanoAnual
    Cliente *-- Matricula
    Cliente o-- Treino
    Instrutor o-- Cliente
    GerenciadorAcademia ..> Pessoa : Gerencia (CRUD)
    GerenciadorAcademia ..> AcademiaException : Lança

    %% --- CLASSES ---
    class Pessoa {
        <<Abstract>>
        # string nome
        # string cpf
        # string email
        + Pessoa(string nome, string cpf, string email)
        + ~Pessoa()*
        + getNome() string
        + getCpf() string
    }

    class Cliente {
        - string codigoMatricula
        - Plano* planoAtual
        - Treino* treinoDesignado
        + Cliente(string nome, string cpf, string email, Plano* plano)
        + associarTreino(Treino* t) void
        + getPlanoAtual() Plano*
        + operator~plus~~plus~(ostream& os, const Cliente& c)$ void
        + operator~greater~~greater~(istream& is, Cliente& c)$ void
    }

    class Instrutor {
        - string cref
        - string especialidade
        - list~Cliente*~ alunosSobSupervisao
        + Instrutor(string nome, string cpf, string email, string cref, string esp)
        + prescreverTreino(Cliente* aluno, Treino* novoTreino) void
    }

    class Plano {
        <<Abstract>>
        # double precoBase
        + Plano(double preco)
        + ~Plano()*
        + calcularMensalidade()* double
        + aplicarRestricaoAcesso(string horaAcesso)* bool
    }

    class PlanoMensal {
        - double taxaInscricaoAvulsa
        + PlanoMensal(double preco, double taxa)
        + calcularMensalidade() double
        + aplicarRestricaoAcesso(string horaAcesso) bool
    }

    class PlanoAnual {
        - int mesesFidelidade
        - double descontoFidelidade
        + PlanoAnual(double preco, int meses, double desc)
        + calcularMensalidade() double
        + aplicarRestricaoAcesso(string horaAcesso) bool
    }

    class Treino {
        - string focoMuscular
        - list~string~ exercicios
        - int duracaoEstimadaMin
        + Treino(string foco, list~string~ ex, int duracao)
        + getFoco() string
    }

    class Matricula {
        %% Estado Dinâmico
        - string statusAtual
        - string dataInicio
        + Matricula(string data)
        + alterarEstado(string novoEstado) void
        + validarTransicao(string atual, string novo) bool
        + getStatus() string
    }

    class GerenciadorAcademia {
        - vector~Cliente*~ clientes
        - vector~Instrutor*~ instrutores
        + cadastrarCliente(Cliente* c) void
        + consultarCliente(string cpf) Cliente*
        + atualizarCliente(string cpf, Cliente* novoC) void
        + removerCliente(string cpf) void
        + salvarEmArquivo(string nomeArq) void
        + carregarDeArquivo(string nomeArq) void
    }

    class AcademiaException {
        - string mensagemErro
        + AcademiaException(string msg)
        + what() const char*
    }
```
<!-- <img width="7637" height="8192" alt="Cliente Management Framework-2026-06-17-013321" src="https://github.com/user-attachments/assets/66b8d54f-1f82-4c84-b334-e11a5d0b07be" /> -->
