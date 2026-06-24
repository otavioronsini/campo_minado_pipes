#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h> //para não dar erro feio no terminal :)

typedef char byte;

#define RESET    "\033[0m"
#define VERMELHO "\033[91m"
#define VERDE    "\033[92m"
#define AMARELO  "\033[93m"
#define AZUL     "\033[94m"
#define ROSA     "\033[95m"
#define CIANO    "\033[96m"
#define BRANCO   "\033[97m"

// número de linhas e colunas das matrizes
#define N 10

void zera_mat(byte mat[N][N])
{
    for (byte lin = 0; lin < N; lin++) // linha
    {
        for (byte col = 0; col < N; col++) // coluna
        {
            mat[lin][col] = 0;
        }
    }
}

void coloca_bombas(byte mat[N][N])
{
    srand(time(NULL));
    
    for (byte i = 0; i < 15; i++)
    {
        byte linha  = rand() % N;
        byte coluna = rand() % N;
        
        if (mat[linha][coluna] != -1)
        {
             mat[linha][coluna] = -1;
        }
    }
}

// TODO: garantir que duas bombas não fiquem no mesmo lugar e
// sejam colocadas depois da primeira posição que o usuário escolher

void conta_bombas(byte mat[N][N])
{
    // pra cada elemento da matriz que não for uma bomba
     
    for (byte lin = 0; lin < N; lin++)
    {
        for (byte col = 0; col < N; col++)
        {
            if (mat[lin][col] == -1)
            {
                continue;
            }
                
            byte bombas = 0;
            
            // conta as bombas das posições adjacentes:
            // da linha e coluna anteriores     [lin_adj - 1][col_adj - 1]
            // até a linha e coluna posteriores [lin_adj + 1][col + 1]
            
            for (byte lin_adj = lin - 1; lin_adj <= lin + 1; lin_adj++)
            {
                for (byte col_adj = col - 1; col_adj <= col + 1; col_adj++)
                {
                    // verifica se está dentro do limite da matriz e se o elemento é uma bomba
                    
                    if (lin_adj >= 0 && lin_adj < N && col_adj >= 0 && col_adj < N && mat[lin_adj][col_adj] == -1)
                    {
                        bombas++;
                    }
                }
            }
            
            mat[lin][col] = bombas;
        }
    }
}


byte set_visivel(byte mat[N][N], byte visivel[N][N], byte usr_lin, byte usr_col)
{
    // se a posição já está visível, não faz nada e retorna o valor dela

    if (visivel[usr_lin][usr_col] == 1)
    {
        return mat[usr_lin][usr_col];
    }

    // se estiver perto de alguma bomba (> 0), marca só ela e retorna o número

    if (mat[usr_lin][usr_col] > 0)
    {
        visivel[usr_lin][usr_col] = 1;

        return mat[usr_lin][usr_col];
    }
    
    // se a posição é uma bomba (-1), marca todas as células e retorna -1

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

    // se é uma posição vazia (0), marca as posições ao redor usando busca em largura

    byte fila_linha[N * N];
    byte fila_coluna[N * N];
    
    byte inicio = 0;
    byte fim = 0;

    fila_linha[fim] = usr_lin;
    fila_coluna[fim] = usr_col;
    
    fim++;

    visivel[usr_lin][usr_col] = 1;
    
    while (inicio < fim)
    {
        byte lin = fila_linha[inicio];
        byte col = fila_coluna[inicio];

        inicio++;

        if (mat[lin][col] > 0)
        {
            continue;
        }

        for (byte dlin = -1; dlin <= 1; dlin++)
        {
            for (byte dcol = -1; dcol <= 1; dcol++)
            {
                if (dlin == 0 && dcol == 0)
                    continue;

                byte nlin = lin + dlin;
                byte ncol = col + dcol;

                if (nlin >= 0 && nlin < N && ncol >= 0 && ncol < N)
                {
                    if (visivel[nlin][ncol] == 0 && mat[nlin][ncol] != -1)
                    {
                        visivel[nlin][ncol] = 1;
                        fila_linha[fim] = nlin;
                        fila_coluna[fim] = ncol;
                        fim++;
                    }
                }
            }
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

void print_mat_bombas(byte mat[N][N])
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
    // mostra todas as bombas

    for (byte i = 0; i < N; i++)
        for (byte j = 0; j < N; j++)
            if (mat[i][j] == -1)
                visivel[i][j] = 1;

    return 1;
}

int servidor_campo_minado(int readfd,int writefd)
{
    byte mat[N][N];            // matriz com os valores de -1 a 8, cada número indica quantas bombas têm ao redor daquela posição
    byte visivel[N][N];        // matriz com valores 0 e 1, indica se os números daquela posição em mat estão visíveis ao usuário

    int status = 0;            //define o estado do jogo, sendo "0" = jogando, "1" = a venceu e "-1" = perdeu

    zera_mat(mat);
    zera_mat(visivel);
    
    coloca_bombas(mat);
    conta_bombas(mat);
    
    byte usr_lin; // número da linha digita pelo usuário
    byte usr_col; // número da coluna digitada pelo usuário

    while(1)
    {
        write(writefd, mat, N*N);
        write(writefd, visivel, N*N);
        write(writefd, &status, sizeof(int));

        if(status != 0) break;

        read(readfd, &usr_lin, sizeof(byte));
        read(readfd, &usr_col, sizeof(byte));

        byte res = set_visivel(mat, visivel, usr_lin, usr_col);

        if(res == -1)
        {
            status =- 1;
        }

        else if(venceu(mat, visivel))
        {
            status = 1;
        }
    }
}

int cliente_campo_minado(int readfd, int writefd)
{
    byte mat[N][N];
    byte visivel[N][N];

    int status = 0;

    byte usr_lin;
    byte usr_col;

    while(1)
    {
        read(readfd, mat, N*N);
        read(readfd, visivel, N*N);
        read(readfd, &status, sizeof(int));

        clear();
        print_mat(mat, visivel);

        if(status == -1)
        {
            printf(VERMELHO "\nperdeu!\n" RESET);
        }

        else if(status == 1)
        {
            printf(AZUL "Venceu!" RESET);
        }

        printf("\nSelecione a linha e coluna: ");

        if(scanf("%hhd%hhd", &usr_lin, &usr_col) != 2) break;

        if(usr_col < 0 || usr_col > N - 1 || usr_lin < 0 || usr_lin > N - 1)
        {
            break;
        }

        write(writefd, &usr_lin, sizeof(byte));
        write(writefd, &usr_col, sizeof(byte));
    }
}

int main(void)
{

    int descritor,
        pipe1[2],
        pipe2[2];

    if (pipe(pipe1)<0 || pipe(pipe2) <0)
    { printf("Erro na chamada PIPE");
        exit(0);
    }

    if ( (descritor = fork()) <0)
    { printf("Erro na chamada FORK");
        exit(0);
    }

    else if (descritor >0) // PROCESSO PAI
    { 
        close(pipe1[0]);
        close(pipe2[1]);

        cliente_campo_minado(pipe2[0], pipe1[1]);
        
        close(pipe1[1]);
        close(pipe2[0]);
        exit(0);
    } 

    else // PROCESSO FILHO
    { 
        close(pipe1[1]);
        close(pipe2[0]);

        servidor_campo_minado(pipe1[0], pipe2[1]);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
}


    //campo_minado();
    
    return 0;
}

/*int campo_minado(void)               //deixou de existir para ser particionada em servidor_campo_minado e cliente_campo_minado
{
    byte mat[N][N];     // matriz com os valores de -1 a 8, cada número indica quantas bombas têm ao redor daquela posição
    byte visivel[N][N]; // matriz com valores 0 e 1, indica se os números daquela posição em mat estão visíveis ao usuário

    zera_mat(mat);
    zera_mat(visivel);
    
    coloca_bombas(mat);
    conta_bombas(mat);

    clear();
    print_mat(mat, visivel);
    // print_mat_bombas(N, mat);
    printf("\n"  "linha coluna: " RESET);
    
    byte usr_lin; // número da linha digita pelo usuário
    byte usr_col; // número da coluna digitada pelo usuário

    while (1)
    {
        scanf("%hhd%hhd", &usr_lin, &usr_col);

        if (usr_col < 0 || usr_col > N - 1 || usr_lin < 0 || usr_lin > N - 1)
        {
            break;
        }
        
        byte res = set_visivel(mat, visivel, usr_lin, usr_col);
        
        clear();
        print_mat(mat, visivel);
        // print_mat_bombas(N, mat);

        if (res == -1)
        {
            printf(VERMELHO "\nPERDEU!!!\n" RESET);
            break;
        }

        if (venceu(mat, visivel))
        {
            clear();
            print_mat(mat, visivel);
            printf(AZUL  "\nVENCEU!!!\n" RESET);
            break;
        }

        printf("\n"  "linha coluna: " RESET);
    }        
    
    return 0;
}*/

// te odeio otávio