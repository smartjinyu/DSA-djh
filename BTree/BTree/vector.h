/*
数据结构C++ 第二章
*/
#include <iostream>
using namespace std;
typedef int Rank;
#define DEFAULT_CAPACITY 3
template <typename T> class Vector {
protected:
	Rank _size; int _capacity; T* _elem;
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	bool bubble(Rank lo, Rank hi);//扫描交换
	void bubbleSort(Rank lo, Rank hi);//起泡排序
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);//选择排序
	void merge(Rank lo, Rank mi, Rank hi);//归并算法
	void mergeSort(Rank lo, Rank hi);//归并排序算法
	Rank partition(Rank lo, Rank hi);//轴点构造算法
	void quickSort(Rank lo, Rank hi);//快速排序算法
	void heapSort(Rank lo, Rank hi);//堆排序算法

									//比较器
	template<typename T> static bool lt(T* a, T* b) { return lt(*a, *b); }
	template<typename T> static bool lt(T& a, T& b) { return a<b; }
	template<typename T> static bool eq(T* a, T* b) { return eq(*a, *b); }
	template<typename T> static bool eq(T& a, T& b) { return a == b; }

public:
	//constructor
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {//容量为c，规模为s，所有元素初始化为v
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }//数组区间复制
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }//数组整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }//向量区间复制
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }//向量整体复制
																 //destructor
	~Vector() { delete[] _elem; }
	//只读接口
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const;//是否有序
	Rank find(T const& e) const { return find(e, 0, (Rank)_size); }//无序向量整体查找
	Rank find(T const& e, Rank lo, Rank hi)const;//无序向量区间查找
	Rank search(T const& e)const { return (0 >= _size) ? -1 : search(e, 0, _size); }//有序向量整体查找
	Rank search(T const& e, Rank lo, Rank hi)const;//有序向量区间查找

												   //可写访问接口
	T& operator[](Rank r) { return _elem[r]; }
	Vector<T> & operator=(Vector<T> const&);
	T remove(Rank r) { T e = _elem[r]; remove(r, r + 1); return e; }
	int remove(Rank lo, Rank hi);//区间删除算法
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }//在末尾插入元素
	void sort(Rank lo, Rank hi);//对[lo,hi)区间排序
	void sort() { sort(0, _size); }//整体排序
	void unsort(Rank lo, Rank hi);//区间置乱
	void unsort() { unsort(0, _size); }//整体置乱
	int deduplicate();//无序去重
	int uniquify();//整体去重

				   //遍历
	void traverse(void(*visit)(T&)) {//遍历（使用函数指针，适用于只读或局部性修改）
		for (Rank i = 0; i < _size; i++) {
			visit(_elem[i]);
		}
	}
	template<typename VST> void traverse(VST& visit) { //遍历（使用函数对象，全局性修改）
		for (Rank i = 0; i < _size; i++) {
			visit(_elem[i]);
		}
	}
};

template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while (lo < hi) {
		_elem[_size++] = A[lo++];
	}
}

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
/*gcc环境下rand() compile error
template<typename T>
void Vector<T> :: unsort(Rank lo, Rank hi) {
T* V = _elem + lo;
for (Rank i = hi - lo; i > 0; i--) {
swap(V[i-1], V[rand() % i]);
}
}*/
template<typename T>//无序向量的顺序查找，返回最后一个元素的秩，失败返回lo-1
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ((lo < hi--) && (_elem[hi] != e));
	return hi;
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

template<typename T>
int Vector<T>::deduplicate() {
	int oldsize = _size;
	Rank i = 1;
	while (i < _size) {
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i);
	}
	return _size - oldsize;//返回向量规模变化量
}
template<typename T>
int Vector<T>::uniquify() {//有序向量的唯一化（高效版本）
	Rank i = 0, j = 0;
	while (++j < _size) {
		if (_elem[i] != _elem[j]) {
			_elem[++i] = _elem[j];
		}
	}
	_size = ++i; shrink();
	return j - i;//返回被删除的元素总数
}

template<typename T>
static Rank binSearchA(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		if (e < A[mi]) { hi = mi; }
		else if (A[mi] > e) { lo = mi; }
		else return mi;
	}
	return -1;//fail
}

//omit fibnacci search

template<typename T>
static Rank binSearchC(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi) {
		Rank mi = (lo + hi) >> 1;
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}
template<typename T>
Rank Vector<T>::search(T const & e, Rank lo, Rank hi) const
{
	return binSearchC(_elem, e, lo, hi);
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
	mergeSort(lo, hi);
}
//归并排序
template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2) { return; }
	Rank mi = (lo + hi) >> 1;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem + lo;//合并后向量A[0,hi-li)=_elem[lo,hi)
	Rank lb = mi - lo; T* B = new T[lb];
	for (Rank i = 0; i < lb; B[i] = A[i++]);//复制前子向量B[lo,mi)
	Rank lc = hi - mi; T* C = _elem + mi;//后子向量C[mi,hi)
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) { A[i++] = B[j++]; }
		if ((k < lc) && (!(j < lb) || (C[k] < B[j]))) { A[i++] = C[k++]; }
	}
	delete[] B;

}
