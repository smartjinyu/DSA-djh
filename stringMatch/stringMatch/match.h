#include <string>
#include <algorithm>
typedef int size_t;
//Brutal-Force 最坏O(m*n) expected可达O(n)
/*
* Text:     0 1 2 3 - - - - - i-j - - - - i - - - - n-1
							   |- - - - - |
* Pattern:					   0 - - - - -j
*/
int BFmatch1(char* P, char *T) {//串匹配算法(Brute-force-1)
	size_t n = strlen(T), i = 0;//主串长度，当前接受比对字符位置
	size_t m = strlen(P), j = 0;//模式串长度
	while (i < n && j < m) {
		if (T[i] == P[j]) {
			i++; j++;
		}
		else {
			i -= j - 1; j = 0;//主串回退，模式串复位
		}
	}
	return i - j;//若成功，则i-j为首个匹配位置；若失败，则i-j>n-m
}


/*
* Text:     0 1 2 3 - - - - - i i+1 - - - i+j - - - - n-1
|  - - - - - |
* Pattern:					  0  1 - - - - j
*/
int BFmatch2(char* P, char *T) {//串匹配算法(Brute-force-2)
	size_t n = strlen(T), i = 0;//主串长度，与模式串首字符对齐的位置
	size_t m = strlen(P), j;//模式串长度,当前接受比对位置
	for (i = 0; i < n - m + 1; i++) {
		for (j = 0; j < m; j++) {
			if (T[i + j] != P[j])break;
		}
		if (j >= m)break;//找到匹配字串
	}
	
	return i - j;//若成功，则i-j为首个匹配位置；若失败，则i-j>n-m
}
//

//KMP 最坏O(n+m)
int *buildNext(char* P) {
	size_t m = strlen(P), j = 0;
	int* N = new int[m];
	int t = N[0] = -1;
	while (j < m - 1) {
		if (0 > t || P[j] == P[t]) {
			j++; t++;
			//N[j]=t;
			//改进
			N[j] = (P[j] != P[t]) ? t : N[t];
		}
		else {
			t = N[t];
		}
	}
	return N;
}
/*
* Text:     0 1 2 3 - - - - - i-j - - - - i - - - - n-1
							   |- - - - - |
* Pattern:					   0 - - - - -j
*/
int KMPmatchz(char* P, char* T) {//KMP算法
	int* next = buildNext(P);
	int n = (int)strlen(T), i = 0;
	int m = (int)strlen(P), j = 0;
	while ((j < m) && (i < n)) {
		if ((0 > j) || (T[i] == P[j])) {
			i++; j++;
		}
		else {
			j = next[j];
		}
	}
	delete[] next;
	return i - j;
}

//BM
int* buildBC(char* P) {
	int* bc = new int[256];//与字符表等长
	for (size_t j = 0; j < 256; j++)bc[j] = 0;
	for (size_t m = strlen(P), j = 0; j < m; j++) {
		bc[P[j]] = j;//画家算法，后面重复的覆盖前面的，保证所取的j为最大
	}
	return bc;
}
int* buildSS(char* P) {
	int m = strlen(P); int* SS = new int[m];
	SS[m - 1] = m;
	for (int lo = m - 1, hi = m - 1, j = lo - 1; j >= 0; j--) {
		if ((lo < j) && (SS[m - hi + j - 1] <= j - lo)) {
			SS[j] = SS[m - hi + j - 1];
		}
		else {
			hi = j; lo = _min(lo, hi);
			while ((0 <= lo) && (P[lo] == P[m - hi + lo - 1])) {
				lo--;
			}
			SS[j] = hi - lo;
		}
	}
	return SS;
}
int* buildGS(char* P) {
	int* SS = buildSS(P);
	size_t m = strlen(P); int* GS = new int[m];
	for (size_t j = 0; j < m; j++)GS[j] = m;
	for (size_t i = 0, j = m - 1; j < UINT_MAX; j--) {
		if (j + 1 == SS[j])
			while (i < m - j - 1)
				GS[i++] = m - j - 1;
	}
	for (size_t j = 0; j < m - 1; j++)
		GS[m - SS[j] - 1] = m - j - 1;
	delete[] SS; return SS;
}
int BMmatch(char* P, char* T) {//Boyer-Morre算法，兼顾Bad Character and Good Suffix
	int* bc = buildBC(P); int* gs = buildGS(P);
	size_t i = 0;//模式串相对于主串的起始位置
	while (strlen(T) >= i + strlen(P)) {
		int j = strlen(P) - 1;//从模式串的最后一个字符开始
		while (P[j] == T[i + j]) {
			if (0 > --j)break;
		}
		if (0 > j) {
			break;//极大匹配后缀==整个模式串 则匹配成功
		}
		else {
			i += _max(gs[j], j - bc[T[i + j]]);//位移量根据BC表和GS表选择大者
		}
	}
	delete[] gs; delete[] bc;
	return i;
}//最好O(n/m) 最坏O(n+m)