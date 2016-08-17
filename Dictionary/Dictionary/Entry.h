template<typename K,typename V>
struct Entry {//词条模板类
	K key; V value;
	Entry(K k, V v) :key(k), value(v) {};
	Entry(Entry<K, V> const& e)key(e.key), value(e.value) {};//复制构造函数
	bool operator==(Entry<K, V> const& e) { return key == e.key; }
	bool operator!=(Entry<K, V> const&e) { return key != e.key; }
	//在Dict中只需要支持判等即可
};