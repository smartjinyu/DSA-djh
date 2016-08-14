typedef enum{UNDISCOVERED,DISCOVERED,VISITED} VStatus;
typedef enum{UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EStatus;
template <typename Tv,typename Te>//顶点类型，边类型
class Graph {
private:
	void reset() {
		for (int i = 0; i < n; i++) {
			status(i) = UNDISCOVERED; dtime(i) = ftime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; j++) {
				if (exists(i, j)) status(i, j) = UNDETERMINED;
			}
		}
	}
	void BFS(int, int &);//(连通域)广度优先
	void DFS(int, int &);//(连通域)深度优先
	void BCC(int, int &, Stack<int>&);//(连通域)基于DFS的双连通分量分解算法
	bool TSort(int, int &, Stack<Tv>*);//(连通域)基于DFS的拓扑排序算法
	template<typename PU> void PFS(int, PU);//(连通域)优先级搜索框架
pulic:
	//顶点
	int n;
	virtual int insert(T const&) = 0;//return i
	virtual Tv remove(int) = 0;
	virtual Tv& vertex(int) = 0;//顶点v的数据
	virtual int inDegree(int) = 0;//入度
	virtual int outDegree(int) = 0;//出度
	virtual int firstNbr(int) = 0;//顶点v的首个邻接节点
	virtual int nextNbr(int, int) = 0;//顶点v相对于顶点j的下一个邻接节点
	virtual VStatus& status(int) = 0;
	virtual int& dTime(int) = 0;
	virtual int& fTime(int) = 0;
	virtual int& parent(int) = 0;//顶点v的父亲
	virtual int& priority(int) = 0;//顶点v的优先级数
	//边 约定将无向边统一转化为互逆的一对有向边，从而把无向图视作有向图的特例
	int e;
	virtual bool exists(int, int) = 0;//边u，v是否存在
	virtual bool insert(Te const&, int, int, int) = 0;//在顶点v与u之间插入权重为w的边e
	virtual Te remove(int, int) = 0;
	virtual EStatus& status(int, int) = 0;//边(u,v)的数据
	virtual Te& edge(int, int) = 0;
	virtual int& weight(int, int) = 0;//边(u,v)的权重
	//algorithm
	void bfs(int);
	void dfs(int);
	void bcc(int);
	Stack<Tv>* tSort(int);//基于DFS的拓扑排序算法
	void prim(int);//最小支撑树Prim算法
	void dijkstra(int);//最短路径Dijkstra算法
	template<typename PU> void pfs(int, PU) = 0;//优先级搜索框架
};