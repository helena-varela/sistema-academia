#include "Excecoes.hpp"

// ---------------- DadosInvalidosException ----------------
DadosInvalidosException::DadosInvalidosException(const std::string& campo, const std::string& mensagem)
    : AcademiaException(mensagem), campo(campo) {}

std::string DadosInvalidosException::getCampo() const { return campo; }

// ---------------- RegistroNaoEncontradoException ----------------
RegistroNaoEncontradoException::RegistroNaoEncontradoException(const std::string& tipoRegistro,
                                                               const std::string& identificador)
    : AcademiaException(tipoRegistro + " nao encontrado(a): " + identificador + "."),
      tipoRegistro(tipoRegistro), identificador(identificador) {}

std::string RegistroNaoEncontradoException::getTipoRegistro() const { return tipoRegistro; }
std::string RegistroNaoEncontradoException::getIdentificador() const { return identificador; }

// ---------------- RegistroDuplicadoException ----------------
RegistroDuplicadoException::RegistroDuplicadoException(const std::string& tipoRegistro,
                                                       const std::string& identificador)
    : AcademiaException("Ja existe " + tipoRegistro + " cadastrado(a) com " + identificador + "."),
      tipoRegistro(tipoRegistro), identificador(identificador) {}

std::string RegistroDuplicadoException::getTipoRegistro() const { return tipoRegistro; }
std::string RegistroDuplicadoException::getIdentificador() const { return identificador; }

// ---------------- TransicaoEstadoInvalidaException ----------------
TransicaoEstadoInvalidaException::TransicaoEstadoInvalidaException(const std::string& entidade,
                                                                   const std::string& estadoAtual,
                                                                   const std::string& estadoDestino,
                                                                   const std::string& motivo)
    : AcademiaException("Transicao invalida de " + entidade + ": " + estadoAtual + " -> " +
                        estadoDestino + "." + (motivo.empty() ? "" : " " + motivo)),
      entidade(entidade), estadoAtual(estadoAtual), estadoDestino(estadoDestino) {}

std::string TransicaoEstadoInvalidaException::getEntidade() const { return entidade; }
std::string TransicaoEstadoInvalidaException::getEstadoAtual() const { return estadoAtual; }
std::string TransicaoEstadoInvalidaException::getEstadoDestino() const { return estadoDestino; }

// ---------------- AutenticacaoException ----------------
AutenticacaoException::AutenticacaoException(const std::string& perfil, const std::string& mensagem)
    : AcademiaException(mensagem), perfil(perfil) {}

std::string AutenticacaoException::getPerfil() const { return perfil; }

// ---------------- AcessoNegadoException ----------------
AcessoNegadoException::AcessoNegadoException(const std::string& recurso, const std::string& mensagem)
    : AcademiaException(mensagem), recurso(recurso) {}

std::string AcessoNegadoException::getRecurso() const { return recurso; }

// ---------------- PagamentoRecusadoException ----------------
PagamentoRecusadoException::PagamentoRecusadoException(const std::string& formaPagamento,
                                                       const std::string& mensagem)
    : AcademiaException(mensagem), formaPagamento(formaPagamento) {}

std::string PagamentoRecusadoException::getFormaPagamento() const { return formaPagamento; }

// ---------------- PersistenciaException ----------------
PersistenciaException::PersistenciaException(const std::string& caminhoArquivo, const std::string& mensagem)
    : AcademiaException(mensagem + " [arquivo: " + caminhoArquivo + "]"),
      caminhoArquivo(caminhoArquivo) {}

std::string PersistenciaException::getCaminhoArquivo() const { return caminhoArquivo; }
