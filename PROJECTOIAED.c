
#include <stdio.h>
#include <string.h>

#define MAX 1000

/*
estrutura que guarda informacao de um aeroporto*/
typedef struct
{			
	char id[4]; 	/*id e o nome do aeroporto (apenas contem 3 caracteres)*/
	int capacidade; /*capacidade total do mesmo*/
	int estado;		/*estado do mesmo*/
} Aeroporto;

int matrizAdjacencias[MAX][MAX]; /*matriz que vai guardar os voos entre os diferentes aeroportos*/
Aeroporto aeroportos[MAX];       /*vector que vai conter todos os aeroportos*/
int nrAeroportos = 0;			 /*contador que nos ajuda a saber quantos aeroportos temos no sistema*/

/*comando do projecto (auto-explicativos) */
void comandoAdicionaNovoAeroporto();
void comandoAlteraCapacidadeMaxima();
void comandoAdicionaVoo();
void comandoAdicionaVooRT();
void comandoRemoveVoo();
void comandoRemoveVooRT();
void comandoNumeroVoos();
void comandoAeroportoComMaisVoos();
void comandoAeroportoComMaisLigacoes();
void comandoVooMaisPopular();
void comandoEncerrarAeroporto();
void comandoReabrirAeroporto();
void comandoListagem();
/*funcao que da o output final especificado no enunciado*/
void output();


/*AUXILIARES*/
void insertionSort(Aeroporto vec[], int l, int r);
int posicaoDoAeroporto(char id[]);
int numeroDePartidas(int aeroporto);
int numeroDeChegadas(int aeroporto);
int numeroTotalDeVoos(int aeroporto);
int contaLigacoes(int aeroporto);

int main()
{
	char command;

	while ((command = getchar()) != 'X') {  /* le o comando */
        switch (command) 
        {
	        case 'A':
	        	comandoAdicionaNovoAeroporto();
	        	break;

	        case 'I':
	        	comandoAlteraCapacidadeMaxima();
	        	break;

	        case 'F':
	            comandoAdicionaVooRT();
	        	break;

	        case 'G':
	            comandoAdicionaVoo();
	       		break;

	        case 'R':
	            comandoRemoveVoo();
	        	break;

            case 'S':
            	comandoRemoveVooRT();
            	break;

            case 'N':
           		comandoNumeroVoos();
            	break;

            case 'P':
            	comandoAeroportoComMaisVoos();
            	break;

            case 'Q':
            	comandoAeroportoComMaisLigacoes();
            	break;

            case 'V':
           		comandoVooMaisPopular();
            	break;

            case 'C':
            	comandoEncerrarAeroporto();
            	break;

            case 'O':
            	comandoReabrirAeroporto();
            	break;

           	case 'L':
            	comandoListagem();
            	break;
        default:
            printf("ERRO: Comando desconhecido\n");
        }
        getchar(); 							/* le o '\n' introduzido pelo utilizador */
    }
    output();
	return 0;
}


/*
	algoritmo de ordenacao basico que ordena os aeroporto por ordem alfabetica ascendente
*/
void insertionSort(Aeroporto vec[], int l, int r)
{
    int i, j;
    Aeroporto auxiliar;  /**/
    
    for (i = 0; i < r ; ++i)
    {
        auxiliar = vec[i];
        j = i - 1;
        while ((j >= l) && (strcmp(auxiliar.id, vec[j].id) < 0))                          
        {
            vec[j + 1] = vec[j];
            j --;
        }
        vec[j + 1] = auxiliar;
    }
}

/*
	Neste comando lemos um numero do terminal que vai identificas o tipo do comando: 
	Se for 0 imprime sobre todos os aeroportos (por ordem de criacao) o seu id, capacidade, numero de partidas e numero de chegadas. 
	Se for 1 imprime a mesma informacao que no comando 0 mas os aeroportos sao apresentados com ordenacao alfabetica.
	Se for 2 imprime a distribuicao d(k) do numero de aeroportos como especificado no enunciado
*/
void comandoListagem()
{
	int tipo, i, maximo = 0, k, count, aux;
	Aeroporto vectorAuxiliar[MAX];

	scanf("%d", &tipo);
	if (tipo == 0)
	{
		for(i = 0; i < nrAeroportos; i++)
		{
			printf("%s:%d:%d:%d\n", aeroportos[i].id, aeroportos[i].capacidade, numeroDePartidas(i), numeroDeChegadas(i));
		}
	}
	else if (tipo == 1)
	{
		for (i = 0; i < nrAeroportos; i++)
		{
			vectorAuxiliar[i].capacidade = aeroportos[i].capacidade;
			strcpy(vectorAuxiliar[i].id, aeroportos[i].id);
			vectorAuxiliar[i].estado = aeroportos[i].estado;
		}

		insertionSort(vectorAuxiliar, 0, nrAeroportos);
		for(i = 0; i < nrAeroportos; i++)
		{
			printf("%s:%d:%d:%d\n", vectorAuxiliar[i].id, vectorAuxiliar[i].capacidade, 
					numeroDePartidas(posicaoDoAeroporto(vectorAuxiliar[i].id)), numeroDeChegadas(posicaoDoAeroporto(vectorAuxiliar[i].id)));
		}
	}
	else
	{
		for (i = 0; i < nrAeroportos; i++)
		{
			aux = numeroTotalDeVoos(i);
			if (aux > maximo)
				maximo = aux;
		}

		for (k = 0; k < (maximo+1); k++)
		{
			count = 0;
			for (i = 0; i < nrAeroportos; i++)
				if (numeroTotalDeVoos(i) == k)
					count++;

			if (count > 0)
				printf("%d:%d\n", k, count);
		}
	}
}

/*comando reabre aeroporto pondo o seu estado a um*/

void comandoReabrirAeroporto()
{
	char id[4];
	int aeroporto;

	scanf("%s", id);

	aeroporto = posicaoDoAeroporto(id);

	if (aeroporto == -1)
	{
		printf("*Aeroporto %s inexistente\n", id);
		return;
	}

	aeroportos[aeroporto].estado = 1;
}

/*comando encerra o aeroporto pondo o seu estado a zero*/

void comandoEncerrarAeroporto()
{
	char id[4];
	int aeroporto, i;

	scanf("%s", id);

	aeroporto = posicaoDoAeroporto(id);

	if (aeroporto == -1)
	{
		printf("*Aeroporto %s inexistente\n", id);
		return;
	}

	aeroportos[aeroporto].estado = 0;
	for (i = 0; i < nrAeroportos; i++)
	{
		matrizAdjacencias[aeroporto][i] = 0;
		matrizAdjacencias[i][aeroporto] = 0;
	}
}

/*comando que de linha em linha e coluna em coluna ve qual e o maior numero de voos sendo assim este o mais popular*/

void comandoVooMaisPopular()
{
	int i, j, linha = 0, coluna = 0, maximo = 0;

	for (i = 0; i < nrAeroportos; i++)
	{
		for (j = 0; j < nrAeroportos; j++)
		{
			if (matrizAdjacencias[i][j] > maximo)
			{
				maximo = matrizAdjacencias[i][j];
				linha = i;
				coluna = j;
			}
		}
	}

	printf("Voo mais popular %s:%s:%d\n", aeroportos[linha].id, aeroportos[coluna].id, maximo);
}

/*funcao auxiliar que ve quantas celulas da matriz sao diferentes de zero, sendo que uma ligacao simboliza uma celula diferente de zero*/

int contaLigacoes(int aeroporto)
{
	int i, count = 0;

	for(i = 0; i < nrAeroportos; i ++)
	{
		if ((matrizAdjacencias[i][aeroporto] != 0) || (matrizAdjacencias[aeroporto][i] != 0))
		{
			count++;
		}
	}
	return count;
}

/*comando que usa a funcao auxiliar contaLigacoes para ver qual linha e coluna tem mais celulas diferentes de zero*/

void comandoAeroportoComMaisLigacoes()
{
	int aeroporto = 0, i, maximo = -1;

	for (i = 0; i < nrAeroportos; i++)
	{
		if (maximo < contaLigacoes(i))
		{
			maximo = contaLigacoes(i);
			aeroporto = i;
		}
	}

	printf("Aeroporto com mais ligacoes %s:%d\n", aeroportos[aeroporto].id, maximo);
}

/*Adiciona o aeroporto com o identificador e capacidade respectivas*/

void comandoAdicionaNovoAeroporto()
{
	char id[4];
	int capacidade;
	scanf("%s %d", id, &capacidade);

	aeroportos[nrAeroportos].capacidade = capacidade;
	strcpy(aeroportos[nrAeroportos].id, id);
	aeroportos[nrAeroportos].estado = 1;
	nrAeroportos++;
}

int posicaoDoAeroporto(char id[])
{
	int i = 0;

	for(i = 0; i < nrAeroportos; i++)
		if (strcmp(aeroportos[i].id, id) == 0)
			return i;

	return -1;
}

/*Altera a capacidade maxima de operacao do aeroporto com o id respectivo de acordo com o valor 
introduzido*/

void comandoAlteraCapacidadeMaxima()
{
	char id[4];
	int capacidade, nrVoosActuais, aeroporto;

	scanf("%s %d", id, &capacidade);

	aeroporto = posicaoDoAeroporto(id);
	nrVoosActuais = numeroTotalDeVoos(aeroporto);
 	
	if ((aeroporto == -1) || (aeroportos[aeroporto].estado == 0) || 
		((nrVoosActuais > (aeroportos[aeroporto].capacidade + capacidade)) && (nrVoosActuais != 0))
		|| ((aeroportos[aeroporto].capacidade + capacidade) < 0))
	{
		printf("*Capacidade de %s inalterada\n", id);
		return;
	}

	if ((aeroportos[aeroporto].capacidade + capacidade) < 0)
		aeroportos[aeroporto].capacidade = 0;
	else
		aeroportos[aeroporto].capacidade += capacidade;
}

/*Adiciona o voo de ida de id1 para id2*/

void comandoAdicionaVoo()
{
	char id1[4], id2[4];
	int posicao1, posicao2;

	scanf("%s %s", id1, id2);

	posicao1 = posicaoDoAeroporto(id1);
	posicao2 = posicaoDoAeroporto(id2);

	if ((posicao1 == -1) || (posicao2 == -1) 
		|| (aeroportos[posicao1].estado == 0) || (aeroportos[posicao2].estado == 0)
		|| (aeroportos[posicao1].capacidade < (numeroTotalDeVoos(posicao1) +1))
		|| (aeroportos[posicao2].capacidade < (numeroTotalDeVoos(posicao2) +1))) 
	{
		printf("*Impossivel adicionar voo %s %s\n", id1, id2);
		return;
	}

	
	matrizAdjacencias[posicao1][posicao2]++;
	
}

/*Adiciona os voos de ida e volta entre os aeroportos indicados*/

void comandoAdicionaVooRT()
{
	char id1[4], id2[4];
	int posicao1, posicao2;

	scanf("%s %s", id1, id2);

	posicao1 = posicaoDoAeroporto(id1);
	posicao2 = posicaoDoAeroporto(id2);


	if ((posicao1 == -1) || (posicao2 == -1) 
		|| (aeroportos[posicao1].estado == 0) || (aeroportos[posicao2].estado == 0)
		|| (aeroportos[posicao1].capacidade < (numeroTotalDeVoos(posicao1) + 2)) 
		|| (aeroportos[posicao2].capacidade < (numeroTotalDeVoos(posicao2) + 2))) 
	{
		printf("*Impossivel adicionar voo RT %s %s\n", id1, id2);
		return;
	}

	matrizAdjacencias[posicao1][posicao2]++;
	matrizAdjacencias[posicao2][posicao1]++;
}

/*Acede a matriz de adjacencias e tira um ao voo*/

void comandoRemoveVoo()
{
	char id1[4], id2[4];
	int posicao1, posicao2;

	scanf("%s %s", id1, id2);

	posicao1 = posicaoDoAeroporto(id1);
	posicao2 = posicaoDoAeroporto(id2);

	if (matrizAdjacencias[posicao1][posicao2] == 0)
	{
		printf("*Impossivel remover voo %s %s\n", id1, id2);
	}
	else
	{
		matrizAdjacencias[posicao1][posicao2]--;
	}
}

/*Acede a matriz de adjacencias e tira um ao voo de ida e de volta*/


void comandoRemoveVooRT()
{
	char id1[4], id2[4];
	int posicao1, posicao2;

	scanf("%s %s", id1, id2);

	posicao1 = posicaoDoAeroporto(id1);
	posicao2 = posicaoDoAeroporto(id2);

	if ((matrizAdjacencias[posicao1][posicao2] == 0) || (matrizAdjacencias[posicao2][posicao1] == 0))
	{
		printf("*Impossivel remover voo RT %s %s\n", id1, id2);
	}
	else
	{
		matrizAdjacencias[posicao1][posicao2]--;
		matrizAdjacencias[posicao2][posicao1]--;
	}
}

/*Retorna o numero de voos entre os aeroportos id1 e id2, saindo de id1
para id2 e de id2 para id1 ou seja as respectivas linhas e colunas*/

void comandoNumeroVoos()
{
	char id1[4], id2[4];
	int posicao1, posicao2;

	scanf("%s %s", id1, id2);

	posicao1 = posicaoDoAeroporto(id1);
	posicao2 = posicaoDoAeroporto(id2);

	if (posicao1 == -1)
		printf("*Aeroporto %s inexistente\n", id1);

	if (posicao2 == -1)
		printf("*Aeroporto %s inexistente\n", id2);

	if ((posicao1 != -1) && (posicao2 != -1))
		printf("Voos entre cidades %s:%s:%d:%d\n", id1, id2, matrizAdjacencias[posicao1][posicao2], matrizAdjacencias[posicao2][posicao1]);
}

/*funcao auxiliar que conta quantos voos de partida ha*/

int numeroDePartidas(int aeroporto)
{
	int i, count = 0;
	for (i = 0; i < nrAeroportos; i++)
	{
		count += matrizAdjacencias[aeroporto][i];
	}
	return count;
}

/*funcao auxiliar que conta quantos voos de chegada ha*/

int numeroDeChegadas(int aeroporto)
{
	int i, count = 0;
	for (i = 0; i < nrAeroportos; i++)
	{
		count += matrizAdjacencias[i][aeroporto];
	}
	return count;
}

/*funcao auxiliar que soma o numero de partidas e o numero de chegadas*/

int numeroTotalDeVoos(int aeroporto)
{
	int i, numeroPartidas = 0, numeroChegadas = 0;

	for (i = 0; i < nrAeroportos; i++)
	{
		numeroPartidas += matrizAdjacencias[aeroporto][i];
		numeroChegadas += matrizAdjacencias[i][aeroporto];
	}

	return numeroPartidas + numeroChegadas;
}

/*Retorna o aeroporto id com o maior numero total de voos (soma das partidas e 
chegadas) */

void comandoAeroportoComMaisVoos()
{
	int i, aeroporto = 0, nrVoos = 0;

	for (i = 0; i < nrAeroportos; i++)
	{	
		if (numeroTotalDeVoos(i) > nrVoos)
		{
			aeroporto = i;
			nrVoos = numeroTotalDeVoos(i);
		}
	}

	printf("Aeroporto com mais rotas %s:%d:%d\n", aeroportos[aeroporto].id, numeroDePartidas(aeroporto), numeroDeChegadas(aeroporto));
}

/*funcao que da o output final especificado no enunciado*/

void output()
{
	int nrVoos = 0, i, j;

	for (i = 0; i < nrAeroportos; i++)
	{
		for (j = 0; j < nrAeroportos; j++)
		{
			nrVoos += matrizAdjacencias[i][j];
		}
	}

	printf("%d:%d\n",nrVoos,nrAeroportos);
}