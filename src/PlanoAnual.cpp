#include "PlanoAnual.hpp"
#include "Excecoes.hpp"
#include <iostream>

PlanoAnual::PlanoAnual(double preco, int meses, double desconto) 
   : Plano(preco), mesesFidelidade(meses), descontoFidelidade(desconto) {}

double PlanoAnual::calcularMensalidade() {
  double valor = precoBase - descontoFidelidade;
  if (valor < 0) return 0.0;
  return valor;
}

bool PlanoAnual::aplicarRestricaoAcesso(int horaAcesso) {
 if(horaAcesso < 0 || horaAcesso > 23) {
  throw DadosInvalidosException("hora", "Hora invalida! Use um valor de 0 a 23.");
 }
  return true;
}

std::string PlanoAnual::getTipoPlano() const
{
  return "Plano Anual";
}

Plano* PlanoAnual::clone() const {
  return new PlanoAnual(precoBase, mesesFidelidade, descontoFidelidade);
}

// Regra: plano anual (fidelidade) permite parcelar a mensalidade em ate 3x
int PlanoAnual::getMaxParcelasCartao() const {
  return 3;
}
