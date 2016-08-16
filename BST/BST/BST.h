#include "BinTree.h"
template<typename T>
class BST :public BinTree<T> {
protected:
	BinNodePosi(T) _hot;//BST::search()最后访问的非空节点位置
	BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), //按照3+4结构，连接三个节点及其子树
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T))；
		BinNodePosi(T) rotateAt(BinNodePosi(T) x);//对x及其父亲、祖父做统一旋转调整
public:
	virtual BinNodePosi(T) & search(const T& e);
	virtual BinNodePosi(T) insert(const T& e);
	virtual bool remove(const T& e);
};

template<typename T>
static BinNodePosi(T) & searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) {
	if (!v || (e == v->data)) return v;
	hot = v;//记下当前节点再继续
	return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot);
}//返回值指向命中节点或假想的命中节点，hot指向其父亲
template<typename T>
BinNodePosi(T) & BST<T>::search(const T& e) {//在整棵BST中查找
	return searchIn(_root, e, _hot = NULL);//返回目标节点位置的引用
}
template<typename T>
BinNodePosi(T) BST<T>::insert(const T& e) {
	BinNodePosi(T) & x = search(e);
	if (x) return x;//确认目标节点不存在
	x = new BinNode<T>(e, _hot);
	_size++;
	updateHeightAbove(x);
	return x;
}
template<typename T>
bool BST<T>::remove(const T&e) {
	BinNodePosi(T) & x = search(e);
	if (!x)return false;
	removeAt(x, _hot); _size--;
	updateHeightAbove(_hot);
	return true;
}//返回是否成功删除
template<typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	/* BST节点的删除算法，删除位置x所指的节点，适用于AVL、Splay、RedBlack等各种BST
	*  调用时保证x非NULL
	*  返回值指向实际被删除节点的接替者，hot指向被删除节点的父亲 均可能为NULL
	*/
	BinNodePosi(T) w = x;//实际被摘除的节点，初值为x
	BinNodePosi(T) succ = NULL;
	if (!HasLChild(*x))//左子树为空
		succ = x = x->rChild;
	else if (!HasRChild(*x))//右子树为空
		succ = x = x->lChild;
	else {//左右子树均存在，则选择x的直接后继作为实际被摘除的节点
		w = w->succ();
		swap(x->data, w->data);
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rChild : u->lChild) = succ = w->rChild;//隔离节点w
	}
	hot = w->parent;
	if (succ) succ->parent = hot;
	release(w->data); release(w); return succ;

}
/*
* 按照3+4结构对三个顶点四棵子树进行重组，返回重组后局部子树根节点的位置b
* 子树根节点与上层节点之间的双向连接，均由上层调用者完成
* 用于AVL和RedBlack的局部平衡调整
* 中序遍历序列为{T0,a,T1,b,T2,c,T3}转化为
*                  b
*             a         c
*           T0  T1    T2   T3
*/
template<typename T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3) {
	a->lChild = T0; if (T0)T0->parent = a;
	a->rChild = T1; if (T1)T1->parent = a;
	c->lChild = T2; if (T2)T2->parent = c;
	c->rChild = T3; if (T3)T3->parent = c;
	b->lChild = a; a->parent = b;
	b->rChild = c; c->parent = b;
	updateHeight(b);
	return b;
}

/*
* BST节点旋转统一算法，返回调整之后局部子树根节点位置
* 子树的根指向上层节点，但是上层节点到子树的连接应由调用者完成
*/
template<typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {//v为非空的孙辈节点 g-p-v家族
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent;
	//视g-p-v的相对位置分四种情况
	if (IsLChild(*p)) {/* zig */
		if (IsLChild(*v)) {/* zig-zig */
			p->parent = g->parent;
			return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
		}
		else {/* zig-zag */
			v->parent = g->parent;
			return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
		}
	}
	else {/* zag */
		if (IsLChild(*v)) {/* zag-zig */
			v->parent = g->parent;
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
		}
		else {/* zig-zig */
			p->parent = g->parent;
			return connect34(g, p, c, g->lChild, p->lChild, v->lChild, v->rChild);
		}
	}

}

