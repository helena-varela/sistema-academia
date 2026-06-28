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
    {
        throw AcademiaException("Aluno invalido.");
    }

    if (novoTreino == nullptr)
    {
        throw AcademiaException("Treino invalido.");
    }

    if (!supervisionaAluno(aluno))
    {
        throw AcademiaException("Instrutor so pode prescrever treino para alunos supervisionados.");
    }

    aluno->associarTreino(novoTreino);
}

void Instrutor::adicionarAlunoSupervisionado(Cliente* aluno)
{
    if (aluno == nullptr)
    {
        throw AcademiaException("Aluno invalido.");
    }

    auto it = std::find(alunosSobSupervisao.begin(),
                        alunosSobSupervisao.end(),
                        aluno);

    if (it == alunosSobSupervisao.end())
    {
        alunosSobSupervisao.push_back(aluno);
    }
}

void Instrutor::removerAlunoSupervisionado(Cliente* aluno)
{
    if (aluno == nullptr)
    {
        throw AcademiaException("Aluno invalido.");
    }

    alunosSobSupervisao.remove(aluno);
}

bool Instrutor::supervisionaAluno(Cliente* aluno) const
{
    return std::find(alunosSobSupervisao.begin(),
                     alunosSobSupervisao.end(),
                     aluno) != alunosSobSupervisao.end();
}

std::string Instrutor::getCref() const
{
    return cref;
}

std::string Instrutor::getEspecialidade() const
{
    return especialidade;
}