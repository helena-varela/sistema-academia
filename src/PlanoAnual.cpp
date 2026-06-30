#include "PlanoAnual.hpp"
#include "AcademiaException.hpp"
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
  throw AcademiaException("Hora invalida!");
 }
 
  std::cout << "Plano Anual. Acesso liberado!";
  return true;
}

std::string PlanoAnual::getTipoPlano() const
{
  return "Plano Anual";
}