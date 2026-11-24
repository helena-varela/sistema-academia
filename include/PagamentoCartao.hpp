#ifndef PAGAMENTOCARTAO_HPP
#define PAGAMENTOCARTAO_HPP

#include "Pagamento.hpp"

class PagamentoCartao : public Pagamento
{
    private:
        int numeroParcelas;
        double taxaJurosParcela;
        std::string ultimos4Digitos;

    protected:
        void validarEspecifico() const override;

    public:
        static constexpr double TAXA_JUROS_PADRAO = 0.025;  // 2,5% por parcela acima do limite
        static const int PARCELAS_SEM_JUROS = 2;
        static constexpr double VALOR_MINIMO_PARCELA = 30.0;
        static const int PRAZO_ESTORNO_DIAS = 7;

        PagamentoCartao(Cliente* cliente,
                        const std::string& mesReferencia,
                        const std::string& dataPagamento,
                        int numeroParcelas,
                        const std::string& ultimos4Digitos,
                        double taxaJurosParcela = TAXA_JUROS_PADRAO);

        double calcularValorFinal() const override;
        bool confirmacaoImediata() const override;
        bool podeEstornar(const std::string& dataAtual) const override;
        std::string getTipo() const override;
        std::string getDescricao() const override;
        std::string serializarDetalhes() const override;

        double getValorParcela() const;
        int getNumeroParcelas() const;
        std::string getUltimos4Digitos() const;
};

#endif
