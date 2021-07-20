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

//MIS，繼承node
class MIS: public node
{
	public:
		void printMisStatus(){
			printf("node %d status: %d\n", id, status);
		}
		int status; //blue = 0 | red = 1 | black = 2;
};

//CDS，繼承node
class CDS: public node
{
	public:
		
		void printCdsStatus(){
			printf("node %d status: %d\n", id, status);
		}
		
		void copyDistance(int inputDistance[], int nodeCnt){
			distance = new int[nodeCnt];
			for(int i = 0; i < nodeCnt; i++){
				distance[i] = inputDistance[i];
			}
		}
		
		int find_proxy(vector< list<int> > AdjList, CDS *CdsNodeList){
				list<int>::iterator itr;
				for(itr = AdjList[id].begin(); itr != AdjList[id].end(); itr++){  
					if(CdsNodeList[*itr].status == 4){ 
						return *itr; 
					}
				}
		}
		
		int *distance;
		int status;	//orange = 3 | purple = 4( Set red(1) and orange(3) nodes to purple(4) )
								//purple node is CDS's member;
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
		if(0 == distance[i]){
			routingTable[i] = 0;
		}
		else if(1 == distance[i]){
			routingTable[i] = i; 
		}
		else if(1 < distance[i]){
			int dist = distance[i], tmp = i;
			while(1 < dist){
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
    Graph():num_vertex(0){};          // default constructor
    Graph(int nodeCnt);								// constructor with input: number of vertex
    void AddEdgeList(int node1, int node2);
    void BFS(int Start);
		int *color,             // 0:白色(沒找過), 1:灰色(被找過), 2:黑色(queue.pop)
        *distance,          // 0:起點, 無限大:從起點走不到的vertex
        *predecessor;       // -1:沒有predecessor, 表示為起點vertex
    vector< list<int> > AdjList;
		void displayAdjList();
		void find_MIS_node(MIS *MisNodeList);
		void find_CDS_node(CDS *CdsNodeList);
		
	private:
    int num_vertex; //圖的節點數量 
};

Graph::Graph(int nodeCnt): num_vertex(nodeCnt)
{        // constructor with input: number of vertex
      // initialize Adjacency List
    	AdjList.resize(num_vertex);
}

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

void Graph::find_MIS_node(MIS *MisNodeList)
{
	list<int>::iterator itr;
	int flag = 0, check_blue_flag = 0;
	
	vector< list<int> > tmpAdjList;
	tmpAdjList.resize(num_vertex);
	tmpAdjList = AdjList; 

	while(check_blue_flag == 0){
		//-----判斷List中還有沒有藍點-----// 
		for(int i = 0; i<num_vertex; i++){
			if(MisNodeList[i].status == 0){ //若有node的status是0，表示他是藍色， 
				check_blue_flag = 1;
			}
		}
		//若check_blue_flag為0，表示List中沒有blue node，便跳出迴圈 
		if(check_blue_flag == 0){
			break; 
		}
		
		//-----找MIS node-----// 
		for(int i = 0; i<num_vertex; i++) {
			if(MisNodeList[i].status == 0){
      	for(itr = tmpAdjList[i].begin(); itr != tmpAdjList[i].end(); ++itr) {
					if(MisNodeList[*itr].status != 0){
						continue;
					}	
					//若該node的list中有成員小於他，設flag = 1，再跳出來；
				 	//若該node的list中沒有成員小於他，flag 不變（flag == 0）
				 	if(*itr < i){
						flag = 1; 
						break; 
				 	}
      	}
      	//若flag == 1, 這不是MIS 
      	if(flag == 1){
						flag = 0; //重新賦值給flag 
      			continue;
				}else{  //else表示這是MIS, 將其status設為紅色（1）
						MisNodeList[i].status = 1;
						//將該node的鄰居設為黑色（2） 
				 		for(itr = tmpAdjList[i].begin(); itr != tmpAdjList[i].end(); ++itr) {
				 				MisNodeList[*itr].status = 2;
      			}
				}
   		}
   	}
   	//-----找MIS node-----//
 	}
}

void Graph::find_CDS_node(CDS *CdsNodeList)
{
	list<int>::iterator itr;
	int flag = 0, redNodeCnt = 0, counter = 0;
	int srcID, dstID;
	
	vector< list<int> > tmpAdjList;
	tmpAdjList.resize(num_vertex);
	tmpAdjList = AdjList;
	
	for(int i = 0; i<num_vertex; i++){
			if(CdsNodeList[i].status == 1){
				redNodeCnt++;
			}
	}
	for(int i = 0; i < num_vertex; i++){
			//在全部的node中找紅點，並將其設為起點 
			if(CdsNodeList[i].status == 1){
				srcID = i;
			}else continue;
			//printf("srcID: %d\n", srcID);
			
			for(int j = 0; j < num_vertex; j++){
				//若該點的status為1（red）且不是srcID，便判斷這個紅點與srcID之間的距離是不是小於等於3 
				if( (CdsNodeList[j].status == 1 ) && (j != srcID) )
				{
					if(CdsNodeList[j].distance[srcID] <= 3){
						dstID = j;
					}else{ continue; }
				}else{ continue; }
				
				unsigned int curID = srcID;
				while(curID != dstID){
					if(CdsNodeList[curID].status != 1){
						CdsNodeList[curID].status = 3;
					}
					curID = CdsNodeList[curID].sendTo(dstID);
				}
			}
	}
		
		//將紅點和橙點設為紫點 
	for(int i=0; i< num_vertex; i++){
			if(CdsNodeList[i].status == 1 || CdsNodeList[i].status == 3){
				CdsNodeList[i].status = 4; 
			}
	}
}

int main()
{
	unsigned int nodeCnt = 0, edgeCnt = 0, cnt = 0, ith_edge = 0, output = 0, ith_output = 0;
	unsigned int Node1, Node2, srcID[100] = {0}, dstID[100] = {0};
	
	cin >> nodeCnt >> edgeCnt;
	
	Graph g1(nodeCnt); //build Graph to do bfs
	
	while(cnt < edgeCnt-1)
	{
			cin >> ith_edge >> Node1 >> Node2;
			g1.AddEdgeList(Node1, Node2);
			cnt = ith_edge;
	}cnt = 0;
	
	//get output//
	cin >> output;
	while(cnt < output){
		cin >> ith_output >> srcID[cnt] >> dstID[cnt];
		cnt++;
	}cnt = 0;
	
  //creat a MisNodeList array
	MIS MisNodeList[nodeCnt];
	
	for(int i=0; i<nodeCnt; i++){
		MisNodeList[i].status = 0; //active, blue
		MisNodeList[i].id = i; 
	}
	
	//找出所有紅色的點，即red node，status為2的點 
	g1.find_MIS_node(MisNodeList);
	
	//找CDS:
	//對每個紅點做BFS，當distance小於等於3的點是紅點時，便找它們之間的shorting path
	//並把該shorting path中的節點設成橙色（3），表示他是CDS的成員 
	CDS CdsNodeList[nodeCnt];
	
	//initial CDS node list
	for(int i=0; i<nodeCnt; i++){
		CdsNodeList[i].status = MisNodeList[i].status; //先把MIS中的點的status複製到CDS中，方便後來處理 
		CdsNodeList[i].id = i; 
	}
	
	while(cnt < nodeCnt){
		g1.BFS(cnt); //對每個點做BFS
		CdsNodeList[cnt].copyDistance(g1.distance, nodeCnt);
		CdsNodeList[cnt].bulid_routingTable(g1.distance, g1.AdjList, nodeCnt);
		cnt++;
	}cnt = 0;
	
	//找出所有橙色的點，即orange node，status為3的點，並將red和orange變成purple(4) 
	g1.find_CDS_node(CdsNodeList);
	
	while(cnt < output){
			int srcProxy, dstProxy; //起點和終點的代理人 
			unsigned int curID = srcID[cnt];
			cout << cnt << " " << srcID[cnt] << " ";
			
			if(CdsNodeList[ srcID[cnt] ].status == 2){
				srcProxy = CdsNodeList[srcID[cnt]].find_proxy(g1.AdjList, CdsNodeList);
				curID = CdsNodeList[srcID[cnt]].find_proxy(g1.AdjList, CdsNodeList);
				cout << curID << " ";
			}
			
			if(CdsNodeList[ dstID[cnt] ].status == 2){
				dstProxy = CdsNodeList[dstID[cnt]].find_proxy(g1.AdjList, CdsNodeList);
				//printf("dstProxy: %d\n", dstProxy);
				
				while(curID != dstProxy){
					curID = CdsNodeList[curID].sendTo(dstProxy);
					cout << curID << " ";
				}
				cout << dstID[cnt] << " " << endl;
				cnt++;
			}else{
				while(curID != dstID[cnt]){
					curID = CdsNodeList[curID].sendTo(dstID[cnt]);
					cout << curID << " ";
				}cout << endl;
				cnt++;
			}
	}

	return 0;
}
