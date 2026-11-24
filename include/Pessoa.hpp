#ifndef PESSOA_HPP
#define PESSOA_HPP

#include <string>
#include "Excecoes.hpp"

class Pessoa {
    private:
        std::string senhaHash; // a senha nunca e guardada em texto puro

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

        // autenticacao
        void definirSenha(const std::string& senha);
        void alterarSenha(const std::string& senhaAtual, const std::string& novaSenha);
        bool autenticar(const std::string& senha) const;
        std::string getSenhaHash() const;
        void restaurarSenhaHash(const std::string& hash);
        static std::string gerarHash(const std::string& texto);

        virtual ~Pessoa() = 0;
};

#endif 
