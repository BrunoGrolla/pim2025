#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <conio.h>

#define MAX_PRODUTOS 100
#define MAX_FUNCIONARIOS 5

void escondeSenha(char senha[], int tamanhoMax) {
    int i = 0;
    char caracSenha;

    while (1) {
        caracSenha = getch();
        if (caracSenha == 13) { 
            senha[i] = '\0';
            break;
        } else if (caracSenha == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < tamanhoMax - 1) {
            senha[i] = caracSenha;
            i++;
            printf("*");
        }
    }
}

struct Produto {
    char nome[50];
    int quantidade;
};

typedef struct {
    char produto[50];
    int tipoVenda;
    float quantidadeProduto;
    float precoUnitario;
    float total;
    char formaPagamento[10];
} Venda;

void pesarProdutos(float pesoProduto, char unidadeProduto[]) {
    if (pesoProduto < 0) {
        printf("Peso inválido!\n");
    } else {
        printf("O peso do produto é: %.2f %s\n", pesoProduto, unidadeProduto);
    }
}

void salvarProdutos(struct Produto produtos[], int numProdutos) {
    FILE *file = fopen("produtos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos!\n");
        return;
    }
    for (int i = 0; i < numProdutos; i++) {
        fprintf(file, "%s %d\n", produtos[i].nome, produtos[i].quantidade);
    }
    fclose(file);
}

int carregarProdutos(struct Produto produtos[]) {
    FILE *file = fopen("produtos.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos!\n");
        return 0;
    }
    int numProdutos = 0;
    while (fscanf(file, "%s %d", produtos[numProdutos].nome, &produtos[numProdutos].quantidade) != EOF) {
        numProdutos++;
    }
    fclose(file);
    return numProdutos;
}

void exibirEstoque(struct Produto produtos[], int numProdutos) {
    printf("\nESTOQUE:\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("%s: %d unidades\n", produtos[i].nome, produtos[i].quantidade);
    }
    printf("\n");
}

void removerProduto(struct Produto produtos[], int *numProdutos) {
    char nome[50];
    printf("Digite o nome do produto que deseja remover: ");
    scanf("%s", nome);
    int encontrado = 0;
    for (int i = 0; i < *numProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
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

void adicionarProduto(struct Produto produtos[], int *numProdutos) {
    if (*numProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos cadastrados atingido.\n");
        return;
    }
    char nome[50];
    int quantidade;
    printf("Digite o nome do produto: ");
    scanf("%s", nome);
    printf("Digite a quantidade em estoque: ");
    scanf("%d", &quantidade);
    for (int i = 0; i < *numProdutos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            produtos[i].quantidade += quantidade;
            printf("Produto já existente! Quantidade atualizada com sucesso.\n");
            salvarProdutos(produtos, *numProdutos);
            return;
        }
    }
    strcpy(produtos[*numProdutos].nome, nome);
    produtos[*numProdutos].quantidade = quantidade;
    (*numProdutos)++;
    salvarProdutos(produtos, *numProdutos);
    printf("Produto adicionado com sucesso!\n");
}

void salvarUsuarios() {
    FILE *file = fopen("usuarios.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários!\n");
        return;
    }
    fprintf(file, "admin admin123\n");
    fprintf(file, "estoque estoque123\n");
    fprintf(file, "caixa caixa123\n");
    fclose(file);
}

int verificarLogin(char *login, char *senha, char *tipoUsuario) {
    FILE *file = fopen("usuarios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários!\n");
        return 0;
    }
    char loginArquivo[50], senhaArquivo[50];
    while (fscanf(file, "%s %s", loginArquivo, senhaArquivo) != EOF) {
        if (strcmp(login, loginArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
            strcpy(tipoUsuario, loginArquivo);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void caixa() {
    Venda venda;
    char continuar;
    float totalCompra = 0;
    do {
        printf("Digite o nome do produto: ");
        scanf(" %[^\n]", venda.produto);
        printf("Escolha o tipo de venda (1- Unidade, 2- Peso): ");
        scanf("%d", &venda.tipoVenda);
        if (venda.tipoVenda == 1) {
            printf("Digite a quantidade de unidades: ");
            scanf("%f", &venda.quantidadeProduto);
        } else if (venda.tipoVenda == 2) {
            printf("Digite o peso em gramas: ");
            scanf("%f", &venda.quantidadeProduto);
        }
        printf("Digite o preço unitário (por unidade ou 1000 gramas): ");
        scanf("%f", &venda.precoUnitario);
        if (venda.tipoVenda == 2) {
            venda.total = (venda.quantidadeProduto / 1000) * venda.precoUnitario;
        } else {
            venda.total = venda.quantidadeProduto * venda.precoUnitario;
        }
        totalCompra += venda.total;
        printf("\nProduto: %s\n", venda.produto);
        if (venda.tipoVenda == 1) {
            printf("Quantidade: %.0f unidades\n", venda.quantidadeProduto);
        } else {
            printf("Peso: %.0f gramas\n", venda.quantidadeProduto);
        }
        printf("Subtotal: %.2f\n", venda.total);
        printf("Deseja adicionar outro produto? (S/N): ");
        scanf(" %c", &continuar);
    } while (continuar == 'S' || continuar == 's');
    printf("\nTotal da compra: R$%.2f\n", totalCompra);
    printf("Forma de pagamento (1- Cartão, 2- Dinheiro, 3- PIX): ");
    int opcaoPagamento;
    scanf("%d", &opcaoPagamento);
    switch (opcaoPagamento) {
        case 1:
            strcpy(venda.formaPagamento, "Cartão");
            break;
        case 2:
            strcpy(venda.formaPagamento, "Dinheiro");
            break;
        case 3:
            strcpy(venda.formaPagamento, "PIX");
            break;
        default:
            printf("Opção inválida!\n");
            return;
    }
    printf("\nCompra finalizada com sucesso! Forma de pagamento: %s\n", venda.formaPagamento);
}

struct Funcionario {
    char nome[50];
    char sobrenome[50];
    int idade;
    char cargo[50];
};

void salvarFuncionarios(struct Funcionario funcionarios[], int numFuncionarios) {
    FILE *file = fopen("funcionarios.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários!\n");
        return;
    }
    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(file, "%s %s %d %s\n", funcionarios[i].nome, funcionarios[i].sobrenome, funcionarios[i].idade, funcionarios[i].cargo);
    }
    fclose(file);
}

int carregarFuncionarios(struct Funcionario funcionarios[]) {
    FILE *file = fopen("funcionarios.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários!\n");
        return 0;
    }
    int numFuncionarios = 0;
    while (fscanf(file, "%s %s %d %s", funcionarios[numFuncionarios].nome, funcionarios[numFuncionarios].sobrenome, &funcionarios[numFuncionarios].idade, funcionarios[numFuncionarios].cargo) != EOF) {
        numFuncionarios++;
    }
    fclose(file);
    return numFuncionarios;
}

void exibirFuncionarios(struct Funcionario funcionarios[], int numFuncionarios) {
    printf("\nLISTA DE FUNCIONÁRIOS:\n");
    for (int i = 0; i < numFuncionarios; i++) {
        printf("%s %s, Idade: %d, Cargo: %s\n", funcionarios[i].nome, funcionarios[i].sobrenome, funcionarios[i].idade, funcionarios[i].cargo);
    }
    printf("\n");
}

void adicionarFuncionario(struct Funcionario funcionarios[], int *numFuncionarios) {
    if (*numFuncionarios >= MAX_FUNCIONARIOS) {
        printf("Limite de funcionários cadastrados atingido.\n");
        return;
    }
    char nome[50], sobrenome[50], cargo[50];
    int idade;
    printf("Digite o nome do funcionário: ");
    scanf("%s", nome);
    printf("Digite o sobrenome do funcionário: ");
    scanf("%s", sobrenome);
    printf("Digite a idade do funcionário: ");
    scanf("%d", &idade);
    printf("Digite o cargo do funcionário: ");
    scanf("%s", cargo);
    strcpy(funcionarios[*numFuncionarios].nome, nome);
    strcpy(funcionarios[*numFuncionarios].sobrenome, sobrenome);
    funcionarios[*numFuncionarios].idade = idade;
    strcpy(funcionarios[*numFuncionarios].cargo, cargo);
    (*numFuncionarios)++;
    salvarFuncionarios(funcionarios, *numFuncionarios);
    printf("Funcionário adicionado com sucesso!\n");
}

void removerFuncionario(struct Funcionario funcionarios[], int *numFuncionarios) {
    char nome[50];
    printf("Digite o nome do funcionário que deseja remover: ");
    scanf("%s", nome);
    int encontrado = 0;
    for (int i = 0; i < *numFuncionarios; i++) {
        if (strcmp(funcionarios[i].nome, nome) == 0) {
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

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    struct Produto produtos[MAX_PRODUTOS];
    int numProdutos = carregarProdutos(produtos);

    struct Funcionario funcionarios[MAX_FUNCIONARIOS];
    int numFuncionarios = carregarFuncionarios(funcionarios);

    salvarUsuarios();

    char login[50], senha[50], tipoUsuario[50];
    printf("Digite seu login: ");
    scanf("%s", login);
    printf("Digite sua senha: ");
    escondeSenha(senha, sizeof(senha));
    printf("\n");

    if (verificarLogin(login, senha, tipoUsuario)) {
        printf("Login bem-sucedido! Tipo de usuário: %s\n", tipoUsuario);
        if (strcmp(tipoUsuario, "admin") == 0) {
            int opcaoAdmin;
            do {
                printf("\nMenu Admin:\n");
                printf("1 - Adicionar Produto\n");
                printf("2 - Remover Produto\n");
                printf("3 - Exibir Estoque\n");
                printf("4 - Adicionar Funcionário\n");
                printf("5 - Remover Funcionário\n");
                printf("6 - Exibir Funcionários\n");
                printf("7 - Sistema de caixa\n");
                printf("0 - Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoAdmin);
                switch (opcaoAdmin) {
                    case 1:
                        adicionarProduto(produtos, &numProdutos);
                        break;
                    case 2:
                        removerProduto(produtos, &numProdutos);
                        break;
                    case 3:
                        exibirEstoque(produtos, numProdutos);
                        break;
                    case 4:
                        adicionarFuncionario(funcionarios, &numFuncionarios);
                        break;
                    case 5:
                        removerFuncionario(funcionarios, &numFuncionarios);
                        break;
                    case 6:
                        exibirFuncionarios(funcionarios, numFuncionarios);
                        break;
                    case 7:
                        caixa();
                        break;
                    case 0:
                        printf("Programa Encerrado!\n");
                        break;
                    default:
                        printf("Opção inválida!\n");
                }
            } while (opcaoAdmin != 0);
        } else if (strcmp(tipoUsuario, "estoque") == 0) {
            int opcaoEstoque;
            do {
                printf("\nMenu Estoque:\n");
                printf("1 - Adicionar Produto\n");
                printf("2 - Remover Produto\n");
                printf("3 - Exibir Estoque\n");
                printf("0 - Sair\n");
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoEstoque);
                switch (opcaoEstoque) {
                    case 1:
                        adicionarProduto(produtos, &numProdutos);
                        break;
                    case 2:
                        removerProduto(produtos, &numProdutos);
                        break;
                    case 3:
                        exibirEstoque(produtos, numProdutos);
                        break;
                    case 0:
                        printf("Programa Encerrado!\n");
                        break;
                    default:
                        printf("Opção inválida!\n");
                }
            } while (opcaoEstoque != 0);
        } else if (strcmp(tipoUsuario, "caixa") == 0) {
            caixa();
        }
    } else {
        printf("Login e/ou senha inválidos!\n");
        printf("Esqueceu a senha? Entre em contato com o seu administrador!");
    }
    return 0;
}
