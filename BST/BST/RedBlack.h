#include "BST.h"
#define IsBlack(p) (!(p)||(RB_BLACK == (p)->color)) //外部节点也视作黑节点
#define IsRed(p) (!IsBlack(p))
#define BlackHeightUpdated(x)(\
	(stature((x).lChild)==stature((x).rChild)) && \
	((x).height == (IsRed(&x)?stature((x).lChild):stature((x).lChild)+1))\
)//x的黑高度是否已更新
template<typename T>
class RedBlack :public BST<T> {
protected:
	void solveDoubleRed(BinNodePosi(T) x);//双红修正
	void solveDoubleBlack(BinNodePosi(T) x);//双黑修正
	int updateHeightAbove(BinNodePosi(T) x);
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T&e);
	//BST:search等接口可直接沿用
};
template<typename T>
int RedBlack<T>::updateHeightAbove(BinNodePosi(T) x) {
	x->height = max(stature(x->lChild), stature(x->rChild));//孩子的黑高度一般相等，除非出现双黑
	return IsBlack(x) ? x->height++ : x->height;
}//统一定义stature(NULL)=-1

template<typename T>
BinNodePosi(T) RedBlack<T>::insert(const T& e) {
	BinNodePosi(T) & x = search(e); if (x) return x;
	x = new BinNode<T>(e, _hot, NULL, NULL, -1); _size++;
	//BlackHeight=-1 color=RB_RED
	solveDoubleRed(x);
	return x ? x : _hot->parent;

}
/*
* RedBlack双红调整算法，解决节点x和父亲均为红色的问题，分为两种情况
* RR-1: u为黑色，需要2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
* RR-2: u为红色，需要3次颜色翻转，3次黑高度更新，0次旋转，需要递归
* 我们约定: x为插入的节点，p为	其父亲，g为祖父，u为g的另一个孩子，即p的兄弟，x的叔叔。
* u可能为外部节点，但必存在。因为是双红缺陷，故x、p均为红色，因此g存在且必为黑色
*/
template<typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x) {
	if (IsRoot(*x)) {//递归基：已到根节点，将其转黑，高度增加
		_root->color = RB_BLACK; _root->height++; return;
	}
	BinNodePosi(T) p = x->parent; if (IsBlack(p))return;
	BinNodePosi(T) g = p->parent;
	BinNodePosi(T) u = uncle(x);//BinNode<T>中已经实现
	if (IsBlack(u)) {//u为黑色（包括NULL）
		BinNodePosi(T) gg = g->parent;//great-grand parent
		BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//r为调整后子树的根节点
		r->color = RB_BLACK; r->lChild->color = RB_RED; r->rChild->color = RB_RED;
		//可以增加判断只需两次染色，但是似乎得不偿失
		r->parent = gg;
	}
	else {//u为红色
		p->color = RB_BLACK; p->height++;
		u->color = RB_BLACK; u->height++;
		if (!IsRoot(*g)) g->color = RB_RED;
		solveDoubleRedI(g);//上升两层，递归解决（可参考尾递归优化成迭代形式）
	}
}

template<typename T>
bool RedBlack<T>::remove(const T& e) {
	BinNodePosi(T) x = search(e); if (!e) retuen false;
	BinNodePosi(T) r = removeAt(x, _hot); if (0 >= --_size)return true;
	//_hot的某一孩子刚被删除，且被r所指向的节点（可能是NULL）接替。以下检查是否失衡并作必要调整
	if (!_hot) {//刚刚被删除的是根节点
		_root->color = RB_BLACK; updateHeight(_root); return true;
	}
	//至此，则原x（现r）必非根，故_hot非空
	if (BlackHeightUpdated(*(_hot))) {//所有祖先的黑深度依旧平衡，则无须调整
		return true;
	}
	if (IsRed(r)) {//否则，若r为红色，那么只需将r转黑
		r->color = RB_BLACK; r->height++; return true;
	}
	//至此，原x（现r）均为黑色
	solveDoubleBlack(r); return true;
}

/*
* RedBlack的双黑缺陷调整算法，解决节点x与被其替代的节点r均为黑色的问题
* 分为BB-1 BB-2R BB-2B BB-3四种情况
*/
template <typename T> void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r) {
	BinNodePosi(T) p = r ? r->parent : _hot; if (!p) return; //r的父亲
	BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc; //r的兄弟
	if (IsBlack(s)) { //兄弟s为黑
		BinNodePosi(T) t = NULL; //s的红孩子（若左、右孩子皆红，左者优先；皆黑时为NULL）
		if (IsRed(s->rc)) t = s->rc; //右子
		if (IsRed(s->lc)) t = s->lc; //左子
		if (t) { //黑s有红孩子：BB-1
				 //*DSA*/printf("  case BB-1: Child ("); print(s->lc); printf(") of BLACK sibling ("); print(s); printf(") is RED\n");
			RBColor oldColor = p->color; //备份原子树根节点p颜色，并对t及其父亲、祖父
										 // 以下，通过旋转重平衡，并将新子树的左、右孩子染黑
			BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t); //旋转
			if (HasLChild(*b)) { b->lc->color = RB_BLACK; updateHeight(b->lc); } //左子
			if (HasRChild(*b)) { b->rc->color = RB_BLACK; updateHeight(b->rc); } //右子
			b->color = oldColor; updateHeight(b); //新子树根节点继承原根节点的颜色
												  //*DSA*/printBinTree(b, 0, 0);
		}
		else { //黑s无红孩子
			s->color = RB_RED; s->height--; //s转红
			if (IsRed(p)) { //BB-2R
							//*DSA*/printf("  case BB-2R: Both children ("); print(s->lc); printf(") and ("); print(s->rc); printf(") of BLACK sibling ("); print(s); printf(") are BLACK, and parent ("); print(p); printf(") is RED\n"); //s孩子均黑，p红
				p->color = RB_BLACK; //p转黑，但黑高度不变
									 //*DSA*/printBinTree(p, 0, 0);
			}
			else { //BB-2B
				   //*DSA*/printf("  case BB-2R: Both children ("); print(s->lc); printf(") and ("); print(s->rc); printf(") of BLACK sibling ("); print(s); printf(") are BLACK, and parent ("); print(p); printf(") is BLACK\n"); //s孩子均黑，p黑
				p->height--; //p保持黑，但黑高度下降
							 //*DSA*/printBinTree(p, 0, 0);
				solveDoubleBlack(p); //递归上溯
			}
		}
	}
	else { //兄弟s为红：BB-3
		   //*DSA*/printf("  case BB-3: sibling ("); print(s); printf(" is RED\n"); //s红（双子俱黑）
		s->color = RB_BLACK; p->color = RB_RED; //s转黑，p转红
		BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc; //取t与其父s同侧
		_hot = p; FromParentTo(*p) = rotateAt(t); //对t及其父亲、祖父做平衡调整
												  //*DSA*/printBinTree<T>(s, 0, 0);
		solveDoubleBlack(r); //继续修正r处双黑——此时的p已转红，故后续只能是BB-1或BB-2R
	}
}