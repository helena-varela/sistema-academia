#include "Cliente.hpp"

Cliente::Cliente(const std::string& nome,
                 const std::string& cpf,
                 const std::string& email,
                 Plano* plano,
                 const std::string& dataInicio)
    : Pessoa(nome, cpf, email),
      planoAtual(plano),
      treinoDesignado(nullptr),
      matricula(dataInicio)
{
    if(plano == nullptr) {
        throw DadosInvalidosException("plano", "Cliente nao pode ser cadastrado sem um plano valido.");
    }
}

Cliente::~Cliente()
{
    delete planoAtual;
}

void Cliente::associarTreino(Treino* treino)
{
    if (treino == nullptr)
    {
        throw DadosInvalidosException("treino", "Treino invalido.");
    }
    treinoDesignado = treino;
}

void Cliente::setNome(const std::string& nome)
{
    if (nome.empty())
    {
        throw DadosInvalidosException("nome", "Nome nao pode ser vazio.");
    }

    this->nome = nome;
}

void Cliente::setCPF(const std::string& cpf)
{
    if (!validarCPF(cpf))
    {
        throw DadosInvalidosException("cpf", "CPF invalido.");
    }

    this->cpf = cpf;
}

void Cliente::setEmail(const std::string& email)
{
    if (email.empty())
    {
        throw DadosInvalidosException("email", "Email nao pode ser vazio.");
    }

    this->email = email;
}

void Cliente::setPlano(Plano* plano)
{
    if (plano == nullptr)
    {
        throw DadosInvalidosException("plano", "Plano invalido.");
    }
    delete planoAtual;
    planoAtual = plano;
}

Plano* Cliente::getPlanoAtual() const
{
    return planoAtual;
}

Treino* Cliente::getTreinoDesignado() const
{
    return treinoDesignado;
}

const Matricula& Cliente::getMatricula() const {
    return matricula;
}

Matricula& Cliente::getMatricula() {
    return matricula;
}

int Cliente::getCodigoMatricula() const
{
    return matricula.getCodigoMatricula();
}

bool Cliente::ValidarEntrada() const
{
    return Pessoa::ValidarEntrada() &&
           planoAtual != nullptr;
}

std::ostream& operator<<(std::ostream& os, const Cliente& cliente)
{
    os << cliente.getNome() << ';'
       << cliente.getCpf() << ';'
       << cliente.getEmail() << ';'
       << cliente.getCodigoMatricula();

    return os;
}

std::istream& operator>>(std::istream& is, Cliente& cliente)
{
    std::string nome;
    std::string cpf;
    std::string email;
    std::string codigo;

    if (std::getline(is, nome, ';') &&
        std::getline(is, cpf, ';') &&
        std::getline(is, email, ';') &&
        std::getline(is, codigo))
    {
        cliente.setNome(nome);
        cliente.setCPF(cpf);
        cliente.setEmail(email);
    }

    return is;
}

void Cliente::restaurarMatricula(int codigo, const std::string& data, StatusMatricula status){
    matricula.setCodigoEData(codigo, data, status);
}

std::string Cliente::getDataInicio() const {
    return matricula.getDataInicio();
}

void Cliente::iniciarTreino(int horaAcesso) const
{
    if (treinoDesignado == nullptr)
    {
        throw AcessoNegadoException("treino", "Voce precisa ter um treino designado antes de inicia-lo.");
    }

    // Regra: apenas matriculas ativas podem treinar
    if (matricula.getStatus() == StatusMatricula::INADIMPLENTE)
    {
        throw AcessoNegadoException("treino", "Acesso bloqueado: matricula inadimplente. Regularize o pagamento.");
    }
    if (matricula.getStatus() == StatusMatricula::TRANCADA)
    {
        throw AcessoNegadoException("treino", "Acesso bloqueado: matricula trancada.");
    }

    // Regra: restricao de horario depende do plano (polimorfismo)
    planoAtual->aplicarRestricaoAcesso(horaAcesso);
}
