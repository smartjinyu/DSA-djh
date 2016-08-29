/*
祖玛(Zuma)
Description
Let's play the game Zuma!

There are a sequence of beads on a track at the right beginning. All the beads are colored but no three adjacent ones are allowed to be with a same color. You can then insert beads one by one into the sequence. Once three (or more) beads with a same color become adjacent due to an insertion, they will vanish immediately.



Note that it is possible for such a case to happen for more than once for a single insertion. You can't insert the next bead until all the eliminations have been done.

Given both the initial sequence and the insertion series, you are now asked by the fans to provide a playback tool for replaying their games. In other words, the sequence of beads after all possible eliminations as a result of each insertion should be calculated.

Input
The first line gives the initial bead sequence. Namely, it is a string of capital letters from 'A' to 'Z', where different letters correspond to beads with different colors.

The second line just consists of a single interger n, i.e., the number of insertions.

The following n lines tell all the insertions in turn. Each contains an integer k and a capital letter Σ, giving the rank and the color of the next bead to be inserted respectively. Specifically, k ranges from 0 to m when there are currently m beads on the track.

Output
n lines of capital letters, i.e., the evolutionary history of the bead sequence.

Specially, "-" stands for an empty sequence.

Example
Input

ACCBA
5
1 B
0 A
2 B
4 C
0 A
Output

ABCCBA
AABCCBA
AABBCCBA
-
A
Restrictions
0 <= n <= 10^4

0 <= length of the initial sequence <= 10^4

Time: 2 sec

Memory: 256 MB

Hints
List

描述
祖玛是一款曾经风靡全球的游戏，其玩法是：在一条轨道上初始排列着若干个彩色珠子，其中任意三个相邻的珠子不会完全同色。此后，你可以发射珠子到轨道上并加入原有序列中。一旦有三个或更多同色的珠子变成相邻，它们就会立即消失。这类消除现象可能会连锁式发生，其间你将暂时不能发射珠子。

开发商最近准备为玩家写一个游戏过程的回放工具。他们已经在游戏内完成了过程记录的功能，而回放功能的实现则委托你来完成。

游戏过程的记录中，首先是轨道上初始的珠子序列，然后是玩家接下来所做的一系列操作。你的任务是，在各次操作之后及时计算出新的珠子序列。

输入
第一行是一个由大写字母'A'~'Z'组成的字符串，表示轨道上初始的珠子序列，不同的字母表示不同的颜色。

第二行是一个数字n，表示整个回放过程共有n次操作。

接下来的n行依次对应于各次操作。每次操作由一个数字k和一个大写字母Σ描述，以空格分隔。其中，Σ为新珠子的颜色。若插入前共有m颗珠子，则k ∈ [0, m]表示新珠子嵌入之后（尚未发生消除之前）在轨道上的位序。

输出
输出共n行，依次给出各次操作（及可能随即发生的消除现象）之后轨道上的珠子序列。

如果轨道上已没有珠子，则以“-”表示。

样例
见英文题面

限制
0 ≤ n ≤ 10^4

0 ≤ 初始珠子数量 ≤ 10^4

时间：2 sec

内存：256 MB

提示
列表
*/
//WA on test2 TLE on test20
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
using namespace std;

typedef int Rank;
#define ListNodePosi(T) ListNode<T>*//列表节点位置

template <typename T> struct ListNode { //列表节点模板类（以双向链表形式实现）
										// 成员
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //数值、前驱、后继
														// 构造函数
	ListNode() {} //针对header和trailer的构造
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {} //默认构造器
									   // 操作接口
	ListNodePosi(T) insertAsPred(T const& e); //紧靠当前节点之前插入新节点
	ListNodePosi(T) insertAsSucc(T const& e); //紧随当前节点之后插入新节点
};

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) x = new ListNode(e, pred, this);
	pred->succ = x;
	pred = x;
	return x;
}
template<typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) x = new ListNode(e, this, succ);
	succ->pred = x;
	succ = x;
	return x;
}
template<typename T> class List {
public:
	int _size; ListNodePosi(T) header; ListNodePosi(T) tailor;
protected:
	void init();//列表创建时初始化
	int clear();
public:
	//constructor
	List() { init(); }
	~List() { clear(); delete header; delete tailor; }
	//read-only interface
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[](int r) const;//循秩访问，效率低下
	ListNodePosi(T) first() const { return header->succ; }
	ListNodePosi(T) last() const { return tailor->pred; }
	bool valid(ListNodePosi(T) p) { return p && (p != tailor)&(p != header); }//判断p是否合法
	void printAll()const;
	//read-write interface
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAt(Rank i, T const& e);
	ListNodePosi(T) visitByRank(Rank r);
	T remove(ListNodePosi(T) p);//删除p处的节点并返回被删除的节点内容

};

template<typename T>
void List<T>::init() {//初始化，在创建列表对象时统一调用
	header = new ListNode<T>;
	tailor = new ListNode<T>;
	header->succ = tailor; header->pred = NULL;
	tailor->pred = header; tailor->succ = NULL;
	_size = 0;
}
template<typename T>
void List<T>::printAll() const
{
	if (empty()) {
		printf("-\n");
	}
	else {
		for (ListNodePosi(T) p = header->succ; p != tailor; p = p->succ) {
			printf("%c", p->data);
		}
		printf("\n");
	}
}
template<typename T>
ListNodePosi(T) List<T>::visitByRank(Rank r)
{
	ListNodePosi(T) p = first();
	while (0 < r--)p = p->succ;
	return p;
}
template<typename T>
T& List<T>::operator[](int r) const {
	ListNodePosi(T) p = first();
	while (0 < r--)p = p->succ;
	return p->data;
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
ListNodePosi(T) List<T>::insertAt(Rank i, T const & e)
{
	ListNodePosi(T) p = first();
	while (0 < i--)p = p->succ;
	_size++;
	return p->insertAsPred(e);
}

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



#define maxL 10010
List<char> str;
void solve(int n) {
	bool flag = true;
	int l = n, r = n;
	ListNodePosi(char) pl = str.visitByRank(n);
	while (flag) {
		ListNodePosi(char) p = pl;;
		ListNodePosi(char) pr = p;
		for (; pl->data == p->data; pl = pl->pred) {
			l--;
			if (pl == str.header) break;
			
		}
		for (; pr->data == p->data; pr = pr->succ) {
			r++;
			if (pr == str.tailor)break;
		}
		if (r - l >= 4) {
			for (int i = 1; i < r - l; i++) {
				str.remove(pl->succ);
			}				
			flag = true;
			r = l;

		}
		else {
			flag = false;
		}
	}

}
int main() {
	char s[maxL];
	scanf("%s", s);
	for (int i = 0; s[i] != '\0'; i++) {
		str.insertAsLast(s[i]);
	}
	int N; scanf("%d", &N);
	int id; char letter;
	for (int i = 0; i < N; i++) {
		scanf("%d %c", &id, &letter);
		str.insertAt(id,letter);
		solve(id);
		str.printAll();
	}
	return 0;
}