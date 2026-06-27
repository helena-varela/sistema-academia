#ifndef PLANO_HPP
#define PLANO_HPP

class Plano 
{
    protected:
        double precoBase;
    public:
        Plano(double precoBase);
        virtual double calcularMensalidade() = 0;
        virtual bool aplicarRestricaoAcesso(int horaAcesso) = 0;
        virtual ~Plano();
};

#endif