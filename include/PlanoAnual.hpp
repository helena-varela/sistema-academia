#ifndef PLANOANUAL_HPP
#define PLANOANUAL_HPP

#include "Plano.hpp"

class PlanoAnual : public Plano
{
    private:
        int mesesFidelidade;
        double descontoFidelidade;
    public:
        PlanoAnual(double precoBase, int mesesFidelidade, double descontoFidelidade);
        double calcularMensalidade() override;
        bool aplicarRestricaoAcesso(int horaAcesso) override;
        std::string getTipoPlano() const override;
        Plano* clone() const override;
};

#endif