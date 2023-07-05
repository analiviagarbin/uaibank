#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <windows.h>
#include <ctype.h>

/*
Bem-vindo(a) ao UAIBank!
O UAIBank é um sistema bancário que possui cadastro de usuários,
cadastro de múltiplos usuários, listagem por id
transferência de saldo entre eles e remoção de usuário por ID.

Os ID's são gerados automaticamente.

- Para funcionamento, deve conter na pasta do código um arquivo TXT "usuarios.txt"

Integrantes:
Ana Flor de O. Stefani 4770143
Ana Lívia de M. Garbin 14557394

*/

// define as variaveis do usuario
typedef struct
{
    // campos do usuario
    int id;
    char nome[100];
    int idade;
    double saldo_atual;
} Usuario;

// protótipo das funções

// exibe a tabela para seleçõo das opções do usuario
void tabela();

// aloca as informações do usuario armazenadas no arquivo para o array
void carregar_usuario_arquivo();

// faz o inverso de carregar_usuario_arquivo(), atualiza as informações que estão no array para o arquivo ao final das alterações
void carregar_usuario_array();

// verificação de memória disponível
// parâmetro: ponteiro do array, se nulo (memória insuficiente)
void memoria(Usuario *c_memoria);

// adiciona um usuario por vez
void criar_usuario();

// adiciona a quantidade de usuarios determinada na execução
void pesquisar_id();

// realiza a transferencia de saldo entre contador por meio do id
void transferir();

// remove um usuario existente por meio do id
void remover_id();

// inicializa um ponteiro do tipo Usuario (struct) apontando para o nulo
// utilizado durante a execução do codigo no array
Usuario *infos_usuario = NULL;

// variaveis para contagem ao longo do id, vão auxiliar na geração automatica de ids
// o ultimoId armazena os ids dos ultimos usuarios criados, para que os proximos sejam sempre ultimoId+1
// iId é usado como contador
int iId = 0, ultimoId = 0;

int main()
{
    // formatação para acentuação e decimais com ","
    setlocale(LC_ALL, "Portuguese");

    // determina a memoria inicial do array
    //  vai ser alterada durante a execução
    infos_usuario = (Usuario *)malloc(0 * sizeof(Usuario));
    memoria(infos_usuario);

    // inicia o codigo carregando os arquivos que ja existem no arquivo para o array
    carregar_usuario_arquivo();

    int opcao;

    printf("------------------------------ UAIBank ------------------------------");

    do
    {
        tabela();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 0:
            break;
        case 1:
            criar_usuario();
            break;
        case 2:
            criar_usuarios();
            break;
        case 3:
            pesquisar_id(infos_usuario);
            break;
        case 4:
            transferir();
            break;
        case 5:
            remover_id();
            break;
        default:
            printf("\nOpção inválida.\n");
            break;
        }
    } while (opcao != 0);

    // atualiza as informaçães do arquivo com as atualizações feitas no array
    carregar_usuario_array();

    // limpa o ponteiro
    free(infos_usuario);
    return 0;
}

void carregar_usuario_arquivo()
{
    // abre o arquivo para leitura
    FILE *arq_usuario = fopen("usuarios.txt", "r");

    // verificação se o arquivo está disponivel
    if (arq_usuario == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        system("pause");
        exit(1);
    }

    Usuario novo_usuario;

    // fscanf armazena quantas atribuições foram feitas com sucesso
    // no caso, se for igual a 4 (nome, idade, saldo_atual e id), as atribuições estao corretas
    while (fscanf(arq_usuario, "%s %d %lf %d", novo_usuario.nome, &novo_usuario.idade, &novo_usuario.saldo_atual, &novo_usuario.id) == 4)
    {
        iId++;
        if (iId > ultimoId)
        {
            ultimoId = iId;
        }
        infos_usuario = (Usuario *)realloc(infos_usuario, iId * sizeof(Usuario));
        infos_usuario[iId - 1] = novo_usuario;
    }
    // fecha o arquivo
    fclose(arq_usuario);
}

void carregar_usuario_array()
{
    FILE *arq_usuario = fopen("usuarios.txt", "w");
    if (arq_usuario == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        system("pause");
        exit(1);
    }

    for (int i = 0; i < iId; i++)
    {
        // escreve as informações no arquivo txt com a formatação certa
        fprintf(arq_usuario, "%s %d %.2f %d\n", infos_usuario[i].nome, infos_usuario[i].idade, infos_usuario[i].saldo_atual, infos_usuario[i].id);
    }

    fclose(arq_usuario);
}

void memoria(Usuario *c_memoria)
{
    if (c_memoria == NULL)
    {
        printf("Memória insuficiente\n");
        system("pause");
        exit(1);
    }
}

void tabela()
{
    printf("\n\n(1) - Cadastrar um usuário.\n");
    printf("(2) - Cadastrar vários usuários.\n");
    printf("(3) - Buscar usuário por ID.\n");
    printf("(4) - Fazer transferência.\n");
    printf("(5) - Apagar usuário por ID.\n");
    printf("(0) - Sair.\n\n");
}

void criar_usuario()
{

    printf("\nCriar novo usuário:\n");

    Usuario novo_usuario;

    printf("Insira o nome: ");

    fflush(stdin);

    // verificação de erro de alocação
    if (fgets(novo_usuario.nome, sizeof(novo_usuario.nome), stdin) == NULL)
    {
        printf("Erro ao ler o nome do usuário.\n");
        return;
    }

    // verifica se o usuário digitou mais de 100 caracteres e devolve um erro
    int tam = 0;
    tam = strlen(novo_usuario.nome);
    if (tam >= 98)
    {
        printf("Caracteres de nome excedidos (100). Tente novamente.\n");
        return;
    }

    // limpa as linhas e espaço a mais
    char *p;
    if (p = strchr(novo_usuario.nome, '\n'))
    {
        *p = 0;
    }
    else
    {
        scanf("%*[^\n]");
        scanf("%*c");
    }
    novo_usuario.nome[strcspn(novo_usuario.nome, "\n")] = '\0';

    // recebe a idade do usuario
    // verifica se a idade é diferente de um int
    printf("Insira a idade: ");

    if (scanf("%d", &novo_usuario.idade) != 1)
    {
        printf("Erro ao ler a idade do usuário.\n");
        return;
    }

    // verifica errro de idade menor que 0
    if (novo_usuario.idade <= 0)
    {
        printf("A idade não pode ser igual ou menor que 0. Tente novamente.\n");
        return;
    }

    // recebe o saldo do usuario

    printf("Insira o saldo atual: ");
    // verifica se o saldo é diferente de um int
    if (scanf("%lf", &novo_usuario.saldo_atual) != 1)
    {
        printf("Erro ao ler o saldo do usuário.\n");
        return;
    }

    // verifica erro de saldo menor que 0
    if (novo_usuario.saldo_atual <= 0)
    {
        printf("O saldo não pode ser menor ou igual a 0. Tente novamente.\n");
        return;
    }

    // processo para criação do id automatico
    iId++;

    if (iId > ultimoId)
    {
        ultimoId = iId;
    }
    else
    {
        ultimoId++;
    }

    // atualiza o ultimo id
    novo_usuario.id = ultimoId;

    // atualiza a memoria para a nova quantidade de usuarios cadastrados (quantidade de id's armazenados)
    infos_usuario = (Usuario *)realloc(infos_usuario, iId * sizeof(Usuario));
    memoria(infos_usuario);

    infos_usuario[iId - 1] = novo_usuario;

    printf("\nUsuário %s de ID %d adicionado com sucesso!\n", novo_usuario.nome, novo_usuario.id);
}

void criar_usuarios()
{
    int qnt;
    printf("\nEscreva a quantidade de usuários que deseja adicionar: ");
    scanf("%d", &qnt);
    if (qnt <= 0)
    {
        printf("Você deve digitar uma quantidade maior que 0.");
        return;
    }
    for (int i = 0; i < qnt; i++)
    {
        // utiliza a função de criar usuarios em looping
        criar_usuario();
    }
}

void pesquisar_id()
{

    int pesq_id;

    // recebe o id a ser procurado
    printf("\nDigite o ID do usuário: ");
    scanf("%d", &pesq_id);

    // verifica se o id existe
    // se for menor que 0 não existe
    // se maior que o ultimo cadastrado também nao existe
    if (pesq_id > ultimoId || pesq_id <= 0)
    {
        printf("O ID não existe.");
        return;
    }

    // procura por um id compativel
    for (int i = 0; i <= pesq_id; i++)
    {
        if (infos_usuario[i].id == pesq_id)
        {
            printf("O usuário %s tem saldo de R$%.2lf\n", infos_usuario[i].nome, infos_usuario[i].saldo_atual);
            return;
        }
    }

    // resultado em caso de id's removidos
    printf("\nUsuário não encontrado.");
    return;
}

void transferir()
{
    int id_origem, id_destino, index_origem, index_destino;

    // verificação
    int id_origem_encontrado = 0, id_destino_encontrado = 0;

    // valor da transferencia
    float valor;

    printf("\nID de origem: ");
    scanf("%d", &id_origem);
    printf("\nID de destino: ");
    scanf("%d", &id_destino);
    printf("\nValor da transferência: ");
    scanf("%f", &valor);
    if (valor <= 0)
    {
        printf("O saldo para transferência deve ser maior que 0. Tente novamente.\n");
        return;
    }

    if (id_origem > 0 && id_destino > 0)
    {
        // busca a localização dos ids no array
        for (int i = 0; i < iId; i++)
        {
            if (infos_usuario[i].id == id_origem)
            {
                index_origem = i;
                id_origem_encontrado = 1;
            }
            if (infos_usuario[i].id == id_destino)
            {
                index_destino = i;
                id_destino_encontrado = 1;
            }
        }
    }

    if ((id_origem_encontrado == 1) && (id_destino_encontrado == 1))
    {
        // verifica se o valor está disponivel
        if (infos_usuario[index_origem].saldo_atual - valor >= 0)
        {
            infos_usuario[index_origem].saldo_atual = infos_usuario[index_origem].saldo_atual - valor;
            infos_usuario[index_destino].saldo_atual = infos_usuario[index_destino].saldo_atual + valor;
            printf("\nTransferência realizada com sucesso.\n");
        }
        else
        {
            printf("\nSaldo insuficiente na conta de origem. Tente novamente.\n");
        }
    }
    else
    {
        printf("\nUsuário não encontrado. Tente novamente.\n");
    }
}

void remover_id()
{
    int id_remover, id_check = 0;

    // armazena a localização do id a ser removido
    int loc_remover;

    printf("\nDigite o ID do usuário a ser removido: ");

    if (scanf("%d", &id_remover) != 1)
    {
        printf("Erro ao ler o ID.\n");
        return;
    }

    if (id_remover > 0)
    {
        // verifica se o id esta disponivel
        for (int i = 0; i < iId; i++)
        {
            if (infos_usuario[i].id == id_remover)
            {
                // busca a localização do id no array
                loc_remover = i;
                id_check = 1;
            }
        }
    }

    if (id_check == 1)
    {
        // move as informações uma posição para a esquerda da localização até o contador do id
        for (int i = loc_remover; i < iId; i++)
        {
            infos_usuario[i] = infos_usuario[i + 1];
        }

        iId--;

        // atualiza a memoria para a nova quantidade de usuarios
        if (iId == 0)
        {
            infos_usuario = (Usuario *)realloc(infos_usuario, 0);
        }
        else
        {
            infos_usuario = (Usuario *)realloc(infos_usuario, iId * sizeof(Usuario));
            memoria(infos_usuario);
        }

        printf("Usuário removido com sucesso!\n");
    }
    else
    {
        // usuario já removido ou não existente
        printf("Usuário não encontrado.\n");
    }
}
