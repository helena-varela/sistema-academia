#include <iostream>
#include "Matricula.hpp"

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
        throw TransicaoEstadoInvalidaException("Matricula", statusParaTexto(statusAtual), statusParaTexto(novoEstado));
    }

    statusAtual = novoEstado;
}

bool Matricula::validarTransicao(StatusMatricula atual, StatusMatricula novoEstado) //Função validadora
{
    if (atual == novoEstado) //Não é permitido o mesmo estado
    {
        throw TransicaoEstadoInvalidaException("Matricula", statusParaTexto(atual), statusParaTexto(novoEstado),
                                               "Esse ja e o estado atual da matricula.");
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
    throw TransicaoEstadoInvalidaException("Matricula", statusParaTexto(atual), statusParaTexto(novoEstado));
}

StatusMatricula Matricula::getStatus() const //getter de status
{
    return statusAtual;
}

int Matricula::getCodigoMatricula() const //getter do código ID
{
    return codigoMatricula;
}

void Matricula::setCodigoEData(int codigo, const std::string& data, StatusMatricula status){
    codigoMatricula = codigo;
    dataInicio = data;
    statusAtual = status;
    if(codigo > ultimoCodigo){
        ultimoCodigo = codigo;
    }
}

std::string Matricula::getDataInicio() const {
    return dataInicio;
}

std::string Matricula::statusParaTexto(StatusMatricula status)
{
    switch (status)
    {
        case StatusMatricula::ATIVA:        return "ATIVA";
        case StatusMatricula::INADIMPLENTE: return "INADIMPLENTE";
        case StatusMatricula::TRANCADA:     return "TRANCADA";
    }
    return "DESCONHECIDO";
}
