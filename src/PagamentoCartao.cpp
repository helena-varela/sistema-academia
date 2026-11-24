#include "PagamentoCartao.hpp"

#include <cctype>
#include <sstream>
#include <iomanip>

PagamentoCartao::PagamentoCartao(Cliente* cliente,
                                 const std::string& mesReferencia,
                                 const std::string& dataPagamento,
                                 int numeroParcelas,
                                 const std::string& ultimos4Digitos)
    : Pagamento(cliente, mesReferencia, dataPagamento),
      numeroParcelas(numeroParcelas),
      ultimos4Digitos(ultimos4Digitos)
{
    if (numeroParcelas < 1)
    {
        throw DadosInvalidosException("parcelas", "Numero de parcelas deve ser no minimo 1.");
    }
    if (ultimos4Digitos.size() != 4)
    {
        throw DadosInvalidosException("cartao", "Informe os 4 ultimos digitos do cartao.");
    }
    for (char c : ultimos4Digitos)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            throw DadosInvalidosException("cartao", "Os ultimos digitos do cartao devem ser numericos.");
        }
    }
}

void PagamentoCartao::validarEspecifico() const
{
    // Regra: o numero maximo de parcelas depende do plano do cliente (polimorfismo de Plano)
    int maxParcelas = cliente->getPlanoAtual()->getMaxParcelasCartao();
    if (numeroParcelas > maxParcelas)
    {
        throw PagamentoRecusadoException(getTipo(), "O " + cliente->getPlanoAtual()->getTipoPlano() +
                                " permite no maximo " + std::to_string(maxParcelas) + " parcela(s).");
    }
}

double PagamentoCartao::calcularValorFinal() const
{
    // Regra: cartao nao tem desconto, cobra o valor cheio da mensalidade
    return valorBase;
}

bool PagamentoCartao::confirmacaoImediata() const
{
    // Regra: cartao fica pendente ate a autorizacao da operadora
    return false;
}

std::string PagamentoCartao::getTipo() const
{
    return "CARTAO";
}

std::string PagamentoCartao::getDescricao() const
{
    std::ostringstream os;
    os << std::fixed << std::setprecision(2)
       << "Cartao final " << ultimos4Digitos << " - " << numeroParcelas
       << "x de R$ " << getValorParcela();
    return os.str();
}

std::string PagamentoCartao::serializarDetalhes() const
{
    std::ostringstream os;
    os << numeroParcelas << ";" << ultimos4Digitos;
    return os.str();
}

double PagamentoCartao::getValorParcela() const
{
    return calcularValorFinal() / numeroParcelas;
}

int PagamentoCartao::getNumeroParcelas() const { return numeroParcelas; }
std::string PagamentoCartao::getUltimos4Digitos() const { return ultimos4Digitos; }
