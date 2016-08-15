#include "vector.h"
#define BTNodePosi(T) BTNode<T>*//B树节点位置

template<typename T>
struct BTNode {
	BTNodePosi(T) parent;
	Vector<T> key;//关键码向量
	Vector<BTNodePosi(T)> child;//孩子向量 长度总比key多一
	//constructor 只提供作为根节点创建的constructor
	BTNode() { parent = NULL; child.insert(0, NULL); }
	BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL) {
		parent = NULL;
		key.insert(0, e);
		child.insert(0, lc); child.insert(1, rc);
		if (lc)lc->parent = this;
		if (rc)rc->parent = this;
	}
};