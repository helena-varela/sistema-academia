#include "PlanoAnual.hpp"

PlanoAnual::PlanoAnual(double preco, int meses, double desconto) 
  : Plano(preco), mesesFidelidade(meses), descontoFidelidade(desconto) {}

double PlanoAnual::calcularMensalidade() {
  return 0.0;
}

bool PlanoAnual::aplicarRestricaoAcesso(std::string horaAcesso) {
  return true;
}