#include <stdio.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define MAX_FUNCIONARIOS 5

struct Produto {
    int codigo;
    char nome[50];
    int quantidade;
    float precoUnidade; 
    float precoQuilo;   
};

struct Funcionario {
    char nome[50];
    char sobrenome[50];
    int idade;
    char cargo[50];
    char login[50];
    char senha[50];
};

void salvarProdutos(struct Produto produtos[], int numProdutos) {
    FILE *file = fopen("produtos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
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
    int numProdutos = 0;
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
    }
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
        return 0;
    }
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
    scanf("%d", &codigo);
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
        if (produtos[i].codigo == codigo) {
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

    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    int encontrado = 0;
    for (int i = 0; i < *numProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            encontrado = 1;
            printf("Produto: %s\n", produtos[i].nome);
            printf("Quantidade disponível: %d\n", produtos[i].quantidade);

            printf("Digite a unidade (unidade ou quilograma): ");
            scanf("%s", unidade);

            printf("Digite a quantidade a vender: ");
            scanf("%d", &quantidade);

            if (quantidade > produtos[i].quantidade) {
                printf("Quantidade insuficiente no estoque!\n");
                return;
            }

            float precoVenda = (strcmp(unidade, "quilograma") == 0) ? produtos[i].precoQuilo : produtos[i].precoUnidade;

            float valorTotal = precoVenda * quantidade;
            printf("Preço por %s: %.2f\n", unidade, precoVenda);
            printf("Valor total da venda: %.2f\n", valorTotal);

            printf("Forma de pagamento (cartao/pix/dinheiro): ");
            scanf("%s", formaPagamento);
            printf("Venda realizada com sucesso!\n");

            produtos[i].quantidade -= quantidade;
            salvarProdutos(produtos, *numProdutos);
            break;
        }
    }

    if (!encontrado) {
        printf("Produto não encontrado.\n");
    }
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
    }
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
    }
    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(file, "%s %s %d %s %s %s\n", funcionarios[i].nome, funcionarios[i].sobrenome, funcionarios[i].idade, funcionarios[i].cargo, funcionarios[i].login, funcionarios[i].senha);
    }
    fclose(file);
}

void imprimirEstoque(struct Produto produtos[], int numProdutos) {
    printf("----- Estoque de Produtos -----\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("Código: %d, Produto: %s, Quantidade: %d, Preço por unidade: %.2f, Preço por quilo: %.2f\n",
            produtos[i].codigo, produtos[i].nome, produtos[i].quantidade, produtos[i].precoUnidade, produtos[i].precoQuilo);
    }
}

void imprimirFuncionarios(struct Funcionario funcionarios[], int numFuncionarios) {
    printf("----- Lista de Funcionários -----\n");
    for (int i = 0; i < numFuncionarios; i++) {
        printf("Nome: %s %s, Idade: %d, Cargo: %s, Login: %s\n",
            funcionarios[i].nome, funcionarios[i].sobrenome, funcionarios[i].idade, funcionarios[i].cargo, funcionarios[i].login);
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
    printf("Senha: ");
    scanf("%s", senha);

    if (verificarLogin(login, senha, tipoUsuario)) {
        if (strcmp(tipoUsuario, "admin") == 0) {
            int opcao;
            do {
                printf("----- Menu Admin -----\n");
                printf("1. Adicionar produto\n");
                printf("2. Remover produto\n");
                printf("\n----------------------\n");
                printf("3. Adicionar funcionário\n");
                printf("4. Remover funcionário\n");
                printf("\n----------------------\n");
                printf("5. Imprimir estoque\n");
                printf("6. Imprimir funcionários\n");
                printf("\n----------------------\n");
                printf("7. Realizar venda (Caixa)\n");
                printf("\n----------------------\n");
                printf("0. Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcao);

                switch (opcao) {
                    case 1:
                        adicionarProduto(produtos, &numProdutos);
                        break;
                    case 2:
                        removerProduto(produtos, &numProdutos);
                        break;
                    case 3:
                        adicionarFuncionario(funcionarios, &numFuncionarios);
                        break;
                    case 4:
                        removerFuncionario(funcionarios, &numFuncionarios);
                        break;
                    case 5:
                        imprimirEstoque(produtos, numProdutos);
                        break;
                    case 6:
                        imprimirFuncionarios(funcionarios, numFuncionarios);
                        break;
                    case 7:
                        caixa(produtos, &numProdutos);
                        break;
                    case 0:
                        printf("Programa encerrado!\n");
                        break;
                    default:
                        printf("Opção inválida.\n");
                }
            } while (opcao != 0);
        } else if (strcmp(tipoUsuario, "caixa") == 0) {
            caixa(produtos, &numProdutos);
        } else {
            printf("Acesso negado. Usuário sem permissões.\n");
        }
    } else {
        printf("Login ou senha inválidos.\n");
    }

    return 0;
}

