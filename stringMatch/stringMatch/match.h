#include <string>
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