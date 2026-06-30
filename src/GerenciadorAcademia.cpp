#include "GerenciadorAcademia.hpp"
#include <fstream>
#include <iostream>

GerenciadorAcademia::~GerenciadorAcademia()
{
  for(Cliente* c : clientes){
    delete c;
  }

  for(Instrutor* i : instrutores){
    delete i;
  }
  clientes.clear();
  instrutores.clear();
}

void GerenciadorAcademia::cadastrarCliente(Cliente* c)
{
  for(Cliente* clienteAtual : clientes){
    if(c->getCpf() == clienteAtual->getCpf()) {
      throw AcademiaException("Erro: Cliente com este CPF já cadastrado.");
    }
  } 
  clientes.push_back(c);
  std::cout << "Cliente cadastrado com sucesso!" << std::endl;
  return;
}

Cliente* GerenciadorAcademia::consultarCliente(const std::string& cpf)
{
  for(Cliente* clienteAtual : clientes) {
    if(clienteAtual->getCpf() == cpf){
      return clienteAtual;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

Cliente* GerenciadorAcademia::consultarClienteBase(int codigoMatricula)
{
  for(Cliente* clienteAtual : clientes) {
    if(clienteAtual->getCodigoMatricula() == codigoMatricula){
      return clienteAtual;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

void GerenciadorAcademia::atualizarCliente(const std::string& cpf, Cliente* novoC)
{
  for(Cliente*& clienteAtual : clientes) {
    if(clienteAtual->getCpf() == cpf) {
      delete clienteAtual;
      clienteAtual = novoC;
      std::cout << "Cliente atualizado com sucesso!" << std::endl;
      return;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

void GerenciadorAcademia::removerCliente(const std::string& cpf)
{
  for(size_t i = 0; i < clientes.size();i++) {
    if(clientes[i]->getCpf() == cpf){
      delete clientes[i];
      clientes.erase(clientes.begin() + i);
      std::cout << "Cliente removido com sucesso!" << std::endl;
      return;
    }
  }
  throw AcademiaException("Erro: Cliente não existe na base de dados!");
}

void GerenciadorAcademia::cadastrarInstrutor(Instrutor* i)
{
  for(Instrutor* instrutorAtual : instrutores){
    if(i->getCpf() == instrutorAtual->getCpf()) {
      throw AcademiaException("Erro: Instrutor com este CPF já cadastrado.");
    }
  } 
  instrutores.push_back(i);
  std::cout << "Instrutor cadastrado com sucesso!" << std::endl;
  return;
}

void GerenciadorAcademia::removerInstrutor(const std::string& cpf)
{
  for(size_t i = 0; i < instrutores.size();i++) {
    if(instrutores[i]->getCpf() == cpf){
      delete instrutores[i];
      instrutores.erase(instrutores.begin() + i);
      std::cout << "Instrutor removido com sucesso!" << std::endl;
      return;
    }
  }
  throw AcademiaException("Erro: Instrutor não existe na base de dados!");
}

Instrutor* GerenciadorAcademia::consultarInstrutor(const std::string& cref)
{
  for(Instrutor* instrutorAtual : instrutores) {
    if(instrutorAtual->getCref() == cref){
      return instrutorAtual;
    }
  }
  throw AcademiaException("Erro: Instrutor não cadastrado!");
}

void GerenciadorAcademia::salvarEmArquivo(std::string nomeArquivo)
{
  std::ofstream arquivo(nomeArquivo);

  if (!arquivo.is_open()){
    throw AcademiaException("Erro: Não foi possível abrir o arquivo para salvar.");
  }

  for(Cliente* clienteAtual : clientes) {
    arquivo << *clienteAtual << std::endl;
  }

  arquivo.close();
}


void GerenciadorAcademia::carregarDeArquivo(std::string nomeArquivo)
{
  std::ifstream arquivo(nomeArquivo);

  if(!arquivo.is_open()) {
    throw AcademiaException("Erro: Não foi possivel abrir o arquivo para leitura.");
  }

  for (Cliente* c : clientes) {
    delete c;
  }

  clientes.clear();

  std::string nome, cpf, email, codigoMatricula;

  while(std::getline(arquivo, nome, ';') &&
        std::getline(arquivo, cpf, ';') &&
        std::getline(arquivo, email, ';') &&
        std::getline(arquivo, codigoMatricula))
  {
    Cliente* novoCliente = new Cliente(nome, cpf, email, nullptr, "01/01/2026");
    clientes.push_back(novoCliente);
  }
  arquivo.close();
}
