#include <iostream>
#include <queue>
using namespace std;

void printBits(unsigned char s) {
	//print the bit pattern
	for (int i = 0; i < 8; i++)
		if (s & (1 << (7 - i)))
			cout << 1;
		else
			cout << 0;
}

class BitMatrixGraph {
protected:
	unsigned char** bitAdjacencyMatrix;
	int n; //number of nodes in graph
	int* bfsParentArray = NULL;
	int* dfsParentArray = NULL;
	queue<int> Q;
	int* bfsVisited = NULL;
	int* dfsVisited = NULL;
public:
	BitMatrixGraph();
	BitMatrixGraph(int numNodes);
	BitMatrixGraph(const BitMatrixGraph& graphCopy);
	friend ostream& operator<<(ostream& o, BitMatrixGraph *graph);
	void operator=(const BitMatrixGraph& graphCopy);
	void addEdge(int u, int v);
	bool isAnEdge(int u, int v); //very important for BFS and DFS
	int* getBFSArray();
	int* getDFSArray();

	//DFS implementation
	void depthFirstSearch(int vertex, int size);
	void dfs(int vertex);

	//BFS implementation
	void breadthFirstSearch(int u, int size);
	void bfs();
};

BitMatrixGraph::BitMatrixGraph() {
	bitAdjacencyMatrix = NULL;
	n = 0;
}

BitMatrixGraph::BitMatrixGraph(int numNodes) {
	int size;
	n = numNodes;

	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;

	bitAdjacencyMatrix = new unsigned char*[n];
	for (int i = 0; i < n; i++)
		bitAdjacencyMatrix[i] = new unsigned char[size];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < size; j++)
			bitAdjacencyMatrix[i][j] = 0x00 << 8;
}

BitMatrixGraph::BitMatrixGraph(const BitMatrixGraph& graphCopy) {
	n = graphCopy.n;
	int size;

	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;

	bitAdjacencyMatrix = new unsigned char *[n];
	for (int i = 0; i < n; i++)
		bitAdjacencyMatrix[i] = new unsigned char[size];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < size; j++)
			bitAdjacencyMatrix[i][j] = 0x00 << 8;
}

void BitMatrixGraph::operator=(const BitMatrixGraph& graphCopy) {
	n = graphCopy.n;
	int size;

	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;

	bitAdjacencyMatrix = new unsigned char *[n];
	for (int i = 0; i < n; i++)
		bitAdjacencyMatrix[i] = new unsigned char[size];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < size; j++)
			bitAdjacencyMatrix[i][j] = 0x00 << 8;
}

void BitMatrixGraph::addEdge(int u, int v) {
	int p = v / 8;
	int w;
	if (v < 8) {
		w = 7 - v;
		
		bitAdjacencyMatrix[u][p] |= 0x01 << w;
	}
	else {
		w = 7 - (v % 8);
		bitAdjacencyMatrix[u][p] |= 0x01 << w;
	}
}

bool BitMatrixGraph::isAnEdge(int u, int v) {
	int p = v / 8;

	int w = 7 - (v % 8);

	unsigned char l = bitAdjacencyMatrix[u][p];

	if (l & (1 << w))
		return true;
	else
		return false;
}

ostream& operator<<(ostream& o, BitMatrixGraph *graph) {
	int v = graph->n;

	int p;
	if (v % 8 == 0)
		p = v / 8;
	else
		p = (int)(v / 8) + 1;

	cout << "BitAdjacencyMatrix for graph: " << endl;

	for (int i = 0; i < v; i++) {
		for (int j = 0; j < p; j++) {
			printBits(graph->bitAdjacencyMatrix[i][j]);
		}
		cout << endl;
	}

	o << endl;
	return o;
}

void BitMatrixGraph::breadthFirstSearch(int u, int size) {
	bfsParentArray = new int[size];
	bfsVisited = new int[size];

	for (int i = 0; i < size; i++) {
		bfsVisited[i] = false;
		bfsParentArray[i] = -1;
	}
	for (int i = 0; i < size; i++) {
		if (!bfsVisited[i]) {
			bfsVisited[i] = true;
			Q.push(i);
			bfs();
		}
	}
}

void BitMatrixGraph::bfs() {
	while (!Q.empty()) {
		int x = Q.front();
		Q.pop();
		int u = this->n;

		for (int j = 0; j < u; j++) {
				if (this->isAnEdge(x, j)) {
					if (!bfsVisited[j]) {
						bfsVisited[j] = true;
						bfsParentArray[j] = x;
						Q.push(j);
					}
			}
		}
	}
}

int* BitMatrixGraph::getBFSArray() {
	return bfsParentArray;
}

void BitMatrixGraph::depthFirstSearch(int vertex, int size) {
	dfsParentArray = new int[size];
	dfsVisited = new int[size];

	for (int i = 0; i < size; i++) {
		dfsVisited[i] = false;
		dfsParentArray[i] = -1;
	}
	for (int i = 0; i < size; i++) {
		if (!dfsVisited[i])
			dfs(i);
	}
}

void BitMatrixGraph::dfs(int vertex) {
	dfsVisited[vertex] = true;

	for (int i = 0; i < this->n; i++) {
		if (this->isAnEdge(vertex, i)) {
			if (!dfsVisited[i]) {
				dfsParentArray[i] = vertex;
				dfs(i);
			}
		}
	}
}

int* BitMatrixGraph::getDFSArray() {
	return dfsParentArray;
}

int main() {
	int para1, para2;
	int size;
	cin >> size;
	
	BitMatrixGraph* myGraph = new BitMatrixGraph(size);

	while (!cin.eof()) {
		cin >> para1 >> para2;
		myGraph->addEdge(para1, para2);
	}

	

	cout << myGraph;
	

	myGraph->breadthFirstSearch(0, size);
	cout << "BFS Parent Array: ";
	for (int i = 0; i < size; i++) {
		cout << (myGraph->getBFSArray())[i];
		if (i < size - 1)
		cout << ", ";
	}
	cout << endl << endl;

	myGraph->depthFirstSearch(0, size);
	cout << "DFS Parent Array: ";
	for (int i = 0; i < size; i++) {
		cout << (myGraph->getDFSArray())[i];
		if (i < size - 1)
			cout << ", ";
	}
	cout << endl;
}