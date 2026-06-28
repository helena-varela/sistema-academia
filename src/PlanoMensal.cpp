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
    throw AcademiaException("Horário de pico. Acesso bloqueado!");
  }
  return true;
}
