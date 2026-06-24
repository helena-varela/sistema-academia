#ifndef MATRICULA_HPP
#define MATRICULA_HPP

#include <string>

enum class statusMatricula
{
    ATIVA,
    INADIMPLENTE,
    TRANCADA
};

class Matricula 
{
    private:
        std::string codigoMatricula;
        statusMatricula statusAtual; //Aqui era pra ser um enum, suponho
        std::string dataInicio; //da pra formatar acho
    public:
        Matricula(std::string codigoMatricula, std::string dataInicio);
        void alterarEstado(statusMatricula novoEstado); //aplicar Enum class
        bool validarTransicao(statusMatricula statusAtual, statusMatricula novoEstado);
        statusMatricula getStatus();
};

#endif