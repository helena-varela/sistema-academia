#ifndef PLANO_HPP
#define PLANO_HPP
#include <string>

class Plano 
{
    protected:
        double precoBase;
    public:
        Plano(double precoBase);
        virtual double calcularMensalidade() = 0;
        virtual bool aplicarRestricaoAcesso(int horaAcesso) = 0;
        virtual std::string getTipoPlano() const = 0;
        virtual Plano* clone() const = 0;
        virtual ~Plano();
};

#endif