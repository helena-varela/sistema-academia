#ifndef PLANOANUAL_HPP
#define PLANOANUAL_HPP

#include "Plano.hpp"
#include <string>

class PlanoAnual : public Plano
{
    private:
        int mesesFidelidade;
        double descontoFidelidade;
    public:
        PlanoAnual(double precoBase, int mesesFidelidade, double descontoFidelidade);
        double calcularMensalidade() override;
        bool aplicarRestricaoAcesso(std::string horaAcesso) override;
};

#endif