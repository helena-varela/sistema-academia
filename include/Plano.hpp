#ifndef PLANO_HPP
#define PLANO_HPP

#include <string>

class Plano 
{
    protected:
        double precoBase;
    public:
        Plano(double precoBase);
        virtual double calcularmensalidade() = 0;
        virtual bool aplicarRestricaoAcesso(std::string horaAcesso) = 0;
        virtual ~Plano();
};

#endif