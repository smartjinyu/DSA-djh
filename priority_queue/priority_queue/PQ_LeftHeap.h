#include "PQ.h"
#include "BinTree.h"

template<typename T>
class PQ_LeftHeap :public PQ<T>, public BinTree<T> {
public:
	PQ_LeftHeap() {}
	PQ_LeftHeap(T* E, int n) {//可改进为Floyd建堆算法,O(n)
		for (int i = 0; i < n; i++) insert(E[i]);//O(nlogn)
	}
	void insert(T);
	T getMax();
	T delMax();
};

template<typename T>
static void exchangeChildren(BinNodePosi(T) v) {//exchange v's left and right child
	BinNodePosi(T) t = v->lChild;
	v->lChild = v->rChild;
	v->rChild = t;
}

template<typename T>
static BinNodePosi(T) merge1(BinNodePosi(T) a, BinNodePosi(T) b) {//合并以a和b为根节点的两个左式堆，其中a的优先级更高
	if (!HasLChild(*a)) {//左式堆没有左孩子，那么就是光杆
		a->lChild = b; b->parent = a;
	}
	else {
		a->rChild = merge(a->rChild, b);
		a->rChild->parent = a;
		if (a->lChild->npl < a->rChild->npl) {
			exchangeChildren(a);
		}
		a->npl = a->rChild->npl + 1;
	}
	return a;//返回合并后的堆顶
}

template<typename T>
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b) {//根据相对优先级确定适宜的方式，合并以a和b为根节点的两个左式堆
	if (!a) return b;
	if (!b) return a;//递归基
	return lt(a->data, b->data) ? merge1(b, a) : merge1(a, b);
}//堆的规模等由调用者负责更新

template<typename T>
T PQ_LeftHeap<T>::delMax() {
	BinNodePosi(T) lHeap = _root->lChild;
	BinNodePosi(T) rHeap = _root->rChild;
	T e = _root->data; delete _root; _size--;
	_root = merge(lHeap, rHeap);
	if (_root) _root->parent = NULL;
	return e;
}

template<typename T>
void PQ_LeftHeap<T>::insert(T e) {
	BinNodePosi(T) v = new BinNode<T>(e);
	_root = merge(_root, e);
	_root->parent = NULL;
	_size++;
}