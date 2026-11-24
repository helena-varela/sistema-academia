#ifndef EXCECOES_HPP
#define EXCECOES_HPP

#include <string>
#include "AcademiaException.hpp"

// Excecoes personalizadas do sistema. Todas herdam de AcademiaException,
// entao quem nao precisa diferenciar pode capturar apenas a classe base.

// Dado informado pelo usuario e invalido (validacao de entrada)
class DadosInvalidosException : public AcademiaException
{
    private:
        std::string campo;
    public:
        DadosInvalidosException(const std::string& campo, const std::string& mensagem);
        std::string getCampo() const;
};

// Busca por um registro (cliente, instrutor, pagamento...) que nao existe
class RegistroNaoEncontradoException : public AcademiaException
{
    private:
        std::string tipoRegistro;
        std::string identificador;
    public:
        RegistroNaoEncontradoException(const std::string& tipoRegistro, const std::string& identificador);
        std::string getTipoRegistro() const;
        std::string getIdentificador() const;
};

// Tentativa de cadastrar um registro que ja existe (ex: CPF repetido)
class RegistroDuplicadoException : public AcademiaException
{
    private:
        std::string tipoRegistro;
        std::string identificador;
    public:
        RegistroDuplicadoException(const std::string& tipoRegistro, const std::string& identificador);
        std::string getTipoRegistro() const;
        std::string getIdentificador() const;
};

// Mudanca de estado nao permitida (Matricula, Pagamento)
class TransicaoEstadoInvalidaException : public AcademiaException
{
    private:
        std::string entidade;
        std::string estadoAtual;
        std::string estadoDestino;
    public:
        TransicaoEstadoInvalidaException(const std::string& entidade,
                                         const std::string& estadoAtual,
                                         const std::string& estadoDestino,
                                         const std::string& motivo = "");
        std::string getEntidade() const;
        std::string getEstadoAtual() const;
        std::string getEstadoDestino() const;
};

// Falha de login ou de confirmacao de senha
class AutenticacaoException : public AcademiaException
{
    private:
        std::string perfil;
    public:
        AutenticacaoException(const std::string& perfil, const std::string& mensagem);
        std::string getPerfil() const;
};

// Usuario autenticado, mas sem permissao para a operacao (autorizacao / regra de acesso)
class AcessoNegadoException : public AcademiaException
{
    private:
        std::string recurso;
    public:
        AcessoNegadoException(const std::string& recurso, const std::string& mensagem);
        std::string getRecurso() const;
};

// Pagamento recusado por regra de negocio
class PagamentoRecusadoException : public AcademiaException
{
    private:
        std::string formaPagamento;
    public:
        PagamentoRecusadoException(const std::string& formaPagamento, const std::string& mensagem);
        std::string getFormaPagamento() const;
};

// Erro ao ler ou gravar o arquivo de dados
class PersistenciaException : public AcademiaException
{
    private:
        std::string caminhoArquivo;
    public:
        PersistenciaException(const std::string& caminhoArquivo, const std::string& mensagem);
        std::string getCaminhoArquivo() const;
};

#endif
