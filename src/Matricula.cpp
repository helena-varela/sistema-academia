#include <iostream>
#include "../include/Matricula.hpp"

int Matricula::ultimoCodigo = 0;

Matricula::Matricula(std::string dataInicio)
{
    codigoMatricula = ++ultimoCodigo;
    this->dataInicio = dataInicio;
    statusAtual = StatusMatricula::ATIVA;
}

void Matricula::alterarEstado(StatusMatricula novoEstado)
{
    if (!validarTransicao(statusAtual, novoEstado))
    {
        throw AcademiaException("Transicao de estado invalida.");
    }

    statusAtual = novoEstado;
}

bool Matricula::validarTransicao(StatusMatricula atual, StatusMatricula novoEstado)
{
    if (atual == StatusMatricula::INADIMPLENTE && novoEstado == StatusMatricula::TRANCADA)
    {
        return false;
    }

    return true;
}

StatusMatricula Matricula::getStatus() const
{
    return statusAtual;
}

int Matricula::getCodigoMatricula() const
{
    return codigoMatricula;
}