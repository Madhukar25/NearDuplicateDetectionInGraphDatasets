#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<time.h>
#include<fstream>
#include<bits/stdc++.h>
#include "graph.h"
#include "graph.cpp"


void parseGraphDataset(ifstream &dataset_file, vector<Graph> &graph_dataset);
int getRandomGraphNo(int NGraphs);
int getRandomOperations(int nop);
int getRandomVertexLabel(int maxV);


using namespace std;

//One way of generating near duplicates is that keep track of array of edges considered for geneating graph like this you have for each graph a array now what we do is that vary like last two elements of the present array try to generate the graps for these and introduce them into dataset but this requires lot of space 

void addVertex(vector<Graph> &graph_dataset,int i,int maxV,int ged);
void delVertex(vector<Graph> &graph_dataset,int i,int maxV,int ged);
void addEdge(vector<Graph> &graph_dataset,int i,int maxV,int ged);
void delEdge(vector<Graph> &graph_dataset,int i,int maxV,int ged);




int main(int argc, char const *argv[])
{

         
    srand((unsigned) time(NULL));

    ifstream dataset_file(argv[1]);
    if(!dataset_file.is_open())       //Checking the dataset file 
    {
        cerr << "Unable to open dataset file" << endl;
        exit(0);
    }
    
    vector<Graph> graph_dataset;

    //Parsing Input dataset file  
    parseGraphDataset(dataset_file, graph_dataset);
    cout << argv[1] << ": Graph Dataset parsed." << endl;

    int NGraphs = graph_dataset.size();
     
    ifstream modification_file(argv[2]);  //The file to which we are writing
    if(!modification_file.is_open())        
    {
        cerr << "Unable to open dataset file" << endl;
        exit(0);
    }

    int noofmodi;    //Total Number Of Modifications that are to be made 
    int maxV = 10;
    modification_file >> noofmodi;
    int nop = 4;
    for(int i = 0;i < noofmodi;i++)
    {
        int noofgraphstochange,ged;

        modification_file >> noofgraphstochange; //The Number of Graphs we have to change with given graph Edit Distance 

	    modification_file >> ged;   //Given Graph  Edit Distance

        //cout << noofgraphstochange << '\n';
        //cout << ged << '\n';

        for(int j = 0;j < noofgraphstochange;j++)
	    {   
            int victimGraphNo = getRandomGraphNo(NGraphs); //Select a random Graph Number 

            int randomoper = getRandomOperations(nop); //Select a random operation that has to be made to the given graph 

            cout << victimGraphNo << '\n';
            cout << randomoper << '\n';

            if(randomoper == 1)
            {
                addVertex(graph_dataset,victimGraphNo,maxV,ged); //Calling addVertex Function
            }
            else if(randomoper == 2)
            {
                if(graph_dataset[victimGraphNo].s.size() > ged)
                    delVertex(graph_dataset,victimGraphNo,maxV,ged); //Calling delVertex Function do it only when you have sufficient vertices to delete 
                else 
                    j--;
            }
            else if(randomoper == 3)
            {
                addEdge(graph_dataset,victimGraphNo,maxV,ged);
            }
            else if(randomoper == 4)
            {
                if(graph_dataset[victimGraphNo].edges.size() > ged)
                    delEdge(graph_dataset,victimGraphNo,maxV,ged); //Calling delVertex Function do it only when you have sufficient edges to delete 
                else 
                    j--;
            }                  
        }
        cout << '\n';
        cout << '\n';
    }


}

bool isEdgeExist(unsigned i,unsigned j,vector<pair<unsigned, unsigned> > edges){
   
   auto p = make_pair(i, j); 
    if(find(edges.begin(), edges.end(), p) != edges.end()) 
        return true;
   return false;
}

int getRandomGraphNo(int NGraphs){
	return rand()%(NGraphs)+1;
}
int getRandomOperations(int nop){
	return rand()%(nop)+1;
}
int getRandomVertexLabel(int maxV){
	return rand()%(maxV)+1;
}

//Adding a Vertex 
void addVertex(vector<Graph> &graph_dataset,int i,int maxV,int ged)
{

    int va = 0;
    while(va < ged)
    {

        int p = getRandomVertexLabel(maxV);
        auto pos = graph_dataset[i].s.find(p);
        if(pos == graph_dataset[i].s.end())
        {
            graph_dataset[i].s.insert(p);
            graph_dataset[i].vertices.push_back(p);
            graph_dataset[i].vertexdeg.insert({p,1});
             va++;
        }   
    }

}


//Deleting a vertex 
void delVertex(vector<Graph> &graph_dataset,int i,int maxV,int ged)
{
     
    int vd = ged;
    while(vd > 0)
    {
        int p = rand() % graph_dataset[i].s.size();
        graph_dataset[i].s.erase(p);
        vd--;
    }
}

//Adding an Edge 
void addEdge(vector<Graph> &graph_dataset,int i,int maxV,int ged)
{
    unsigned ecount = 0;
    while(ecount<ged)
    {
                int p = rand() % graph_dataset[i].vertices.size();
                int q = rand() % graph_dataset[i].vertices.size();//Generating random indices 

        		if(p != q) //Making sur the same vertex is not repeated 
               	{
					if((!isEdgeExist(graph_dataset[i].vertices[p],graph_dataset[i].vertices[q],graph_dataset[i].edges)) && (!isEdgeExist(graph_dataset[i].vertices[q],graph_dataset[i].vertices[p],graph_dataset[i].edges))) //No Back Edge
					{       
						graph_dataset[i].edges.push_back({graph_dataset[i].vertices[p],graph_dataset[i].vertices[q]});
						ecount++;
                        graph_dataset[i].vertexdeg[graph_dataset[i].vertices[p]]++;
                        graph_dataset[i].vertexdeg[graph_dataset[i].vertices[q]]++;
                        auto posa = graph_dataset[i].s.find(graph_dataset[i].vertices[p]);
                        if(posa != graph_dataset[i].s.end())
                        {
                            graph_dataset[i].s.erase(graph_dataset[i].vertices[p]);
                        }
                        auto pos = graph_dataset[i].s.find(graph_dataset[i].vertices[q]);
                        if(pos != graph_dataset[i].s.end())
                        {
                                graph_dataset[i].s.erase(graph_dataset[i].vertices[q]);
                        }
                    }
			    }
    }

}



//Deleting an Edge 
void delEdge(vector<Graph> &graph_dataset,int i,int maxV,int ged)
{
    unsigned  ecount = ged;
    
    while(ecount > 0)
    {
     
       int p = rand() % graph_dataset[i].edges.size();
       unsigned fir = graph_dataset[i].edges[p].first;
       unsigned seco = graph_dataset[i].edges[p].second;
       graph_dataset[i].edges.erase(graph_dataset[i].edges.begin()+p);
       graph_dataset[i].vertexdeg[fir]--;
       graph_dataset[i].vertexdeg[seco]--;
       if(graph_dataset[i].vertexdeg[fir] == 0)
       {
           graph_dataset[i].s.insert(fir);
       }
       if(graph_dataset[i].vertexdeg[seco] == 0)
       {
           graph_dataset[i].s.insert(fir);
       }

       ecount--;
    }

}



// For parsing the input graph dataset
void parseGraphDataset(ifstream &dataset_file, vector<Graph> &graph_dataset)
{
	int size;
	dataset_file >> size;
    cout << size;
	graph_dataset.resize(size);
	for(auto g_iter = graph_dataset.begin(); g_iter != graph_dataset.end(); g_iter++)
	{
		g_iter->readGraph(dataset_file);
		g_iter->displayGraph();
	}	
}



