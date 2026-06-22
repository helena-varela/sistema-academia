#ifndef MATRICULA_HPP
#define MATRICULA_HPP

#include <string>

class Matricula 
{
    private:
        std::string codigoMatricula;
        std::string statusAtual; //Aqui era pra ser um enum, suponho
        std::string dataInicio; //da pra formatar acho
    public:
        Matricula(std::string codigoMatricula, std::string dataInicio);
        void alterarEstado(std::string novoEstado); //aplicar Enum class
        bool validarTransicao(std::string statusAtual, std::string novoEstado);
        std::string getStatus();
};

#endif