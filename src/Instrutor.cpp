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

void Instrutor::setNome(const std::string& nome)
{
    if (nome.empty())
    {
        throw AcademiaException("Nome nao pode ser vazio.");
    }

    this->nome = nome;
}

void Instrutor::setCPF(const std::string& cpf)
{
    if (!validarCPF(cpf))
    {
        throw AcademiaException("CPF invalido.");
    }

    this->cpf = cpf;
}

void Instrutor::setEmail(const std::string& email)
{
    if (email.empty())
    {
        throw AcademiaException("Email nao pode ser vazio.");
    }

    this->email = email;
}

void Instrutor::setEspecialidade(const std::string& especialidade)
{
    if (especialidade.empty())
    {
        throw AcademiaException("Especialidade nao pode ser vazia.");
    }

    this->especialidade = especialidade;
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

std::list<Cliente*> Instrutor::getAlunosSobSupervisao() const
{
    return alunosSobSupervisao;
}

bool Instrutor::ValidarEntrada() const
{
    return Pessoa::ValidarEntrada() &&
           !cref.empty() &&
           !especialidade.empty();
}

Cliente* Instrutor::consultarAlunoSupervisionado(int codigoMatricula) const
{
    for (Cliente* aluno : alunosSobSupervisao)
    {
        if (aluno->getCodigoMatricula() == codigoMatricula)
        {
            return aluno;
        }
    }
    throw AcademiaException("Aluno nao esta sob supervisao deste instrutor.");
}
