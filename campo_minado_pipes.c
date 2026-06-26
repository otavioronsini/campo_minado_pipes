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

typedef struct
{
    byte minas[N][N];     // matriz com o número de minas ao redor de cada posição (-1 se for uma mina)
    byte visivel[N][N];   // matriz que indica se a posição é visível ao usuário
    byte status;          // estado atual do jogo, 0 = jogando, 1 = venceu, -1 = perdeu
} campo_minado;

void zera_minas(byte minas[N][N]) // faz cada posição da matriz = 0
{
    for (byte lin = 0; lin < N; lin++) // linha
    {
        for (byte col = 0; col < N; col++) // coluna
        {
            minas[lin][col] = 0;
        }
    }
}


void coloca_minas(byte minas[N][N]) // coloca 15 minas em posições aleatórioas no campo minado
{
    srand(time(NULL));

    byte minas_colocadas = 0;
    
    while (minas_colocadas < 15)
    {
        byte lin = rand() % N;    
        byte col = rand() % N;

        if (minas[lin][col] == -1)
        {
            continue;
        }

        minas[lin][col] = -1;

        minas_colocadas++;
    }
}

void conta_minas(byte minas[N][N]) // conta quantas minas têm ao redor de cada posição
{
    for (byte lin = 0; lin < N; lin++) // pra cada elemento da matriz que não for uma mina
    {
        for (byte col = 0; col < N; col++)
        {
            if (minas[lin][col] == -1)
            {
                continue;
            }
                
            byte minas_contadas = 0;
            
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

                    if (minas[lin_at][col_at] != -1)
                    {
                        continue;        
                    }

                    minas_contadas++;
                }
            }
            
            minas[lin][col] = minas_contadas;
        }
    }
}

// marca as posições que serão mostradas ao usuário e retorna o valor da posição atual (-1, 0, > 0)

byte set_visivel(byte minas[N][N], byte visivel[N][N], byte usr_lin, byte usr_col)
{
    // se a posição já está visível, não faz nada e retorna o valor dela

    if (visivel[usr_lin][usr_col] == 1)
    {
        return minas[usr_lin][usr_col];
    }

    // se estiver perto de alguma mina (> 0), marca só ela e retorna o número

    if (minas[usr_lin][usr_col] > 0)
    {
        visivel[usr_lin][usr_col] = 1;

        return minas[usr_lin][usr_col];
    }
    
    // se a posição é uma mina (-1), marca todas as posições e retorna -1

    if (minas[usr_lin][usr_col] == -1)
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

            if (minas[lin_at][col_at] == -1) // pula se é uma mina
            {
                continue;        
            }

            if (minas[lin_at][col_at] > 0) // se for > 0 só marca como visível
            {
                visivel[lin_at][col_at] = 1;
                    
                continue;
            }

            set_visivel(minas, visivel, lin_at, col_at); // se for 0, marca como visível e chama recursivamente
        }
    }
    
    return 0;
}

void clear(void) // limpa a tela com chamada de sistema
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_minas(byte minas[N][N], byte visivel[N][N]) // mostra o campo minado na tela 
{
    printf("  ");
    
    for (byte i = 0; i < N; i++) // linha com número das colunas
    {
        printf("%4d", i);
    }

    printf("\n\n");   
    
    for (byte i = 0; i < N; i++)
    {
        // número das linhas
        
        printf("%-2d", i);

        // percorre todas as posições e mostra as visíves ou mostra "-"
        
        for (byte j = 0; j < N; j++)
        {
            if (visivel[i][j] == 0)
            {
                printf("   -");

                continue;
            }
            
            switch (minas[i][j])
            {
                case -1:
                    printf(VERMELHO "   #" RESET);
                    break;
                case 0:
                    printf(BRANCO "%4d" RESET, minas[i][j]);
                    break;
                case 1:
                    printf(AZUL "%4d" RESET, minas[i][j]);
                    break;
                case 2:
                    printf(CIANO "%4d" RESET, minas[i][j]);
                    break;
                case 3:
                    printf(VERDE "%4d" RESET, minas[i][j]);
                    break;
                case 4:
                    printf(AMARELO "%4d" RESET, minas[i][j]);
                    break;
                        
                default:
                    printf(ROSA "%4d" RESET, minas[i][j]);
                    break;
            }
        }
        
        printf("\n");
    }
}

void print_respostas(byte minas[N][N]) // mostra o campo minado na tela 
{
    printf("\n  ");
    
    for (byte i = 0; i < N; i++) // linha com número das colunas
    {
        printf("%4d", i);
    }

    printf("\n\n");   
    
    for (byte i = 0; i < N; i++)
    {
        // número das linhas
        
        printf("%-2d", i);

        // percorre e mostra todas as posições
        
        for (byte j = 0; j < N; j++)
        {
            switch (minas[i][j])
            {
                case -1:
                    printf(VERMELHO "   #" RESET);
                    break;
                case 0:
                    printf(BRANCO "%4d" RESET, minas[i][j]);
                    break;
                case 1:
                    printf(AZUL "%4d" RESET, minas[i][j]);
                    break;
                case 2:
                    printf(CIANO "%4d" RESET, minas[i][j]);
                    break;
                case 3:
                    printf(VERDE "%4d" RESET, minas[i][j]);
                    break;
                case 4:
                    printf(AMARELO "%4d" RESET, minas[i][j]);
                    break;
                        
                default:
                    printf(ROSA "%4d" RESET, minas[i][j]);
                    break;
            }
        }
        
        printf("\n");
    }
}

byte venceu(byte minas[N][N], byte visivel[N][N]) // verifica se todas as posições >= 0 já estão visíveis
{
    for (byte i = 0; i < N; i++)
    {
        for (byte j = 0; j < N; j++)
        {
            if (minas[i][j] >= 0 && visivel[i][j] == 0)
            {
                return 0;
            }
        }
    } 

    for (byte i = 0; i < N; i++) // mostra todas as minas como visíveis
    {
        for (byte j = 0; j < N; j++)
        {
            if (minas[i][j] == -1)
            {
                visivel[i][j] = 1;
            }
        }
    }

    return 1;
}

void servidor(int readfd, int writefd) // (filho) processa a matriz
{
    campo_minado estado;        
    byte usr_lin, usr_col; // posições recebidas do cliente

    zera_minas(estado.minas);
    zera_minas(estado.visivel);
    
    coloca_minas(estado.minas);
    
    conta_minas(estado.minas);
    
    estado.status = 0; // 0 = jogando

    while (1)
    {
        write(writefd, &estado, sizeof(campo_minado)); // envia o estado atual para o cliente

        if (estado.status != 0) // sai do loop se o jogo terminou
        {
            break;
        }

        // recebe a posição do cliente
    
        read(readfd, &usr_lin, sizeof(byte));
        read(readfd, &usr_col, sizeof(byte));

        
        if (set_visivel(estado.minas, estado.visivel, usr_lin, usr_col) == -1)
        {
            estado.status = -1;
        }

        else if (venceu(estado.minas, estado.visivel))
        {
            estado.status = 1;
        }
    }
}

void cliente(int readfd, int writefd) // cliente (pai) mostra o campo minado e recebe entrada do usuário
{
    campo_minado campo;

    byte usr_lin, usr_col; // coordenadas digitadas pelo usuário

    while (1)
    {
        read(readfd, &campo, sizeof(campo_minado)); // recebe o estado atual do servidor

        clear();
        print_minas(campo.minas, campo.visivel);
        
        // print_respostas(campo.minas); // se quiser ver as respostas

        // verifica se ganhou ou perdeu
    
        if (campo.status == -1)
        {
            printf(VERMELHO "\nPERDEU!!!\n" RESET);
            
            break;
        }
        
        if (campo.status == 1)
        {
            printf(AZUL "\nVENCEU!!!\n" RESET);
            
            break;
        }

        printf("\nlinha coluna: ");

        scanf("%hhd %hhd", &usr_lin, &usr_col); // pega entrada do usuário

        if (usr_lin < 0 || usr_lin >= N || usr_col < 0 || usr_col >= N) // verifica se é uma posição válida
        {
            break;
        }

        // manda entrada do usuário para o servidor
 
        write(writefd, &usr_lin, sizeof(byte));
        write(writefd, &usr_col, sizeof(byte));
    }
}

int main(void)
{
    // [0] = leitura
    // [1] = escrita
    // 
    // servidor_cliente[0] = cliente lê
    // servidor_cliente[1] = servidor escreve
    //
    // cliente_servidor[0] = servidor lê
    // cliente_servidor[1] = cliente escreve

    int servidor_cliente[2];
    int cliente_servidor[2];

    if (pipe(servidor_cliente) < 0 || pipe(cliente_servidor) < 0)
    {
        printf("erro ao criar pipe!");
        
        return 1;
    }

    pid_t pid = fork(); // cria o processo filho (servidor)
    
    if (pid < 0)
    {
        printf("erro ao criar subprocesso!");
        
        return 1;
    }

    if (pid > 0) // processo pai (cliente)
    {
        // Fecha os descritores dos pipes que o cliente não vai usar

        close(servidor_cliente[1]);
        close(cliente_servidor[0]);

        // cliente lê do servidor_cliente[0] e escreve no cliente_servidor[1]

        cliente(servidor_cliente[0], cliente_servidor[1]);

        // Fecha os descritores restantes quando terminar
    
        close(servidor_cliente[0]);
        close(cliente_servidor[1]);

        exit(0);
    }
    
    // processo filho (servidor)

    // Fecha os descritores dos pipes que o filho não vai usar

    close(servidor_cliente[0]);
    close(cliente_servidor[1]);
    
    // servidor lê do cliente_servidor[0] e escreve no servidor_cliente[1]

    servidor(cliente_servidor[0], servidor_cliente[1]);

    // Fecha os descritores restantes

    close(cliente_servidor[0]);
    close(servidor_cliente[1]);

    exit(0);
}
