#include "Instrutor.hpp"
#include <algorithm>

Instrutor::Instrutor(const std::string& nome,
                     const std::string& cpf,
                     const std::string& email,
                     const std::string& cref,
                     const std::string& especialidade)
    : Pessoa(nome, cpf, email),
      cref(cref),
      especialidade(especialidade)
{
}

void Instrutor::prescreverTreino(Cliente* aluno, Treino* novoTreino)
{
    if (aluno == nullptr)
        return;

    aluno->associarTreino(novoTreino);

    auto it = std::find(alunosSobSupervisao.begin(),
                        alunosSobSupervisao.end(),
                        aluno);

    if (it == alunosSobSupervisao.end())
    {
        alunosSobSupervisao.push_back(aluno);
    }
}

std::string Instrutor::getCref() const
{
    return cref;
}

std::string Instrutor::getEspecialidade() const
{
    return especialidade;
}