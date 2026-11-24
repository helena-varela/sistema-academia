#ifndef PAGAMENTOCARTAO_HPP
#define PAGAMENTOCARTAO_HPP

#include "Pagamento.hpp"

class PagamentoCartao : public Pagamento
{
    private:
        int numeroParcelas;
        std::string ultimos4Digitos;

    protected:
        void validarEspecifico() const override;

    public:
        PagamentoCartao(Cliente* cliente,
                        const std::string& mesReferencia,
                        const std::string& dataPagamento,
                        int numeroParcelas,
                        const std::string& ultimos4Digitos);

        double calcularValorFinal() const override;
        bool confirmacaoImediata() const override;
        std::string getTipo() const override;
        std::string getDescricao() const override;
        std::string serializarDetalhes() const override;

        double getValorParcela() const;
        int getNumeroParcelas() const;
        std::string getUltimos4Digitos() const;
};

#endif
