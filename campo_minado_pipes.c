#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AZUL  "\033[94m"
#define RESET "\033[39m"

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
    
    for (int i = 0; i < 10; i++)
    {
        int linha  = rand() % 10;
        int coluna = rand() % 10;
        
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

void clear()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_mat_num(int tam, int mat[tam][tam])
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
            printf("%4d", mat[i][j]);
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

    zera_mat(tam, mat);
    
    coloca_bombas(tam, mat);
    conta_bombas(tam, mat);

    clear();
    print_mat_num(tam, mat);
    
    // entrada do usuário: [linha coluna]

    int usr_lin;
    int usr_col;


    // TODO: função pra mapear as posições que devem ser mostradas:
    //       se a posição que o usuário digitou vale -1,  perdeu
    //       se valer 0, pra cada posição valendo 0 ao redor dela, mostra as posições valendo > 0 ao redor
    //       se valer > 0, mostra apenas ela
    
    while (1)
    {
        scanf("%d%d", &usr_lin, &usr_col);
        
        clear();
        print_mat_num(tam, mat);
        
        if (usr_col < 0 || usr_col > tam - 1 || usr_lin < 0 || usr_lin > tam - 1)
        {
            break;
        }

    }        
    
    return 0;
}
