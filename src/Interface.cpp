#include "../include/Interface.hpp"
#include <iostream>
#include <limits>
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::list;
using std::numeric_limits;
using std::streamsize;
using std::string;

void interfaceCliente(Cliente& cliente)
{
    int opcao;
    do 
    {
        cout << "====== Bem-vindo Cliente ======" << endl;
        cout << "====== MENU ======" << endl;
        cout << "1. Visualizar Dados Pessoais" << endl;
        cout << "2. Visualizar Matricula" << endl;
        cout << "3. Visualizar Plano Atual" << endl;
        cout << "4. Visualizar Treino" << endl;
        cout << "0. Retornar" << endl;

        cin >> opcao;

        switch (opcao)
        {
            case 1:
                cout << "====== Seus Dados ======" << endl;
                cout << "Nome: " << cliente.getNome() << endl;
                cout << "CPF: " << cliente.getCpf() << endl;
                cout << "E-mail: " << cliente.getEmail() << endl;
                cout << "=========================" << endl;
            case 2:
                cout << "====== Seu ID ======" << endl;
                cout << "Matrícula: " << cliente.getCodigoMatricula() << endl;
                cout << "=========================" << endl; 
            case 3:
                cout << "====== Seu Plano Atual ======" << endl;
                cout << "Plano: " << cliente.getPlanoAtual()->getTipoPlano() << endl;
                cout << "=========================" << endl; 
            case 4:
                cout << "====== Seu Treino Atual ======" << endl;
                cout << "Foco: " << cliente.getTreinoDesignado()->getFoco() << endl;
                cout << "Exercícios: " << *cliente.getTreinoDesignado() << endl;
                cout << "=========================" << endl; 
            case 0:
                cout << "Retornando ao menu principal..." << endl;
                break;
            default:
                throw AcademiaException("Opção Inválida");
        }
    }
    while (opcao != 0); 
}

void interfaceInstrutor(GerenciadorAcademia& gerenciador, Instrutor& instrutor)
{
    int opcao;
    int id;
    int escolha;

    Cliente* cliente = nullptr;

    do{
    cout << "====== Bem-vindo Instrutor ======" << endl;
    cout << "====== MENU ======" << endl;
    cout << "1. Adicionar Aluno Para Supervisão" << endl;
    cout << "2. Prescrever Treino" << endl;
    cout << "3. Remover Aluno da Supervisao" << endl;
    cout << "4. Listar Alunos Supervisionados" << endl;
    cout << "0. Retornar" << endl;

    cin >> opcao;

        switch (opcao)
        {
            case 1:
            {
                cout << "====== Painel de Busca ======" << endl;
                cout << "Digite o id do aluno que deseja supervisionar: ";
                cin >> id;
                cliente = gerenciador.consultarClienteBase(id);
                cout << "Nome: " << cliente->getNome() << endl;
                cout << "E-mail: " << cliente->getEmail() << endl;
                cout << "Esse e o cliente que deseja supervisionar?" << endl;
                cout << "1. Sim" << endl;
                cout << "2. Não" << endl;
                cin >> escolha;
                if (escolha != 1 && escolha != 2)
                {
                    throw AcademiaException("Opcao invalida!");
                }
                else if (escolha == 1)
                {
                    instrutor.adicionarAlunoSupervisionado(cliente);
                    cout << "Cliente Adicionado com Sucesso" << endl;
                }
                cout << "=========================" << endl;
                break;
            }
            case 2:
            {
                int codigo;
                cout << "Informe o codigo da matricula: ";
                cin >> codigo;
                Cliente* cliente = instrutor.consultarAlunoSupervisionado(codigo); 
                cin.ignore();
                string foco;
                cout << "Qual o foco do treino: ";
                getline(cin, foco);
                int duracao;
                cout << "Duracao (em minutos): ";
                cin >> duracao;
                int quantidade;
                cout << "Quantidade de exercicios: ";
                cin >> quantidade;
                cin.ignore();
                list<string> exercicios;
                for (int i = 0; i < quantidade; i++)
                {
                    string exercicio;
                    cout << "Exercicio " << i + 1 << ": ";
                    getline(cin, exercicio);

                    exercicios.push_back(exercicio);
                }
                Treino* treino = new Treino(foco, exercicios, duracao);
                instrutor.prescreverTreino(cliente, treino);
                cout << "Treino prescrito com sucesso!" << endl;
                cout << "=========================" << endl; 
                break;
            }
            case 3:
                cout << "====== Painel de Remocao ======" << endl;
                cout << "Digite o id do aluno que deseja remover da supervisao: ";
                cin >> id;
                cliente = instrutor.consultarAlunoSupervisionado(id); 
                cout << "Nome: " << cliente->getNome() << endl;
                cout << "E-mail: " << cliente->getEmail() << endl;
                cout << "Esse e o cliente que deseja remover?" << endl;
                cout << "1. Sim" << endl;
                cout << "2. Não" << endl;
                cin >> escolha;
                if (escolha != 1 && escolha != 2)
                {
                    throw AcademiaException("Opcao invalida!");
                }
                else if (escolha == 1)
                {
                    instrutor.removerAlunoSupervisionado(cliente);
                    cout << "Cliente Removido com Sucesso" << endl;
                }
                cout << "=========================" << endl; 
                break;
            case 4:
                {
                    cout << "====== Lista de Alunos Supervisionados ======" << endl;
                    std::list<Cliente*> alunos = instrutor.getAlunosSobSupervisao();

                    if (alunos.empty())
                    {
                        cout << "Nenhum aluno sob supervisao." << endl;
                        break;
                    }

                    cout << "===== Alunos Supervisionados =====" << endl;

                    for (Cliente* aluno : alunos)
                    {
                        cout << "=========================" << endl; 
                        cout << "Nome: " << aluno->getNome() << endl;
                        cout << "Matricula: " << aluno->getCodigoMatricula() << endl;
                        cout << "Email: " << aluno->getEmail() << endl;
                        cout << "=========================" << endl; 
                    }
                    cout << "=========================" << endl;
                    break; 
                }
            case 0:
                cout << "Retornando ao menu principal..." << endl;
                break;
            default:
                throw AcademiaException("Opção Inválida");
        }
    }
    while (opcao != 0); 
}

void interfaceGerenciador(GerenciadorAcademia& gerenciador)
{
    int opcao;
    int id;

    do{
    cout << "====== Bem-vindo Gerenciador ======" << endl;
    cout << "====== MENU ======" << endl;
    cout << "1. Cadastrar Cliente" << endl;
    cout << "2. Consultar Cliente" << endl;
    cout << "3. Atualizar Cliente" << endl;
    cout << "4. Remover Cliente" << endl;
    cout << "5. Cadastrar Instrutor" << endl;
    cout << "6. Consultar Instrutor" << endl;
    cout << "7. Atualizar Instrutor" << endl;
    cout << "8. Remover Instrutor" << endl;
    cout << "0. Retornar" << endl;

    cin >> opcao;

    switch (opcao)
        {
            case 1:
                {
                    cout << "====== Painel de Cadastro (Cliente) ======" << endl;
                    string nome, cpf, email, planoTipo;
                    cout << "Digite o Nome do Cliente: ";
                    cin.ignore();
                    getline(cin, nome);
                    cout << "Digite o CPF: ";
                    getline(cin, cpf);
                    cout << "Digite o Email: ";
                    getline(cin, email);
                    cout << "Digite o Plano (Mensal/Anual): ";
                    getline(cin, planoTipo);

                    Plano* plano = nullptr;
                    if (planoTipo == "Mensal") {
                        plano = new PlanoMensal(100.00, 10.00);
                    } else if (planoTipo == "Anual") {
                        plano = new PlanoAnual(90.00, 0, 0.0);
                    }

                    Cliente* c = new Cliente(nome, cpf, email, plano, "01/01/2026");
                    gerenciador.cadastrarCliente(c);

                    cout << "Cliente cadastrado!" << endl;
                    break;
                    cout << "=========================" << endl;
                    break;
                }
            case 2:
                {
                    cout << "====== Painel de Consulta (Cliente) ======" << endl;
                    cout << "Digite o id do aluno que deseja consultar: ";
                    cin >> id;
                    Cliente* cliente = gerenciador.consultarClienteBase(id);
                    cout << "Nome: " << cliente->getNome() << endl;
                    cout << "CPF: " << cliente->getCpf() << endl;
                    cout << "E-mail: " << cliente->getEmail() << endl;
                    cout << "=========================" << endl; 
                    break;
                }
            case 3:
                {
                    int escolha;
                    cout << "====== Painel de Atualizacao (Cliente) ======" << endl;
                    cout << "Digite o id do aluno que deseja atualizar: ";
                    cin >> id;
                    Cliente* cliente = gerenciador.consultarClienteBase(id);
                    cout << "Nome: " << cliente->getNome() << endl;
                    cout << "CPF: " << cliente->getCpf() << endl;
                    cout << "E-mail: " << cliente->getEmail() << endl;  
                    cout << "Esse e o cliente que deseja atualizar?" << endl;
                    cout << "1. Sim" << endl;
                    cout << "2. Não" << endl;
                    cin >> escolha;
                    if (escolha != 1 && escolha != 2)
                    {
                        throw AcademiaException("Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        int choice;
                        cout << "Qual informação deseja atualizar? (1.nome, 2.cpf, 3.email, 4.plano)" << endl;
                        cin >> choice;
                        switch(choice)
                        {
                            case 1:
                                {
                                    string nomeNovo;
                                    cout << "Digite o novo nome:";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, nomeNovo);
                                    if (nomeNovo.empty())
                                    {
                                        throw AcademiaException("Nome inválido");
                                    }
                                    else
                                    {
                                        cliente->setNome(nomeNovo);  
                                    }
                                    break;
                                }
                            case 2:
                                {
                                    string cpfNovo;
                                    cout << "Digite o novo CPF:";
                                    cliente->setCPF(cpfNovo);  
                                    break;
                                }
                            case 3:
                                {
                                    string emailNovo;
                                    cout << "Digite o novo email:";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, emailNovo);
                                    if (emailNovo.empty())
                                    {
                                        throw AcademiaException("Email inválido");
                                    }
                                    else
                                    {
                                        cliente->setEmail(emailNovo);  
                                    }
                                    break;
                                }
                            case 4:
                                {
                                    string planoNovoTipo;
                                    cout << "Digite o novo tipo de Plano (Mensal/Anual):";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, planoNovoTipo);
                                    if (planoNovoTipo.empty())
                                    {
                                        throw AcademiaException("Tipo de Plano inválido");
                                    }
                                    else
                                    {
                                        Plano* planoNovo = nullptr;
                                        if (planoNovoTipo == "Mensal")
                                        {
                                            double preco, taxa;

                                            cout << "Preco base: ";
                                            cin >> preco;

                                            cout << "Taxa de inscricao: ";
                                            cin >> taxa;

                                            planoNovo = new PlanoMensal(preco, taxa);
                                        }
                                        else if (planoNovoTipo == "Anual")
                                        {
                                            double preco, desconto;
                                            int meses;

                                            cout << "Preco base: ";
                                            cin >> preco;

                                            cout << "Meses de fidelidade: ";
                                            cin >> meses;

                                            cout << "Desconto: ";
                                            cin >> desconto;

                                            planoNovo = new PlanoAnual(preco, meses, desconto);
                                        }
                                        cliente->setPlano(planoNovo);  
                                    }

                                }
                                break;
                        }
                    }
                    cout << "=========================" << endl; 
                    break;
                }    
            case 4:
                {
                    int escolha;
                    cout << "====== Painel de Remocao (Cliente) ======" << endl;
                    cout << "Digite o id do aluno que deseja remover: ";
                    cin >> id;
                    Cliente* cliente = gerenciador.consultarClienteBase(id);
                    cout << "Nome: " << cliente->getNome() << endl;
                    cout << "CPF: " << cliente->getCpf() << endl;
                    cout << "E-mail: " << cliente->getEmail() << endl;  
                    cout << "Esse e o cliente que deseja remover?" << endl;
                    cout << "1. Sim" << endl;
                    cout << "2. Não" << endl;
                    cin >> escolha;
                    if (escolha != 1 && escolha != 2)
                    {
                        throw AcademiaException("Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        gerenciador.removerCliente(cliente->getCpf());
                        cout << "Cliente removido com sucesso" << endl;
                    }
                    cout << "=========================" << endl; 
                    break;
                }
            case 5:
                {
                    cout << "====== Painel de Cadastro (Instrutor) ======" << endl;
                    string nome, cpf, email, cref, especialidade;
                    cout << "Digite o Nome do Instrutor: ";
                    cin.ignore();
                    getline(cin, nome);
                    cout << "Digite o CPF: ";
                    getline(cin, cpf);
                    cout << "Digite o Email: ";
                    getline(cin, email);
                    cout << "Digite o cref: ";
                    getline(cin, cref);
                    cout << "Digite a especialidade: ";
                    getline(cin, especialidade);

                    Instrutor* i = new Instrutor(nome, cpf, email, cref, especialidade);
                    gerenciador.cadastrarInstrutor(i); 

                    cout << "Instrutor cadastrado!" << endl;
                    break;
                    cout << "=========================" << endl;
                    break;
                }
            case 6:
                {
                    string cref;
                    cout << "====== Painel de Consulta (Instrutor) ======" << endl;
                    cout << "Digite o cref do instrutor que deseja consultar: ";
                    cin >> cref;
                    Instrutor* instrutor = gerenciador.consultarInstrutor(cref); 
                    cout << "Nome: " << instrutor->getNome() << endl;
                    cout << "E-mail: " << instrutor->getEmail() << endl;
                    cout << "=========================" << endl; 
                    break;
                }
            case 7:
                {
                    int escolha;
                    string cref;
                    cout << "====== Painel de Atualizacao (Instrutor) ======" << endl;
                    cout << "Digite o cref do instrutor que deseja atualizar: ";
                    cin >> cref;
                    Instrutor* instrutor = gerenciador.consultarInstrutor(cref);
                    cout << "Nome: " << instrutor->getNome() << endl;
                    cout << "CPF: " << instrutor->getCpf() << endl;
                    cout << "E-mail: " << instrutor->getEmail() << endl; 
                    cout << "Esse e o Instrutor que deseja atualizar?" << endl;
                    cout << "1. Sim" << endl;
                    cout << "2. Não" << endl;
                    cin >> escolha;
                    if (escolha != 1 && escolha != 2)
                    {
                        throw AcademiaException("Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        int choice;
                        cout << "Qual informação deseja atualizar? (1.nome, 2.cpf, 3.email, 4.especialidade)" << endl;
                        cin >> choice;
                        switch(choice)
                        {
                            case 1:
                                {
                                    string nomeNovo;
                                    cout << "Digite o novo nome:";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, nomeNovo);
                                    if (nomeNovo.empty())
                                    {
                                        throw AcademiaException("Nome inválido");
                                    }
                                    else
                                    {
                                        instrutor->setNome(nomeNovo);  
                                    }
                                    break;
                                }
                            case 2:
                                {
                                    string cpfNovo;
                                    cout << "Digite o novo CPF:";
                                    instrutor->setCPF(cpfNovo);  
                                    break;
                                }
                            case 3:
                                {
                                    string emailNovo;
                                    cout << "Digite o novo email:";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, emailNovo);
                                    if (emailNovo.empty())
                                    {
                                        throw AcademiaException("Email inválido");
                                    }
                                    else
                                    {
                                        instrutor->setEmail(emailNovo);  
                                    }
                                    break;
                                }
                            case 4:
                                {
                                    string especialidadeNova;
                                    cout << "Digite a nova especialidade:";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    getline(cin, especialidadeNova);
                                    if (especialidadeNova.empty())
                                    {
                                        throw AcademiaException("Tipo de Plano inválido");
                                    }
                                    else
                                    {
                                        instrutor->setEspecialidade(especialidadeNova);  
                                    }

                                }
                                break;
                        }
                    }
                    cout << "=========================" << endl; 
                    break;
                }
            case 8:
                {
                    string cref;
                    int escolha;
                    cout << "====== Painel de Remocao (Instrutor) ======" << endl;
                    cout << "Digite o cref do instrutor que deseja remover: ";
                    cin >> cref;
                    Instrutor* instrutor = gerenciador.consultarInstrutor(cref);
                    cout << "Nome: " << instrutor->getNome() << endl;
                    cout << "CPF: " << instrutor->getCpf() << endl;
                    cout << "E-mail: " << instrutor->getEmail() << endl; 
                    cout << "Esse e o Instrutor que deseja remover?" << endl;
                    cout << "1. Sim" << endl;
                    cout << "2. Não" << endl;
                    cin >> escolha;
                    if (escolha != 1 && escolha != 2)
                    {
                        throw AcademiaException("Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        gerenciador.removerInstrutor(instrutor->getCpf()); 
                        cout << "Instrutor removido com sucesso" << endl;
                    }
                    cout << "=========================" << endl; 
                    break;
                }
            case 0:
                cout << "Retornando ao menu principal..." << endl;
                break;
            default:
                throw AcademiaException("Opção Inválida");
        }

    }
    while (opcao != 0);
}