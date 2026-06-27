#ifndef MATRICULA_HPP
#define MATRICULA_HPP

#include <string>
#include "AcademiaException.hpp"

enum class StatusMatricula
{
    ATIVA,
    INADIMPLENTE,
    TRANCADA
};

class Matricula 
{
    private:
        static int ultimoCodigo;
        int codigoMatricula;
        StatusMatricula statusAtual; //Aqui era pra ser um enum, suponho
        std::string dataInicio; //da pra formatar acho
    public:
        Matricula(std::string dataInicio);
        void alterarEstado(StatusMatricula novoEstado); //aplicar Enum class
        bool validarTransicao(StatusMatricula statusAtual, StatusMatricula novoEstado);
        int getCodigoMatricula() const;
        StatusMatricula getStatus() const;
};

#endif