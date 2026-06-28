#include "GerenciadorAcademia.hpp"
#include <fstream>

void GerenciadorAcademia::cadastrarCliente(Cliente* c)
{
  for(Cliente* clienteAtual : clientes){
    if(c->getCpf() == clienteAtual->getCpf()) {
      throw AcademiaException("Erro: Cliente com este CPF já cadastrado.");
    }
  } 
  clientes.push_back(c);
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

void GerenciadorAcademia::atualizarCliente(const std::string& cpf, Cliente* novoC)
{
  for(Cliente*& clienteAtual : clientes) {
    if(clienteAtual->getCpf() == cpf) {
      clienteAtual = novoC;
      return;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

void GerenciadorAcademia::removerCliente(const std::string& cpf)
{
  for(int i = 0; i < clientes.size();i++) {
    if(clientes[i]->getCpf() == cpf){
      delete clientes[i];
      clientes.erase(clientes.begin() + i);
      return;
    }
  }
  throw AcademiaException("Erro: Cliente não existe na base de dados!");
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
