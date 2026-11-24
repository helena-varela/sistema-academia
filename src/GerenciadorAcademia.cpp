#include "GerenciadorAcademia.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "UtilData.hpp"
GerenciadorAcademia::GerenciadorAcademia()
{
  planosCadastrados.push_back(new PlanoMensal(100.0, 15.0));
  planosCadastrados.push_back(new PlanoAnual(80.0, 12, 0.1));

  std::list<std::string> exerciciosPadrao = {"Supino Reto", "Agachamento Livre", "Corrida"};
  treinosCadastrados.push_back(new Treino("Treino Geral", exerciciosPadrao, 50));
}

GerenciadorAcademia::~GerenciadorAcademia()
{
  for(auto p : pagamentos){
    delete p;
  }
  pagamentos.clear();
  for(auto c : clientes){
    delete c;
  }
  for(auto i : instrutores){
    delete i;
  }
  for(auto plano : planosCadastrados){
    delete plano;
  }
  for(auto treino : treinosCadastrados){
    delete treino;
  }
  clientes.clear();
  instrutores.clear();
  planosCadastrados.clear();
  treinosCadastrados.clear();
}

// CRUD cliente
void GerenciadorAcademia::cadastrarCliente(Cliente *c)
{
  for (Cliente *clienteAtual : clientes)
  {
    if (c->getCpf() == clienteAtual->getCpf())
    {
      throw AcademiaException("Erro: Cliente com este CPF ja cadastrado.");
    }
  }
  clientes.push_back(c);
  std::cout << "Cliente cadastrado com sucesso!" << std::endl;
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
  throw AcademiaException("Erro: Cliente nao cadastrado!");
}

Cliente* GerenciadorAcademia::consultarClienteBase(int codigoMatricula)
{
  for (Cliente *clienteAtual : clientes)
  {
    if (clienteAtual->getCodigoMatricula() == codigoMatricula)
    {
      return clienteAtual;
    }
  }
  throw AcademiaException("Erro: Cliente nao cadastrado!");
}

void GerenciadorAcademia::atualizarCliente(const std::string &cpf, Cliente *novoC)
{
  for(size_t i = 0; i < clientes.size(); i++) {
    if(clientes[i]->getCpf() == cpf) {
      Cliente* clienteAntigo = clientes[i];
      for (Instrutor* instrutor : instrutores) {
        if (instrutor->supervisionaAluno(clienteAntigo)) {
          instrutor->removerAlunoSupervisionado(clienteAntigo);
          instrutor->adicionarAlunoSupervisionado(novoC);
        }
      }
      for (Pagamento* p : pagamentos) {
        if (p->getCliente() == clienteAntigo) {
          p->reassociarCliente(novoC);
        }
      }
      delete clienteAntigo;
      clientes[i] = novoC;
      std::cout << "Cliente atualizado com sucesso!" << std::endl;
      return;
    }
  }
  throw AcademiaException("Erro: Cliente nao cadastrado!");
}

void GerenciadorAcademia::removerCliente(const std::string &cpf)
{
  for (size_t i = 0; i < clientes.size(); i++){
    if (clientes[i]->getCpf() == cpf){
      Cliente* clienteARemover = clientes[i];
      for (Instrutor* instrutor : instrutores) {
        if (instrutor->supervisionaAluno(clienteARemover)) {
          instrutor->removerAlunoSupervisionado(clienteARemover);
        }
      }
      removerPagamentosDoCliente(clienteARemover);
      delete clientes[i];
      clientes.erase(clientes.begin() + i);
      std::cout << "Cliente removido com sucesso!" << std::endl;
      return;
    }
  }
  throw AcademiaException("Erro: Cliente nao existe na base de dados!");
}

// métodos de busca
Plano *GerenciadorAcademia::buscarPlano(const std::string &id)
{
  for (auto plano : planosCadastrados)
  {
    if (plano->getTipoPlano() == id)
      return plano;
  }
  return nullptr;
}

Plano *GerenciadorAcademia::criarPlanoDoCatalogo(const std::string &tipo)
{
  Plano *modelo = buscarPlano("Plano " + tipo);
  if (modelo == nullptr)
  {
    throw AcademiaException("Tipo de Plano invalido (use Mensal ou Anual).");
  }
  return modelo->clone(); // cada cliente recebe sua propria copia
}

Treino *GerenciadorAcademia::buscarTreino(const std::string &id)
{
  for (auto treino : treinosCadastrados)
  {
    if (treino->getFoco() == id)
      return treino;
  }
  return nullptr;
}

Instrutor *GerenciadorAcademia::buscarInstrutor(const std::string &cpf)
{
  for (auto instrutor : instrutores)
  {
    if (instrutor->getCpf() == cpf)
      return instrutor;
  }
  return nullptr;
}

// instrutor
void GerenciadorAcademia::cadastrarInstrutor(Instrutor* i)
{
  for(Instrutor* instrutorAtual : instrutores){
    if(i->getCpf() == instrutorAtual->getCpf()) {
      throw AcademiaException("Erro: Instrutor com este CPF ja cadastrado.");
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
  throw AcademiaException("Erro: Instrutor nao existe na base de dados!");
}

Instrutor* GerenciadorAcademia::consultarInstrutorPorCpf(const std::string& cpf) {
  for (auto instrutor : instrutores) {
    if (instrutor->getCpf() == cpf) return instrutor;
  }
  throw AcademiaException("Erro: Instrutor nao cadastrado!");
}

void GerenciadorAcademia::cadastrarTreino(Treino* t)
{
    if (t != nullptr) {
        treinosCadastrados.push_back(t);
    }
}

Instrutor* GerenciadorAcademia::consultarInstrutor(const std::string& cref)
{
  for(Instrutor* instrutorAtual : instrutores) {
    if(instrutorAtual->getCref() == cref){
      return instrutorAtual;
    }
  }
  throw AcademiaException("Erro: Instrutor nao cadastrado!");
}

// pagamentos
bool GerenciadorAcademia::existePagamentoAtivo(Cliente* cliente, const std::string& mesReferencia) const
{
  for (Pagamento* p : pagamentos)
  {
    if (p->getCliente() == cliente && p->getMesReferencia() == mesReferencia &&
        (p->getStatus() == StatusPagamento::CONFIRMADO || p->getStatus() == StatusPagamento::PENDENTE))
    {
      return true;
    }
  }
  return false;
}

bool GerenciadorAcademia::existePagamentoConfirmado(Cliente* cliente, const std::string& mesReferencia) const
{
  for (Pagamento* p : pagamentos)
  {
    if (p->getCliente() == cliente && p->getMesReferencia() == mesReferencia &&
        p->getStatus() == StatusPagamento::CONFIRMADO)
    {
      return true;
    }
  }
  return false;
}

void GerenciadorAcademia::removerPagamentosDoCliente(Cliente* cliente)
{
  for (auto it = pagamentos.begin(); it != pagamentos.end();)
  {
    if ((*it)->getCliente() == cliente)
    {
      delete *it;
      it = pagamentos.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

Pagamento* GerenciadorAcademia::registrarPagamento(Pagamento* p)
{
  if (p == nullptr)
  {
    throw AcademiaException("Pagamento invalido.");
  }

  try
  {
    // Regra: o cliente precisa estar cadastrado na academia
    if (std::find(clientes.begin(), clientes.end(), p->getCliente()) == clientes.end())
    {
      throw AcademiaException("Cliente do pagamento nao esta cadastrado.");
    }

    // Regra: nao pode haver dois pagamentos ativos para o mesmo mes
    if (existePagamentoAtivo(p->getCliente(), p->getMesReferencia()))
    {
      throw AcademiaException("Ja existe um pagamento confirmado ou pendente para " +
                              p->getMesReferencia() + ".");
    }

    p->validar(); // regras comuns + especificas de Pix/Cartao
  }
  catch (...)
  {
    delete p;
    throw;
  }

  p->gerarCodigo();
  pagamentos.push_back(p);

  // Regra: Pix e confirmado na hora; cartao aguarda autorizacao
  if (p->confirmacaoImediata())
  {
    confirmarPagamento(p->getCodigo());
  }
  return p;
}

Pagamento* GerenciadorAcademia::consultarPagamento(int codigo) const
{
  for (Pagamento* p : pagamentos)
  {
    if (p->getCodigo() == codigo)
    {
      return p;
    }
  }
  throw AcademiaException("Pagamento nao encontrado.");
}

void GerenciadorAcademia::confirmarPagamento(int codigo)
{
  Pagamento* p = consultarPagamento(codigo);
  p->confirmar();
  reavaliarSituacaoFinanceira(p->getCliente(), UtilData::hoje());
}

void GerenciadorAcademia::cancelarPagamento(int codigo)
{
  consultarPagamento(codigo)->cancelar();
}

void GerenciadorAcademia::estornarPagamento(int codigo)
{
  Pagamento* p = consultarPagamento(codigo);
  std::string hoje = UtilData::hoje();
  p->estornar(hoje);
  reavaliarSituacaoFinanceira(p->getCliente(), hoje);
}

std::vector<Pagamento*> GerenciadorAcademia::historicoPagamentos(const std::string& cpf) const
{
  std::vector<Pagamento*> historico;
  for (Pagamento* p : pagamentos)
  {
    if (p->getCliente()->getCpf() == cpf)
    {
      historico.push_back(p);
    }
  }
  return historico;
}

bool GerenciadorAcademia::clienteEmDia(Cliente* cliente, const std::string& dataAtual) const
{
  std::string mes = UtilData::mesDe(dataAtual);

  // Regra: o cliente esta em dia se pagou o mes atual,
  // se ainda nao passou do vencimento ou se a matricula comecou neste mes
  if (existePagamentoConfirmado(cliente, mes))
    return true;
  if (UtilData::diaDe(dataAtual) <= Pagamento::DIA_VENCIMENTO)
    return true;
  if (UtilData::dataValida(cliente->getDataInicio()) && UtilData::mesDe(cliente->getDataInicio()) == mes)
    return true;
  return false;
}

void GerenciadorAcademia::reavaliarSituacaoFinanceira(Cliente* cliente, const std::string& dataAtual)
{
  Matricula& matricula = cliente->getMatricula();
  bool emDia = clienteEmDia(cliente, dataAtual);

  // Regra: quitar o mes reativa a matricula inadimplente
  if (matricula.getStatus() == StatusMatricula::INADIMPLENTE && emDia)
  {
    matricula.alterarEstado(StatusMatricula::ATIVA);
  }
  // Regra: matricula ativa sem pagamento apos o vencimento fica inadimplente
  else if (matricula.getStatus() == StatusMatricula::ATIVA && !emDia)
  {
    matricula.alterarEstado(StatusMatricula::INADIMPLENTE);
  }
}

std::vector<Cliente*> GerenciadorAcademia::atualizarInadimplencia(const std::string& dataAtual)
{
  std::vector<Cliente*> novosInadimplentes;
  for (Cliente* c : clientes)
  {
    StatusMatricula antes = c->getMatricula().getStatus();
    reavaliarSituacaoFinanceira(c, dataAtual);
    if (antes != StatusMatricula::INADIMPLENTE &&
        c->getMatricula().getStatus() == StatusMatricula::INADIMPLENTE)
    {
      novosInadimplentes.push_back(c);
    }
  }
  return novosInadimplentes;
}

std::vector<Cliente*> GerenciadorAcademia::listarInadimplentes() const
{
  std::vector<Cliente*> inadimplentes;
  for (Cliente* c : clientes)
  {
    if (c->getMatricula().getStatus() == StatusMatricula::INADIMPLENTE)
    {
      inadimplentes.push_back(c);
    }
  }
  return inadimplentes;
}

// parte de arquivos
void GerenciadorAcademia::salvarEmArquivo(std::string nomeArquivo)
{
  // garante que a pasta de destino (ex: data/) exista antes de abrir o arquivo
  std::filesystem::path pasta = std::filesystem::path(nomeArquivo).parent_path();
  if (!pasta.empty())
  {
    std::error_code erro;
    std::filesystem::create_directories(pasta, erro);
  }

  // escreve primeiro em um arquivo temporario e so depois substitui o original,
  // assim uma interrupcao no meio da escrita nao apaga o backup anterior
  std::string arquivoTemporario = nomeArquivo + ".tmp";
  std::ofstream arquivo(arquivoTemporario);

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
      std::string statusStr;
      switch (c->getMatricula().getStatus())
      {
        case StatusMatricula::ATIVA:        statusStr = "ATIVA";        break;
        case StatusMatricula::INADIMPLENTE: statusStr = "INADIMPLENTE"; break;
        case StatusMatricula::TRANCADA:     statusStr = "TRANCADA";     break;
      }

      arquivo << "CLIENTE;" << c->getCodigoMatricula() << ";" << c->getDataInicio() << ";" 
              << c->getNome() << ";" << c->getCpf() << ";" << c->getEmail() << ";" 
              << planoId << ";" << treinoId << ";" << instrutorCpf << ";" << statusStr << "\n";
    }
  }

  // pagamentos sao gravados depois dos clientes, pois referenciam o CPF
  for (Pagamento *p : pagamentos)
  {
    arquivo << "PAGAMENTO;" << p->getTipo() << ";" << p->getCodigo() << ";"
            << p->getCliente()->getCpf() << ";" << p->getMesReferencia() << ";"
            << p->getDataPagamento() << ";" << p->getValorBase() << ";"
            << p->getStatusTexto() << ";" << p->serializarDetalhes() << "\n";
  }

  arquivo.close();
  if (arquivo.fail())
  {
    throw AcademiaException("Erro: Falha ao gravar o arquivo de backup.");
  }

  std::error_code erroRename;
  std::filesystem::rename(arquivoTemporario, nomeArquivo, erroRename);
  if (erroRename)
  {
    throw AcademiaException("Erro: Nao foi possivel substituir o arquivo de backup.");
  }
}


void GerenciadorAcademia::carregarDeArquivo(std::string nomeArquivo)
{
  std::ifstream arquivo(nomeArquivo);

  if (!arquivo.is_open())
  {
    throw AcademiaException("Erro: Nao foi possivel abrir o arquivo para leitura.");
  }

  // limpa a memoria
  for (Pagamento *p : pagamentos)
    delete p;
  pagamentos.clear();
  for (Cliente *c : clientes)
    delete c;
  clientes.clear();
  for (Instrutor *i : instrutores)
    delete i;
  instrutores.clear();
  for (Treino *t : treinosCadastrados)
    delete t;
  treinosCadastrados.clear();

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
      continue;
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
      std::string matStr, dataInicio, nome, cpf, email, planoId, treinoId, instrutorCpf, statusStr;
      std::getline(ss, matStr, ';');
      std::getline(ss, dataInicio, ';');
      std::getline(ss, nome, ';');
      std::getline(ss, cpf, ';');
      std::getline(ss, email, ';');
      std::getline(ss, planoId, ';');
      std::getline(ss, treinoId, ';');
      std::getline(ss, instrutorCpf, ';');
      std::getline(ss, statusStr);

      StatusMatricula status = StatusMatricula::ATIVA;
      if (statusStr == "INADIMPLENTE") status = StatusMatricula::INADIMPLENTE;
      else if (statusStr == "TRANCADA") status = StatusMatricula::TRANCADA;

      Plano *planoCatalogo = buscarPlano(planoId);
      Plano *p = planoCatalogo ? planoCatalogo->clone() : nullptr;
      Treino *t = buscarTreino(treinoId);
      Instrutor *inst = buscarInstrutor(instrutorCpf);

      Cliente *novoCliente = new Cliente(nome, cpf, email, p, dataInicio);

      novoCliente->restaurarMatricula(std::stoi(matStr), dataInicio, status);

      if (t)
        novoCliente->associarTreino(t);

      if (inst)
      {
        inst->adicionarAlunoSupervisionado(novoCliente);
      }

      clientes.push_back(novoCliente);
    }
    else if (tipo == "PAGAMENTO")
    {
      std::string tipoPag, codStr, cpf, mesRef, dataPag, valorStr, statusStr;
      std::getline(ss, tipoPag, ';');
      std::getline(ss, codStr, ';');
      std::getline(ss, cpf, ';');
      std::getline(ss, mesRef, ';');
      std::getline(ss, dataPag, ';');
      std::getline(ss, valorStr, ';');
      std::getline(ss, statusStr, ';');

      Cliente *dono = nullptr;
      for (Cliente *c : clientes)
      {
        if (c->getCpf() == cpf)
        {
          dono = c;
          break;
        }
      }
      if (dono == nullptr)
        continue; // pagamento de cliente inexistente e ignorado

      Pagamento *p = nullptr;
      if (tipoPag == "PIX")
      {
        std::string chave, descontoStr;
        std::getline(ss, chave, ';');
        std::getline(ss, descontoStr);
        p = new PagamentoPix(dono, mesRef, dataPag, chave, std::stod(descontoStr));
      }
      else if (tipoPag == "CARTAO")
      {
        std::string parcelasStr, taxaStr, digitos;
        std::getline(ss, parcelasStr, ';');
        std::getline(ss, taxaStr, ';');
        std::getline(ss, digitos);
        p = new PagamentoCartao(dono, mesRef, dataPag, std::stoi(parcelasStr), digitos, std::stod(taxaStr));
      }
      else
      {
        continue;
      }

      // restaura sem revalidar: o pagamento ja passou pelas regras quando foi registrado
      p->restaurar(std::stoi(codStr), std::stod(valorStr), Pagamento::textoParaStatus(statusStr));
      pagamentos.push_back(p);
    }
  }
  arquivo.close();
}
