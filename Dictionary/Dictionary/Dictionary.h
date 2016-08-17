template<typename V,typename K>
struct Dictionary {//´ÊµäÄ£°åÀà
	virtual int size() const = 0;
	virtual bool put(K, V) = 0;
	virtual V* get(K k) = 0;
	virtual bool remove(K k) = 0;
};