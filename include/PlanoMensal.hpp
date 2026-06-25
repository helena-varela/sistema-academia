#ifndef PLANOMENSAL_HPP
#define PLANOMENSAL_HPP

#include "Plano.hpp"

class PlanoMensal : public Plano
{
    private:
        double taxaInscricaoAvulsa;
    public:
        PlanoMensal(double precoBase, double taxaInscricaoAvulsa);
        double calcularMensalidade() override;
        bool aplicarRestricaoAcesso(int horaAcesso) override;
};

#endif