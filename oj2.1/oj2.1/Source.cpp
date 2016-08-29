/*
列车调度(Train)
Description
Figure 1 shows the structure of a station for train dispatching.



Figure 1

In this station, A is the entrance for each train and B is the exit. S is the transfer end. All single tracks are one-way, which means that the train can enter the station from A to S, and pull out from S to B. Note that the overtaking is not allowed. Because the compartments can reside in S, the order that they pull out at B may differ from that they enter at A. However, because of the limited capacity of S, no more that m compartments can reside at S simultaneously.

Assume that a train consist of n compartments labeled {1, 2, …, n}. A dispatcher wants to know whether these compartments can pull out at B in the order of {a1, a2, …, an} (a sequence). If can, in what order he should operate it?

Input
Two lines:

1st line: two integers n and m;

2nd line: n integers separated by spaces, which is a permutation of {1, 2, …, n}. This is a compartment sequence that is to be judged regarding the feasibility.

Output
If the sequence is feasible, output the sequence. “Push” means one compartment goes from A to S, while “pop” means one compartment goes from S to B. Each operation takes up one line.

If the sequence is infeasible, output a “no”.

Example 1
Input

5 2
1 2 3 5 4
Output

push
pop
push
pop
push
pop
push
push
pop
pop
Example 2
Input

5 5
3 1 2 4 5
Output

No
Restrictions
1 <= n <= 1,600,000

0 <= m <= 1,600,000

Time: 2 sec

Memory: 256 MB

*/
// 100/100 AC
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
typedef int Rank;
#define DEFAULT_CAPACITY 3000
template <typename T> class Vector {
public:
	Rank _size; int _capacity; T* _elem;
	void expand();
	void shrink();
public:
	//constructor
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {//容量为c，规模为s，所有元素初始化为v
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	 //destructor
	~Vector() { delete[] _elem; }
	//只读接口
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	//可写访问接口
	T& operator[](Rank r) { return _elem[r]; }
	Vector<T> & operator=(Vector<T> const&);
	T remove(Rank r) { T e = _elem[r]; remove(r, r + 1); return e; }
	int remove(Rank lo, Rank hi);//区间删除算法
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }//在末尾插入元素
};

template<typename T>
Vector<T> & Vector<T>::operator=(Vector<T> const& V) {
	if (_elem) { delete[] _elem; }
	copyFrom(V._elem, 0, V._size);
	return *this;
}

template<typename T>
void Vector<T>::expand() {
	if (_size < _capacity) { return; }
	if (_capacity < DEFAULT_CAPACITY) { _capacity = DEFAULT_CAPACITY; }
	T* oldelem = _elem;
	_elem = new T[_capacity <<= 1];
	for (Rank i = 0; i < _size; i++) {
		_elem[i] = oldelem[i];
	}
	delete[] oldelem;
}

template<typename T>
void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY) { return; }
	if (_size << 2 > _capacity) { return; }//以25%为界
	T* oldelem = _elem;
	_elem = new T[_capacity >>= 1];
	for (Rank i = 0; i < _size; i++) {
		_elem[i] = oldelem[i];
	}
	delete[] oldelem;
}


template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (Rank i = _size; i > r; i--) {
		_elem[i] = _elem[i - 1];
	}
	_elem[r] = e; _size++;
	return r;
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi) { return 0; }
	while (hi < _size) { _elem[lo++] = _elem[hi++]; }
	_size = lo;
	shrink();
	return hi - lo;
}


template<typename T>class Stack :public Vector<T> {
public:
	void push(T const& e) { Vector<T>::insert(Vector<T>::size(), e); }
	T pop() { return Vector<T>::remove(Vector<T>::size() - 1); }
	T& top() { return (*this)[Vector<T>::size() - 1]; }
};

#define maxN 16000010	
int B[maxN];
int cmd[maxN*2+1];//in cmd, 0 represent1 push and 1 represents pop
int main() {
	int N, M;
	Stack<int> A, S;
	scanf("%d %d", &N, &M);
	A.push(maxN + 1);
	for (int i = N; i >= 1; i--) {
		A.push(i);
		scanf("%d", &B[N-i]);
	}
	int br = 0;
	int curb;
	int id = 0;
	while (br!=N) {
		curb = B[br];
		while (curb >= A.top()) {
			S.insert(A.pop());
			cmd[id++]=0;
		}
		if (S.size() > M)break;
		if (curb != S.top()) {
			break;
		}
		else {
			S.pop();
			cmd[id++]=1;
		}
		br++;
	}
	if (br == N) {//succeed
		for (int i = 0; i < id; i++) {
			if (cmd[i] == 0) {
				printf("push\n");
			}
			else {
				printf("pop\n");
			}
		}
	}
	else {
		printf("No\n");
	}
	return 0;
}