#include "PlanoAnual.hpp"
#include <iostream>

PlanoAnual::PlanoAnual(double preco, int meses, double desconto) 
   : Plano(preco), mesesFidelidade(meses), descontoFidelidade(desconto) {}

double PlanoAnual::calcularMensalidade() {
  double valor;

  valor = precoBase - descontoFidelidade;

  return valor;
}

bool PlanoAnual::aplicarRestricaoAcesso(int horaAcesso) {
  std::cout << "Plano Anual. Acesso liberado!";
  return true;
}

std::string PlanoAnual::getTipoPlano() const
{
  return "Plano Anual";
}