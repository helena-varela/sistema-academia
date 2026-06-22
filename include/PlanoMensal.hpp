#ifndef PLANOMENSAL_HPP
#define PLANOMENSAL_HPP

#include "Plano.hpp"
#include <string>

class PlanoMensal : public Plano
{
    private:
        double taxaInscricaoAvulsa;
    public:
        PlanoMensal(double precoBase, double taxaInscricaoAvulsa);
        double calcularMensalidade() override;
        bool aplicarRestricaoAcesso(std::string horaAcesso) override;
};

#endif