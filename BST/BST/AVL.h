#include "BST.h"
#define Balanced(x) (stature((x).lChild)==stature((x).rChild))//理想平衡条件
#define BalFac(x) (stature((x).lChild)-stature((x).rChild))//平衡因子
#define AvlBalanced(x) ((-2<BalFac(x)) && (BalFac(x)<2))
/*
*返回x更高的孩子，等高则与父亲p同侧者优先，借此确定重构方案
*/
#define tallerChild(x) (\
	stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : (\
	stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : (\
	IsLChild(*(x))?(x)->lChild:(x)->rChild\
	)\
	)\
)
template<typename T>
class AVL :public BST<T> {
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
	//其他接口均可沿用
};
template<typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (x)return x;//确保x不存在
	BinNodePosi(T) xx = new BinNode(e, _hot); _size++;//此处可能导致祖父及祖父以上失衡
	for (BinNodePosi(T) g = _hot; g; g = g->parent) {//从x的父亲开始逐层向上检查历代祖先
		if (!AvlBalanced(*g)) {
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));//将子树连至原父亲
			break;//g的局部复衡后，则其祖先也必然复衡
		}
		else
			updateHeight(g);//若g未失衡，更新其高度即可
	}//至多一次调整后，全树重新平衡，高度复原
	return xx;
}

template<typename T>
bool AVL<T>::remove(const T& e) {
	BinNodePosi(T) & x = search(e);
	if (!x) return false;
	removeAt(x, _hot); _size--;//先按照BST的规则进行删除，此处可能导致失衡
	for (BinNodePosi(T) g = _hot; g; g = g->parent) {//从_hot出发向上，逐层检查各代祖先g
		if (!AvlBalanced(*g))
			g = FromParentTo(*x) = rotateAt(tallerChild(tallerChild(g)));
		updateHeight(g);
	}//可能需要O(logn)次调整，无论是否调整，全树高度都可能降低
	//删除节点导致的失衡具有传播性，故不可一次调整后直接break
	return true;
}


