#include "BTNode.h"
template<typename T>
class BTree {
protected:
	int _size;//存放的关键码总数
	int _order;//BTree的阶次，大于等于3，创建后一般不能修改
	BTNodePosi(T) _root;
	BTNodePosi(T) _hot;//BTree:search()最后访问的非空节点位置
	void solveOverflow(BTNodePosi(T));
	void solveUnderflow(BTNodePosi(T));
public:
	BTree(int order = 3) :_order(order), _size(0) { _root = new BTNode<T>(); }
	~BTree() { if (_root)release(_root); }
	int const order() { return _order; }
	int const size() { return _size; }
	BTNodePosi(T) & root() { return _root; }
	bool empty()const { return !_root; }
	BTNodePosi(T) search(const T& e);
	bool insert(const T& e);
	bool remove(const T& e);
};

template<typename T>
BTNodePosi(T) BTree<T>::search(const T& e) {
	BTNodePosi(T) v = _root; _hot = NULL;
	while (v) {//逐层深入
		Rank r = v->key.search(e);//返回不大于e的最大关键码
		if ((0 <= r) && (e == v->key[r]))return v;
		_hot = v; v = v->child[r + 1];
	}
	return NULL;//fail
}
template<typename T>
bool BTree<T>::insert(const T&e) {
	BTNodePosi(T) v = search(e);
	if (v)return false;
	Rank r = _hot->key.search(e);//查找合适的插入位置
	_hot->child.insert(r + 1, e);
	_hot->child.insert(r + 2, NULL);
	_size++;
	solveOverflow(_hot);
	return true;
}
template<typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
	if (_order >= v->child.size())return;//递归基 当前节点未发生上溢
	Rank s = _order / 2;//轴点 此时必有_order=key.size()=child.size()-1
	BTNodePosi(T) u = new BTNode<T>();//新节点已有一个空孩子
	for (Rank j = 0; j < _order - s - 1; j++) {
		u->child.insert(j, v->child.remove(s + 1));
		u->key.insert(j, v->key.remove(s + 1));
	}//可改进
	u->child[_order - s - 1] = v->child.remove(s - 1);//移动v最靠右的孩子
	if (u->child[0]) {//u的孩子们非空
		for (Rank j = 0; j < _order - s; j++) {
			u->child[j]->parent = u;//设置父亲节点
		}
	}
	BTNodePosi(T) p = v->parent;
	if (!p) { _root = p = new BinNode<T>(); p->child[0] = v; v->parent = p; }//若p未空则创建之
	Rank r = 1 + p->key.search(v->key[0]);
	p->key.insert(r, v->key.remove(s));//轴点上升
	p->child.insert(r + 1, u); u->parent = p;
	solveOverflow(p);
}
template<typename T>
bool BTree<T>::remove(const T& e) {
	BTNodePosi(T) v = search(e); 
	if (!v) return false;
	Rank r = v->key.search(e);//确定秩
	if (v->child[0]) {//v不是叶子
		BTNodePosi(T) u = v->child[r + 1];
		while (u->child[0]) u = u->child[0];//在右子树中一路向左，找出后继
		v->key[r] = u->key[0]; v = u; r = 0;//交换
	}
	//此时v位于最底层，且第r个关键码就是待删除者
	v->key.remove(r); v->child.remove(r + 1); _size - ;
	solveUnderflow(v);
	return true;
}

template<typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v) {
	if ((_order + 1) / 2 <= v->child.size())return;//递归基:当前节点未下溢
	BTNodePosi(T) p = v->parent;
	if (!p) {//递归基：到达根节点
		if (!v->key.size() && v->child[0]) {
			//作为树根的v不含关键码，但有（唯一的）非空孩子
			_root = v->child[0]; _root->parent = NULL;
			v->child[0] = NULL; release(v);
		}//树的高度-1
		return;
	}
	Rank r = 0; while (p->child[r] != v)r++;
	//确定r 此时v可能不含关键码，故不能采用search
	//如果实现了判等器，则可以通过vector::find()
	//情况1：向左兄弟借关键码
	if (0 < r) {//若v不是父亲p的第一个孩子
		BTNodePosi(T) ls = p->child[r - 1];//左兄弟必存在
		if ((_order + 1) / 2 > ls->child.size()) {
			//该兄弟足够胖可以接济他人
			v->key.insert(0, p->key[r - 1]);
			p->key[r - 1] = ls->key.remove(ls->key.size() - 1);
			v->child.insert(0，ls->child.remove(ls->child.size() - 1);//把ls的最右侧孩子过继给v
			if (v->child[0]) v->child[0]->parent = v;
			return;//完成右旋(当前层次及以上层次均已完成)
		}
	}
	//若至此，则左兄弟要么为空，要么太瘦
	//情况2：向右兄弟借关键码
	if (r < p->child.size() - 1 ) {
		BTNodePosi(T) rs = p->child[r + 1];
		if ((_order + 1) / 2 > rs->child.size()) {
			v->key.insert(v->key.size(), p->key[r]);
			p->key[r] = rs->key.remove(0);
			v->child.insert(v->child.size(), rs->child.remove(0));
			if (v->child[v->child.size() - 1]) v->child[v->child.size() - 1]->parent = v;
			return;//完成左旋(当前层次及以上层次均已完成)
		}
	}
	//若至此，则左右兄弟要么为空，要么太瘦
	//情况3：都太瘦或都为空，则合并
	if (0 < r) {//v与左兄弟合并
		BTNodePosi(T) ls = p->child[r - 1];
		ls->key.insert(ls->key.size(), p->key.remove(r - 1));
		p->child.remove(r);
		//p的第r-1个关键码转入ls，v不再是p的第r个孩子
		ls->child.insert(ls->child.size(), v->child.remove(0));
		if (ls->child[ls->child.size() - 1]) {//v的最左侧孩子过继给ls做最右侧孩子
			ls->child[ls->child.size() - 1]->parent = ls;
		}
		while (!v->key.empty()) {//v剩余的关键码和孩子统统过继给ls(这日子还要不要过了)
			ls->key.insert(ls->key.size(), v->key.remove(0));
			ls->child.insert(ls->child.size(), v->child.remove(0));
			if (ls->child[ls->child.size() - 1]) { 
				ls->child[ls->child.size() - 1]->parent = ls; 
			}
		}
		release(v);//（日子没法过了，散伙分家产吧）
	}
	else {//v与右兄弟合并
		BTNodePosi(T) rs = p->child[r + 1];
		rs->key.insert(0, p->key.remove(r));
		p->child.remove(r);
		rs->child.insert(0, v->child.remove(v->child.size() - 1));
		if (rs->child[0])rs->child[0]->parent = rs;
		while (!v->key.empty()) {
			rs->key.insert(0, v->key.remove(v->key.size() - 1));
			rs->child.insert(0, v->child.remove(v->child.size() - 1));
			if (rs->child[0])rs->child[0]->parent = rs;
		}
		release(v);
	}
	//此时上一层减少一个关键码，如有必要继续分裂
	solveUnderflow(p);
	return;
	
}