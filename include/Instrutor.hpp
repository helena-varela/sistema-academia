#ifndef INSTRUTOR_HPP
#define INSTRUTOR_HPP

#include "pessoa.hpp"
#include "Cliente.hpp"
#include <string>
#include <list>

class Instrutor : public Pessoa
{
    private:
        std::string cref;
        std::string especialidade;
        std::list<Cliente*> alunossobsupervisao;
    public:
        Instrutor(std::string nome, std::string cpf, std::string email, std::string cref, std::string especialidade);
        void prescreverTreino(Cliente* aluno, Treino* novoTreino);
};
#endif // INSTRUTOR_HPP