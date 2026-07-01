#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include<iostream>

#include <string>
#include "Pessoa.hpp"
#include "Plano.hpp"
#include "Treino.hpp"
#include "Matricula.hpp"
#include "AcademiaException.hpp"

class Cliente : public Pessoa
{
    private:
        Plano* planoAtual;
        Treino* treinoDesignado;
        Matricula matricula;
    public:
        Cliente(const std::string& nome,
             const std::string& cpf,
             const std::string& email,
             Plano* plano,
             const std::string& dataInicio);

         ~Cliente() override;

        void associarTreino(Treino* treino);

        void setNome(const std::string& nome);
        void setCPF(const std::string& cpf);
        void setEmail(const std::string& email);
        void setPlano(Plano* plano);

        Plano* getPlanoAtual() const;

        Treino* getTreinoDesignado() const;

        Matricula getMatricula() const;

        int getCodigoMatricula() const;

        bool ValidarEntrada() const override;

        void restaurarMatricula(int codigo, const std::string& data);
        std::string getDataInicio() const;

        // void consultarCliente(std::string cpf);

        friend std::ostream& operator<<(std::ostream& os, const Cliente& cliente);
        friend std::istream& operator>>(std::istream& is, Cliente& cliente);
};

#endif
