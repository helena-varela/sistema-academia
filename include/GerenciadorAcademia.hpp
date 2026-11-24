#ifndef GERENCIADORACADEMIA_HPP
#define GERENCIADORACADEMIA_HPP

#include <string>
#include <vector>
#include "Instrutor.hpp"
#include "Cliente.hpp"
#include "Plano.hpp"
#include "PlanoMensal.hpp"
#include "PlanoAnual.hpp"
#include "Excecoes.hpp"
#include "Treino.hpp"
#include "Pagamento.hpp"
#include "PagamentoPix.hpp"
#include "PagamentoCartao.hpp"

inline const std::string ARQUIVO_BACKUP = "data/backup.txt";

class GerenciadorAcademia
{
    private:
        std::vector<Cliente*> clientes;
        std::vector<Instrutor*> instrutores;
        std::vector<Plano*> planosCadastrados;
        std::vector<Treino*> treinosCadastrados;
        std::vector<Pagamento*> pagamentos;
        std::string senhaGerenteHash;

        bool existePagamentoAtivo(Cliente* cliente, const std::string& mesReferencia) const;
        bool existePagamentoConfirmado(Cliente* cliente, const std::string& mesReferencia) const;
        void removerPagamentosDoCliente(Cliente* cliente);
        void reavaliarSituacaoFinanceira(Cliente* cliente, const std::string& dataAtual);
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
        
        // autenticacao
        static constexpr const char* SENHA_GERENTE_PADRAO = "admin123";
        Cliente* autenticarCliente(const std::string& cpf, const std::string& senha);
        Instrutor* autenticarInstrutor(const std::string& cref, const std::string& senha);
        void autenticarGerente(const std::string& senha) const;
        void alterarSenhaGerente(const std::string& senhaAtual, const std::string& novaSenha);

        // pagamentos
        Pagamento* registrarPagamento(Pagamento* p);
        Pagamento* consultarPagamento(int codigo) const;
        void confirmarPagamento(int codigo);
        void cancelarPagamento(int codigo);
        void estornarPagamento(int codigo);
        std::vector<Pagamento*> historicoPagamentos(const std::string& cpf) const;
        bool clienteEmDia(Cliente* cliente, const std::string& dataAtual) const;
        std::vector<Cliente*> atualizarInadimplencia(const std::string& dataAtual);
        std::vector<Cliente*> listarInadimplentes() const;

        // salvar e carregar arquivos
        void salvarEmArquivo(std::string nomeArquivo);
        void carregarDeArquivo(std::string nomeArquivo);

        // funções de busca auxiliares
        Plano* buscarPlano(const std::string& id);
        Plano* criarPlanoDoCatalogo(const std::string& tipo); // "Mensal" ou "Anual" -> copia do catalogo
        Treino* buscarTreino(const std::string& id);
        Instrutor* buscarInstrutor(const std::string& cpf);
};

#endif