#include "ListNode.h"
using namespace std;
template<typename T> class List {
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) tailor;
protected:
	void init();//列表创建时初始化
	int clear();
	void copyNodes(ListNodePosi(T), int);//复制自位置p起的n项
	void merge(ListNodePosi(T), int, List<T>&, ListNodePosi(T), int);//有序列表区间合并
	void mergeSort(ListNodePosi(T)&, int);//对p开始的连续n个节点归并排序
	void selectionSort(ListNodePosi(T), int);//选择排序
	void insertionSort(ListNodePosi(T), int);//插入排序
public:
	//constructor
	List() { init(); }
	List(List<T> const& L);//整体复制列表L
	List(List<T> const& L, Rank r, int n);//复制自第r项起的n项
	List(ListNodePosi(T) p, int n);//复制列表中自位置p起的n项
								   //destructor
	~List() { clear(); delete header; delete tailor; }
	//read-only interface
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[](int r) const;//循秩访问，效率低下
	ListNodePosi(T) first() const { return header->succ; }
	ListNodePosi(T) last() const { return tailor->pred; }
	bool valid(ListNodePosi(T) p) { return p && (p != tailor)&(p != header); }//判断p是否合法
	int disordered() const;
	ListNodePosi(T) find(T const& e)const { return find(e, _size, tailor); }
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;//无序列表区间查找(p以及前n-1个前驱)
	ListNodePosi(T) search(T const& e)const { return find(e, _size, tailor); }
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;//有序列表区间查找(p以及前n-1个前驱)
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n) const;//在p以及n-1个后继中选择最大者
	ListNodePosi(T) selectMax() const { return selectMax(header->succ, _size); }

	//read-write interface
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);//删除p处的节点并返回被删除的节点内容
	void merge(List<T>& L) { merge(first(), size(), L, L.first(), L.size()); }//全列表归并
	void sort(ListNodePosi(T) p, int n);//列表区间排序
	void sort() { sort(first(), _size); }
	int deduplicate();//无序去重
	int uniqualify();//有序去重
	void reverse();//前后倒置

				   //traverse
	void traverse(void(*visit)(T&)) {//函数指针
		for (ListNodePosi(T) p = header->succ; p != tailor; p = p->succ)visit(p->data);
	}
	template<typename VST>
	void traverse(VST& visit) {//函数对象
		for (ListNodePosi(T) p = header->succ; p != tailor; p = p->succ) visit(p->data);
	}
	template <typename T> //有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
	void List<T>::merge(ListNodePosi(T) & p, int n, List<T>& L, ListNodePosi(T) q, int m) {
		// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
		//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
		// 注意：在归并排序之类的场合，有可能 this == L && rank(p) + n = rank(q)
		ListNodePosi(T) pp = p->pred; //借助前驱（可能是header），以便返回前 ...
		while (0 < m) //在q尚未移出区间之前
			if ((0 < n) && (p->data <= q->data)) //若p仍在区间内且v(p) <= v(q)，则
			{
				if (q == (p = p->succ)) break; n--;
			} //p归入合并的列表，并替换为其直接后继
			else //若p已超出右界或v(q) < v(p)，则
			{
				insertB(p, L.remove((q = q->succ)->pred)); m--;
			} //将q转移至p之前
		p = pp->succ; //确定归并后区间的（新）起点
	}
};

template<typename T>
void List<T>::init() {//初始化，在创建列表对象时统一调用
	header = new ListNode<T>;
	tailor = new ListNode<T>;
	header->succ = tailor; header->pred = nullptr;
	tailor->pred = header; tailor->succ = nullptr;
	_size = 0;
}
template<typename T>
T& List<T>::operator[](int r) const {
	ListNodePosi(T) p = first();
	while (0 < r--)p = p->succ;
	return p->data;
}
template<typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--) {
		if (e == (p = p - < pred)->data)return p;//逐个比对
	}
	return NULL;
}
template<typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e) {
	_size++;
	return header->insertAsSucc(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertAsLast(T const & e)
{
	_size++;
	return tailor->insertAsPred(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const & e)
{
	_size++;
	return p->insertAsPred(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const & e)
{
	_size++;
	return p->insertAsSucc(e);
}
template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n) {//复制列表中自p起的n项，要求p合法并且至少有n-1个后继
	init();
	while (n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}
template<typename T>
List<T>::List(ListNodePosi(T) p, int n) { copyNodes(p, n); }
template<typename T>
List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }//整体复制列表L
template<typename T>
List<T>::List(List<T> const& L, int r, int n) { copyNodes(L[r], n); }//复制列表L自第r项起的n项

template<typename T>
T List<T>::remove(ListNodePosi(T) p) {
	T e = p->data;
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p; _size--;
	return e;
}
template<typename T>
int List<T>::clear() {
	int oldSize = _size;
	while (0 < _size)remove(header->succ);
	return oldSize;
}
template<typename T>
int List<T>::deduplicate() {
	if (_size < 2)return 0;
	int oldSize = _size;
	ListNodePosi(T) p = header; Rank r = 0;
	while (tailor != (p = p->succ)) {
		ListNodePosi(T) q = find(p->data, r, p);//在p的r个前驱中寻找相同元素
		q ? remove(q) : r++;
	}
	return _size - oldSize;
}
template<typename T>
int List<T>::uniqualify() {//有序列表的唯一化
	if (_size < 2)return 0;
	int oldSize = _size;
	ListNodePosi(T) p; ListNodePosi(T) q;
	for (p = header, q = p->succ; tailor != q; p = q, q = q->succ) {
		if (p->data == q->data) { remove(q); q = p; }
	}
	return oldSize - _size;
}
template<typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {//在p的n个真前驱中返回不大于e的最大者
	while (0 <= n--) {//逐个比较
		if (((p = p->pred)->data) <= e)break;
	}
	return p;
}//失败时可能返回header，应调用valid()函数确认有效性
template<typename T>
void List<T>::sort(ListNodePosi(T) p, int n) {
	//insertionSort(p, n);
	//selectionSort(p, n);
	mergeSort(p, n);

	//rand() may cause compile error in gcc and g++
}
template<typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n) {
	for (int r = 0; r < n; r++) {
		insertAfter(search(p->data, r, p), p->data);
		p = p->succ; remove(p->pred);
	}
}
template<typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n) {
	ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++)tail = tail->succ;//待排序区间为(head,tail)
	while (1 < n) {
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertBefore(tail, remove(max));
		tail = tail->pred; n--;
	}
}
template<typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) const {
	ListNodePosi(T) max = p;
	for (ListNodePosi(T) cur = p; i < n; n--) {
		if (!((cur = cur->succ)->data < max->data)) {//可能需要重载<
			max = cur;
		}
	}
	return max;
}/*
 template<typename T>
 void List<T>::merge(ListNodePosi(T) p, int n, List<T>& L, ListNodePosi(T) q, int m) {
 //有序列表的归并：当前列表中自p起的n个元素与列表L中自q起的m个元素归并
 //assert: this.valid(p) && rank(p) + n < size && this.sorted(p,n)
 //        L.valid(p) && rank(q) + m <= L._size && L.sorted(q,m)
 ListNodePosi(T) pp = p->pred;
 while (0 < m) {
 if ((0 < n) && (p->data <= q->data)) {
 if (q == (p = p->succ))break;
 n--;
 }
 else {
 insertBefore(p, L.remove((q = q->succ)->pred));
 m--;
 }
 }
 p = pp->succ;
 }*/
template <typename T> //有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
void List<T>::merge(ListNodePosi(T)  p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	// 注意：在归并排序之类的场合，有可能 this == L && rank(p) + n = rank(q)
	ListNodePosi(T) pp = p->pred; //借助前驱（可能是header），以便返回前 ...
	while (0 < m) //在q尚未移出区间之前
		if ((0 < n) && (p->data <= q->data)) //若p仍在区间内且v(p) <= v(q)，则
		{
			if (q == (p = p->succ)) break; n--;
		} //p归入合并的列表，并替换为其直接后继
		else //若p已超出右界或v(q) < v(p)，则
		{
			insertBefore(p, L.remove((q = q->succ)->pred)); m--;
		} //将q转移至p之前
	p = pp->succ; //确定归并后区间的（新）起点
}

template<typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
	if (n < 2)return;
	int m = n >> 1;
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++)q = q->succ;
	mergeSort(p, m); mergeSort(q, n - m);//分别排序
	merge(p, m, *this, q, n - m);
}
