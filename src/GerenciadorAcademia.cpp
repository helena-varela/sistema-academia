#include "GerenciadorAcademia.hpp"
#include <fstream>
#include <sstream>
#include <string>

GerenciadorAcademia::GerenciadorAcademia()
{
  planosCadastrados.push_back(new PlanoMensal(100.0, 15.0));
  planosCadastrados.push_back(new PlanoAnual(80.0, 12, 0.1));

  std::list<std::string> exerciciosPadrao = {"Supino Reto", "Agachamento Livre", "Corrida"};
  treinosCadastrados.push_back(new Treino("Treino Geral", exerciciosPadrao, 50));
}

void GerenciadorAcademia::cadastrarCliente(Cliente *c)
{
  for (Cliente *clienteAtual : clientes)
  {
    if (c->getCpf() == clienteAtual->getCpf())
    {
      throw AcademiaException("Erro: Cliente com este CPF já cadastrado.");
    }
  }
  clientes.push_back(c);
  return;
}

Cliente *GerenciadorAcademia::consultarCliente(const std::string &cpf)
{
  for (Cliente *clienteAtual : clientes)
  {
    if (clienteAtual->getCpf() == cpf)
    {
      return clienteAtual;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

Cliente *GerenciadorAcademia::consultarClienteBase(const int &codigoMatricula)
{
  for (Cliente *clienteAtual : clientes)
  {
    if (clienteAtual->getCodigoMatricula() == codigoMatricula)
    {
      return clienteAtual;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

void GerenciadorAcademia::atualizarCliente(const std::string &cpf, Cliente *novoC)
{
  for (Cliente *&clienteAtual : clientes)
  {
    if (clienteAtual->getCpf() == cpf)
    {
      clienteAtual = novoC;
      return;
    }
  }
  throw AcademiaException("Erro: Cliente não cadastrado!");
}

void GerenciadorAcademia::removerCliente(const std::string &cpf)
{
  for (size_t i = 0; i < clientes.size(); i++)
  {
    if (clientes[i]->getCpf() == cpf)
    {
      delete clientes[i];
      clientes.erase(clientes.begin() + i);
      return;
    }
  }
  throw AcademiaException("Erro: Cliente não existe na base de dados!");
}

// métodos de busca
Plano *GerenciadorAcademia::buscarPlano(const std::string &id)
{
  for (auto *plano : planosCadastrados)
  {
    if (plano->getTipoPlano() == id)
      return plano;
  }
  return nullptr;
}

Instrutor *GerenciadorAcademia::descobrirInstrutorDoCliente(Cliente *c)
{
  for (auto *instrutor : instrutores)
  {
    if (instrutor->supervisionaAluno(c))
    {
      return instrutor;
    }
  }

  return nullptr;
}

Treino *GerenciadorAcademia::buscarTreino(const std::string &id)
{
  for (auto *treino : treinosCadastrados)
  {
    if (treino->getFoco() == id)
      return treino;
  }
  return nullptr;
}

Instrutor *GerenciadorAcademia::buscarInstrutor(const std::string &cpf)
{
  for (auto *instrutor : instrutores)
  {
    if (instrutor->getCpf() == cpf)
      return instrutor;
  }
  return nullptr;
}

// parte de arquivos
void GerenciadorAcademia::salvarEmArquivo(std::string nomeArquivo)
{
  std::ofstream arquivo(nomeArquivo);

  if (!arquivo.is_open())
  {
    throw AcademiaException("Erro: Nao foi possivel abrir o arquivo para salvar.");
  }

  for (Plano *p : planosCadastrados)
  {
    if (p)
    {
      arquivo << "PLANO;" << p->getTipoPlano() << ";" << p->calcularMensalidade() << "\n";
    }
  }

  for (Treino *t : treinosCadastrados)
  {
    if (t)
    {
      arquivo << "TREINO;" << t->getFoco() << ";" << t->getDuracao() << ";";
      std::string exStr = "";
      for (const auto &ex : t->getExercicios())
      {
        exStr += ex + "|";
      }
      if (!exStr.empty())
        exStr.pop_back();
      arquivo << exStr << "\n";
    }
  }

  for (Instrutor *i : instrutores)
  {
    if (i)
    {
      arquivo << "INSTRUTOR;" << i->getCpf() << ";" << i->getNome() << ";"
              << i->getEmail() << ";" << i->getCref() << ";" << i->getEspecialidade() << "\n";
    }
  }

  for (Cliente *c : clientes)
  {
    if (c)
    {
      std::string planoId = c->getPlanoAtual() ? c->getPlanoAtual()->getTipoPlano() : "NULL";
      std::string treinoId = c->getTreinoDesignado() ? c->getTreinoDesignado()->getFoco() : "NULL";

      std::string instrutorCpf = "NULL";
      for (Instrutor *inst : instrutores)
      {
        if (inst && inst->supervisionaAluno(c))
        {
          instrutorCpf = inst->getCpf();
          break;
        }
      }

      arquivo << "CLIENTE;1;" << c->getNome() << ";" << c->getCpf() << ";"
              << c->getEmail() << ";" << planoId << ";" << treinoId << ";" << instrutorCpf << "\n";
    }
  }

  arquivo.close();

}


void GerenciadorAcademia::carregarDeArquivo(std::string nomeArquivo)
{
  std::ifstream arquivo(nomeArquivo);

  if (!arquivo.is_open())
  {
    throw AcademiaException("Erro: Não foi possivel abrir o arquivo para leitura.");
  }

  // limpa a memoria
  for (Cliente *c : clientes)
    delete c;
  clientes.clear();
  for (Instrutor *i : instrutores)
    delete i;
  instrutores.clear();
  for (Treino *t : treinosCadastrados)
    delete t;
  treinosCadastrados.clear();
  for (Plano *p : planosCadastrados)
    delete p;
  planosCadastrados.clear();

  std::string linha;
  // leitura do arquivo
  while (std::getline(arquivo, linha))
  {
    if (linha.empty())
      continue;

    std::stringstream ss(linha);
    std::string tipo;
    std::getline(ss, tipo, ';');

    if (tipo == "PLANO")
    {
      std::string tipoPlano, precoStr;
      std::getline(ss, tipoPlano, ';');
      std::getline(ss, precoStr);
      double preco = std::stod(precoStr);

      if (tipoPlano == "MENSAL" || tipoPlano == "Plano Mensal")
      {
        planosCadastrados.push_back(new PlanoMensal(preco, 15.0));
      }
      else if (tipoPlano == "ANUAL" || tipoPlano == "Plano Anual")
      {
        planosCadastrados.push_back(new PlanoAnual(preco, 12, 0.1));
      }
    }
    else if (tipo == "TREINO")
    {
      std::string idFoco, duracaoStr, exerciciosAgrupados;
      std::getline(ss, idFoco, ';');
      std::getline(ss, duracaoStr, ';');
      std::getline(ss, exerciciosAgrupados);

      std::list<std::string> listaExercicios;
      std::stringstream ssEx(exerciciosAgrupados);
      std::string ex;
      while (std::getline(ssEx, ex, '|'))
      {
        if (!ex.empty())
          listaExercicios.push_back(ex);
      }

      treinosCadastrados.push_back(new Treino(idFoco, listaExercicios, std::stoi(duracaoStr)));
    }
    else if (tipo == "INSTRUTOR")
    {
      std::string cpf, nome, email, cref, esp;
      std::getline(ss, cpf, ';');
      std::getline(ss, nome, ';');
      std::getline(ss, email, ';');
      std::getline(ss, cref, ';');
      std::getline(ss, esp);

      instrutores.push_back(new Instrutor(nome, cpf, email, cref, esp));
    }
    else if (tipo == "CLIENTE")
    {
      std::string matStr, nome, cpf, email, planoId, treinoId, instrutorCpf;
      std::getline(ss, matStr, ';');
      std::getline(ss, nome, ';');
      std::getline(ss, cpf, ';');
      std::getline(ss, email, ';');
      std::getline(ss, planoId, ';');
      std::getline(ss, treinoId, ';');
      std::getline(ss, instrutorCpf);

      Plano *p = buscarPlano(planoId);
      Treino *t = buscarTreino(treinoId);
      Instrutor *inst = buscarInstrutor(instrutorCpf);

      Cliente *novoCliente = new Cliente(nome, cpf, email, p, "01/01/2026");

      if (t)
        novoCliente->associarTreino(t);

      if (inst)
      {
        inst->adicionarAlunoSupervisionado(novoCliente);
      }

      clientes.push_back(novoCliente);
    }
  }
  arquivo.close();
}
