#include "PlanoMensal.hpp"

PlanoMensal::PlanoMensal(double preco, double taxa) 
  : Plano(preco), taxaInscricaoAvulsa(taxa) {}

double PlanoMensal::calcularMensalidade() {
  return 0.0;
}

bool PlanoMensal::aplicarRestricaoAcesso(std::string horaAcesso) {
  return true;
}
