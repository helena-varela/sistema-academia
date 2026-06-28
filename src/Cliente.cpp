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
    treinoDesignado = treino;
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

    std::getline(is, nome, ';');
    std::getline(is, cpf, ';');
    std::getline(is, email, ';');
    std::getline(is, codigo);

    return is;
}