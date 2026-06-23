// TODO: comentar melhor cada parte do código

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RESET    "\033[0m"
#define VERMELHO "\033[91m"
#define VERDE    "\033[92m"
#define AMARELO  "\033[93m"
#define AZUL     "\033[94m"
#define ROSA     "\033[95m"
#define CIANO    "\033[96m"
#define BRANCO   "\033[97m"

// TODO: trocar tipo da mat2 pra byte pra gastar menos memória

void zera_mat(int tam, int mat[tam][tam])
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            mat[i][j] = 0;
        }
    }
}

void coloca_bombas(int tam, int mat[tam][tam])
{
    srand(time(NULL));
    
    for (int i = 0; i < 15; i++)
    {
        int linha  = rand() % tam;
        int coluna = rand() % tam;
        
        if (mat[linha][coluna] != -1)
        {
             mat[linha][coluna] = -1;
        }
    }
}

// TODO: garantir que duas bombas não fiquem no mesmo lugar e
// sejam colocadas depois da primeira posição que o usuário escolher

void conta_bombas(const int tam, int mat[tam][tam])
{
    // pra cada elemento da matriz que não for uma bomba
     
    for (int lin = 0; lin < tam; lin++)
    {
        for (int col = 0; col < tam; col++)
        {
            if (mat[lin][col] == -1)
            {
                continue;
            }
                
            int bombas = 0;
            
            // conta as bombas das posições adjacentes:
            // da linha e coluna anteriores     [lin_adj - 1][col_adj - 1]
            // até a linha e coluna posteriores [lin_adj + 1][col + 1]
            
            for (int lin_adj = lin - 1; lin_adj <= lin + 1; lin_adj++)
            {
                for (int col_adj = col - 1; col_adj <= col + 1; col_adj++)
                {
                    // verifica se está dentro do limite da matriz e se o elemento é uma bomba
                    
                    if (lin_adj >= 0 && lin_adj < tam && col_adj >= 0 && col_adj < tam && mat[lin_adj][col_adj] == -1)
                    {
                        bombas++;
                    }
                }
            }
            
            mat[lin][col] = bombas;
        }
    }
}


int set_visivel(int tam, int mat[tam][tam], int visivel[tam][tam], int usr_lin, int usr_col)
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
        for (int i = 0; i < tam; i++)
        {
            for (int j = 0; j < tam; j++)
            {
                visivel[i][j] = 1;
            }
        }
        return -1;
    }

    // se é uma posição vazia (0), marca as posições ao redor usando busca em largura

    int fila_linha[tam * tam];
    int fila_coluna[tam * tam];
    
    int inicio = 0;
    int fim = 0;

    fila_linha[fim] = usr_lin;
    fila_coluna[fim] = usr_col;
    
    fim++;

    visivel[usr_lin][usr_col] = 1;
    
    while (inicio < fim)
    {
        int lin = fila_linha[inicio];
        int col = fila_coluna[inicio];

        inicio++;

        if (mat[lin][col] > 0)
        {
            continue;
        }

        for (int dlin = -1; dlin <= 1; dlin++)
        {
            for (int dcol = -1; dcol <= 1; dcol++)
            {
                if (dlin == 0 && dcol == 0)
                    continue;

                int nlin = lin + dlin;
                int ncol = col + dcol;

                if (nlin >= 0 && nlin < tam && ncol >= 0 && ncol < tam)
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

void print_mat(int tam, int mat[tam][tam], int visivel[tam][tam])
{
    // número das colunas em azul
    
    printf("\n  ");

    for (int i = 0; i < tam; i++)
    {
        printf("%4d", i);
    }

    printf("\n\n");   
    
    for (int i = 0; i < tam; i++)
    {
        // número das linhas em azul
        
        printf("%-2d", i);

        // numeros de cada posição
        
        for (int j = 0; j < tam; j++)
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

void print_mat_bombas(int tam, int mat[tam][tam])
{
    printf("\n  ");

    // cabeçalho das colunas
    for (int i = 0; i < tam; i++)
        printf("%4d", i);
    printf("\n\n");

    for (int i = 0; i < tam; i++)
    {
        // número da linha
        printf("%-2d", i);

        for (int j = 0; j < tam; j++)
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

int venceu(int tam, int mat[tam][tam], int visivel[tam][tam])
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            if (mat[i][j] != -1 && visivel[i][j] == 0)
                return 0;   // ainda existe uma célula segura não revelada
        }
    }
    // TODO: função pra colocar toda a matriz como visível
    // Se chegou até aqui, todas as células seguras estão visíveis.
    // Revela as bombas para a tela final de vitória

    for (int i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)
            if (mat[i][j] == -1)
                visivel[i][j] = 1;

    return 1;   // vitória
}

int main()
{
    // cria matriz

    int tam = 10;
    int mat[tam][tam];
    int visivel[tam][tam];

    zera_mat(tam, mat);
    zera_mat(tam, visivel);
    
    coloca_bombas(tam, mat);
    conta_bombas(tam, mat);

    clear();
    print_mat(tam, mat, visivel);
    // print_mat_bombas(tam, mat);
    printf("\n"  "linha coluna: " RESET);
    
    // entrada do usuário: [linha coluna]

    int usr_lin;
    int usr_col;

    while (1)
    {
        scanf("%d%d", &usr_lin, &usr_col);

        if (usr_col < 0 || usr_col > tam - 1 || usr_lin < 0 || usr_lin > tam - 1)
        {
            break;
        }
        
        int res = set_visivel(tam, mat, visivel, usr_lin, usr_col);
        
        clear();
        print_mat(tam, mat, visivel);
        // print_mat_bombas(tam, mat);

        if (res == -1)
        {
            printf(VERMELHO "\nPERDEU!!!\n" RESET);
            break;
        }

        if (venceu(tam, mat, visivel))
        {
            clear();
            print_mat(tam, mat, visivel);
            printf(AZUL  "\nVENCEU!!!\n" RESET);
            break;
        }

        printf("\n"  "linha coluna: " RESET);
    }        
    
    return 0;
}
