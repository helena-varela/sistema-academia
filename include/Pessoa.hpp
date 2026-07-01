#ifndef PESSOA_HPP
#define PESSOA_HPP

#include <string>
#include "AcademiaException.hpp"

class Pessoa {
    protected:
        std::string nome;
        std::string cpf;
        std::string email;

        static bool validarCPF(const std::string& cpf);

    public:
        Pessoa(const std::string& nome,
           const std::string& cpf,
           const std::string& email);

        std::string getNome() const;
        std::string getCpf() const;
        std::string getEmail() const;

        virtual bool ValidarEntrada() const;

        virtual ~Pessoa() = 0;
};

#endif 
