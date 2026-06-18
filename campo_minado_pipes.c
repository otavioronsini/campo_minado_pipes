#include <_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AZUL     "\033[94m"
#define AMARELO  "\033[33m"
#define VERDE    "\033[32m"
#define VERMELHO "\033[31m"
#define RESET    "\033[39m"

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

// TODO: usar busca em largura com fila pra melhorar a velocidade

int set_visivel(int tam, int mat[tam][tam], int visivel[tam][tam], int usr_lin, int usr_col)
{
    // posição já é visível
    
    if (visivel[usr_lin][usr_col] == 1)
    {
        return mat[usr_lin][usr_col];
    }

    // se não for, marca posição como visível

    visivel[usr_lin][usr_col] = 1;
    
    // posição vale >= 1, marca só ela (já foi)

    if (mat[usr_lin][usr_col] >= 1)
    {
        
        return mat[usr_lin][usr_col];
    }

    // posição vale -1, marca tudo

    if (mat[usr_lin][usr_col] == -1)
    {
        for (int lin = 0; lin < tam; lin++)
        {
            for (int col = 0; col < tam; col++)
            {
                visivel[lin][col] = 1;
            }
        }

        return -1;
    }

    // posição vale 0, mostra as posições >= 0 ligadas

    for (int lin = usr_lin - 1; lin <= usr_lin + 1; lin++)
    {
        for (int col = usr_col - 1; col <= usr_col + 1; col++)
        {
            // Verifica limites
            
            if (lin >= 0 && lin < tam && col >= 0 && col < tam)
            {
                // se valer > 0, marca
                
                if (mat[lin][col] > 0 && visivel[lin][col] == 0)
                {
                    visivel[lin][col] = 1;
                }

                if (mat[lin][col] == 0)
                {
                    set_visivel(tam, mat, visivel, lin, col);
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

// TODO: mostrar só o que for visível

void print_mat(int tam, int mat[tam][tam])
{
    // número das colunas em azul
    
    printf("\n  " AZUL);

    for (int i = 0; i < tam; i++)
    {
        printf("%4d", i);
    }

    printf("\n\n" RESET);   
    
    for (int i = 0; i < tam; i++)
    {
        // número das linhas em azul
        
        printf(AZUL "%-2d" RESET, i);

        // numeros de cada posição
        
        for (int j = 0; j < tam; j++)
        {
            switch (mat[i][j])
            {
                case -1:
                    printf(VERMELHO "%4d" RESET, mat[i][j]);
                    break;
                case 1:
                    printf(VERDE "%4d" RESET, mat[i][j]);
                    break;
                case 2:
                    printf(AMARELO "%4d" RESET, mat[i][j]);
                    break;
                default:
                    printf("%4d", mat[i][j]);
                    break;
            }
        }
        
        printf("\n");
    }
    
    printf("\n" AZUL "linha coluna: " RESET);
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
    print_mat(tam, mat);
    printf("\n");
    print_mat(tam, visivel);
    
    // entrada do usuário: [linha coluna]

    int usr_lin;
    int usr_col;

    // TODO: fazer a lógica de vitória/derrota

    while (1)
    {
        scanf("%d%d", &usr_lin, &usr_col);
        
        clear();
        print_mat(tam, mat);
        set_visivel(tam, mat, visivel, usr_lin, usr_col);
        printf("\n");
        print_mat(tam, visivel);
        
        if (usr_col < 0 || usr_col > tam - 1 || usr_lin < 0 || usr_lin > tam - 1)
        {
            break;
        }

    }        
    
    return 0;
}
