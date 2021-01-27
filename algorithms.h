
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define MAX 10

int visited[MAX];

using namespace std;

typedef struct node{
	int data;
	struct node *next;
}Node;

class STACK{
	private:
		Node *top;
	public:
		STACK(){
			//top=new Node;
			top=NULL;
		}

		bool isEmpty(){
			return top==NULL;
		}

		void push(int val){
			Node *newTop=new Node;
			newTop->data=val;
			newTop->next=top;
			top=newTop;
		}

		int pop(){
			if (top==NULL) return -1;
			Node *oldTop=top;
			top=top->next;
			int result=oldTop->data;
			//oldTop->next=NULL;
			free(oldTop);
			return result;
		}

		int peek(){
			if (top==NULL) return -1;
			return top->data;
		}

};

class QUEUE{
	private:
		Node *first;
		Node *last;
	public:
		QUEUE(){
			//first=new Node;
			first=last=NULL;
		}

		bool isEmpty(){
			return first==NULL;
		}

		void push(int val){
			Node *newLast=new Node;
			newLast->data=val;
			newLast->next=NULL;
			if (first==NULL) {
				first=last=newLast;
				return;
			}
			last->next=newLast;
			last=newLast;
		}

		int pop(){
			if (isEmpty()) return -1;
			int result=first->data;
			Node *oldFirst=first;
			first=first->next;
			if (first==NULL) {
				last=NULL;
			}
			free(oldFirst);
			return result;
		}
};

void refreshVisited(){
	for (int i=0;i<MAX;i++) visited[i]=0;
}

string convertItoS(int x){
    /*
    if (x<0) return "\0";
    string index;
    if (x<10) index.push_back((char)(x+'0'));
    else{
        index.push_back((char)(1+'0'));
        index.push_back((char)(0+'0'));
    }
    return index;
    */
    char *k=new char[5];
    string index=string(itoa(x,k,10));
    return index;
}

int** initilizeMatrix (int n){
	const int k=n;
	int **result=new int*[n];
	for (int i=0;i<n;i++){
		result[i]=new int[n];
		for (int k=0;k<n;k++){
			result[i][k]=0;
		}
	}
	return result;
}

void deleteMatrix(int **matrix, int n){
	for (int i=0;i<n;i++){
		delete[] matrix[i];
	}
	delete matrix;
}

int* DFS (int **matrix,int n,int start){
	STACK stack;
	//refreshVisited();
	int *order=new int[n];
	for (int j=0;j<n;j++) order[j]=-1;
	visited[start]=1;
	stack.push(start);
	order[0]=start;
	int k=0;
	while (!stack.isEmpty()){
		int node=stack.pop();
		for (int i=0;i<n;i++){
			if (matrix[node][i]!=0 && visited[i]==0){
				visited[i]=1;
				stack.push(node);
				stack.push(i);
				order[++k]=i;
				//cout << k << " --> " << order[k] << endl;
				break;
			}
		}
	}
	/*
	for (int j=0;j<=k;j++){
		cout << order[j] << " --> ";
	}
	cout << endl;
	*/
	return order;
}

int* BFS(int **matrix,int n,int start){
	QUEUE queue;
	//refreshVisited();
	int *order=new int[n];
	for (int j=0;j<n;j++) order[j]=-1;
	visited[start]=1;
	queue.push(start);
	order[0]=start;
	int k=0;
	while (!queue.isEmpty()){
		int node=queue.pop();
		for (int i=0;i<n;i++){
			if (matrix[node][i]!=0 && visited[i]==0){
				queue.push(i);
				visited[i]=1;
				order[++k]=i;
			}
		}
	}

	return order;
}

class ConEleMt{
    public:
        int **matrix[MAX];
        int n;
        ConEleMt(){
            n=0;
        }
        void add(int *order){
            if (n==MAX) return;
            matrix[n]=new int*;
            *matrix[n++]=order;
        }
};

ConEleMt connective(int **matrix,int n){
	//int numOfConEle=0;
	//int **conEleMatrix=new int*[n];
	ConEleMt mt;
	refreshVisited();
	for (int i=0;i<n;i++){
		if (visited[i]==0){
			mt.add(DFS(matrix,n,i));
			/*numOfConEle++;
			delete[] temp;*/
		}
	}
    return mt;
}



int* duyetTru(int **matrix,int n){
	//refreshVisited();
	bool exist=false;
	int *arr=new int[n];
	for (int i=0;i<n;i++) arr[i]=-1;
	int index=0;
	for (int a=0;a<n;a++){
		refreshVisited();
		visited[a]=1;
		for (int b=0;b<n;b++){
			if (visited[b]==0){
				DFS(matrix,n,b);
				break;
			}
		}
		for (int c=0;c<n;c++){
			if (visited[c]==0){
				arr[index++]=a;
				exist=true;
				break;
			}
		}

	}
	return arr;
}

void duyetCau(int **matrix,int n){
	refreshVisited();
	bool exist=false;
	for (int i=0;i<n;i++){
		for (int k=0;k<n;k++){
			if (matrix[i][k]!=0){
				int t1=matrix[i][k];
				int t2=matrix[k][i];
				matrix[i][k]=0;
				matrix[i][k]=0;
				DFS(matrix,n,0);
				for (int j=0;j<n;j++){
					if (visited[j]==0){
						cout << "Canh cau: " << i << " -- " << k << endl;
						bool exist=true;
						break;
					}
				}
				matrix[i][k]=t1;
				matrix[k][i]=t2;
				refreshVisited();
			}
		}
	}
	if (!exist) cout << "Khong co canh cau" << endl;
}

void deleteNode(int **matrix, int n, int **temp ,int k){
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			if (k==i){
				temp[i][j]=0;
				temp[j][i]=0;
			}
			else temp[i][j]=matrix[i][j];
		}
	}
}

void duyetThat(int **matrix,int n,int u, int v){
	int **tempMatrix=initilizeMatrix(n);
	bool isExist=false;
	for (int a=0;a<n;a++){
		if (a!=u && a!=v){
			deleteNode(matrix,n,tempMatrix,a);
			refreshVisited();
			DFS(tempMatrix,n,u);
			if (visited[v]==1) {
				cout << "Dinh that: " << a << endl;
				isExist=true;
			}
		}
	}
	if (!isExist) cout << "Khong co dinh that" << endl;
	deleteMatrix(tempMatrix,n);
}

bool isConnective(int **matrix,int n){
	for (int i=0;i<n;i++){
		bool allVisited=true;
		refreshVisited();
		DFS(matrix,n,i);
		for (int k=0;k<n;k++){
			if (visited[k]==0) {
				allVisited=false;
				break;
			}
		}
		if (allVisited) return true;
	}
	return false;
}

bool isEuler(int **matrix,int n){
	if (!isConnective(matrix,n)) return false;
	for (int r=0;r<n;r++){
		int od=0;
		int id=0;
		for (int c=0;c<n;c++){
			if (matrix[r][c]!=0) od++;
			if (matrix[c][r]!=0) id++;
		}
		if (od!=id) return false;
	}
	return true;

}

int isHalfEuler(int **matrix,int n){
	if (!isConnective(matrix,n)) return -1;
	int w=0;
	int e=0;
	int d;
	for (int i=0;i<n;i++){
		int od=0;
		int id=0;
		for (int k=0;k<n;k++){
			if (matrix[i][k]!=0) od++;
			if (matrix[k][i]!=0) id++;
		}
		if (od==id) e++;
		else if (od-id==1) {
			w++;
			d=i;
		}
		if (w>2) return -1;
	}
	if (w==2 && w+e==n) return d;
	return -1;
}

int* eulerCircularPath(int **matrix, int n,int start){
	if (!isEuler(matrix,n)) return NULL;
	int *ce=new int[n];
	int ceIndex=0;
	int matrixTemp[n][n];
	for (int k=0;k<n;k++){
		for (int j=0;j<n;j++) matrixTemp[k][j]=matrix[k][j];
	}
	STACK stack;
	stack.push(start);
	while(!stack.isEmpty()){
		int node=stack.peek();
		bool existPath=false;
		for (int i=0;i<n;i++){
			if (matrixTemp[node][i]!=0){
				stack.push(i);
				matrixTemp[node][i]=0;
				matrixTemp[i][node]=0;
				existPath=true;
				break;
			}
		}
		if (!existPath){
			ce[ceIndex++]=stack.pop();
		}
	}

	return ce;
}


int* Dijkstra(int **matrix,int n, int s, int t){
	int dist[n];
	int minNode;
	int pre[n];
	bool sptSet[n];
	for (int i=0;i<n;i++){
		dist[i]=INT_MAX;
		pre[i]=s;
		sptSet[i]=false;
	}
	dist[s]=0;
	sptSet[s]=true;
	while (!sptSet[t]){
		int min=INT_MAX;

		for (int k=0;k<n;k++){
			if (!sptSet[k] && dist[k]<min){
				min=dist[k];
				minNode=k;
			}
		}
		sptSet[minNode]=true;
		if (!sptSet[t]){
			for (int k=0;k<n;k++){
				if ( !sptSet[k] && dist[k]>dist[minNode]+matrix[minNode][k] ){
					dist[k]=dist[minNode]+matrix[minNode][k];
					pre[k]=minNode;
				}
			}
		}
	}
	return pre;
}

// apply to DFS,BFS
string getResultStr (int *order,int n) {
    string result="";

    for (int i=0;i<n;i++){
        if (order[i]==-1) break;

        result+=convertItoS(order[i])+" ";
    }
    return result;
}

string getConnEleResult(ConEleMt mt,int k){
    int n=mt.n;
    string result="";

    for (int i=0;i<n;i++){
        if (i!=0 && i%3==0) result+="\n\n";
        result+="Thanh phan "+convertItoS(i+1)+": ";
        int *temp=*(mt.matrix[i]);
        result+=getResultStr(temp,k)+" | ";
        delete temp;
    }
    return result;
}

string getTruResult(int *arr,int n){
    if (arr[0]==-1) return "Khong co dinh tru";
    string result="Dinh tru: ";
    for (int i=0;i<n;i++){
        if (arr[i]!=-1) result+=convertItoS(arr[i])+" ";
        else break;
    }
    return result;
}

