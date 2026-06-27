#include "Cliente.hpp"

Cliente::Cliente(const std::string& nome,
                 const std::string& cpf,
                 const std::string& email,
                 Plano* plano,
                 const std::string& codigoMatricula,
                 const std::string& dataInicio)
    : Pessoa(nome, cpf, email),
      codigoMatricula(codigoMatricula),
      planoAtual(plano),
      treinoDesignado(nullptr),
      matricula(new Matricula(codigoMatricula, dataInicio))
{
}

Cliente::~Cliente()
{
    delete matricula;
}

void Cliente::associarTreino(Treino* treino)
{
    treinoDesignado = treino;
}

Plano* Cliente::getPlanoAtual() const
{
    return planoAtual;
}

Matricula* Cliente::getMatricula() const
{
    return matricula;
}

Treino* Cliente::getTreinoDesignado() const
{
    return treinoDesignado;
}

std::string Cliente::getCodigoMatricula() const
{
    return codigoMatricula;
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