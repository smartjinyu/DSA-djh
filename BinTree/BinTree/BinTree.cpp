#include <stack>
#include "BinNode.h"
using namespace std;
template<typename T> class BinTree {
protected:
	int _size;
	BinNodePosi(T) _root;
	virtual int updateHeight(BinNodePosi(T) x);//更新x的高度
	void updateHeightAbove(BinNodePosi(T) x);//更新x及其祖先的高度
public:
	BinTree() :_size(0), _root(NULL) {}//constructor
	~BinTree() { if (0 < _size) remove(_root); }//destructor
	int size() const { return _size; }
	bool empty() const { return !_root; }
	BinNodePosi(T) root() const { return _root; }
	BinNodePosi(T) insertAsRoot(T const& e);
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);//insert e as left child of x
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T);//insert T as left subtree of x
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T);
	int remove(BinNodePosi(T) x);//delete the subtree whose root is x, return original size of the subtree
	BinTree<T>* secede(BinNodePosi(T) x);//摘除子树x并作为一棵独立子树返回
	template<typename VST>
	void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }//层次遍历
	template<typename VST>
	void travPre(VST& visit) { if (_root) _root->travPre(visit); }
	template<typename VST>
	void travIn(VST& visit) { if (_root) _root->travIn(visit); }
	template<typename VST>
	void travPost(VST& visit) { if (_root) _root->travPost(visit); }

	//comparator
	bool operator<(BinTree<T> const& t) { return _root && t._root && lt(_root, t._root); }
	bool operator==(BinTree<T> const& t) { return _root &&t._root && (_root == t._root); }
};

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) {//O(depth(v)+1)
	return x->height = 1 + max(stature(x->lChild), stature(x->rChild));//具体规则根据树决定
}
template<typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
	while (x) { updateHeight(x); x = x->parent; }
}
template<typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) {
	//将S作为节点x的左子树插入，S本身置空
	if (x->lChild = S->_root) x->lChild->parent = x;//??
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;
}
template<typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) {
	//将S作为节点x的右子树插入，S本身置空
	if (x->rChild = S->_root) x->rChild->parent = x;//??
	_size += S->_size; updateHeightAbove(x);
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x;
}
template<typename T>
int BinTree<T>::remove(BinNodePosi(T) x) {//assert:x is valid in the tree
	FromParentTo(*x) = NULL;
	updateHeightAbove(x->parent);//更新祖先高度
	int n = removeAt(x); _size -= n; return n;
}

template<typename T>
static int removeAt(BinNodePosi(T) x) {//删除x及其后代，返回被删除节点的数值
	if (!x)return 0;
	int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
	release(x->data); release(x); return n;
}

template<typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {//将子树x从当前树中摘除，并封装为一棵独立子树返回
	FromParentTo(*x) = NULL;
	updateHeightAbove(x->parent);
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL;
	S->_size = x->size(); _size -= S->_size; return S;
}
//先序遍历
template<typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	visit(x->data);
	travPre_R(x->lChild, visit);
	travPre_R(x->rChild, visit);
}
template<typename T, typename VST>
void travPre_I1(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	if (x) S.push(x);
	while (!S.empty()) {
		x = S.pop();
		if (HasRChild(*x))S.push(x->rChild);
		if (HasLChild(*x))S.push(x->lChild);
	}
}
template<typename T, typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, stack<BinNodePosi(T)>& S) {
	//从当前节点出发，沿左分支不断深入直至没有左分支的节点，沿途遇到节点立即访问
	while (x) {
		visit(x->data);
		S.push(x->rChild);
		x = x->lChild;
	}
}
template<typename T, typename VST>
void travPre_I2(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	while (true) {
		visitAlongLeftBranch(x, visit, S);
		if (S.empty())break;
		x = S.pop();
	}
}
//

//中序遍历
template<typename T,typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) {
	if (x) return;
	if (HasRChild(*x)) travIn_R(x->rChild, visit);
	visit(x->data);
	if (HasLChild(*x))travIn_R(x->lChild, visit);
}

template<typename T>
static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)>& S) {
	while (x) { S.push(x); x = x->lChild; }
}
template<typename T,typename VST>
void travIn_I1(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	while (true) {
		goAlongLeftBranch(x, S);
		if (S.empty())break;
		x = S.pop();
		visit(x->data);
		x = x->rChild;
	}
}
template<typename T,typename VST>
void travIn_I2(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	while (true) {
		if (x) {
			S.push(x);
			x = x->lChild;
		}
		else if (!S.empty()) {
			x = S.pop();
			visit(x->data);
			x = x->rChild;
		}
		else break;
	}//为I的等效形式
}
template<typename T,typename VST>
void travIn_I3(BinNodePosi(T) x, VST& visit) {
	//由2发展而来，无须辅助栈
	bool backtrack = false;//前一步是否刚从右子树回溯
	while (true) {
		if (!backtrack && HasLchild(*x)) //存在左子树且不是刚刚回溯
			x = x->lChild;
		else {//不存在左子树或刚刚回溯（均无须访问左子树）
			visit(x->data);
			if (HasRChild(*x)) {
				x = x->rChild;
				backtrack = false;
			}
			else {//右子树为空
				if (!(x = x->succ())) break;//回溯，并且抵达末节点时退出循环
				backtrack = true;
			}

		}
	}
}

//后序遍历
template<typename T>
void travPost_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	if (HasRChild(*x)) travPost_R(x->rChild, visit);
	if (HasLChild(*x)) travPost_R(x->lChild, visit);
	visit(x->data);
}
template<typename T>
static void gotoHLVFL(stack<BinNodePosi(T)>& S) {
	//在以S的栈顶节点为根的子树中，找到最左侧的可见叶节点，可能是左孩子也可能是右孩子，即后序遍历最早访问的节点
	while (BinNodePosi(T) x = S.top()) {//自顶而下反复检查栈顶节点
		if (HasLChild(*x)) {
			if (HasRChild(*x))S.push(x->rChild);
			S.push(x->lChild);
		}
		else
			S.push(x->rChild);
		S.pop();
	}
}
template<typename T,typename VST>
void travPost_I(BinNodePosi(T) x, VST& visit) {
	stack<BinNodePosi(T)> S;
	if (x) S.push();//根节点入栈
	while (!S.empty()) {
		if (S.top() != x->parent)//若栈顶不是当前节点之父，则必为其右兄弟
			gotoHLVFL(S);//在以其右兄为根的子树中，找到HLVFL
		x = S.pop(); visit(x->data);
	}
}

int main() {
	return 0;
}