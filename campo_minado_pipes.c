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

/**
 * Função que revela as células a partir de uma posição (usr_lin, usr_col)
 * usando Busca em Largura (BFS) com fila manual.
 * 
 * Parâmetros:
 *   tam     : tamanho da matriz (tam x tam)
 *   mat     : matriz com os valores (-1 = bomba, 0 = vazio, 1..8 = número de bombas)
 *   visivel : matriz que indica se a célula já foi revelada (1) ou não (0)
 *   usr_lin, usr_col : coordenada escolhida pelo usuário
 * 
 * Retorno:
 *   -1 se clicou em bomba (e revela tudo)
 *    0 se abriu células com segurança
 *    ou o valor da célula se já estava visível (opcional)
 */
 
int set_visivel(int tam, int mat[tam][tam], int visivel[tam][tam], int usr_lin, int usr_col)
{
    // 1) Se a posição já está visível, não faz nada e retorna o valor dela
    if (visivel[usr_lin][usr_col] == 1)
    {
        return mat[usr_lin][usr_col];
    }

    // 2) Se a posição é uma bomba (-1), revela todas as células e termina o jogo
    if (mat[usr_lin][usr_col] == -1)
    {
        // Revela tudo (perdeu)
        for (int i = 0; i < tam; i++)
        {
            for (int j = 0; j < tam; j++)
            {
                visivel[i][j] = 1;
            }
        }
        return -1;   // indica que foi bomba
    }

    // 3) Se não é bomba, vamos usar uma fila para fazer a expansão em largura
    // Criamos dois vetores para armazenar as coordenadas (linha e coluna) das células a serem processadas.
    // O tamanho máximo da fila é tam * tam (todas as células da matriz).
    int fila_linha[tam * tam];
    int fila_coluna[tam * tam];
    
    // Índices para controlar a fila:
    // - 'inicio' aponta para o próximo elemento a ser processado
    // - 'fim' aponta para a próxima posição vazia onde podemos adicionar um novo elemento
    int inicio = 0;
    int fim = 0;

    // Colocamos a célula inicial (usr_lin, usr_col) na fila e a marcamos como visível
    fila_linha[fim] = usr_lin;
    fila_coluna[fim] = usr_col;
    fim++;  // incrementa para a próxima posição livre

    visivel[usr_lin][usr_col] = 1;   // marca como já revelada

    // Agora, enquanto houver elementos na fila (inicio < fim), processamos cada um
    while (inicio < fim)
    {
        // Pega a próxima célula da fila (a que está no índice 'inicio')
        int lin = fila_linha[inicio];
        int col = fila_coluna[inicio];
        inicio++;   // avança o ponteiro de início

        // Se a célula atual tem um número > 0, ela não deve se expandir para os vizinhos.
        // Apenas a revelamos (já está visível) e pulamos a parte de adicionar vizinhos.
        if (mat[lin][col] > 0)
        {
            continue;   // vai para a próxima iteração do while, sem expandir
        }

        // Se a célula atual é 0, precisamos olhar todos os seus vizinhos (8 direções)
        // e, se eles ainda não estiverem visíveis e não forem bombas, adicioná-los à fila.
        for (int dlin = -1; dlin <= 1; dlin++)
        {
            for (int dcol = -1; dcol <= 1; dcol++)
            {
                // Ignora o próprio (deslocamento 0,0)
                if (dlin == 0 && dcol == 0)
                    continue;

                // Calcula as coordenadas do vizinho
                int nlin = lin + dlin;
                int ncol = col + dcol;

                // Verifica se o vizinho está dentro dos limites da matriz
                if (nlin >= 0 && nlin < tam && ncol >= 0 && ncol < tam)
                {
                    // Se o vizinho ainda não está visível e não é uma bomba
                    if (visivel[nlin][ncol] == 0 && mat[nlin][ncol] != -1)
                    {
                        // Marca como visível e adiciona na fila para ser processado depois
                        visivel[nlin][ncol] = 1;
                        fila_linha[fim] = nlin;
                        fila_coluna[fim] = ncol;
                        fim++;   // aumenta o tamanho da fila
                    }
                }
            }
        }
    }

    // Se chegou até aqui, tudo correu bem e nenhuma bomba foi aberta
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
