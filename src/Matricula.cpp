#include <iostream>
#include "../include/Matricula.hpp"

Matricula::Matricula(std::string codigoMatricula, std::string dataInicio)
{
    this->codigoMatricula = codigoMatricula;
    this->dataInicio = dataInicio;
    statusAtual = statusMatricula::ATIVA;
}

void Matricula::alterarEstado(statusMatricula novoEstado)
{
    if (!validarTransicao(statusAtual, novoEstado))
    {
        throw AcademiaException("Transicao de estado invalida.");
    }

    statusAtual = novoEstado;
}

bool Matricula::validarTransicao(statusMatricula atual, statusMatricula novoEstado)
{
    if (atual == statusMatricula::INADIMPLENTE && novoEstado == statusMatricula::TRANCADA)
    {
        return false;
    }

    return true;
}

statusMatricula Matricula::getStatus()
{
    return statusAtual;
}