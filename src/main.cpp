#include <iostream>
#include <limits>
#include "Excecoes.hpp"
#include "GerenciadorAcademia.hpp"
#include "Interface.hpp"
using namespace std;

int main()
{
    GerenciadorAcademia gerenciador;

    bool executando = true;
    int opcao;
    string cpf;
    string cref;

    try
    {
        gerenciador.carregarDeArquivo(ARQUIVO_BACKUP);
    }
    catch (const PersistenciaException& e)
    {
        cout << "Aviso: " << e.what() << " Iniciando banco de dados vazio.\n";
    }

    while (executando)
    {
        try
        {
            cout << "=====================================" << endl;
            cout << "        SISTEMA UPESTEEM" << endl;
            cout << "=====================================" << endl;
            cout << "1 - Login Cliente" << endl;
            cout << "2 - Login Instrutor" << endl;
            cout << "3 - Login Gerenciador" << endl;
            cout << "0 - Sair" << endl;
            cout << "=====================================" << endl;
            cout << "Escolha uma opcao: ";
            if (!(cin >> opcao))
            {
                if (cin.eof()) // entrada encerrada (Ctrl+D): sai do sistema salvando
                {
                    executando = false;
                    continue;
                }
                // entrada nao numerica: limpa o estado do cin para evitar loop infinito
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw DadosInvalidosException("opcao", "Digite apenas o numero da opcao.");
            }

            switch (opcao)
            {
                case 1:
                    {
                        cout << "[Interface do Cliente]" << endl;
                        string senha;
                        cout << "Digite seu CPF: ";
                        cin >> cpf;
                        cout << "Digite sua senha: ";
                        cin >> senha;
                        Cliente* cliente = gerenciador.autenticarCliente(cpf, senha);
                        interfaceCliente(gerenciador, *cliente);
                        break;
                    }
                case 2:
                    {
                        cout << "[Interface do Instrutor]" << endl;
                        string senha;
                        cout << "Digite seu CREF: ";
                        cin >> cref;
                        cout << "Digite sua senha: ";
                        cin >> senha;
                        {
                            Instrutor* instrutor = gerenciador.autenticarInstrutor(cref, senha);
                            interfaceInstrutor(gerenciador, instrutor);
                        }
                        break;
                    }
                case 3:
                    {
                        string senha;
                        cout << "[Interface do Gerenciador]" << endl;
                        cout << "Digite a senha do gerente: ";
                        cin >> senha;
                        gerenciador.autenticarGerente(senha);
                        interfaceGerenciador(gerenciador);
                        break;
                    }

                case 0:
                    executando = false;
                    cout << "Encerrando sistema..." << endl;
                    break;

                default:
                    throw DadosInvalidosException("opcao", "Opcao invalida!");
            }
        }
        // tratamento especifico por tipo de excecao (da mais especifica para a mais geral)
        catch (const AutenticacaoException& e)
        {
            cout << "Falha no login (" << e.getPerfil() << "): " << e.what() << '\n';
        }
        catch (const AcessoNegadoException& e)
        {
            cout << "Acesso negado [" << e.getRecurso() << "]: " << e.what() << '\n';
        }
        catch (const DadosInvalidosException& e)
        {
            cout << "Dado invalido [" << e.getCampo() << "]: " << e.what() << '\n';
        }
        catch (const RegistroNaoEncontradoException& e)
        {
            cout << "Nao encontrado: " << e.what() << '\n';
        }
        catch (const RegistroDuplicadoException& e)
        {
            cout << "Cadastro duplicado: " << e.what() << '\n';
        }
        catch (const TransicaoEstadoInvalidaException& e)
        {
            cout << "Operacao nao permitida: " << e.what() << '\n';
        }
        catch (const PagamentoRecusadoException& e)
        {
            cout << "Pagamento recusado (" << e.getFormaPagamento() << "): " << e.what() << '\n';
        }
        catch (const PersistenciaException& e)
        {
            cout << "Erro ao acessar os dados: " << e.what() << '\n';
        }
        catch (const AcademiaException& e) // qualquer outra excecao do sistema
        {
            cout << "Erro: " << e.what() << '\n';
        }
        catch (const std::exception& e) // erros inesperados da biblioteca padrao
        {
            cout << "Erro inesperado: " << e.what() << '\n';
        }

        // salva ao voltar ao menu principal (inclusive apos erros),
        // para nao perder alteracoes se o programa for encerrado abruptamente
        try
        {
            gerenciador.salvarEmArquivo(ARQUIVO_BACKUP);
        }
        catch (const PersistenciaException& e)
        {
            cout << "Erro ao salvar backup: " << e.what() << '\n';
        }

    }

    try 
    {
        gerenciador.salvarEmArquivo(ARQUIVO_BACKUP);
    } 
    catch (const PersistenciaException& e)
    {
        cout << "Erro ao salvar backup: " << e.what() << '\n';
    }


    return 0;
}