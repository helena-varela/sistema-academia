#ifndef PAGAMENTO_HPP
#define PAGAMENTO_HPP

#include <string>
#include "Cliente.hpp"
#include "Excecoes.hpp"

enum class StatusPagamento
{
    PENDENTE,    // aguardando confirmacao (ex: autorizacao do cartao)
    CONFIRMADO,  // pagamento efetivado
    CANCELADO,   // pendente que foi cancelado antes de confirmar (estado final)
    ESTORNADO    // confirmado que foi devolvido ao cliente (estado final)
};

class Pagamento
{
    private:
        static int ultimoCodigo;

        void alterarStatus(StatusPagamento novoStatus); // aplica as regras de transicao

    protected:
        int codigo;
        Cliente* cliente;
        std::string mesReferencia;   // MM/AAAA
        std::string dataPagamento;   // DD/MM/AAAA
        double valorBase;            // mensalidade do plano no momento do pagamento
        StatusPagamento status;

        // regras especificas de cada forma de pagamento (Template Method)
        virtual void validarEspecifico() const = 0;

    public:
        static const int DIA_VENCIMENTO = 10;

        Pagamento(Cliente* cliente, const std::string& mesReferencia, const std::string& dataPagamento);
        virtual ~Pagamento();

        // comportamentos polimorficos
        virtual double calcularValorFinal() const = 0;
        virtual bool confirmacaoImediata() const = 0;
        virtual std::string getTipo() const = 0;
        virtual std::string getDescricao() const = 0;
        virtual std::string serializarDetalhes() const = 0;

        void validar() const;     // regras comuns + validarEspecifico()
        void confirmar();
        void cancelar();
        void estornar();

        int getCodigo() const;
        Cliente* getCliente() const;
        std::string getMesReferencia() const;
        std::string getDataPagamento() const;
        double getValorBase() const;
        StatusPagamento getStatus() const;
        std::string getStatusTexto() const;

        void reassociarCliente(Cliente* novoCliente);
        void gerarCodigo();
        void restaurar(int codigo, double valorBase, StatusPagamento status);

        static std::string statusParaTexto(StatusPagamento status);
        static StatusPagamento textoParaStatus(const std::string& texto);
};

#endif
