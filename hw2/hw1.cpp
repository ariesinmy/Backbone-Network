#include <iostream>
#include <list>
#include <vector>
#include <queue>

using namespace std;

class node{
	
	public:
		unsigned int sendTo(int dstID); //return next node
		void bulid_routingTable(int distance[], vector< list<int> > AdjList, int nodeCnt);
		unsigned int id; //該節點的ID
		void printRoutingTable(int nodeCnt);
		
	private: 
		unsigned int routingTable[100]; //每個點的routing table
		
};

unsigned int node::sendTo(int dstID)
{
	return routingTable[dstID]; 
}

void node::bulid_routingTable(int distance[], vector< list<int> > AdjList, int nodeCnt)
{
	list<int>::iterator itr;
	int flag = 0, i, j;
	
	for(i=0; i<nodeCnt; i++){
		//cout << "distance[" << i << "]: " << distance[i] <<endl; 
		if(0 == distance[i]){
			routingTable[i] = 0;
		}
		else if(1 == distance[i]){
			routingTable[i] = i; 
		}
		else if(1 < distance[i]){
			int dist = distance[i], tmp = i;
			while(1 < dist){
				//cout << "tmp: " << tmp << endl;	
				dist--;
				for(j=0; j<nodeCnt; j++){
					if(distance[j] == dist){
						//判斷 j 有沒有在 tmp 的 AdjList 中
						for(itr = AdjList[tmp].begin(); itr != AdjList[tmp].end(); itr++){  
							if(*itr == j){ flag = 1; break; } //做個記號，break出來 
						}
					}
					if(flag == 1) break;  //如果flag為1，表示j有在tmp的AdjList中 
				}flag = 0;
				tmp = j;
			}
			routingTable[i] = j;
		}
	}
}

void node::printRoutingTable(int nodeCnt)
{
	for(int i=0; i<nodeCnt; i++){
		cout << routingTable[i] << " ";
	}cout << endl;
}

class Graph{
	public:
    Graph():num_vertex(0){};           // default constructor
    Graph(int nodeCnt);								// constructor with input: number of vertex
    void AddEdgeList(int node1, int node2);
    void BFS(int Start);
		int *color,             // 0:白色(沒找過), 1:灰色(被找過), 2:黑色(queue.pop)
        *distance,          // 0:起點, 無限大:從起點走不到的vertex
        *predecessor;       // -1:沒有predecessor, 表示為起點vertex
    vector< list<int> > AdjList;
		void displayAdjList();
		
	private:
    int num_vertex;
    
};

Graph::Graph(int nodeCnt): num_vertex(nodeCnt)
{        // constructor with input: number of vertex
      // initialize Adjacency List
    	AdjList.resize(num_vertex);
};

void Graph::AddEdgeList(int node1, int node2){
    AdjList[node1].push_back(node2);
    AdjList[node2].push_back(node1);
}

void Graph::BFS(int Start){

    color = new int[num_vertex];
    predecessor = new int[num_vertex];
    distance = new int[num_vertex];

    for (int i = 0; i < num_vertex; i++) {  // 初始化
        color[i] = 0;                       // 0:白色;
        predecessor[i] = -1;                // -1表示沒有predecessor
        distance[i] = num_vertex+1;         // num_vertex個vertex, 
    }                                       // 最長距離 distance = num_vertex -1條edge

    queue<int> q;
    int i = Start;

    for (int j = 0; j < num_vertex; j++) {  // j從0數到num_vertex-1, 因此j會走過graph中所有vertex
        if (color[i] == 0) {                // 第一次i會是起點vertex, 如圖二(c)
            color[i] = 1;                   // 1:灰色
            distance[i] = 0;                // 每一個connected component的起點之距離設成0
            predecessor[i] = -1;            // 每一個connected component的起點沒有predecessor
            q.push(i);
            while (!q.empty()) {
                int u = q.front();                  // u 為新的搜尋起點
                for (list<int>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {                         // 分成兩段
                    if (color[*itr] == 0) {                // 若被「找到」的vertex是白色
                        color[*itr] = 1;                   // 塗成灰色, 表示已經被「找到」
                        distance[*itr] = distance[u] + 1;  // 距離是predecessor之距離加一
                        predecessor[*itr] = u;             // 更新被「找到」的vertex的predecessor
                        q.push(*itr);                      // 把vertex推進queue
                    }
                }
                q.pop();        // 把u移出queue
                color[u] = 2;   // 並且把u塗成黑色
            }
        }
        // 若一次回圈沒有把所有vertex走過, 表示graph有多個connected component
        // 就把i另成j, 繼續檢查graph中的其他vertex是否仍是白色, 若是, 重複while loop
        i = j;
    }
    
}

void Graph::displayAdjList() {
	
   for(int i = 0; i<num_vertex; i++) {
   	
      cout << i << "--->";
      list<int> :: iterator it;
      for(it = AdjList[i].begin(); it != AdjList[i].end(); ++it) {
         cout << *it << " ";
      }
      cout << endl;
   }
   
}

int main()
{
	unsigned int nodeCnt = 0, edgeCnt = 0, cnt = 0, ith_edge = 0, output = 0, ith_output = 0;
	unsigned int Node1, Node2, srcID[100] = {0}, dstID[100] = {0};
	
	cin >> nodeCnt >> edgeCnt;
	//cout << "nodeCnt: " << nodeCnt << endl;
	//cout << "edgeCnt: " << edgeCnt << endl;
	
	Graph g1(nodeCnt); //build Graph to do bfs
	while(cnt < edgeCnt-1)
	{
			cin >> ith_edge >> Node1 >> Node2;
			g1.AddEdgeList(Node1, Node2);
			cnt = ith_edge;
	}cnt = 0;
	
	//g1.displayAdjList();
	
	//get output//
	cin >> output;
	//cout << "output: " << output << endl;
	while(cnt < output){
		cin >> ith_output >> srcID[cnt] >> dstID[cnt]; //cout << ith_output << " "<< srcID[cnt] << " " << dstID[cnt] << endl;
		cnt++;
	}cnt = 0;
	
	node NodeList[nodeCnt];
	while(cnt < nodeCnt){
		g1.BFS(cnt); //對每個點做BFS
		NodeList[cnt].id = cnt;    //賦予節點的ID 
		NodeList[cnt].bulid_routingTable(g1.distance, g1.AdjList, nodeCnt); //傳入g1.distance，建立routingTable 
		//NodeList[cnt].printRoutingTable(nodeCnt);
		cnt++;
	}cnt = 0;
	
	while(cnt < output){
		unsigned int curID = srcID[cnt];
		cout << cnt << " " << srcID[cnt] << " ";
		while(curID != dstID[cnt]){
			curID = NodeList[curID].sendTo(dstID[cnt]);
			cout << curID << " ";
		}cout << endl;
		cnt++;
	}cnt = 0;
	
	return 0;
}
