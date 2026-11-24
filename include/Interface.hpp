  #ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "GerenciadorAcademia.hpp"

void interfaceCliente(GerenciadorAcademia& gerenciador, Cliente& cliente);
void interfaceInstrutor(GerenciadorAcademia& gerenciador, Instrutor* instrutor);
void interfaceGerenciador(GerenciadorAcademia& gerenciador);

#endif