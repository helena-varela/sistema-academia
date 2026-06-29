#include "Pessoa.hpp"

#include <algorithm>
#include <cctype>

Pessoa::Pessoa(const std::string& nome,
               const std::string& cpf,
               const std::string& email)
    : nome(nome),
      cpf(cpf),
      email(email)
{
    if (nome.empty())
    {
        throw AcademiaException("Nome nao pode ser vazio.");
    }

    if (email.empty())
    {
        throw AcademiaException("Email nao pode ser vazio.");
    }

    if (!validarCPF(cpf))
    {
        throw AcademiaException("CPF inválido.");
    }
}

Pessoa::~Pessoa()
{
}

std::string Pessoa::getNome() const
{
    return nome;
}

std::string Pessoa::getCpf() const
{
    return cpf;
}

std::string Pessoa::getEmail() const
{
    return email;
}

bool Pessoa::ValidarEntrada() const
{
    return !nome.empty() &&
           !email.empty() &&
           validarCPF(cpf);
}

bool Pessoa::validarCPF(const std::string& cpf) const
{
    if (cpf.length() != 11)
        return false;

    return std::all_of(cpf.begin(), cpf.end(), ::isdigit);
}
