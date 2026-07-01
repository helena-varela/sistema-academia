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
  if(horaAcesso >= 18 && horaAcesso <= 20) 
  {
    throw AcademiaException("Horario de pico. Acesso bloqueado!");
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