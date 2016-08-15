#include "BST.h"
template<typename T>
class Splay :publi BST<T> {
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);//将v伸展至树根
public:
	BinNodePosi(T) & search(const T&e);
	BinNodePosi(T) & insert(const T&e);
	bool remove(const T& e);
};
template<typename NodePosi>inline//在节点*p和*lc之间建立父亲与左子关系
//内联函数，编译时调用处直接替换，避免过大的栈开销
void attachAsLChild(NodePosi p, NodePosi lc) { p->lChild = lc; if (lc) lc->parent = p; }
template<typename NodePosi>inline
void attachAsRChild(NodePosi p, NodePosi rc) { p->rChild = rc; if (rc) rc->parent = p; }

template<typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
	if (!v) return NULL;
	BinNodePosi(T) p, BinNodePosi(T) g;//parent and grandparent
	while ((p = v->parent) && (g = p->parent)) {//自下而上进行双层伸展
		BinNodePosi(T) gg = g->parent;//每轮之后*v都以原曾祖父(great-grand parent)为父
		if (IsLChild(*v)) {
			if (IsLChild(*p)) {//自己画个图验证下
				attachAsLChild(p, v->rChild); attachAsLChild(g, p->rChild);
				attachAsRChild(v, p); attachAsRChild(p, g);
			}
			else {
				attachAsRChild(g, v->lChild); attachAsLChild(p, v->rChild);
				attachAsLChild(v, g); attachAsRChild(v, p);
			}
		}
		else if (IsRChild(*p)) {
			attachAsRChild(g, p->lChild); attachAsRChild(p, v->lChild);
			attachAsLChild(p, g); attachAsLChild(v, p);
		}
		else {
			attachAsRChild(p, v->lChild); attachAsLChild(g, v->rChild);
			attachAsRChild(v, g); attachAsLChild(v, p);
		}
		if (!gg) v->parent = NULL;//若gg不存在，则*v为树根
		else(g == gg->lChild) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
		updateHeight(g); updateHeight(p); updateHeight(v);
	}//双层伸展结束，g一定为NULL，但p可能非空，此时需要再进行一次单旋
	if (p = v->parent) {
		if(IsLChild(*v)){
			attachAsLChild(p, v->rChild); attachAsRChild(v, p);
		}
		else {
			attachAsRChild(p, v->lChild;); attachAsLChild(v, p);
		}
		updateHeight(p); updateHeight(v);
	}
	v->parent = NULL; 
	return v;
}
template<typename T>
BinNodePosi(T) & Splay<T>::search(const T&e) {
	BinNodePosi(T) p = searchIn(_root, e, _hot = NULL);
	_root = splay((p ? p : hot));
	return _root;
}//无论查找是否成功，_root都指向最后被访问的节点
template<typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {
	if (!_root) { _size++; return _root = new BinNode<T>(e); }//原树为空的退化情况
	if (e == search(e)->data) return _root;//确认目标节点不存在
	_size++; BinNodePosi(T) t = _root;
	if (_root->data < e) {
		t->parent = _root = new BinNode<T>(e, NULL, t, t->rChild);
		if (HasRChild(*t)) { t->rChild->parent = _root; t->rChild = NULL; }
	}
	else {
		t->parent = _root = new BinNode<T>(e, NULL, t->lChild, t);
		if (HasLChild(*t)) { t->lChild->parent = _root; t->lChild = NULL; }
	}
	updateHeightAbove(t);//更新t及其祖先的高度（实际上只有_root）
	return _root;
}


template<typename T>
bool Splay<T>::remove(const T& e) {
	if (!root || (e != search(e) - data))return false;//退化情况
	BinNodePosi(T) w = _root;//search之后待删除节点已经提升至树根
	if (!HasLChild(*_root)) {//无左子树
		_root = _root->rChild; if (_root)_root->parent = NULL;
	}
	else if (!HasRChild(*_root)) {
		_root = _root->lChild; if (_root)_root->parent = NULL;
	}
	else {//左右子树同时存在
		BinNodePosi(T) lTree = _root->lChild;
		lTree->parent = NULL; _root->lChild = NULL;//暂时切下左子树
		_root = _root->rChild; _root->parent = NULL;//保留右子树
		search(w->data);//必然失败的查找，导致右子树中的最小元素被提升到root位置，左子树为空
		_root->lChild = lTree; lTree->parent = _root;
	}
	release(w->data); release(w); _size--;
	if (_root) updateHeight(_root);//扫尾工作
	return true;
}
