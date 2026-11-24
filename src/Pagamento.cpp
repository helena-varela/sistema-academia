#include "Pagamento.hpp"
#include "UtilData.hpp"

int Pagamento::ultimoCodigo = 0;

Pagamento::Pagamento(Cliente* cliente, const std::string& mesReferencia, const std::string& dataPagamento)
    : codigo(0), // o codigo so e gerado quando o pagamento e aceito (gerarCodigo)
      cliente(cliente),
      mesReferencia(mesReferencia),
      dataPagamento(dataPagamento),
      valorBase(0.0),
      status(StatusPagamento::PENDENTE)
{
    if (cliente == nullptr)
    {
        throw AcademiaException("Pagamento precisa estar associado a um cliente.");
    }
    if (!UtilData::mesValido(mesReferencia))
    {
        throw AcademiaException("Mes de referencia invalido (use MM/AAAA).");
    }
    if (!UtilData::dataValida(dataPagamento))
    {
        throw AcademiaException("Data de pagamento invalida (use DD/MM/AAAA).");
    }
    if (cliente->getPlanoAtual() == nullptr)
    {
        throw AcademiaException("Cliente sem plano nao pode realizar pagamento.");
    }
    // o valor cobrado e a mensalidade do plano atual do cliente (polimorfismo de Plano)
    valorBase = cliente->getPlanoAtual()->calcularMensalidade();
}

Pagamento::~Pagamento()
{
}

void Pagamento::validar() const
{
    // Regra: matricula trancada nao recebe pagamentos
    if (cliente->getMatricula().getStatus() == StatusMatricula::TRANCADA)
    {
        throw AcademiaException("Matricula trancada nao pode receber pagamentos.");
    }

    validarEspecifico();
}

void Pagamento::alterarStatus(StatusPagamento novoStatus)
{
    bool permitido = false;
    switch (status)
    {
        case StatusPagamento::PENDENTE:
            permitido = (novoStatus == StatusPagamento::CONFIRMADO ||
                         novoStatus == StatusPagamento::CANCELADO);
            break;
        case StatusPagamento::CONFIRMADO:
            permitido = (novoStatus == StatusPagamento::ESTORNADO);
            break;
        case StatusPagamento::CANCELADO:
        case StatusPagamento::ESTORNADO:
            permitido = false; // estados finais
            break;
    }

    if (!permitido)
    {
        throw AcademiaException("Transicao invalida do pagamento: " + statusParaTexto(status) +
                                " -> " + statusParaTexto(novoStatus) + ".");
    }
    status = novoStatus;
}

void Pagamento::confirmar()
{
    alterarStatus(StatusPagamento::CONFIRMADO);
}

void Pagamento::cancelar()
{
    if (status != StatusPagamento::PENDENTE)
    {
        throw AcademiaException("Apenas pagamentos pendentes podem ser cancelados.");
    }
    alterarStatus(StatusPagamento::CANCELADO);
}

void Pagamento::estornar()
{
    if (status != StatusPagamento::CONFIRMADO)
    {
        throw AcademiaException("Apenas pagamentos confirmados podem ser estornados.");
    }
    alterarStatus(StatusPagamento::ESTORNADO);
}

int Pagamento::getCodigo() const { return codigo; }
Cliente* Pagamento::getCliente() const { return cliente; }
std::string Pagamento::getMesReferencia() const { return mesReferencia; }
std::string Pagamento::getDataPagamento() const { return dataPagamento; }
double Pagamento::getValorBase() const { return valorBase; }
StatusPagamento Pagamento::getStatus() const { return status; }
std::string Pagamento::getStatusTexto() const { return statusParaTexto(status); }

void Pagamento::reassociarCliente(Cliente* novoCliente)
{
    if (novoCliente == nullptr)
    {
        throw AcademiaException("Cliente invalido.");
    }
    cliente = novoCliente;
}

void Pagamento::gerarCodigo()
{
    if (codigo == 0)
    {
        codigo = ++ultimoCodigo;
    }
}

void Pagamento::restaurar(int codigo, double valorBase, StatusPagamento status)
{
    this->codigo = codigo;
    this->valorBase = valorBase;
    this->status = status;
    if (codigo > ultimoCodigo)
    {
        ultimoCodigo = codigo;
    }
}

std::string Pagamento::statusParaTexto(StatusPagamento status)
{
    switch (status)
    {
        case StatusPagamento::PENDENTE:   return "PENDENTE";
        case StatusPagamento::CONFIRMADO: return "CONFIRMADO";
        case StatusPagamento::CANCELADO:  return "CANCELADO";
        case StatusPagamento::ESTORNADO:  return "ESTORNADO";
    }
    return "DESCONHECIDO";
}

StatusPagamento Pagamento::textoParaStatus(const std::string& texto)
{
    if (texto == "PENDENTE")   return StatusPagamento::PENDENTE;
    if (texto == "CONFIRMADO") return StatusPagamento::CONFIRMADO;
    if (texto == "CANCELADO")  return StatusPagamento::CANCELADO;
    if (texto == "ESTORNADO")  return StatusPagamento::ESTORNADO;
    throw AcademiaException("Status de pagamento desconhecido: " + texto);
}
