#include <iostream>
#include <list>
#include <vector>
#include <queue>

using namespace std;

class node{
	public:
		unsigned int sendTo(int dstID); //return next node
		void bulid_routingTable(int distance[], vector< list<int> > AdjList, int nodeCnt);
		unsigned int id; //�Ӹ`�I��ID
		void printRoutingTable(int nodeCnt);
		
	private: 
		unsigned int routingTable[100]; //�C���I��routing table
};

//MIS�A�~��node
class MIS: public node
{
	public:
		void printMisStatus(){
			printf("node %d status: %d\n", id, status);
		}
		int status; //blue = 0 | red = 1 | black = 2;
};

//CDS�A�~��node
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
						//�P�_ j ���S���b tmp �� AdjList ��
						for(itr = AdjList[tmp].begin(); itr != AdjList[tmp].end(); itr++){  
							if(*itr == j){ flag = 1; break; } //���ӰO���Abreak�X�� 
						}
					}
					if(flag == 1) break;  //�p�Gflag��1�A���j���btmp��AdjList�� 
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
		int *color,             // 0:�զ�(�S��L), 1:�Ǧ�(�Q��L), 2:�¦�(queue.pop)
        *distance,          // 0:�_�I, �L���j:�q�_�I�����쪺vertex
        *predecessor;       // -1:�S��predecessor, ��ܬ��_�Ivertex
    vector< list<int> > AdjList;
		void displayAdjList();
		void find_MIS_node(MIS *MisNodeList);
		void find_CDS_node(CDS *CdsNodeList);
		
	private:
    int num_vertex; //�Ϫ��`�I�ƶq 
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

    for (int i = 0; i < num_vertex; i++) {  // ��l��
        color[i] = 0;                       // 0:�զ�;
        predecessor[i] = -1;                // -1��ܨS��predecessor
        distance[i] = num_vertex+1;         // num_vertex��vertex, 
    }                                       // �̪��Z�� distance = num_vertex -1��edge

    queue<int> q;
    int i = Start;

    for (int j = 0; j < num_vertex; j++) {  // j�q0�ƨ�num_vertex-1, �]��j�|���Lgraph���Ҧ�vertex
        if (color[i] == 0) {                // �Ĥ@��i�|�O�_�Ivertex, �p�ϤG(c)
            color[i] = 1;                   // 1:�Ǧ�
            distance[i] = 0;                // �C�@��connected component���_�I���Z���]��0
            predecessor[i] = -1;            // �C�@��connected component���_�I�S��predecessor
            q.push(i);
            while (!q.empty()) {
                int u = q.front();                  // u ���s���j�M�_�I
                for (list<int>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {                         // ������q
                    if (color[*itr] == 0) {                // �Y�Q�u���v��vertex�O�զ�
                        color[*itr] = 1;                   // ��Ǧ�, ��ܤw�g�Q�u���v
                        distance[*itr] = distance[u] + 1;  // �Z���Opredecessor���Z���[�@
                        predecessor[*itr] = u;             // ��s�Q�u���v��vertex��predecessor
                        q.push(*itr);                      // ��vertex���iqueue
                    }
                }
                q.pop();        // ��u���Xqueue
                color[u] = 2;   // �åB��u��¦�
            }
        }
        // �Y�@���^��S����Ҧ�vertex���L, ���graph���h��connected component
        // �N��i�t��j, �~���ˬdgraph������Lvertex�O�_���O�զ�, �Y�O, ����while loop
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
		//-----�P�_List���٦��S�����I-----// 
		for(int i = 0; i<num_vertex; i++){
			if(MisNodeList[i].status == 0){ //�Y��node��status�O0�A��ܥL�O�Ŧ�A 
				check_blue_flag = 1;
			}
		}
		//�Ycheck_blue_flag��0�A���List���S��blue node�A�K���X�j�� 
		if(check_blue_flag == 0){
			break; 
		}
		
		//-----��MIS node-----// 
		for(int i = 0; i<num_vertex; i++) {
			if(MisNodeList[i].status == 0){
      	for(itr = tmpAdjList[i].begin(); itr != tmpAdjList[i].end(); ++itr) {
					if(MisNodeList[*itr].status != 0){
						continue;
					}	
					//�Y��node��list���������p��L�A�]flag = 1�A�A���X�ӡF
				 	//�Y��node��list���S�������p��L�Aflag ���ܡ]flag == 0�^
				 	if(*itr < i){
						flag = 1; 
						break; 
				 	}
      	}
      	//�Yflag == 1, �o���OMIS 
      	if(flag == 1){
						flag = 0; //���s��ȵ�flag 
      			continue;
				}else{  //else��ܳo�OMIS, �N��status�]������]1�^
						MisNodeList[i].status = 1;
						//�N��node���F�~�]���¦�]2�^ 
				 		for(itr = tmpAdjList[i].begin(); itr != tmpAdjList[i].end(); ++itr) {
				 				MisNodeList[*itr].status = 2;
      			}
				}
   		}
   	}
   	//-----��MIS node-----//
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
			//�b������node������I�A�ñN��]���_�I 
			if(CdsNodeList[i].status == 1){
				srcID = i;
			}else continue;
			//printf("srcID: %d\n", srcID);
			
			for(int j = 0; j < num_vertex; j++){
				//�Y���I��status��1�]red�^�B���OsrcID�A�K�P�_�o�Ӭ��I�PsrcID�������Z���O���O�p�󵥩�3 
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
		
		//�N���I�M���I�]�����I 
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
	
	//��X�Ҧ����⪺�I�A�Yred node�Astatus��2���I 
	g1.find_MIS_node(MisNodeList);
	
	//��CDS:
	//��C�Ӭ��I��BFS�A��distance�p�󵥩�3���I�O���I�ɡA�K�䥦�̤�����shorting path
	//�ç��shorting path�����`�I�]�����]3�^�A��ܥL�OCDS������ 
	CDS CdsNodeList[nodeCnt];
	
	//initial CDS node list
	for(int i=0; i<nodeCnt; i++){
		CdsNodeList[i].status = MisNodeList[i].status; //����MIS�����I��status�ƻs��CDS���A��K��ӳB�z 
		CdsNodeList[i].id = i; 
	}
	
	while(cnt < nodeCnt){
		g1.BFS(cnt); //��C���I��BFS
		CdsNodeList[cnt].copyDistance(g1.distance, nodeCnt);
		CdsNodeList[cnt].bulid_routingTable(g1.distance, g1.AdjList, nodeCnt);
		cnt++;
	}cnt = 0;
	
	//��X�Ҧ���⪺�I�A�Yorange node�Astatus��3���I�A�ñNred�Morange�ܦ�purple(4) 
	g1.find_CDS_node(CdsNodeList);
	
	while(cnt < output){
			int srcProxy, dstProxy; //�_�I�M���I���N�z�H 
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
