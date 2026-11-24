#include "PagamentoPix.hpp"

#include <sstream>

PagamentoPix::PagamentoPix(Cliente* cliente,
                           const std::string& mesReferencia,
                           const std::string& dataPagamento,
                           const std::string& chavePix,
                           double percentualDesconto)
    : Pagamento(cliente, mesReferencia, dataPagamento),
      chavePix(chavePix),
      percentualDesconto(percentualDesconto)
{
    if (chavePix.empty())
    {
        throw AcademiaException("Chave Pix nao pode ser vazia.");
    }
}

void PagamentoPix::validarEspecifico() const
{
    if (percentualDesconto < 0.0 || percentualDesconto > 0.10)
    {
        throw AcademiaException("Desconto do Pix deve estar entre 0% e 10%.");
    }
}

double PagamentoPix::calcularValorFinal() const
{
    // Regra: pagamento via Pix tem desconto
    return valorBase * (1.0 - percentualDesconto);
}

bool PagamentoPix::confirmacaoImediata() const
{
    // Regra: Pix e compensado na hora
    return true;
}

std::string PagamentoPix::getTipo() const
{
    return "PIX";
}

std::string PagamentoPix::getDescricao() const
{
    std::ostringstream os;
    os << "Pix (chave: " << chavePix << ") - desconto de " << percentualDesconto * 100 << "%";
    return os.str();
}

std::string PagamentoPix::serializarDetalhes() const
{
    std::ostringstream os;
    os << chavePix << ";" << percentualDesconto;
    return os.str();
}

std::string PagamentoPix::getChavePix() const { return chavePix; }
double PagamentoPix::getPercentualDesconto() const { return percentualDesconto; }
