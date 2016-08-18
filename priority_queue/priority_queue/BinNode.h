#include <cstdlib>
#include <queue>
using namespace std;
#define BinNodePosi(T) BinNode<T>* //node position
#define stature(p) ((p)?(p)->height:-1)// node height(empty tree is -1)
typedef enum { RB_RED, RB_BLACK } RBColor;//node color

										  //用宏来定义一些函数，编译器预处理时直接替换，可以免去传递参数、传递寄存器的开销，有效利用CPU cache，但会增大可执行文件体积
										  //一般宏函数用于简短的被频繁调用的函数

										  /*
										  * BinNode状态与性质的判断
										  */
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x)&&(&(x)==(x).parent->lChild))
#define IsRChild(x) (!IsRoot(x)&&(&(x)==(x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x)||HasRChild(x))
#define HasBothChild(x) (HasLChild(x)&&HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
										  /*
										  * 与BinNode有特定关系的节点和指针（宏函数换行应在行末加/）
										  */
#define sibling(p)(\
	IsLChild(*(p)) ? \
		(p)->parent->rChild: \
		(p)->parent->lChild \
)//兄弟
#define uncle(x)(\
	IsLChild(*((x)->parent)) ? \
		(p)->parent->parent->rChild: \
		(p)->parent->parent->lChild \
)//叔叔
#define FromParentTo(x)(\
	IsRoot(x) ?_root : (\
		IsLChild(x)?(x).parent->lChild:(x).parent->rChild \
	)\
)//返回来自x的父亲的指向x的指针


template<typename T> struct BinNode {
	//members
	T data;
	BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild;
	int height;
	int npl;//Null Path Length(空节点通路长度，用于左式堆,也可直接用height代替??)
	RBColor color;//Red/Black Tree

				  //constructor
	BinNode() :parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NUll, BinNodePosi(T) rc = NULL,
		int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}

	//operations
	int size();//统计当前节点的后代总数，亦即为以其为根的子树的规模
	BinNodePosi(T) insertAsLC(T const&);//insert as left child
	BinNodePosi(T) insertAsRC(T const&);
	BinNodePosi(T) succ();//取当前节点的直接后继
	template<typename VST> void travLevel(VST&);//层次遍历
	template<typename VST> void travPre(VST&);//先序遍历
	template<typename VST> void travIn(VST&);//中序遍历
	template<typename VST> void travPost(VST&);//后序遍历

											   //comparators
	bool operator<(BinNode const& bn) { return data < bn.data; }
	bool operator<=(BinNode const& bn) { return data <= bn.data; }
	bool operator>(BinNode const& bn) { return data > bn.data; }
	bool operator>=(BinNode const& bn) { return data >= bn.data; }
	bool operator==(BinNode const& bn) { return data == bn.data; }
};

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }
template<typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

template<typename T> template<typename VST>
void BinNode<T>::travIn(VST& visit) {//二叉树中序遍历统一接口
	switch (rand() % 5)
	{
	case 1:travIn_I1(this, visit); break;//迭代版#1
	case 2:travIn_I2(this, visit); break;//迭代版#2
	case 3:travIn_I3(this, visit); break;//迭代版#3
	case 4:travIn_I4(this, visit); break;//迭代版#4
	default:travIn_R(this, visit); break;//递归版
	}
}

template<typename T>
BinNodePosi(T) BinNode<T>::succ() {
	//定位当前节点的直接后继，即线性排序后的直接后继，亦为中序遍历后的直接后继
	BinNodePosi(T) s = this;//记录后继的临时变量
	if (rChild) {//如果有右孩子，那么直接后继必然是右子树中最靠左的那个
		s = rChild;
		while (HasLChild(*s)) s = s->lChild;
	}
	else {//否则，直接后继应当是“将当前节点包含于其左子树中的最低祖先”
		while (IsRChild(*s)) s = s->parent;//向左上方移动
		s = s->parent;//再向右上方移动一步，即抵达其直接后继（画图即可发现这一点）
	}
	return s;//返回NULL 说明当前节点已经是中序遍历的终点
}
template<typename T> template<typename VST>
void BinNode<T>::travLevel(VST& visit) {
	//层次遍历
	queue<BinNodePosi(T)> Q;
	Q.enqueue(this);
	while (!Q.empty()) {
		BinNodePosi(T) x = Q.dequeue(); visit(x->data);
		if (HasLChild(*x)) Q.enqueue(x->lChild);
		if (HasRChild(*x)) Q.enqueue(x->rChild);
	}
}
