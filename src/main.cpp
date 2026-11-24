#include <iostream>
#include <limits>
#include "AcademiaException.hpp"
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
    catch (const AcademiaException& e)
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
                throw AcademiaException("Digite apenas o numero da opcao.");
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
                    throw AcademiaException("Opção Invalida");
            }
        }
        catch(const AcademiaException& e){
            std::cout << "Erro: " << e.what() << '\n';
        }

        // salva ao voltar ao menu principal (inclusive apos erros),
        // para nao perder alteracoes se o programa for encerrado abruptamente
        try
        {
            gerenciador.salvarEmArquivo(ARQUIVO_BACKUP);
        }
        catch (const AcademiaException& e)
        {
            cout << "Erro ao salvar backup: " << e.what() << '\n';
        }

    }

    try 
    {
        gerenciador.salvarEmArquivo(ARQUIVO_BACKUP);
    } 
    catch (const AcademiaException& e) 
    {
        cout << "Erro ao salvar backup: " << e.what() << '\n';
    }


    return 0;
}