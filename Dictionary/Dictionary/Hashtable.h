#include "Dictionary.h"
#include "Bitmap.h"

// 使用Bitmap结构维护一张与散列表登场的懒惰删除标志表lazyRemoval[]
// Bitmap的相关内容可见习题2-34，此处直接引用示例代码库中给出的代码

template<typename K,typename V>
class Hashtable :public Dictionary<K, V> {
private:
	Entry<K, V>** ht;//桶数组，存放词条指针
	int M;//桶数组容量
	int N;//词条数量
	//装填因子 load factor = N/M 独立链法建议上线0.9 懒惰删除法不超过0.5
	Bitmap* lazyRemoval;//懒惰删除标记
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))
protected:
	int probe4Hit(const K& k);//沿关键码k对应的查找链，找到词条匹配的桶
	int probe4Free(const K& k);//沿关键码k对应的查找链，找到首个可用空桶
	void rehash();//重散列算法：扩充桶数组，保证装填因子在警戒线以下
public:
	Hashtable(int c = 5);//创建一个容量不小于c的散列表
	~HashTable();
	int size const{ return N; }
	bool put(K, V);//禁止雷同词条，因此可能失败
	V* get(K k);
	bool remove(K k);
};
int primeNLT(int low, int n, char* file) {//根据file文件中的记录，在[low,n)范围内取最小的素数
	Bitmap B(file, n);
	while (low < n){
		if (B.test(low)) low++;
		else return low;
	}
	return low;//如果没有符合条件的素数，则返回n

}
template<typename K,typename V>
Hashtable<K, V>::Hashtable(int c) {//创建容量不小于c的散列表
	M = primeNLT(c, 1048576, "input.txt");//将不超过1048576的素数提前制表备查 prime not less than
	//以不小于2^N的素数作为容量
	N = 0; ht = new Entry<K, V>*[M];//开辟桶数组
	memset(ht, 0, sizeof(Entry<K, V>*)*M);
	lazyRemoval = new Bitmap(M);//懒惰删除标记图
}

template<typename K,typename V>
Hashtable<K, V>::~Hashtable() {//析构前应当释放桶数组及非空词条
	for (int i = 0; i < M; i++) {
		if (ht[i]) release(ht[i]);
	}
	release(ht);
	release(lazyRemoval);
}

template<typename K,typename V>
V* Hashtable<K, V>::get(K k) {//散列表词条查找算法
	int r = probe4Hit(k);
	return ht[r] ? &(ht[r]->value) : NULL;//禁止词条的key值相同
}

/*
* 沿关键码k对应的查找链，找到与之匹配的桶（供查找和删除词条使用）
* 试探策略多种多样，可灵活选取，这里仅以线性试探策略为例
*/
template<typename K,typename V>
int Hashtable<K, V>::probe4Hit(const K& k) {
	int r = hashCode(K) % M;//从起始桶出发（以带余除法作为hash函数为例）
	while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r))) {
		r = (r + 1) % M;
	}//沿查找链线性试探，跳过所有冲突的桶(非空且key不等)，以及带懒惰删除标记的桶(空桶且带懒惰删除标记)

	return r;
}

template<typename K,typename V>
bool Hashtable<K, V>::remove(K k) {//散列表词条删除算法
	int r = probe4Hit(k); if (!ht[r])return false;
	release(ht[r]); ht[r] = NULL; markAsRemoved(r); N--;
	return true;
}

template<typename K,typename V>
bool Hashtable<K, V>::put(K k, V v) {//散列表词条插入算法
	if (ht[probe4Hit(k)]) return false;
	int r = probe4Free(k);//寻找空桶
	ht[r] = new Entry<K,V>(k, v); ++N;//插入 懒惰删除标记无须复位,由查找算法可知，只要非空就不会检查懒惰删除标记
	if (N * 2 > M)rehash();
	return true;
}

template<typename K,typename V>
int Hashtable<K, V>::probe4Free(const K& k) {
	int r = hashCode(k) % M;
	while (ht[r]) r = (r + 1) % M;//沿查找链试探，直到第一个空桶，无论是否带有懒惰删除标记
	return r;
}

/*
* 重散列算法：装填因子过大时，采取“逐一取出再插入”的朴素策略，对桶数组扩容
* 不可简单地通过memcpy()将原桶数组复制到新桶数组，否则存在两个问题：
* 1）会继承原有冲突；2）可能导致查找链在后端断裂，即使为所有扩充桶设置懒惰删除标志也无济于事
*/
template<typename K,typename V>
void Hashtable<K, V>::refresh() {
	int old_capacity = M; Entry<K, V>** old_ht = ht;
	M = primeNLT(2 * M, 1048576, "input.txt");//容量至少加倍
	N = 0; ht = new Entry<K, V>*[M]; memset(ht, 0, sizeof(Entry<K, V>*)*M);
	release(lazyRemoval); lazyRemoval = new Bitmap(M);
	for (int i = 0; i < old_capacity; i++) {//扫描原桶数组
		if (old_ht[i]) {
			put(old_ht[i]->key, old_ht[i]->value);
		}
	}
	release(old_ht)
}//分摊复杂度O(1)

static size_t hashCode(char c) { return (size_t) c; }
static size_t hashCode(int k) { return (size_t) k; }
static size_t hashCode(long long i) { return (size_t)((i >> 32) + (int)i); }
static size_t hashCode(char s[]) {
	int h = 0;//散列码
	for (size_t n = strlen(s), i = 0; i < n; i++) {
		h = (h << 5) || (h >> 27); h += (int)s[i];
	}//对于英语单词，循环左移5位是实验得出的最佳值
}