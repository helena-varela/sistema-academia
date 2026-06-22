#ifndef GERENCIADORACADEMIA_HPP
#define GERENCIADORACADEMIA_HPP

#include <string>
#include <vector>
#include "Instrutor.hpp"
#include "Cliente.hpp"
#include "AcademiaException.hpp"

class GerenciadorAcademia
{
    private:
        std::vector<Cliente*> clientes;
        std::vector<Instrutor*> instrutores;
    public:
        void cadastrarCliente(Cliente* c);
        Cliente* consultarCliente(const std::string& cpf);
        void atualizarCliente(const std::string& cpf, Cliente* novoC);
        void removerCliente(const std::string& cpf);
        void salvarEmArquivo(std::string nomeArquivo);
        void carregarDeArquivo(std::string nomeArquivo);

};

#endif