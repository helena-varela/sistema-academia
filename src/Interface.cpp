#include "Interface.hpp"
#include <iostream>
#include <limits>
#include <iomanip>
#include <functional>
#include "UtilData.hpp"
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::list;
using std::numeric_limits;
using std::streamsize;
using std::string;
using std::vector;

// Le uma linha e, se a validacao lancar uma excecao do sistema, mostra o erro
// e repete apenas aquela pergunta (sem perder os campos ja preenchidos).
static string lerCampo(const string& pergunta, const std::function<void(const string&)>& validar)
{
    string valor;
    while (true)
    {
        cout << pergunta;
        if (!getline(cin, valor))
        {
            throw DadosInvalidosException("entrada", "Entrada encerrada.");
        }
        try
        {
            validar(valor);
            return valor;
        }
        catch (const AcademiaException& e)
        {
            cout << "Erro: " << e.what() << " Tente novamente." << endl;
        }
    }
}

static void validarNaoVazio(const string& campo, const string& valor)
{
    if (valor.empty())
    {
        throw DadosInvalidosException(campo, "O campo " + campo + " nao pode ser vazio.");
    }
}

static void validarFormatoCpf(const string& cpf)
{
    if (!Pessoa::validarCPF(cpf))
    {
        throw DadosInvalidosException("cpf", "CPF invalido (deve ter 11 digitos, somente numeros).");
    }
}

static void validarSenhaNova(const string& senha)
{
    if (senha.size() < 4)
    {
        throw DadosInvalidosException("senha", "A senha deve ter pelo menos 4 caracteres.");
    }
}

static string statusMatriculaTexto(StatusMatricula status)
{
    return Matricula::statusParaTexto(status);
}

static void imprimirPagamento(const Pagamento* p)
{
    cout << std::fixed << std::setprecision(2);
    cout << "[" << p->getCodigo() << "] " << p->getMesReferencia()
         << " | " << p->getTipo()
         << " | Pago em " << p->getDataPagamento()
         << " | Valor base: R$ " << p->getValorBase()
         << " | Total: R$ " << p->calcularValorFinal()
         << " | " << p->getStatusTexto() << endl;
    cout << "      " << p->getDescricao() << endl;
    cout.unsetf(std::ios::fixed);
}

static void imprimirHistorico(const vector<Pagamento*>& historico)
{
    if (historico.empty())
    {
        cout << "Nenhum pagamento registrado." << endl;
        return;
    }
    for (const Pagamento* p : historico)
    {
        imprimirPagamento(p);
    }
}

static int lerCodigoPagamento()
{
    int codigo;
    cout << "Digite o codigo do pagamento: ";
    if (!(cin >> codigo))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw DadosInvalidosException("codigo", "Codigo invalido.");
    }
    return codigo;
}

static void painelRegistrarPagamento(GerenciadorAcademia& gerenciador)
{
    cout << "====== Registrar Pagamento ======" << endl;
    string cpf;
    cout << "CPF do cliente: ";
    cin >> cpf;
    Cliente* cliente = gerenciador.consultarCliente(cpf);
    cout << "Cliente: " << cliente->getNome() << " | " << cliente->getPlanoAtual()->getTipoPlano()
         << " | Matricula " << statusMatriculaTexto(cliente->getMatricula().getStatus()) << endl;

    string mesReferencia;
    cout << "Mes de referencia (MM/AAAA) ou 0 para o mes atual (" << UtilData::mesAtual() << "): ";
    cin >> mesReferencia;
    if (mesReferencia == "0")
    {
        mesReferencia = UtilData::mesAtual();
    }

    int forma;
    cout << "Forma de pagamento (1. Pix, 2. Cartao): ";
    cin >> forma;

    Pagamento* pagamento = nullptr;
    if (forma == 1)
    {
        string chave;
        cout << "Chave Pix: ";
        cin >> chave;
        pagamento = new PagamentoPix(cliente, mesReferencia, UtilData::hoje(), chave);
    }
    else if (forma == 2)
    {
        // Regra: o limite de parcelas vem do plano (polimorfismo de Plano)
        const Plano* plano = cliente->getPlanoAtual();
        int maxParcelas = plano->getMaxParcelasCartao();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string parcelasTexto = lerCampo("Numero de parcelas (max. " + std::to_string(maxParcelas) + " no " +
                                        plano->getTipoPlano() + "): ",
            [maxParcelas, plano](const string& v) {
                size_t lidos = 0;
                int n = 0;
                try {
                    n = std::stoi(v, &lidos);
                } catch (const std::exception&) {
                    lidos = 0;
                }
                if (lidos == 0 || lidos != v.size() || n < 1)
                {
                    throw DadosInvalidosException("parcelas", "Digite um numero de parcelas valido (1 ou mais).");
                }
                if (n > maxParcelas)
                {
                    throw PagamentoRecusadoException("CARTAO", "O " + plano->getTipoPlano() + " permite no maximo " +
                                                     std::to_string(maxParcelas) + " parcela(s).");
                }
            });
        int parcelas = std::stoi(parcelasTexto);
        string digitos = lerCampo("4 ultimos digitos do cartao: ", [](const string& v) {
            if (v.size() != 4 || v.find_first_not_of("0123456789") != string::npos)
            {
                throw DadosInvalidosException("cartao", "Informe exatamente 4 digitos numericos.");
            }
        });
        pagamento = new PagamentoCartao(cliente, mesReferencia, UtilData::hoje(), parcelas, digitos);
    }
    else
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw DadosInvalidosException("forma de pagamento", "Forma de pagamento invalida.");
    }

    gerenciador.registrarPagamento(pagamento); // em caso de erro o gerenciador libera o objeto
    cout << "Pagamento registrado:" << endl;
    imprimirPagamento(pagamento);
    if (pagamento->getStatus() == StatusPagamento::PENDENTE)
    {
        cout << "Pagamento no cartao aguardando autorizacao (use 'Confirmar pagamento de cartao')." << endl;
    }
    cout << "Situacao da matricula: " << statusMatriculaTexto(cliente->getMatricula().getStatus()) << endl;
}

static void painelRelatorioInadimplentes(GerenciadorAcademia& gerenciador)
{
    cout << "====== Relatorio de Inadimplentes ======" << endl;
    vector<Cliente*> inadimplentes = gerenciador.listarInadimplentes();
    if (inadimplentes.empty())
    {
        cout << "Nenhum cliente inadimplente." << endl;
        return;
    }
    double totalEmAberto = 0.0;
    cout << std::fixed << std::setprecision(2);
    for (Cliente* c : inadimplentes)
    {
        double valor = c->getPlanoAtual()->calcularMensalidade();
        totalEmAberto += valor;
        cout << "- " << c->getNome() << " | CPF " << c->getCpf()
             << " | " << c->getPlanoAtual()->getTipoPlano()
             << " | Mensalidade em aberto: R$ " << valor << endl;
    }
    cout << "Total de clientes: " << inadimplentes.size()
         << " | Total em aberto: R$ " << totalEmAberto << endl;
    cout.unsetf(std::ios::fixed);
}

static void lerAlteracaoSenha(string& senhaAtual, string& novaSenha)
{
    string confirmacao;
    cout << "Senha atual: ";
    cin >> senhaAtual;
    cout << "Nova senha (min. 4 caracteres): ";
    cin >> novaSenha;
    cout << "Confirme a nova senha: ";
    cin >> confirmacao;
    if (novaSenha != confirmacao)
    {
        throw DadosInvalidosException("senha", "As senhas nao conferem.");
    }
}

// ===================== menus =====================

void interfaceCliente(GerenciadorAcademia& gerenciador, Cliente& cliente)
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
        cout << "5. Iniciar Treino" << endl;
        cout << "6. Meus Pagamentos" << endl;
        cout << "7. Alterar Senha" << endl;
        cout << "0. Retornar" << endl;

        cin >> opcao;
        if (opcao < 0 || opcao > 7)
        {
            throw DadosInvalidosException("opcao", "Opcao invalida!");
        }

        switch (opcao)
        {
            case 1:
                cout << "====== Seus Dados ======" << endl;
                cout << "Nome: " << cliente.getNome() << endl;
                cout << "CPF: " << cliente.getCpf() << endl;
                cout << "E-mail: " << cliente.getEmail() << endl;
                cout << "=========================" << endl;
                break;
            case 2:
                cout << "====== Seu ID ======" << endl;
                cout << "Matrícula: " << cliente.getCodigoMatricula() << endl;
                cout << "Inicio: " << cliente.getDataInicio() << endl;
                cout << "Situacao: " << statusMatriculaTexto(cliente.getMatricula().getStatus()) << endl;
                cout << "=========================" << endl;
                break;
            case 3:
                cout << "====== Seu Plano Atual ======" << endl;
                if (cliente.getPlanoAtual() != nullptr) {
                    cout << "Plano: " << cliente.getPlanoAtual()->getTipoPlano() << endl;
                } else {
                    cout << "Plano: Nenhum plano ativo ou cadastrado no sistema." << endl;
                }
                cout << "=========================" << endl; 
                break;
            case 4:
                cout << "====== Seu Treino Atual ======" << endl;
                if (cliente.getTreinoDesignado() == nullptr){
                    throw RegistroNaoEncontradoException("Treino", "nenhum treino designado para voce");
                }
                cout << "Foco: " << cliente.getTreinoDesignado()->getFoco() << endl;
                cout << "Exercícios: " << *cliente.getTreinoDesignado() << endl;
                cout << "Duração estimada: " << cliente.getTreinoDesignado()->getDuracao() << " minutos" << endl;
                cout << "=========================" << endl;
                break;
            case 5:
            {
                cout << "Digite o horário de acesso (0-23): ";
                int horaAcesso;
                cin >> horaAcesso;
                cliente.iniciarTreino(horaAcesso); // lanca excecao se o acesso nao for permitido
                {
                    cout << cliente.getPlanoAtual()->getTipoPlano() << ". Acesso liberado!" << endl;
                    cout << "====== Iniciando Treino ======" << endl;
                    cout << "Foco: " << cliente.getTreinoDesignado()->getFoco() << endl;
                    cout << "Exercícios: " << *cliente.getTreinoDesignado() << endl;
                    cout << "Duração estimada: " << cliente.getTreinoDesignado()->getDuracao() << " minutos" << endl;
                    cout << "=========================" << endl;
                }
                break;
            }
            case 6:
                cout << "====== Meus Pagamentos ======" << endl;
                imprimirHistorico(gerenciador.historicoPagamentos(cliente.getCpf()));
                cout << "=========================" << endl;
                break;
            case 7:
            {
                string senhaAtual, novaSenha;
                lerAlteracaoSenha(senhaAtual, novaSenha);
                cliente.alterarSenha(senhaAtual, novaSenha);
                gerenciador.salvarEmArquivo(ARQUIVO_BACKUP);
                cout << "Senha alterada com sucesso!" << endl;
                break;
            }
            case 0:
                cout << "Retornando ao menu principal..." << endl;
                break;
            default:
                throw DadosInvalidosException("opcao", "Opcao invalida!");
        }
    }
    while (opcao != 0); 
}

void interfaceInstrutor(GerenciadorAcademia& gerenciador, Instrutor* instrutor)
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
    cout << "5. Alterar Senha" << endl;
    cout << "0. Retornar" << endl;

    cin >> opcao;
    if (opcao < 0 || opcao > 5)
    {
        throw DadosInvalidosException("opcao", "Opcao invalida!");
    }

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
                    throw DadosInvalidosException("opcao", "Opcao invalida!");
                }
                else if (escolha == 1)
                {
                    instrutor->adicionarAlunoSupervisionado(cliente);
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
                Cliente* cliente = instrutor->consultarAlunoSupervisionado(codigo); 
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
                instrutor->prescreverTreino(cliente, treino);
            
                gerenciador.cadastrarTreino(treino);

                cout << "Treino prescrito com sucesso!" << endl;
                cout << "=========================" << endl; 
                break;
            }
            case 3:
                cout << "====== Painel de Remocao ======" << endl;
                cout << "Digite o id do aluno que deseja remover da supervisao: ";
                cin >> id;
                cliente = instrutor->consultarAlunoSupervisionado(id); 
                cout << "Nome: " << cliente->getNome() << endl;
                cout << "E-mail: " << cliente->getEmail() << endl;
                cout << "Esse e o cliente que deseja remover?" << endl;
                cout << "1. Sim" << endl;
                cout << "2. Não" << endl;
                cin >> escolha;
                if (escolha != 1 && escolha != 2)
                {
                    throw DadosInvalidosException("opcao", "Opcao invalida!");
                }
                else if (escolha == 1)
                {
                    instrutor->removerAlunoSupervisionado(cliente);
                    cout << "Cliente Removido com Sucesso" << endl;
                }
                cout << "=========================" << endl; 
                break;
            case 4:
                {
                    cout << "====== Lista de Alunos Supervisionados ======" << endl;
                    std::list<Cliente*> alunos = instrutor->getAlunosSobSupervisao();

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
            case 5:
                {
                    string senhaAtual, novaSenha;
                    lerAlteracaoSenha(senhaAtual, novaSenha);
                    instrutor->alterarSenha(senhaAtual, novaSenha);
                    cout << "Senha alterada com sucesso!" << endl;
                    break;
                }
            case 0:
                cout << "Retornando ao menu principal..." << endl;
                break;
            default:
                throw DadosInvalidosException("opcao", "Opcao invalida!");
        }
        gerenciador.salvarEmArquivo(ARQUIVO_BACKUP); // salva apos cada operacao
    }
    while (opcao != 0); 
}

void interfaceGerenciador(GerenciadorAcademia& gerenciador)
{
    int opcao;
    string cpf;

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
    cout << "---------- Financeiro ----------" << endl;
    cout << "9. Registrar Pagamento" << endl;
    cout << "10. Confirmar Pagamento de Cartao" << endl;
    cout << "11. Cancelar Pagamento Pendente" << endl;
    cout << "12. Estornar Pagamento" << endl;
    cout << "13. Historico de Pagamentos do Cliente" << endl;
    cout << "14. Atualizar Inadimplencia do Mes" << endl;
    cout << "15. Relatorio de Inadimplentes" << endl;
    cout << "---------- Conta ----------" << endl;
    cout << "16. Alterar Senha do Gerente" << endl;
    cout << "0. Retornar" << endl;

    cin >> opcao;
    if (opcao < 0 || opcao > 16)
    {
        throw DadosInvalidosException("opcao", "Opcao invalida!");
    }

    switch (opcao)
        {
            case 1:
                {
                    cout << "====== Painel de Cadastro (Cliente) ======" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    string nome = lerCampo("Digite o Nome do Cliente: ",
                        [](const string& v) { validarNaoVazio("nome", v); });
                    string cpf = lerCampo("Digite o CPF: ", [&gerenciador](const string& v) {
                        validarFormatoCpf(v);
                        try {
                            gerenciador.consultarCliente(v);
                        } catch (const RegistroNaoEncontradoException&) {
                            return; // CPF livre
                        }
                        throw RegistroDuplicadoException("cliente", "o CPF " + v);
                    });
                    string email = lerCampo("Digite o Email: ",
                        [](const string& v) { validarNaoVazio("email", v); });
                    string planoTipo = lerCampo("Digite o Plano (Mensal/Anual): ", [&gerenciador](const string& v) {
                        delete gerenciador.criarPlanoDoCatalogo(v); // so verifica se o plano existe
                    });
                    string senha = lerCampo("Defina a senha de acesso do cliente (min. 4 caracteres): ",
                        validarSenhaNova);

                    Plano* plano = gerenciador.criarPlanoDoCatalogo(planoTipo);

                    Cliente* c = new Cliente(nome, cpf, email, plano, UtilData::hoje());
                    try {
                        c->definirSenha(senha);
                        gerenciador.cadastrarCliente(c);
                    } catch (...){
                        delete c;
                        throw;
                    }
                    
          

                    cout << "Cliente cadastrado com sucesso!" << endl;
                    cout << "=========================" << endl;
                    break;
                }
            case 2:
                {
                    cout << "====== Painel de Consulta (Cliente) ======" << endl;
                    cout << "Digite o CPF do aluno que deseja consultar: ";
                    cin >> cpf;
                    Cliente* cliente = gerenciador.consultarCliente(cpf);
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
                    cout << "Digite o CPF do aluno que deseja atualizar: ";
                    cin >> cpf;
                    Cliente* cliente = gerenciador.consultarCliente(cpf);
                    cout << "Nome: " << cliente->getNome() << endl;
                    cout << "CPF: " << cliente->getCpf() << endl;
                    cout << "E-mail: " << cliente->getEmail() << endl;  
                    cout << "Esse e o cliente que deseja atualizar?" << endl;
                    cout << "1. Sim" << endl;
                    cout << "2. Não" << endl;
                    cin >> escolha;
                    if (escolha != 1 && escolha != 2)
                    {
                        throw DadosInvalidosException("opcao", "Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        int choice;
                        cout << "Qual informação deseja atualizar? (1.nome, 2.cpf, 3.email, 4.plano, 5.Estado da Matricula)" << endl;
                        cin >> choice;
                        if (choice < 1 || choice > 5)
                        {
                            throw DadosInvalidosException("opcao", "Opcao invalida!");
                        }
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
                                        throw DadosInvalidosException("nome", "Nome inválido");
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
                                    cin >> cpfNovo;
                                    bool cpfEmUso = true;
                                    try { gerenciador.consultarCliente(cpfNovo); }
                                    catch (const RegistroNaoEncontradoException&) { cpfEmUso = false; }

                                    if (cpfEmUso) {
                                        throw RegistroDuplicadoException("cliente", "o CPF " + cpfNovo);
                                    }
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
                                        throw DadosInvalidosException("email", "Email inválido");
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
                                    if (planoNovoTipo.empty() || (planoNovoTipo != "Mensal" && planoNovoTipo != "Anual"))
                                    {
                                        throw DadosInvalidosException("plano", "Tipo de Plano inválido");
                                    }
                                    else
                                    {
                                        // usa o plano do catalogo, o mesmo que e restaurado do backup
                                        Plano* planoNovo = gerenciador.criarPlanoDoCatalogo(planoNovoTipo);
                                        cliente->setPlano(planoNovo);  
                                    }
                                    break;
                                }
                                case 5:
                                {
                                    cout << "Status atual da matricula: " << endl;
                                    auto status = cliente->getMatricula().getStatus();
                                    switch (status)
                                    {
                                        case StatusMatricula::ATIVA:
                                            cout << "ATIVA" << endl;
                                            break;
                                        case StatusMatricula::INADIMPLENTE:
                                            cout << "INADIMPLENTE" << endl;
                                            break;
                                        case StatusMatricula::TRANCADA:
                                            cout << "TRANCADA" << endl;
                                            break;
                                    }
                                    cout << "Digite o novo estado da matricula (1. ATIVA, 2. INADIMPLENTE, 3. TRANCADA): ";
                                    int novoEstado;
                                    cin >> novoEstado;
                                    if (novoEstado < 1 || novoEstado > 3)
                                    {
                                        throw DadosInvalidosException("opcao", "Opcao invalida!");
                                    }
                                    switch (novoEstado)
                                    {
                                        case 1:
                                            cliente->getMatricula().alterarEstado(StatusMatricula::ATIVA);
                                            break;
                                        case 2:
                                            cliente->getMatricula().alterarEstado(StatusMatricula::INADIMPLENTE);
                                            break;
                                        case 3:
                                            cliente->getMatricula().alterarEstado(StatusMatricula::TRANCADA);
                                            break;
                                    }
                                }
                                
                        }
                    }
                    cout << "=========================" << endl; 
                    break;
                }    
            case 4:
                {
                    int escolha;
                    cout << "====== Painel de Remocao (Cliente) ======" << endl;
                    cout << "Digite o CPF do aluno que deseja remover: ";
                    cin >> cpf;
                    Cliente* cliente = gerenciador.consultarCliente(cpf);
                    cout << "Nome: " << cliente->getNome() << endl;
                    cout << "CPF: " << cliente->getCpf() << endl;
                    cout << "E-mail: " << cliente->getEmail() << endl;  
                    cout << "Esse e o cliente que deseja remover?" << endl;
                    cout << "1. Sim" << endl;
                    cout << "2. Não" << endl;
                    cin >> escolha;
                    if (escolha != 1 && escolha != 2)
                    {
                        throw DadosInvalidosException("opcao", "Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        gerenciador.removerCliente(cliente->getCpf());
                        cout << "Cliente removido com sucesso!" << endl;
                    }
                    cout << "=========================" << endl; 
                    break;
                }
            case 5:
                {
                    cout << "====== Painel de Cadastro (Instrutor) ======" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    string nome = lerCampo("Digite o Nome do Instrutor: ",
                        [](const string& v) { validarNaoVazio("nome", v); });
                    string cpf = lerCampo("Digite o CPF: ", [&gerenciador](const string& v) {
                        validarFormatoCpf(v);
                        try {
                            gerenciador.consultarInstrutorPorCpf(v);
                        } catch (const RegistroNaoEncontradoException&) {
                            return; // CPF livre
                        }
                        throw RegistroDuplicadoException("instrutor", "o CPF " + v);
                    });
                    string email = lerCampo("Digite o Email: ",
                        [](const string& v) { validarNaoVazio("email", v); });
                    string cref = lerCampo("Digite o CREF: ", [&gerenciador](const string& v) {
                        validarNaoVazio("CREF", v);
                        try {
                            gerenciador.consultarInstrutor(v);
                        } catch (const RegistroNaoEncontradoException&) {
                            return; // CREF livre
                        }
                        throw RegistroDuplicadoException("instrutor", "o CREF " + v);
                    });
                    string especialidade = lerCampo("Digite a especialidade: ",
                        [](const string& v) { validarNaoVazio("especialidade", v); });
                    string senha = lerCampo("Defina a senha de acesso do instrutor (min. 4 caracteres): ",
                        validarSenhaNova);

                    Instrutor* i = new Instrutor(nome, cpf, email, cref, especialidade);
                    try {
                        i->definirSenha(senha);
                        gerenciador.cadastrarInstrutor(i);
                    } catch (...){
                        delete i;
                        throw;
                    }
                    
                    cout << "Instrutor cadastrado com sucesso!" << endl;
                    cout << "=========================" << endl;
                    break;
                }
            case 6:
                {
                    string cref;
                    cout << "====== Painel de Consulta (Instrutor) ======" << endl;
                    cout << "Digite o cref do instrutor que deseja consultar: ";
                    cin >> cref;
                    if (cref.empty())
                    {
                        throw DadosInvalidosException("cref", "Cref invalido");
                    }
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
                    if (cref.empty())
                    {
                        throw DadosInvalidosException("cref", "Cref invalido");
                    }
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
                        throw DadosInvalidosException("opcao", "Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        int choice;
                        cout << "Qual informação deseja atualizar? (1.nome, 2.cpf, 3.email, 4.especialidade)" << endl;
                        cin >> choice;
                        if (choice < 1 || choice > 4)
                        {
                            throw DadosInvalidosException("opcao", "Opcao invalida!");
                        }
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
                                        throw DadosInvalidosException("nome", "Nome inválido");
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
                                    cin >> cpfNovo;
                                    bool cpfEmUso = true;
                                    try { gerenciador.consultarInstrutorPorCpf(cpfNovo); }
                                    catch (const RegistroNaoEncontradoException&) { cpfEmUso = false; }

                                    if (cpfEmUso) {
                                        throw RegistroDuplicadoException("instrutor", "o CPF " + cpfNovo);
                                    }
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
                                        throw DadosInvalidosException("email", "Email inválido");
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
                                        throw DadosInvalidosException("especialidade", "Especialidade inválida");
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
                    if (cref.empty())
                    {
                        throw DadosInvalidosException("cref", "Cref invalido");
                    }
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
                        throw DadosInvalidosException("opcao", "Opcao invalida!");
                    }
                    else if (escolha == 1)
                    {
                        gerenciador.removerInstrutor(instrutor->getCpf()); 
                        cout << "Instrutor removido com sucesso!" << endl;
                    }
                    cout << "=========================" << endl; 
                    break;
                }
            case 9:
                painelRegistrarPagamento(gerenciador);
                break;
            case 10:
                {
                    cout << "====== Confirmar Pagamento de Cartao ======" << endl;
                    int codigo = lerCodigoPagamento();
                    gerenciador.confirmarPagamento(codigo);
                    cout << "Pagamento confirmado:" << endl;
                    imprimirPagamento(gerenciador.consultarPagamento(codigo));
                    break;
                }
            case 11:
                {
                    cout << "====== Cancelar Pagamento Pendente ======" << endl;
                    int codigo = lerCodigoPagamento();
                    gerenciador.cancelarPagamento(codigo);
                    cout << "Pagamento cancelado." << endl;
                    break;
                }
            case 12:
                {
                    cout << "====== Estornar Pagamento ======" << endl;
                    int codigo = lerCodigoPagamento();
                    gerenciador.estornarPagamento(codigo);
                    Pagamento* p = gerenciador.consultarPagamento(codigo);
                    cout << "Pagamento estornado. Situacao da matricula: "
                         << statusMatriculaTexto(p->getCliente()->getMatricula().getStatus()) << endl;
                    break;
                }
            case 13:
                {
                    cout << "====== Historico de Pagamentos ======" << endl;
                    cout << "CPF do cliente: ";
                    cin >> cpf;
                    Cliente* cliente = gerenciador.consultarCliente(cpf);
                    cout << "Cliente: " << cliente->getNome() << endl;
                    imprimirHistorico(gerenciador.historicoPagamentos(cpf));
                    break;
                }
            case 14:
                {
                    cout << "====== Atualizar Inadimplencia (" << UtilData::hoje() << ") ======" << endl;
                    vector<Cliente*> novos = gerenciador.atualizarInadimplencia(UtilData::hoje());
                    if (novos.empty())
                    {
                        cout << "Nenhum cliente passou para inadimplente." << endl;
                    }
                    for (Cliente* c : novos)
                    {
                        cout << "- " << c->getNome() << " (CPF " << c->getCpf() << ") agora esta INADIMPLENTE." << endl;
                    }
                    break;
                }
            case 15:
                painelRelatorioInadimplentes(gerenciador);
                break;
            case 16:
                {
                    cout << "====== Alterar Senha do Gerente ======" << endl;
                    string senhaAtual, novaSenha;
                    lerAlteracaoSenha(senhaAtual, novaSenha);
                    gerenciador.alterarSenhaGerente(senhaAtual, novaSenha);
                    cout << "Senha alterada com sucesso!" << endl;
                    break;
                }
            case 0:
                cout << "Retornando ao menu principal..." << endl;
                break;
            default:
                throw DadosInvalidosException("opcao", "Opcao invalida!");
        }
        gerenciador.salvarEmArquivo(ARQUIVO_BACKUP); // salva apos cada operacao
    }
    while (opcao != 0);
}