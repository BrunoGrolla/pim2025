#include <stdio.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUTOS 10         // Define o máximo de produtos que pode ser registrado
#define MAX_FUNCIONARIOS 3      // Define o máximo de funcionários que pode ser registrado

struct Produto {
    int codigo;
    char nome[50];
    int quantidade;                                 // Estrutura de cadastro de um produto
    float precoUnidade; 
    float precoQuilo;   
};

struct Funcionario {
    char nome[50];
    char sobrenome[50];
    int idade;                                     // Estrutura de cadastro de um funcionário
    char cargo[50];
    char login[50];
    char senha[50];
};

void limparConsole() {
    system("cls");  // Comando para limpar o console
}

void salvarProdutos(struct Produto produtos[], int numProdutos) {
    FILE *file = fopen("produtos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;                                                                     // Função para salvar os produtos
    }
    for (int i = 0; i < numProdutos; i++) {
        fprintf(file, "%d %s %d %.2f %.2f\n", produtos[i].codigo, produtos[i].nome, produtos[i].quantidade, produtos[i].precoUnidade, produtos[i].precoQuilo);
    }
    fclose(file);
}

int carregarProdutos(struct Produto produtos[]) {
    FILE *file = fopen("produtos.txt", "r");
    if (file == NULL) {
        return 0;
    }
    int numProdutos = 0;                                                    // Função para imprimir os produtos em um arquivo txt
    while (fscanf(file, "%d %s %d %f %f", &produtos[numProdutos].codigo, produtos[numProdutos].nome, &produtos[numProdutos].quantidade, &produtos[numProdutos].precoUnidade, &produtos[numProdutos].precoQuilo) != EOF) {
        numProdutos++;
    }
    fclose(file);
    return numProdutos;
}

int carregarFuncionarios(struct Funcionario funcionarios[]) {
    FILE *file = fopen("funcionarios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return 0;
    }                                                                                      // Função para imprimir os funcionários em um arquivo txt
    int numFuncionarios = 0;
    while (fscanf(file, "%s %s %d %s %s %s", funcionarios[numFuncionarios].nome, funcionarios[numFuncionarios].sobrenome, &funcionarios[numFuncionarios].idade, funcionarios[numFuncionarios].cargo, funcionarios[numFuncionarios].login, funcionarios[numFuncionarios].senha) != EOF) {
        numFuncionarios++;
    }
    fclose(file);
    return numFuncionarios;
}

int verificarLogin(char *login, char *senha, char *tipoUsuario) {
    if (strcmp(login, "admin") == 0 && strcmp(senha, "admin123") == 0) {
        strcpy(tipoUsuario, "admin");
        return 1;
    }

    FILE *file = fopen("funcionarios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return 0;                                                                             // Função para verificar se o login digitado é o mesmo
    }                                                                                         // impresso no arquivo txt
    char loginArquivo[50], senhaArquivo[50], cargo[50];
    while (fscanf(file, "%*s %*s %*d %s %s %s", cargo, loginArquivo, senhaArquivo) != EOF) {
        if (strcmp(login, loginArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
            strcpy(tipoUsuario, cargo);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void adicionarProduto(struct Produto produtos[], int *numProdutos) {
    if (*numProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido.\n");
        return;
    }
    char nome[50];
    int quantidade, codigo;
    float precoUnidade, precoQuilo;
    printf("Digite o código do produto: ");
    scanf("%d", &codigo);                                                       // Função para cadastrar produtos
    printf("Digite o nome do produto: ");
    scanf("%s", nome);
    printf("Digite a quantidade em estoque: ");
    scanf("%d", &quantidade);
    printf("Digite o preço por unidade: ");
    scanf("%f", &precoUnidade);
    printf("Digite o preço por quilograma: ");
    scanf("%f", &precoQuilo);
    produtos[*numProdutos].codigo = codigo;
    strcpy(produtos[*numProdutos].nome, nome);
    produtos[*numProdutos].quantidade = quantidade;
    produtos[*numProdutos].precoUnidade = precoUnidade;
    produtos[*numProdutos].precoQuilo = precoQuilo;
    (*numProdutos)++;
    salvarProdutos(produtos, *numProdutos);
    printf("Produto adicionado com sucesso!\n");
}

void removerProduto(struct Produto produtos[], int *numProdutos) {
    int codigo;
    printf("Digite o código do produto que deseja remover: ");
    scanf("%d", &codigo);
    int encontrado = 0;
    for (int i = 0; i < *numProdutos; i++) {
        if (produtos[i].codigo == codigo) {                                       // Função para remover produtos
            for (int j = i; j < *numProdutos - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            (*numProdutos)--;
            encontrado = 1;
            break;
        }
    }
    if (encontrado) {
        salvarProdutos(produtos, *numProdutos);
        printf("Produto removido com sucesso!\n");
    } else {
        printf("Produto não encontrado.\n");
    }
}

void caixa(struct Produto produtos[], int *numProdutos) {
    int codigo, quantidade;
    char unidade[10], formaPagamento[20];
    float valorTotalCompra = 0.0;

    struct ItemVendido {
        char nome[50];
        int quantidade;
        float precoUnitario;
        float valorTotal;
    } itensVendidos[100];
    int numItensVendidos = 0;

    printf("--------- Caixa ---------\n");

    while (1) {
        printf("Digite o código do produto (ou -1 para finalizar): ");
        scanf("%d", &codigo);

        if (codigo == -1) break;

        int encontrado = 0;
        for (int i = 0; i < *numProdutos; i++) {
            if (produtos[i].codigo == codigo) {
                encontrado = 1;
                printf("Produto: %s\n", produtos[i].nome);
                printf("Quantidade disponível: %d\n", produtos[i].quantidade);

                printf("Digite a unidade (unidade ou quilograma): ");                   // Função de caixa
                scanf("%s", unidade);

                printf("Digite a quantidade a vender: ");
                scanf("%d", &quantidade);

                if (quantidade > produtos[i].quantidade) {
                    printf("Quantidade insuficiente no estoque!\n");
                    return;
                }

                float precoVenda = (strcmp(unidade, "quilograma") == 0) ? produtos[i].precoQuilo : produtos[i].precoUnidade;
                float valorProduto = precoVenda * quantidade;

                printf("Preço por %s: %.2f\n", unidade, precoVenda);
                printf("Valor total do produto: %.2f\n", valorProduto);

                valorTotalCompra += valorProduto;

                produtos[i].quantidade -= quantidade;
                salvarProdutos(produtos, *numProdutos);

                strcpy(itensVendidos[numItensVendidos].nome, produtos[i].nome);
                itensVendidos[numItensVendidos].quantidade = quantidade;
                itensVendidos[numItensVendidos].precoUnitario = precoVenda;            
                itensVendidos[numItensVendidos].valorTotal = valorProduto;
                numItensVendidos++;

                break;
            }
        }

        if (!encontrado) {
            printf("Produto não encontrado.\n");
        }
    }

    printf("Forma de pagamento (cartao/pix/dinheiro): ");
    scanf("%s", formaPagamento);

    printf("\n---- Recibo ----\n");
    printf("Forma de pagamento: %s\n", formaPagamento);
    printf("Itens:\n");
                                                                    // Função de recibo dentro da função de caixa
    for (int i = 0; i < numItensVendidos; i++) {
        printf("%s - Quantidade: %d, Preço Unitário: %.2f, Total: %.2f\n",
               itensVendidos[i].nome,
               itensVendidos[i].quantidade,
               itensVendidos[i].precoUnitario,
               itensVendidos[i].valorTotal);
    }

    printf("Valor total da compra: %.2f\n", valorTotalCompra);
    printf("Compra finalizada com sucesso!\n");
}

void adicionarFuncionario(struct Funcionario funcionarios[], int *numFuncionarios) {
    if (*numFuncionarios >= MAX_FUNCIONARIOS) {
        printf("Limite de funcionários atingido.\n");
        return;
    }
    char nome[50], sobrenome[50], cargo[50], login[50], senha[50];
    int idade;
    printf("Digite o nome do funcionário: ");
    scanf("%s", nome);
    printf("Digite o sobrenome do funcionário: ");
    scanf("%s", sobrenome);
    printf("Digite a idade do funcionário: ");
    scanf("%d", &idade);
    printf("Digite o cargo do funcionário (estoque/caixa): ");
    scanf("%s", cargo);
    printf("Digite o login do funcionário: ");
    scanf("%s", login);
    printf("Digite a senha do funcionário: ");
    scanf("%s", senha);
                                                                        // Função para cadastro de funcionários
    strcpy(funcionarios[*numFuncionarios].nome, nome);
    strcpy(funcionarios[*numFuncionarios].sobrenome, sobrenome);
    funcionarios[*numFuncionarios].idade = idade;
    strcpy(funcionarios[*numFuncionarios].cargo, cargo);
    strcpy(funcionarios[*numFuncionarios].login, login);
    strcpy(funcionarios[*numFuncionarios].senha, senha);
    (*numFuncionarios)++;

    salvarFuncionarios(funcionarios, *numFuncionarios);
    printf("Funcionário adicionado com sucesso!\n");
}

void removerFuncionario(struct Funcionario funcionarios[], int *numFuncionarios) {
    char login[50];
    printf("Digite o login do funcionário que deseja remover: ");
    scanf("%s", login);
    int encontrado = 0;
    for (int i = 0; i < *numFuncionarios; i++) {
        if (strcmp(funcionarios[i].login, login) == 0) {
            for (int j = i; j < *numFuncionarios - 1; j++) {
                funcionarios[j] = funcionarios[j + 1];
            }
            (*numFuncionarios)--; 
            encontrado = 1;
            break;
        }
    }                                                                       // Função para remover funcionários
    if (encontrado) {
        salvarFuncionarios(funcionarios, *numFuncionarios);
        printf("Funcionário removido com sucesso!\n");
    } else {
        printf("Funcionário não encontrado.\n");
    }
}

void salvarFuncionarios(struct Funcionario funcionarios[], int numFuncionarios) {
    FILE *file = fopen("funcionarios.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }                                                                      // Função para imprimir os funcionários em um arquivo txt
    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(file, "%s %s %d %s %s %s\n", funcionarios[i].nome, funcionarios[i].sobrenome, funcionarios[i].idade, funcionarios[i].cargo, funcionarios[i].login, funcionarios[i].senha);
    }
    fclose(file);
}

void imprimirEstoque(struct Produto produtos[], int numProdutos) {
    printf("----- Estoque de Produtos -----\n");
    for (int i = 0; i < numProdutos; i++) {                             // Função para imprimir o estoque do hortifruti atualizado
        printf("Código: %d, Produto: %s, Quantidade: %d, Preço por unidade: %.2f, Preço por quilo: %.2f\n",
            produtos[i].codigo, produtos[i].nome, produtos[i].quantidade, produtos[i].precoUnidade, produtos[i].precoQuilo);
    }
}

void imprimirFuncionarios(struct Funcionario funcionarios[], int numFuncionarios) {
    printf("----- Lista de Funcionários -----\n");
    for (int i = 0; i < numFuncionarios; i++) {                                // Função para exibir funcionários registrados
        printf("Nome: %s %s, Idade: %d, Cargo: %s, Login: %s\n",
            funcionarios[i].nome, funcionarios[i].sobrenome, funcionarios[i].idade, funcionarios[i].cargo, funcionarios[i].login);
    }
}

void lerSenha(char *senha) {
    int i = 0;
    char ch;
    while(1){
        ch = getch();
        if(ch == '\r'){
            senha[i] = '\0';
            break;                                  // Função para mascaramento de senha
        } else if (ch == '\b' && i > 0){
            i--;
            printf("\b \b");
        }else if (ch != '\b'){
            senha[i++] = ch;
            printf("*");
        }
    }
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    struct Produto produtos[MAX_PRODUTOS];
    struct Funcionario funcionarios[MAX_FUNCIONARIOS];
    int numProdutos = carregarProdutos(produtos);
    int numFuncionarios = carregarFuncionarios(funcionarios);

    char login[50], senha[50], tipoUsuario[50];
    printf("Login: ");
    scanf("%s", login);
    printf("Senha: ");                                  // Função de login e senha com o mascaramento de senha
    lerSenha(senha);
    printf("\n");

if (verificarLogin(login, senha, tipoUsuario)) {
    if (strcmp(tipoUsuario, "admin") == 0) {
        int opcaoPrincipal;
        do {
            limparConsole();
            printf("-------- Menu Admin --------\n");
            printf("1. Menu de Produtos\n");
            printf("2. Menu de Funcionários\n");
            printf("3. Menu de Caixa\n");
            printf("0. Sair\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcaoPrincipal);

            switch (opcaoPrincipal) {
                case 1: {
                    int opcaoProdutos;
                    do {
                        limparConsole();
                        printf("\n---- Menu de Produtos ----\n");
                        printf("1. Adicionar produto\n");
                        printf("2. Remover produto\n");
                        printf("3. Imprimir estoque\n");
                        printf("0. Voltar\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcaoProdutos);

                        switch (opcaoProdutos) {
                            case 1:
                                adicionarProduto(produtos, &numProdutos);
                                break;
                            case 2:
                                removerProduto(produtos, &numProdutos);
                                break;
                            case 3:
                                imprimirEstoque(produtos, numProdutos);
                                break;
                            case 0:
                                printf("Voltando ao menu principal...\n");
                                break;
                            default:
                                printf("Opção inválida!\n");                        // MENU DO ADMIN
                        }
                    } while (opcaoProdutos != 0);
                    break;
                }

                case 2: {
                    int opcaoFuncionarios;
                    do {
                        limparConsole();
                        printf("\n---- Menu de Funcionários ----\n");
                        printf("1. Adicionar funcionário\n");
                        printf("2. Remover funcionário\n");
                        printf("3. Imprimir lista de funcionários\n");
                        printf("0. Voltar\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcaoFuncionarios);

                        switch (opcaoFuncionarios) {
                            case 1:
                                adicionarFuncionario(funcionarios, &numFuncionarios);
                                break;
                            case 2:
                                removerFuncionario(funcionarios, &numFuncionarios);
                                break;
                            case 3:
                                imprimirFuncionarios(funcionarios, numFuncionarios);
                                break;
                            case 0:
                                printf("Voltando ao menu principal...\n");
                                break;
                            default:
                                printf("Opção inválida!\n");
                        }
                    } while (opcaoFuncionarios != 0);
                    break;
                }

                case 3:
                    limparConsole();
                    caixa(produtos, &numProdutos);
                    break;

                case 0:
                    printf("Programa encerrado!\n");
                    break;

                default:
                    limparConsole();
                    printf("Opção inválida!\n");
            }
        } while (opcaoPrincipal != 0);
    } else if (strcmp(tipoUsuario, "caixa") == 0) {                     // Menu de caixa
        caixa(produtos, &numProdutos);
    } else if (strcmp(tipoUsuario, "estoque") == 0) {
        int opcaoEstoque;
        do {
            limparConsole();
            printf("-------- Menu Estoque --------\n");
            printf("1. Adicionar produto\n");
            printf("2. Remover produto\n");
            printf("3. Imprimir estoque\n");
            printf("0. Encerrar programa\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcaoEstoque);

            switch (opcaoEstoque) {                                                   // Menu do estoque
                case 1:
                    adicionarProduto(produtos, &numProdutos);
                    break;
                case 2:
                    removerProduto(produtos, &numProdutos);
                    break;
                case 3:
                    imprimirEstoque(produtos, numProdutos);
                    break;
                case 0:
                    printf("Programa Encerrado!\n");
                    break;
                default:
                    limparConsole();
                    printf("Opção inválida!\n");
            }
        } while (opcaoEstoque != 0);
    }
} else {
    limparConsole();
    printf("Login ou senha inválidos!\n");                      // Retorno de erro caso login ou senha inválidos
    printf("Em caso de perda de login ou senha, entre em contato com seu administrador!\n");
}

    return 0;
}


