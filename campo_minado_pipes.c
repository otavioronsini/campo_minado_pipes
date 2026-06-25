#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
 
#define RESET    "\033[0m"
#define VERMELHO "\033[91m"
#define VERDE    "\033[92m"
#define AMARELO  "\033[93m"
#define AZUL     "\033[94m"
#define ROSA     "\033[95m"
#define CIANO    "\033[96m"
#define BRANCO   "\033[97m"

#define N 10 // número de linhas e colunas das matrizes

typedef char byte;

// TODO: criar uma struct com as duas matrizes para ficar mais legível

void zera_mat(byte mat[N][N]) // faz cada posição da matriz = 0
{
    for (byte lin = 0; lin < N; lin++) // linha
    {
        for (byte col = 0; col < N; col++) // coluna
        {
            mat[lin][col] = 0;
        }
    }
}

// TODO: garantir que as minas sejam colocadas depois da primeira posição que o usuário escolher

void coloca_minas(byte mat[N][N]) // coloca 15 minas em posições aleatórioas no campo minado
{
    srand(time(NULL));

    byte minas = 0;
    
    while (minas < 15)
    {
        byte lin = rand() % N;    
        byte col = rand() % N;

        if (mat[lin][col] == -1)
        {
            continue;
        }

        mat[lin][col] = -1;

        minas++;
    }
}



void conta_minas(byte mat[N][N]) // conta quantas minas têm ao redor de cada posição
{
    for (byte lin = 0; lin < N; lin++) // pra cada elemento da matriz que não for uma mina
    {
        for (byte col = 0; col < N; col++)
        {
            if (mat[lin][col] == -1)
            {
                continue;
            }
                
            byte minas = 0;
            
            // da linha e coluna anteriores     [linha da posição atual - 1][coluna da posição atual - 1]
            // até a linha e coluna posteriores [linha da posição atual + 1][coluna da posição atual + 1]
            
            for (byte lin_at = lin - 1; lin_at <= lin + 1; lin_at++) // linha atual
            {
                for (byte col_at = col - 1; col_at <= col + 1; col_at++) // coluna atual
                {
                    // verifica se a posição está fora dos limite da matriz
                    
                    if (lin_at < 0 || lin_at >= N || col_at < 0 || col_at >= N)
                        continue;
                        
                    // verifica se não é uma bomba

                    if (mat[lin_at][col_at] != -1)
                    {
                        continue;        
                    }

                    minas++;
                }
            }
            
            mat[lin][col] = minas;
        }
    }
}

// marca as posições que serão mostradas ao usuário e retorna o valor da posição atual (-1, 0, > 0)

byte set_visivel(byte mat[N][N], byte visivel[N][N], byte usr_lin, byte usr_col)
{
    // se a posição já está visível, não faz nada e retorna o valor dela

    if (visivel[usr_lin][usr_col] == 1)
    {
        return mat[usr_lin][usr_col];
    }

    // se estiver perto de alguma mina (> 0), marca só ela e retorna o número

    if (mat[usr_lin][usr_col] > 0)
    {
        visivel[usr_lin][usr_col] = 1;

        return mat[usr_lin][usr_col];
    }
    
    // se a posição é uma mina (-1), marca todas as células e retorna -1

    if (mat[usr_lin][usr_col] == -1)
    {
        for (byte i = 0; i < N; i++)
        {
            for (byte j = 0; j < N; j++)
            {
                visivel[i][j] = 1;
            }
        }
        return -1;
    }

    // se é uma posição vazia (0), marca as posições ao redor usando recursão
    // verifica as 8 posições ao redor da posição atual
    // se for -1, não faz nada
    // se for > 0, só marca como visível
    // se for 0, marca como visível e chama a função recursivamente

    visivel[usr_lin][usr_col] = 1; // marca a posição atual como visível
        
    for (byte lin_at = usr_lin - 1; lin_at <= usr_lin + 1; lin_at++) // percorre as 8 posições ao redor
    {
        for (byte col_at = usr_col - 1; col_at <= usr_col + 1; col_at++)
        {
            // verifica se a posição está fora dos limites da matriz
                    
            if (lin_at < 0 || lin_at >= N || col_at < 0 || col_at >= N)
            {
                continue;
            }

            if (mat[lin_at][col_at] == -1) // pula se é uma mina
            {
                continue;        
            }

            if (mat[lin_at][col_at] > 0) // se for > 0 só marca como visível
            {
                visivel[lin_at][col_at] = 1;
                    
                continue;
            }

            set_visivel(mat, visivel, lin_at, col_at); // se for 0, marca como visível e chama recursivamente
        }
    }
    
    return 0;
}

void clear()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_mat(byte mat[N][N], byte visivel[N][N])
{
    // número das colunas em azul
    
    printf("\n  ");

    for (byte i = 0; i < N; i++)
    {
        printf("%4d", i);
    }

    printf("\n\n");   
    
    for (byte i = 0; i < N; i++)
    {
        // número das linhas em azul
        
        printf("%-2d", i);

        // numeros de cada posição
        
        for (byte j = 0; j < N; j++)
        {
            if (visivel[i][j] == 0)
            {
                printf("   -");
            }
            else
            { 
                switch (mat[i][j])
                {
                    case -1:
                        printf(VERMELHO "   #" RESET);
                        break;
                    case 0:
                        printf(BRANCO "%4d" RESET, mat[i][j]);
                        break;
                    case 1:
                        printf(AZUL "%4d" RESET, mat[i][j]);
                        break;
                    case 2:
                        printf(CIANO "%4d" RESET, mat[i][j]);
                        break;
                    case 3:
                        printf(VERDE "%4d" RESET, mat[i][j]);
                        break;
                    case 4:
                        printf(AMARELO "%4d" RESET, mat[i][j]);
                        break;
                        
                    default:
                        printf(ROSA "%4d" RESET, mat[i][j]);
                        break;
                }
            }
        }
        
        printf("\n");
    }
}

// cola pra ficar mais fácil de mostrar a vitória

void print_mat_minas(byte mat[N][N])
{
    printf("\n  ");

    // cabeçalho das colunas
    for (byte i = 0; i < N; i++)
        printf("%4d", i);
    printf("\n\n");

    for (byte i = 0; i < N; i++)
    {
        // número da linha
        printf("%-2d", i);

        for (byte j = 0; j < N; j++)
        {
            switch (mat[i][j])
            {
                case -1:
                    printf(VERMELHO "   #" RESET);
                    break;
                case 0:
                    printf(BRANCO "%4d" RESET, mat[i][j]);
                    break;
                case 1:
                    printf(AZUL "%4d" RESET, mat[i][j]);
                    break;
                case 2:
                    printf(CIANO "%4d" RESET, mat[i][j]);
                    break;
                case 3:
                    printf(VERDE "%4d" RESET, mat[i][j]);
                    break;
                case 4:
                    printf(AMARELO "%4d" RESET, mat[i][j]);
                    break;
                default:
                    printf(ROSA "%4d" RESET, mat[i][j]);
                    break;
            }
        }
        printf("\n");
    }
}

byte venceu(byte mat[N][N], byte visivel[N][N])
{
    for (byte i = 0; i < N; i++)
    {
        for (byte j = 0; j < N; j++)
        {
            if (mat[i][j] != -1 && visivel[i][j] == 0)
                return 0;
        }
    }
    
    // se chegou até aqui, todas as posições > 0 estão visíveis
    // mostra todas as minas

    for (byte i = 0; i < N; i++)
        for (byte j = 0; j < N; j++)
            if (mat[i][j] == -1)
                visivel[i][j] = 1;

    return 1;
}

/*
 * ============================================================
 * COMUNICAÇÃO ENTRE PROCESSOS USANDO PIPES
 * ============================================================
 * 
 * Servidor (filho):  processa a lógica do jogo (minas, contagem, etc.)
 * Cliente (pai):     mostra o tabuleiro e recebe os cliques do usuário
 * 
 * Fluxo de comunicação:
 * 
 *   Cliente (Pai)  ←─── pipe_servidor_para_cliente ───  Servidor (Filho)
 *   (mostra jogo)  ──── pipe_cliente_para_servidor ──→  (processa lógica)
 * 
 * pipe_servidor_para_cliente:  Servidor ESCREVE, Cliente LÊ
 * pipe_cliente_para_servidor:  Cliente ESCREVE, Servidor LÊ
 * ============================================================
 */

// Servidor: processa a lógica do jogo
// readfd  = pipe onde o servidor RECEBE dados do cliente (cliente→servidor)
// writefd = pipe onde o servidor ENVIA dados para o cliente (servidor→cliente)

void servidor_campo_minado(int readfd, int writefd)
{
    byte mat[N][N];     // matriz com os valores de -1 a 8 (minas e números)
    byte visivel[N][N]; // matriz com 0 e 1 (o que está visível)
    int status = 0;     // 0 = jogando, 1 = venceu, -1 = perdeu
    
    byte usr_lin;       // linha escolhida pelo usuário
    byte usr_col;       // coluna escolhida pelo usuário
    
    // Inicializa o jogo
    zera_mat(mat);
    zera_mat(visivel);
    coloca_minas(mat);
    conta_minas(mat);
    
    while (1)
    {
        // ============================================================
        // 1. Envia o estado atual do jogo para o cliente
        // ============================================================
        write(writefd, mat, N * N);         // envia a matriz de números
        write(writefd, visivel, N * N);     // envia a matriz de visibilidade
        write(writefd, &status, sizeof(int)); // envia o status do jogo
        
        // Se o jogo terminou (venceu ou perdeu), sai do loop
        if (status != 0)
            break;
        
        // ============================================================
        // 2. Recebe do cliente a posição que o usuário escolheu
        // ============================================================
        read(readfd, &usr_lin, sizeof(byte));
        read(readfd, &usr_col, sizeof(byte));
        
        // ============================================================
        // 3. Processa o clique do usuário
        // ============================================================
        byte res = set_visivel(mat, visivel, usr_lin, usr_col);
        
        // Verifica se o jogador perdeu
        if (res == -1)
        {
            status = -1;  // ATENÇÃO: é "status = -1", NÃO "status =- 1"!
        }
        // Verifica se o jogador venceu
        else if (venceu(mat, visivel))
        {
            status = 1;
        }
        
        // O loop recomeça e envia o novo estado (com o status atualizado)
    }
}

// Cliente: mostra o jogo e recebe os cliques do usuário
// readfd  = pipe onde o cliente RECEBE dados do servidor (servidor→cliente)
// writefd = pipe onde o cliente ENVIA dados para o servidor (cliente→servidor)
void cliente_campo_minado(int readfd, int writefd)
{
    byte mat[N][N];     // matriz com os valores do jogo
    byte visivel[N][N]; // matriz de visibilidade
    int status = 0;     // status do jogo (recebido do servidor)
    
    byte usr_lin;       // linha que o usuário vai digitar
    byte usr_col;       // coluna que o usuário vai digitar
    
    while (1)
    {
        // ============================================================
        // 1. Recebe o estado atual do jogo enviado pelo servidor
        // ============================================================
        read(readfd, mat, N * N);
        read(readfd, visivel, N * N);
        read(readfd, &status, sizeof(int));
        
        // ============================================================
        // 2. Mostra o tabuleiro para o usuário
        // ============================================================
        clear();                      // limpa a tela
        print_mat(mat, visivel);      // mostra o tabuleiro
        // print_mat_minas(mat);
        
        // Se o jogo terminou, mostra a mensagem e sai
        if (status == -1)
        {
            printf(VERMELHO "\nPERDEU!!!\n" RESET);
            break;
        }
        else if (status == 1)
        {
            printf(AZUL "\nVENCEU!!!\n" RESET);
            break;
        }
        
        // ============================================================
        // 3. Pede a posição para o usuário
        // ============================================================
        printf("\nlinha coluna: ");
        
        // Lê as coordenadas
        if (scanf("%hhd %hhd", &usr_lin, &usr_col) != 2)
            break;
        
        // Verifica se as coordenadas são válidas
        if (usr_lin < 0 || usr_lin >= N || usr_col < 0 || usr_col >= N)
            break;
        
        // ============================================================
        // 4. Envia a posição escolhida para o servidor
        // ============================================================
        write(writefd, &usr_lin, sizeof(byte));
        write(writefd, &usr_col, sizeof(byte));
        
        // O loop recomeça: espera o servidor processar e enviar o novo estado
    }
}

int main(void)
{
    int descritor;
    
    // ============================================================
    // PIPES: nomes intuitivos para facilitar o entendimento
    // ============================================================
    // pipe_servidor_para_cliente[0] = leitura  (cliente LÊ do servidor)
    // pipe_servidor_para_cliente[1] = escrita (servidor ESCREVE para o cliente)
    //
    // pipe_cliente_para_servidor[0] = leitura  (servidor LÊ do cliente)
    // pipe_cliente_para_servidor[1] = escrita (cliente ESCREVE para o servidor)
    // ============================================================
    int pipe_servidor_para_cliente[2];
    int pipe_cliente_para_servidor[2];
    
    // Cria os dois pipes
    if (pipe(pipe_servidor_para_cliente) < 0 || pipe(pipe_cliente_para_servidor) < 0)
    {
        printf("Erro na chamada PIPE\n");
        exit(0);
    }
    
    // Cria o processo filho
    if ((descritor = fork()) < 0)
    {
        printf("Erro na chamada FORK\n");
        exit(0);
    }
    
    if (descritor > 0)  // ========================== PROCESSO PAI (CLIENTE)
    {
        // Fecha as pontas dos pipes que o pai NÃO vai usar:
        close(pipe_servidor_para_cliente[1]); // pai não escreve neste pipe
        close(pipe_cliente_para_servidor[0]); // pai não lê deste pipe
        
        // O pai é o CLIENTE:
        // - LÊ  do pipe_servidor_para_cliente[0] (dados vindos do servidor)
        // - ESCREVE no pipe_cliente_para_servidor[1] (envia dados para o servidor)
        cliente_campo_minado(pipe_servidor_para_cliente[0], 
                             pipe_cliente_para_servidor[1]);
        
        // Fecha as pontas restantes
        close(pipe_servidor_para_cliente[0]);
        close(pipe_cliente_para_servidor[1]);
        exit(0);
    }
    else  // ========================== PROCESSO FILHO (SERVIDOR)
    {
        // Fecha as pontas dos pipes que o filho NÃO vai usar:
        close(pipe_servidor_para_cliente[0]); // filho não lê deste pipe
        close(pipe_cliente_para_servidor[1]); // filho não escreve neste pipe
        
        // O filho é o SERVIDOR:
        // - LÊ  do pipe_cliente_para_servidor[0] (dados vindos do cliente)
        // - ESCREVE no pipe_servidor_para_cliente[1] (envia dados para o cliente)
        servidor_campo_minado(pipe_cliente_para_servidor[0], 
                              pipe_servidor_para_cliente[1]);
        
        // Fecha as pontas restantes
        close(pipe_cliente_para_servidor[0]);
        close(pipe_servidor_para_cliente[1]);
        exit(0);
    }
    
    return 0;
}
