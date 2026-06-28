#include <iostream>
#include "../include/Matricula.hpp"

int Matricula::ultimoCodigo = 0; //Códido ID base

Matricula::Matricula(std::string dataInicio) //Construtor da matrícula
{
    codigoMatricula = ++ultimoCodigo; //Iteração do código base
    this->dataInicio = dataInicio;
    statusAtual = StatusMatricula::ATIVA;
}

void Matricula::alterarEstado(StatusMatricula novoEstado) //Função de Alteração
{
    if (!validarTransicao(statusAtual, novoEstado)) //Chama validação: se true, permite, se false, lança exceção
    {
        throw AcademiaException("Transicao de estado invalida.");
    }

    statusAtual = novoEstado;
}

bool Matricula::validarTransicao(StatusMatricula atual, StatusMatricula novoEstado) //Função validadora
{
    if (atual == novoEstado) //Não é permitido o mesmo estado
    {
        throw AcademiaException("Esse já é o estado atual da Matrícula");
    }
    switch (atual)
    {
        case StatusMatricula::ATIVA: 
            if (novoEstado == StatusMatricula::INADIMPLENTE || novoEstado == StatusMatricula::TRANCADA) //Se ativa, pode ser alterada para inadimplente ou trancada
            {
                return true;
            } 
            break;
        case StatusMatricula::INADIMPLENTE:
            if (novoEstado == StatusMatricula::ATIVA) //Se inadimplente, só pode ir para ativa, não trancada
            {
                return true;
            } 
            break;
        case StatusMatricula::TRANCADA:
            if (novoEstado == StatusMatricula::ATIVA) //Se trancada, só pode ir para ativa, não inadimplente
            {
                return true;
            } 
            break;
    }
    throw AcademiaException("Transição de estado inválida.");
}

StatusMatricula Matricula::getStatus() const //getter de status
{
    return statusAtual;
}

int Matricula::getCodigoMatricula() const //getter do código ID
{
    return codigoMatricula;
}