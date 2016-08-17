#include "Dictionary.h"
#include "Entry.h"
#include "List.h"
#include "Quadlist.h"
/*
层与层之间纵向构成一个list，每层为一个Quadlist，对应于一个ListNode的data
*/
template<typename K,typename V>//跳转表模板类
class Skiplist :public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*> {
protected:
	bool skipSearch(ListNode<Quadlist<Entry<K, V>>*>* &qlist, QuadlistNode<Entry<K, V>>* &p, K& k);
public:
	int size() const { return empty() ? 0 : last()->data->size(); }//List::last()对应Quadlist的bottom
	int level() { return List::size(); }//层高 == #Quadlist
	bool put(K, V);//插入（作为dict允许重复必然成功）
	V* get(K k);
	bool remove(K k);//删除
};

template<typename K,typename V>
V* Skiplist<K, V>::get(K k) {//跳转表词条查找算法
	if (empty()) return NULL;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();//顶层Quadlist的
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();//第一个节点开始
	return skipSearch(qlist, p, k) ? &(p->entry.value) : NULL;
}//多个命中靠后者优先

/*
* Skiplist词条查找算法，仅供内部调用
* 入口：qlist为顶层列表，p为qlist的首节点
* 出口：若成功，p为命中关键码所属塔的顶部节点，qlist为p所属列表
* 	   否则，p为所属塔的基座，该塔对应于不大于k的最大且最靠右的关键码，qlist为空
* 约定：多个词条命中时，沿四联表取最靠后者
*/
template<typename K,typename V>
bool Skiplist<K, V>::skipSearch(
	ListNode<Quadlist<Entry<K, V>>*>* &qlist,//指定层的list
	QuadlistNode<Entry<K, V>>* &p,//首节点p出发
	K& k) {//向右向下查找关键码k
	while (true) {
		while (p->succ && (p->entry.key <= k)) {
			p = p->succ;
		}//直到到达更大的key或者p=tailor
		p = p->pred;
		if (p->pred && (k == p->entry.key))return true;//命中
		qlist = qlist->succ;//转入下一层
		if (!qlist->succ)return false;//已到底层
		p = (p->pred) ? p->below() : qlist->data->first();//转至当前塔的下一节点
	}
}

template<typename K,typename V>
bool Skiplist<K, V>::put(K k, V v) {
	Entry<K, V> e = Entry<K, V>(k, v);//待插入的词条
	if (empty()) insertAsFisrt(new Quadlist<Entry<K, V>>);
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();//从顶层四联表的
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();//首节点出发
	if (skipSearch(qlist, p, k))//查找合适的插入位置：不大于关键码k的最后一个节点p
		while (p->below)p = p->below;//强制转入塔底
	qlist = last();//以下，紧邻于p的右侧，一座崭新的塔将自底而上拔地而起逐层生长
	QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);//b为新塔的基座
	while (rand() % 1) {//抛硬币来确定塔是否需要生长一层
		while (qlist->data->valid(p) && !p->above)p = p->pred;//找出不低于此高度的最近前驱（上一层存在的）
		if (!qlist->data->valid(p)) {//若该前驱是header
			if (qlist == first()) {//当前层是最顶层
				insertAsFirst(new Quadlist<Entry<K, V>>);//首先创建新的一层
			}
			p = qlist->pred->data->first()->pred;//p转至上一层skiplist的header
		}
		else {
			p = p->above;
		}
		qlist = qlist->pred;
		b = qlist->data->insertAfterAbove(e, p, b);//插入新节点
	}
	return true;//Dictionary允许重复元素，故插入必然成功，于hashtable等map有异
}

template<typename K,typename V>
bool Skiplist<K, V>::remove(K k) {//跳转表词条删除算法
	if (empty())return false;
	ListNode<Quadlist<Entry<K, V>>*>* qlist = first();//从顶层四联表的
	QuadlistNode<Entry<K, V>>* p = qlist->data->first();//首节点出发
	if (!skipSearch(qlist, p, k))return false;//目标词条不存在，直接返回
	do {
		QuadlistNode<Entry<K, V>>* lower = p->below;
		qlist->data->remove(p);//删除当前层节点
		p = lower; qlist = qlist->succ;//转入下一层
	} while (qlist->succ);//直到塔基
	while (!empty() && first()->data->empty()) {
		List::remove(first());//清除可能不含词条的顶层Quadlist
	}
	return true;
}