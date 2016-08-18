#include "PQ.h"
#include "vector.h"

#define InHeap(n,i) (((-1)<(i))&&((i)<(n)))//PQ[i]是否合法
#define Parent(i) ((i-1)>>1)//求父亲
#define LastInternal(n) Parent(n-1)//最后一个内部节点，即末节点的父亲
#define LChild(i) (1+((i)<<1)) //PQ[i]的左孩子
#define RChild(i) ((1+(i))<<1)
#define ParentValid(i) (0<i) //判断PQ[i]是否有父亲
#define LChildValid(i) InHeap(n,LChild(i))
#define RChildValid(i) InHeap(n,RChild(i)) //完全二叉树的结构 此条等价于判断是否有两个孩子
#define Bigger(PQ,i,j) (lt(PQ[i],PQ[j])?j:i) //取大者，相等前者优先
#define ProperParent(PQ,n,i)\
			(RChildValid(n,i)?Bigger(PQ,Bigger(PQ,i,LChild(i)),RChild(i)):\
			(LChildValid(n,i)?Bigger(PQ,i,LChild(i)):i\
			)\
			)//父子三者中的最大者（相等时父节点优先，可避免不必要的交换）

template<typename T>
class PQ_ComplHeap :public PQ<T>, public Vector<T> {//基于向量，以完全二叉堆形式实现的优先级队列
protected:
	Rank percolateDown(Rank n, Rank i);//下滤
	Rank percolateUp(Rank i);//上滤
	void heapify(Rank n);//Floyd建堆算法
public:
	PQ_ComplHeap() {}
	PQ_ComplHeap(T* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); heapify(hi - lo); }
	PQ_ComplHeap(T* A, Rank n) { copyFrom(A, 0, n); heapify(n); }
	void insert(T);//按照优先级次序插入
	T getMax() { return _elem[0]; }
	T delMax();
};

template<typename T>
void PQ_ComplHeap<T>::insert(T e) {
	Vector<T>::insert(e);//插入末尾，得益于向量，无须考虑溢出扩容等特殊情况
	percolateUp(_size - 1);
}

template<typename T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i) {//对向量中的第i个词条实施上滤操作，i<_size
	while (ParentValid(i)) {//尚未抵达堆顶
		Rank j = Parent(i);
		if (lt(_elem[i], _elem[j])) break;
		swap(_elem[i], _elem[j]); i = j;
		//在常系数意义下可以改进，参见习题 10-3 10-4
	}
	return i;
}

template<typename T>
T PQ_ComplHeap<T>::delMax() {
	T maxElem = _elem[0]; 
	_elem[0] = remove(_size - 1);//用末词条代替堆顶
	percolateDown(_size, 0);
	return maxElem;
}

template<typename T>//对向量前n个词条中的第i个实施下滤操作，i<n
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
	Rank j;//j为i及其至多两个孩子中，堪当父亲者
	while (i != (j = ProperParent(_elem, n, j))) {
		swap(_elem[i], _elem[j]);i=j
	}
	return i;//返回下滤抵达的位置，i=j
}

template<typename T>
void PQ_ComplHeap<T>::heapify(Rank n) {//Floyd建堆算法，O(n)时间
	for (int i = LastInternal(n); InHeap(n, i); i--) {//自底而上
		percolateDown(n, i);
	}
	//时间复杂度下相当于各节点高度的总和，而自上而下进行上滤则相当于各节点深度的总和，为O(nlogn)
}