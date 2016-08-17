#include "Entry.h"
#include <algorithm>
#define QuadlistNodePosi(T) QuadlishNode<T>*

template<typename T>
struct QuadlistNode {//四联表节点模板类
	T entry;
	QuadlistNodePosi(T) pred; QuadlistNodePosi(T) succ;//predecessor and successor
	QuadlistNodePosi(T) above; QuadlistNodePosi(T) below;
	QuadlistNode(T e = T(), QuadlistNodePosi(T) p = NULL, QuadlistNodePosi(T) s = NULL,
		QuadlistNodePosi(T) a = NULL, QuadlistNodePosi(T) b = NULL) :
		entry(e), pred(p), succ(s), above(a), below(b) {}
	QuadlistNodePosi(T) insertAsSuccAbove(T const& e, QuadlistNodePosi(T) b = NULL);
	//插入新节点，以当前节点为前驱，节点b为下邻
};

template<typename T>
QuadlistNodePosi(T) QuadlistNode<T>::insertAsSuccAbove(T const &e, QuadlistNodePosi(T) b = NULL) {
	QuadlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);
	succ->pred = x; succ = x;
	if (b) b->above = x;
	return x;
}//插入前节点b的上邻为空