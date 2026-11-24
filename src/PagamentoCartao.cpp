#include "PagamentoCartao.hpp"
#include "UtilData.hpp"

#include <cctype>
#include <sstream>
#include <iomanip>

PagamentoCartao::PagamentoCartao(Cliente* cliente,
                                 const std::string& mesReferencia,
                                 const std::string& dataPagamento,
                                 int numeroParcelas,
                                 const std::string& ultimos4Digitos,
                                 double taxaJurosParcela)
    : Pagamento(cliente, mesReferencia, dataPagamento),
      numeroParcelas(numeroParcelas),
      taxaJurosParcela(taxaJurosParcela),
      ultimos4Digitos(ultimos4Digitos)
{
    if (numeroParcelas < 1)
    {
        throw AcademiaException("Numero de parcelas deve ser no minimo 1.");
    }
    if (ultimos4Digitos.size() != 4)
    {
        throw AcademiaException("Informe os 4 ultimos digitos do cartao.");
    }
    for (char c : ultimos4Digitos)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            throw AcademiaException("Os ultimos digitos do cartao devem ser numericos.");
        }
    }
}

void PagamentoCartao::validarEspecifico() const
{
    // Regra: o numero maximo de parcelas depende do plano do cliente (polimorfismo de Plano)
    int maxParcelas = cliente->getPlanoAtual()->getMaxParcelasCartao();
    if (numeroParcelas > maxParcelas)
    {
        throw AcademiaException("O " + cliente->getPlanoAtual()->getTipoPlano() +
                                " permite no maximo " + std::to_string(maxParcelas) + " parcela(s).");
    }

    // Regra: cada parcela deve ter um valor minimo
    if (getValorParcela() < VALOR_MINIMO_PARCELA)
    {
        std::ostringstream os;
        os << std::fixed << std::setprecision(2)
           << "Valor da parcela (R$ " << getValorParcela() << ") abaixo do minimo de R$ "
           << VALOR_MINIMO_PARCELA << ".";
        throw AcademiaException(os.str());
    }
}

double PagamentoCartao::calcularValorFinal() const
{
    // Regra: cartao nao tem desconto; paga multa se atrasado
    // e juros por parcela acima de PARCELAS_SEM_JUROS
    double total = valorBase + calcularMultaAtraso();
    if (numeroParcelas > PARCELAS_SEM_JUROS)
    {
        total *= 1.0 + taxaJurosParcela * (numeroParcelas - PARCELAS_SEM_JUROS);
    }
    return total;
}

bool PagamentoCartao::confirmacaoImediata() const
{
    // Regra: cartao fica pendente ate a autorizacao da operadora
    return false;
}

bool PagamentoCartao::podeEstornar(const std::string& dataAtual) const
{
    // Regra: cartao pode ser estornado em ate 7 dias
    int dias = UtilData::diasEntre(dataPagamento, dataAtual);
    return dias >= 0 && dias <= PRAZO_ESTORNO_DIAS;
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
    if (numeroParcelas > PARCELAS_SEM_JUROS)
    {
        os << " (com juros)";
    }
    return os.str();
}

std::string PagamentoCartao::serializarDetalhes() const
{
    std::ostringstream os;
    os << numeroParcelas << ";" << taxaJurosParcela << ";" << ultimos4Digitos;
    return os.str();
}

double PagamentoCartao::getValorParcela() const
{
    return calcularValorFinal() / numeroParcelas;
}

int PagamentoCartao::getNumeroParcelas() const { return numeroParcelas; }
std::string PagamentoCartao::getUltimos4Digitos() const { return ultimos4Digitos; }
