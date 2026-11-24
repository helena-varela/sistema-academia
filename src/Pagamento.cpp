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
    // Regra: o valor cobrado e a mensalidade do plano atual do cliente (polimorfismo de Plano)
    valorBase = cliente->getPlanoAtual()->calcularMensalidade();
}

Pagamento::~Pagamento()
{
}

void Pagamento::validar() const
{
    // Regra: matricula trancada nao gera cobranca
    if (cliente->getMatricula().getStatus() == StatusMatricula::TRANCADA)
    {
        throw AcademiaException("Matricula trancada nao pode receber pagamentos.");
    }

    // Regra: nao se paga mes anterior ao inicio da matricula
    std::string mesInicio = UtilData::mesDe(cliente->getDataInicio());
    if (UtilData::mesComoNumero(mesReferencia) < UtilData::mesComoNumero(mesInicio))
    {
        throw AcademiaException("Mes de referencia anterior ao inicio da matricula (" + mesInicio + ").");
    }

    // Regra: so e possivel adiantar ate o proximo mes
    std::string mesDoPagamento = UtilData::mesDe(dataPagamento);
    if (UtilData::mesComoNumero(mesReferencia) > UtilData::mesComoNumero(mesDoPagamento) + 1)
    {
        throw AcademiaException("So e possivel adiantar o pagamento do proximo mes.");
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

void Pagamento::estornar(const std::string& dataAtual)
{
    if (status != StatusPagamento::CONFIRMADO)
    {
        throw AcademiaException("Apenas pagamentos confirmados podem ser estornados.");
    }
    if (!podeEstornar(dataAtual))
    {
        throw AcademiaException("Prazo de estorno expirado para pagamento via " + getTipo() + ".");
    }
    alterarStatus(StatusPagamento::ESTORNADO);
}

std::string Pagamento::getDataVencimento() const
{
    // Regra: no primeiro mes, quem se matricula depois do dia de vencimento
    // tem como vencimento a propria data da matricula (nao paga multa por isso)
    std::string inicio = cliente->getDataInicio();
    if (UtilData::dataValida(inicio) && UtilData::mesDe(inicio) == mesReferencia &&
        UtilData::diaDe(inicio) > DIA_VENCIMENTO)
    {
        return inicio;
    }

    std::string dia = (DIA_VENCIMENTO < 10 ? "0" : "") + std::to_string(DIA_VENCIMENTO);
    return dia + "/" + mesReferencia;
}

int Pagamento::diasDeAtraso() const
{
    int dias = UtilData::diasEntre(getDataVencimento(), dataPagamento);
    return dias > 0 ? dias : 0;
}

bool Pagamento::pagoEmAtraso() const
{
    return diasDeAtraso() > 0;
}

double Pagamento::calcularMultaAtraso() const
{
    // Regra: atraso gera multa fixa de 2% + juros de 0,1% por dia
    if (!pagoEmAtraso())
    {
        return 0.0;
    }
    return valorBase * (PERCENTUAL_MULTA + PERCENTUAL_JUROS_DIA * diasDeAtraso());
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
