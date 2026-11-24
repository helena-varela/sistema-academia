#ifndef PAGAMENTOPIX_HPP
#define PAGAMENTOPIX_HPP

#include "Pagamento.hpp"

class PagamentoPix : public Pagamento
{
    private:
        std::string chavePix;
        double percentualDesconto;

    protected:
        void validarEspecifico() const override;

    public:
        static constexpr double DESCONTO_PADRAO = 0.05; // 5% de desconto no Pix

        PagamentoPix(Cliente* cliente,
                     const std::string& mesReferencia,
                     const std::string& dataPagamento,
                     const std::string& chavePix,
                     double percentualDesconto = DESCONTO_PADRAO);

        double calcularValorFinal() const override;
        bool confirmacaoImediata() const override;
        std::string getTipo() const override;
        std::string getDescricao() const override;
        std::string serializarDetalhes() const override;

        std::string getChavePix() const;
        double getPercentualDesconto() const;
};

#endif
