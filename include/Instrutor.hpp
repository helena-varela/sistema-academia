#ifndef INSTRUTOR_HPP
#define INSTRUTOR_HPP

#include <string>
#include <list>

#include "Pessoa.hpp"
#include "Cliente.hpp"
#include "Treino.hpp"
#include "AcademiaException.hpp"

class Instrutor : public Pessoa
{
    private:
        std::string cref;
        std::string especialidade;
        std::list<Cliente*> alunosSobSupervisao;
    public:
        Instrutor(const std::string& nome,
              const std::string& cpf,
              const std::string& email,
              const std::string& cref,
              const std::string& especialidade);

        void adicionarAlunoSupervisionado(Cliente* aluno);
        void removerAlunoSupervisionado(Cliente* aluno);
        bool supervisionaAluno(Cliente* aluno) const;
        void prescreverTreino(Cliente* aluno, Treino* novoTreino);

        std::string getCref() const;
        std::string getEspecialidade() const;
};
#endif // INSTRUTOR_HPP