#include <iostream>
#include "../include/Matricula.hpp"
using namespace std;

Matricula::Matricula(std::string codigoMatricula, std::string dataInicio)
{
    this->codigoMatricula = codigoMatricula;
    this->dataInicio = dataInicio;
    statusAtual = statusMatricula::ATIVA;
}

void Matricula::alterarEstado(statusMatricula novoEstado)
{
    statusAtual = novoEstado;
}

statusMatricula Matricula::getStatus()
{
    return statusAtual;
}