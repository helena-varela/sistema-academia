#ifndef PESSOA_HPP
#define PESSOA_HPP

#include <string>

class Pessoa {
    protected:
        std::string nome;
        std::string cpf;
        std::string email;
    public:
        Pessoa(std::string nome, std::string cpf, std::string email);
        std::string getNome();
        std::string getCpf();
        std::string getEmail();
        virtual ~Pessoa() = default;
};

#endif 