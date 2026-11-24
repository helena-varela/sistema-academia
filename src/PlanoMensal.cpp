#include "PlanoMensal.hpp"
#include <iostream>

PlanoMensal::PlanoMensal(double preco, double taxa) 
  : Plano(preco), taxaInscricaoAvulsa(taxa) {}

double PlanoMensal::calcularMensalidade() {
  double valor;

  valor = precoBase + taxaInscricaoAvulsa;

  return valor;
} 

bool PlanoMensal::aplicarRestricaoAcesso(int horaAcesso) {
  if(horaAcesso < 0 || horaAcesso > 23) {
    throw DadosInvalidosException("hora", "Hora invalida! Use um valor de 0 a 23.");
  }
  if(horaAcesso >= 18 && horaAcesso <= 20) 
  {
    throw AcessoNegadoException("treino", "Horario de pico (18h as 20h): acesso bloqueado para o Plano Mensal.");
  }
  return true;
}

std::string PlanoMensal::getTipoPlano() const
{
  return "Plano Mensal";
}

Plano* PlanoMensal::clone() const {
  return new PlanoMensal(precoBase, taxaInscricaoAvulsa);
}

// Regra: plano mensal so pode ser pago no cartao a vista
int PlanoMensal::getMaxParcelasCartao() const {
  return 1;
}
