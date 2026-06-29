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
}

Cliente::~Cliente()
{
}

void Cliente::associarTreino(Treino* treino)
{
    if (treino == nullptr)
    {
        throw AcademiaException("Treino invalido.");
    }

    treinoDesignado = treino;
}

void Cliente::setNome(const std::string& nome)
{
    if (nome.empty())
    {
        throw AcademiaException("Nome nao pode ser vazio.");
    }

    this->nome = nome;
}

void Cliente::setCPF(const std::string& cpf)
{
    if (!validarCPF(cpf))
    {
        throw AcademiaException("CPF invalido.");
    }

    this->cpf = cpf;
}

void Cliente::setEmail(const std::string& email)
{
    if (email.empty())
    {
        throw AcademiaException("Email nao pode ser vazio.");
    }

    this->email = email;
}

void Cliente::setPlano(Plano* plano)
{
    if (plano == nullptr)
    {
        throw AcademiaException("Plano invalido.");
    }

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

int Cliente::getCodigoMatricula() const
{
    return matricula.getCodigoMatricula();
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
