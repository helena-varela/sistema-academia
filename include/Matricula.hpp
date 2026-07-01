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
        StatusMatricula statusAtual; 
        std::string dataInicio; 
    public:
        Matricula(std::string dataInicio);
        void alterarEstado(StatusMatricula novoEstado); 
        bool validarTransicao(StatusMatricula statusAtual, StatusMatricula novoEstado);
        int getCodigoMatricula() const;
        StatusMatricula getStatus() const;
        void setCodigoEData(int codigo, const std::string& data);
        std::string getDataInicio() const;
};

#endif