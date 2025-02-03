// (2, 2, 1)
// 2 соседское
// левая-правая границы - склейка
// 6 8 5 3 - количество ограничений по типам

#include "../include/buddy/bdd.h"
#include <fstream>
using namespace std;

#define N_VAR 144 // чиcло булевых переменных
#define N 9 // чиcло объектов
#define M 4 // чиcло cвойcтв
#define LOG_N 4

ofstream out;

void fun(char* varset, int size); // функция, используемая для вывода решений

double cnt;
bdd p[M][N][N];

int main(void)
{
	// инициализация
	bdd_init(100000, 10000);
	bdd_setvarnum(N_VAR);

	// ->--- вводим функцию p(k, i, j) следующим образом ( pk[i][j] ):

	unsigned I = 0;
	for (unsigned i = 0; i < N; i++)
	{
		for (unsigned j = 0; j < N; j++)
		{
			for (int z = 0; z < M; z++)
			{
				p[z][i][j] = bddtrue;
				for (unsigned k = 0; k < LOG_N; k++)
					p[z][i][j] &= ((j >> k) & 1) ? bdd_ithvar(I + LOG_N * z + k) : bdd_nithvar(I + LOG_N * z + k);
			}
		}
		I += LOG_N * M;
	}

	bdd task = bddtrue;

	//Ограничение 1 типа
  
  // p[номер свойства - 1][номер обьекта][значение]
	//У объекта №4 0 параметр (ну или 1 смотря как считать) имеет значение 3
	task &= p[1][4][3];
	task &= p[0][6][5];
	task &= p[1][7][4];
	task &= p[3][4][4];
	task &= p[2][7][4];
	task &= p[2][5][6];
	task &= p[2][6][0];
	task &= p[3][7][0];
	task &= p[3][8][7];
	task &= p[1][2][8];

  //ДОП
  task &= p[2][4][1];
  task &= p[2][4][1];
  task &= p[3][5][1];
  task &= p[0][0][2];

	//Ограничение 2 типа
	//У объекта со 2 параметром = 8 0 параметр = 3
  // F := F ∧ (p(k1, i, j1) ↔ p(k2, i, j2))

	for (int i = 0; i < N; i++)
	{
		task &= !(p[2][i][8] ^ p[0][i][3]);
	}
	for (int i = 0; i < N; i++)
	{
		task &= !(p[1][i][4] ^ p[0][i][7]);
	}
	for (int i = 0; i < N; i++)
	{
		task &= !(p[2][i][3] ^ p[3][i][3]);
	}
	for (int i = 0; i < N; i++)
	{
		task &= !(p[1][i][3] ^ p[0][i][4]);
	}
	for (int i = 0; i < N; i++)
	{
		task &= !(p[1][i][8] ^ p[0][i][1]);
	}
    for (int i = 0; i < N; i++)
	{
		task &= !(p[1][i][5] ^ p[2][i][7]);
	}
  	for (int i = 0; i < N; i++)
	{
		task &= !(p[0][i][6] ^ p[2][i][6]);
	}
  	for (int i = 0; i < N; i++)
	{
		task &= !(p[0][i][0] ^ p[1][i][2]);
	}

	//Ограничение 3 типа
	//Слева от объекта со 2 параметром = 8 объект, у которого 3 параметр = 1
	for (int i = 0; i < N; i++)
	{
		if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8)
		{
			task &= !(p[0][i][4] ^ p[2][i - 3][8]);
		}
		else //Если у объекта нет соседа слева
		{
			task &= !p[0][i][4];
		}
	}
	for (int i = 0; i < N; i++)
	{
		if (i == 3 || i == 4 || i == 6 || i == 7)
		{
			task &= !(p[0][i][7] ^ p[1][i - 2][1]);
		}
		else
		{
			task &= !p[0][i][7];
		}
	}
	for (int i = 0; i < N; i++)
	{
		if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8)
		{
			task &= !(p[3][i][2] ^ p[0][i - 3][0]);
		}
		else
		{
			task &= !p[3][i][2];
		}
	}
  	for (int i = 0; i < N; i++)
	{
		if (i == 3 || i == 4 || i == 6 || i == 7)
		{
			task &= !(p[0][i][0] ^ p[0][i - 2][3]);
		}
		else
		{
			task &= !p[0][i][0];
		}
	}
  	for (int i = 0; i < N; i++)
	{
		if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8)
		{
			task &= !(p[0][i][5] ^ p[1][i - 3][2]);
		}
		else
		{
			task &= !p[0][i][5];
		}
	}

	//Ограничение 4 типа
	bdd tmp = bddfalse;
	//с рядом(слева или справа) от объекта со 2 параметром = 2 объект со 2 параметром = 6
	for (int i = 0; i < N; i++)
	{
		//Справа
		if (i == 3 || i == 4 || i == 6 || i == 7)
		{
			tmp |= !(p[0][i][5] ^ p[0][i - 2][4]);
		}
		//Или слева
		if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8)
		{
			tmp |= !(p[0][i][5] ^ p[0][i - 3][4]);
		}
		//Если объект не имеет соседей
		if (!(i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8))
		{
			task &= !p[0][i][5];
		}
		else
		{
			task &= tmp;
			tmp = bddfalse;
		}
	}
  	for (int i = 0; i < N; i++)
	{
		//Справа
		if (i == 3 || i == 4 || i == 6 || i == 7)
		{
			tmp |= !(p[2][i][5] ^ p[3][i - 2][8]);
		}
		//Или слева
		if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8)
		{
			tmp |= !(p[2][i][5] ^ p[3][i - 3][8]);
		}
		//Если объект не имеет соседей
		if (!(i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8))
		{
			task &= !p[2][i][5];
		}
		else
		{
			task &= tmp;
			tmp = bddfalse;
		}
	}
    	for (int i = 0; i < N; i++)
	{
		//Справа
		if (i == 3 || i == 4 || i == 6 || i == 7)
		{
			tmp |= !(p[3][i][6] ^ p[1][i - 2][0]);
		}
		//Или слева
		if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8)
		{
			tmp |= !(p[3][i][6] ^ p[1][i - 3][0]);
		}
		//Если объект не имеет соседей
		if (!(i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8))
		{
			task &= !p[3][i][6];
		}
		else
		{
			task &= tmp;
			tmp = bddfalse;
		}
	}
	
	// Ограничение 6 (количество значений параметров равно количеству объектов)
	for (int i = 0; i < N; i++)
	{

		bdd tmp[M];
		for (int j = 0; j < M; j++)
		{
			tmp[j] = bddfalse;
		}
		for (int z = 0; z < N; z++)
		{
			for (int j = 0; j < M; j++)
			{
				tmp[j] |= (p[j][i][z]);
			}
		}
		for (int j = 0; j < M; j++)
		{
			task &= tmp[j];
		}
	}

	// Ограничение 5
	for (int k = 0; k < M; k++)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (i != j)
				{
					for (int z = 0; z < N; z++)
					{
						task &= (p[k][i][z] >> !p[k][j][z]);
					}
				}
			}
		}
	}

	// вывод результатов
	out.open("./results/solutions.txt");
	double satcount = bdd_satcount(task);
	cnt = satcount;
	printf("%lf\n", satcount);
	out << satcount << " solutions:\n"
		<< endl;

	if (satcount)
	{
		bdd_allsat(task, fun);
		printf("ALL");
	}

	out.close();
	printf("%lf\n", cnt);
	bdd_done(); // завершение работы библиотеки
	return 0;
}

char var[N_VAR];

void print(void)
{
	for (unsigned i = 0; i < N; i++)
	{
		out << i << ": ";
		for (unsigned j = 0; j < M; j++)
		{
			unsigned J = i * M * LOG_N + j * LOG_N;
			unsigned num = 0;
			for (unsigned k = 0; k < LOG_N; k++)
				num += (unsigned)(var[J + k] << k);
			out << num << ' ';
		}
		out << endl;
	}
	out << endl;
}

void build(char* varset, unsigned n, unsigned I)
{
	if (I == n - 1)
	{
		if (varset[I] >= 0)
		{
			var[I] = varset[I];
			print();
			return;
		}
		var[I] = 0;
		print();
		var[I] = 1;
		print();
		return;
	}
	if (varset[I] >= 0)
	{
		var[I] = varset[I];
		build(varset, n, I + 1);
		return;
	}
	var[I] = 0;
	build(varset, n, I + 1);
	var[I] = 1;
	build(varset, n, I + 1);
}

void fun(char* varset, int size)
{
	build(varset, size, 0);
}