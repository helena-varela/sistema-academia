#ifndef GERENCIADORACADEMIA_HPP
#define GERENCIADORACADEMIA_HPP

#include <string>
#include <vector>
#include "Instrutor.hpp"
#include "Cliente.hpp"
#include "Plano.hpp"
#include "PlanoMensal.hpp"
#include "PlanoAnual.hpp"
#include "AcademiaException.hpp"
#include "Treino.hpp"

class GerenciadorAcademia
{
    private:
        std::vector<Cliente*> clientes;
        std::vector<Instrutor*> instrutores;
        std::vector<Plano*> planosCadastrados;
        std::vector<Treino*> treinosCadastrados;
    public:
        GerenciadorAcademia();
        ~GerenciadorAcademia();

        void cadastrarCliente(Cliente* c);
        Cliente* consultarCliente(const std::string& cpf);
        Cliente* consultarClienteBase(int codigoMatricula);
        void atualizarCliente(const std::string& cpf, Cliente* novoC);
        void removerCliente(const std::string& cpf);

        void cadastrarInstrutor(Instrutor* i);
        void removerInstrutor(const std::string& cpf);
        Instrutor* consultarInstrutor(const std::string& cref);
        Instrutor* consultarInstrutorPorCpf(const std::string& cpf);
        void cadastrarTreino(Treino* t);
        
        // salvar e carregar arquivos
        void salvarEmArquivo(std::string nomeArquivo);
        void carregarDeArquivo(std::string nomeArquivo);

        // funções de busca auxiliares
        Plano* buscarPlano(const std::string& id);
        Treino* buscarTreino(const std::string& id);
        Instrutor* buscarInstrutor(const std::string& cpf);
};

#endif