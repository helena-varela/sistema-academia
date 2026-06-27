#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include "Plano.hpp"
#include "Treino.hpp"
#include "Matricula.hpp"
#include "Pessoa.hpp"

class Cliente : public Pessoa
{
    private:
        Plano* planoAtual;
        Treino* treinoDesignado;
        Matricula matricula;
    public:
        Cliente(std::string nome, std::string cpf, std::string email, Plano* plano);
        void associarTreino(Treino* t);
        void consultarCliente(std::string cpf);
        Plano* getPlanoAtual();
        friend std::ostream& operator <<(std::ostream& os, const Cliente& c);
        friend std::istream& operator >>(std::istream& is, const Cliente& c);
};

#endif