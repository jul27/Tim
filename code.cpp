#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#define N 100
int C[N] = { 0 }, P[N];
typedef struct{
	int n;//числитель
	int d;//знаминатель
} fraction;
fraction Z = { -1, 1 }, *Plan;
int NOD(int a, int b){/* возвращает НОД чисел a и b*/
	while (a != 0 && b != 0){
		if (a>b)
			a = a%b;
		else
			b = b%a;
	}
	return (a + b);
}
int NOK(int a, int b){/* возвращает НОК чисел a и b*/
	return(abs(a*b) / NOD(a, b));
}
void reducing_f(fraction* a){/* сокращает дробь a*/
	int tmp;
	if ((*a).n != 0){
		tmp = NOD(abs((*a).d), abs((*a).n));
		if (tmp != 1){
			(*a).n /= tmp;
			(*a).d /= tmp;
		}
	}
	else
		(*a).d = 1;
}
fraction addition(fraction a1, fraction a2){/* возвращает дробь, равную сумме дробей a1 и a2*/
	fraction rez;
	int tmp;
	rez.d = NOK(a1.d, a2.d);
	rez.n = a1.n*rez.d / a1.d + a2.n*rez.d / a2.d;
	reducing_f(&rez);
	return rez;
}
fraction subtraction(fraction a1, fraction a2){/* возвращает дробь, равную разности дробей a1 и a2*/
	a2.n *= (-1);
	return(addition(a1, a2));
}
fraction multiplication(fraction a1, fraction a2){/* возвращает дробь, равную произведению дробей a1 и a2*/
	fraction rez;
	rez.n = a1.n*a2.n;
	rez.d = a1.d*a2.d;
	reducing_f(&rez);
	return rez;
}
fraction division(fraction a1, fraction a2){/* возвращает дробь, равную частному дробей a1 и a2*/
	int t = a2.d;
	a2.d = a2.n;
	a2.n = t;
	if (a2.d < 0){
		a2.d *= (-1);
		a2.n *= (-1);
	}
	return(multiplication(a1, a2));
}
int is_st_0(fraction*s, int n){
	int i = 0;
	while (s[i].n == 0)
		i++;
	return (i == n);
}
fraction** basis(fraction **s, int n, int &m){
	int i, j, k, l;
	fraction r, *tmp, t;
	for (i = 0; i < m; i++){
		t = s[i][i];
		j = i;
		while (s[j][i].n == 0 && j < m)
			j++;
		if (j != i){
			tmp = s[i];
			s[i] = s[j];
			s[j] = tmp;
		}
		for (j = 0; j < n; j++)
			s[i][j] = division(s[i][j], t);
		for (j = 0; j < m; j++){
			t = s[j][i];
			if (j != i){
				for (k = 0; k < n; k++){
					r = multiplication({ -1, 1 }, s[i][k]);
					r = multiplication(r, t);
					s[j][k] = addition(s[j][k], r);
				}
				if (is_st_0(s[j], n)){
					for (l = j; l < m-1; l++)
						s[l] = &s[l+ 1][0];
					j--;
					m--;
				}
			}
		}
	}
	return s;
}
fraction** conversion(fraction**s, int n, int m, int x1, int x2){
	int j, i, k=0;
	while (s[k][x1 - 1].n == 0)
		k++;
	fraction t = s[k][x2-1], r;
	for (j = 0; j < n; j++)
		s[k][j] = division(s[k][j], t);
	for (j = 0; j < m; j++){
		t = s[j][x2 - 1];
		if (j != k){
			for (i = 0; i < n; i++){
				r = multiplication({ -1, 1 }, s[k][i]);
				r = multiplication(r, t);
				s[j][i] = addition(s[j][i], r);
			}
		}
	}
	return s;
}
int search(int n, int i){// возвращает 1, если в массиве P размера n есть элемент i, иначе 0
	int j = 0;
	while (i != P[j] && j < n)
		j++;
	return (j < n);
}
void printf_plan(fraction**s, int n, int m, FILE*g){
	int i, j=0, k;
	fraction z = {0, 1}, tmp[N];
	i = 0;
	while (i<m && s[i][n - 1].n > 0){
		z = addition(z, s[i][n - 1]);
		i++;
	}
	if (i == m){
		fprintf(g, "опорный план\t{");
		for (i = 0; i < n; i++){
			if (i == C[j] - 1){
				k = 0;
				while (s[k][C[j] - 1].n == 0)
					k++;
				tmp[i] = s[k][n - 1];
				(s[k][n - 1].d == 1 || s[k][n - 1].n == 0) ? fprintf(g, "%i\t", s[k][n - 1].n) : fprintf(g, "%i/%i\t", s[k][n - 1].n, s[k][n - 1].d);
				j++;
			}
			else{
				fprintf(g, "0 ");
				tmp[i].n = 0; tmp[i].d = 1;
			}
		}
		fprintf(g, "}\n");
		fprintf(g, "z = ");
		(z.d == 1 || z.n == 0) ? fprintf(g, "%i\t", z.n) : fprintf(g, "%i/%i\t", z.n, z.d);
		if (z.n/z.d > Z.n/Z.d){
			Z = z;
			Plan = tmp;
		}
	}
	else
		fprintf(g, "Опорного плана нет\n");
}
void combination(int i, int b, int n, int k, fraction**s, FILE*g){
	int x, j;
	for (x = b; x <= n - k + i; x++)
	{
		C[i] = x;
		if (i == k - 1)
		{
			printf("\n\nСочетание = ");
			for (j = 0; j<k; j++)
				printf("%i ", C[j]);
			printf("\n");
			for (j = 0; j < k; j++){
				if (!search(k, C[j]))
					s = conversion(s, n, k, P[j], C[j]);
			}
			for (int l = 0; l<k; l++){
				for (j = 0; j<n; j++){
					(s[l][j].d == 1 || s[l][j].n == 0) ? printf("%i\t", s[l][j].n) : printf("%i/%i\t", s[l][j].n, s[l][j].d);
				}
				printf("\n");
			}
			printf_plan(s, n, k, g);
			for (j = 0; j < k; j++)
				P[j] = C[j];
		}
		else combination(i + 1, x + 1, n, k, s, g);
	}
}
void main(){
	setlocale(LC_CTYPE, "");
	FILE* g = fopen("result.txt", "a");
	if (g == NULL) {
		perror("Ошибка при открытии");
		return;
	}
	FILE* f=fopen("test.txt", "rt");
	if (f == NULL) {
		perror("Ошибка при открытии");
		return;
	}
	int n, m, i, j;
	//puts("Введите количество уравнений в системе");
	fscanf(f, "%i", &m);
	//puts("Введите количество координат ");
	fscanf(f, "%i", &n);
	n++;
	fraction **b = (fraction**)calloc(m, sizeof(fraction*));
	for (i = 0; i < m; i++)
		b[i] = (fraction*)calloc(n, sizeof(fraction));
	//puts("Введите систему");
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			fscanf(f, "%i", &b[i][j].n);
			b[i][j].d = 1;
		}
	}
	for (i = 0; i < N; i++)
		P[i] = i + 1;
	puts("OK");
	b = basis(b, n, m);
	combination(0, 1, n, m, b, g);
	puts("OK");
	fprintf(g, "Для целевой функции z = x1 + x2+..+xn оптимальный опорный план:\n{");
	for (i = 0; i < n; i++){
		(Plan[i].d == 1 || Plan[i].n == 0) ? fprintf(g, "%i\t", Plan[i].n) : fprintf(g, "%i/%i\t", Plan[i].n, Plan[i].d);
			i++;
	}
	fprintf(g, "}\n");
	fclose(f);
	fclose(g);
	system("pause");
}
