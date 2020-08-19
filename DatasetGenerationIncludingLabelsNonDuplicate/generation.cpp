#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<time.h>
#include<fstream>
#include<bits/stdc++.h>
#include "graph.h"


using namespace std;

//One way of generating near duplicates is that keep track of array of edges considered for geneating graph like this you have for each graph a array now what we do is that vary like last two elements of the present array try to generate the graps for these and introduce them into dataset but this requires lot of space 



void createGraph(unsigned gnumber,unsigned &nv,unsigned &ne,unsigned long maxV,unsigned long minV,unsigned long maxE,unsigned long minE,vector<unsigned>& vertex_ids,unordered_map<unsigned,unordered_set<unsigned> >& edges,unsigned long labelCount,unordered_map<unsigned,char> &vrtxtocharmap);

//Generating Random Values 
unsigned getRandomVertexCount(unsigned maxV,unsigned minV);
unsigned getRandomVertexLabel(unsigned maxV);
unsigned getRandomPercentageofedges(unsigned maxE,unsigned minE);
unsigned getRandomEdgeLabel(unsigned maxE);

//Printing and Writing to resultant files
void printGraph(unsigned gnumber,unsigned nv,unsigned  ne,vector<unsigned>& vertex_ids,unordered_map<unsigned,unordered_set<unsigned> >& edges);
void writeToFile(unsigned gnumber,unsigned nv,ofstream &datafile,vector<unsigned>& vertex_ids,unordered_map<unsigned,unordered_set<unsigned> >& edges,int NGraphs,unordered_map<unsigned,char> &vrtxtocharmap);
void writeToGraphs(int i,vector<unsigned> &vertex_ids,unordered_map<unsigned,unordered_set<unsigned> > &edges,vector<Graph> &graph_dataset);

//Generating  vertix ids for all vertices in present graph  
void getVertex_ids(unsigned  nv,vector<unsigned> &vertex_ids);



int main(int argc, char const *argv[])
{
    
   //Input Constrains 
    int NGraphs = stoi(argv[1]);
    if(NGraphs <= 0)
    {
        cerr<<"Input required: valid number of graphs"<<endl;  //To validate desired Inputs 
		exit(0);
    }
    
    unsigned long maxV =   stoul(argv[2]);
    unsigned long  minV =   stoul(argv[3]);
    unsigned long maxE =   stoul(argv[4]);
    unsigned long minE =   stoul(argv[5]);
	unsigned long labelCount  = stoul(argv[6]);
    
	 
    srand((unsigned) time(NULL));

    unsigned nv,ne; //No of Vertices and Edges in the current Graph 
    ofstream datafile(argv[7]);
    
    vector<Graph> graph_dataset(NGraphs);  //Initializing Graph Dataset
 
    for(int g = 0; g < NGraphs; g++)
   {
	     vector<unsigned> vertex_ids; //VectorOfVectorLabels
	     unordered_map<unsigned,unordered_set<unsigned> > edges;//unordered_map of final edges of graph 
		 unordered_map<unsigned,char> vrtxtocharmap;
	     createGraph(g+1,nv,ne,maxV,minV,maxE,minE,vertex_ids,edges,labelCount,vrtxtocharmap);//Creaters Graph with given Constrains
	     writeToFile(g+1,nv,datafile,vertex_ids,edges,NGraphs,vrtxtocharmap);//Writing graph to resultant dataset file
	     writeToGraphs(g,vertex_ids,edges,graph_dataset);//writing to the Graph Datastrucure
   }
	datafile.close();
}


unsigned getRandomVertexCount(unsigned long maxV,unsigned long minV){
	return rand()%(maxV-minV)+minV;
}
unsigned getRandomVertexLabel(unsigned long maxV){
	return rand()%(maxV)+1;
}
char getRandomVertexCharLabel(unsigned long labelCount)
{
	int z = rand()%(labelCount)+65;
	return static_cast<char>(z);
}
unsigned getRandomEdgeLabel(unsigned long edgecount){
	return rand()%(edgecount)+0;
}
unsigned getRandomPercentageofedges(unsigned long maxE,unsigned long minE){
	return rand()%(maxE-minE)+minE;
}
bool isEdgeExist(unsigned i,unsigned j,unordered_map<unsigned,unordered_set<unsigned> > &edges){
	return edges.find(i)!=edges.end() && edges[i].find(j)!=edges[i].end();
}
/*Returns true if edge is already exist otherwise returns false*/


void getVertex_ids(unsigned nv,vector<unsigned> &vertex_ids,unsigned long maxV,unsigned long labelCount,unordered_map<unsigned,char> &vrtxtocharmap){

	unordered_set<unsigned> uset; //To Check that no vid is repeated 
	unordered_multiset<char> umvl;
	unsigned vid;
	char ch; 
	while(nv){
		vid  = getRandomVertexLabel(maxV);  //we should get random vertices they need not be inside nv nv is "Total No" of vertices
		ch = getRandomVertexCharLabel(labelCount);
		if(uset.find(vid)==uset.end()){
			uset.insert(vid);
			vertex_ids.push_back(vid);
			vrtxtocharmap.insert({vid,ch});
			nv--;			
		}
	}
	
}

void createGraph(unsigned gnumber,unsigned &nv,unsigned &ne,unsigned long maxV,unsigned long minV,unsigned long maxE,unsigned long minE,vector<unsigned>& vertex_ids,unordered_map<unsigned,unordered_set<unsigned> >& edges,unsigned long labelCount,unordered_map<unsigned,char> &vrtxtocharmap){
      
    nv = getRandomVertexCount(maxV,minV); //To get a random vertex count between min no vertices and max no of vertices
    getVertex_ids(nv,vertex_ids,maxV,labelCount,vrtxtocharmap);
    unsigned NEdges = nv*(nv-1)/2; //No of total no of edges possible for current number of vertices 

    unsigned currEP = getRandomPercentageofedges(maxE,minE); //The percentage of edges that are allowded to form the graph  
    
    unsigned curr_no_ed = ((currEP * NEdges)/100);//The percentage of edges that are allowded to form the graph

    unsigned long ecount = 0;//edge count iterator 

    while(ecount<curr_no_ed)
    {
        		int i = rand() % vertex_ids.size();
			    int j = rand() % vertex_ids.size();//Generating random indices 

        		if(i != j) //Making sur the same vertex is not repeated 
               		 {
					if((!isEdgeExist(vertex_ids[i],vertex_ids[j],edges)) && (!isEdgeExist(vertex_ids[j],vertex_ids[i],edges))) //No Back Edge
					{       
						edges[vertex_ids[i]].insert(vertex_ids[j]);
						ecount++;
					}
			}
    }

    cout << ecount << '\n';  

}

void writeToFile(unsigned gnumber,unsigned nv,ofstream &datafile,vector<unsigned>& vertex_ids,unordered_map<unsigned,unordered_set<unsigned> >& edges,int NGraphs,unordered_map<unsigned,char> &vrtxtocharmap){
	if(gnumber == 1)
	{
		datafile  << NGraphs << '\n';
	}
	datafile << "g"<< ' ' << vertex_ids.size() << ' ' << edges.size() << ' ' << gnumber << endl;
	for(auto vid:vertex_ids)
		datafile << "v " << vid << ' ' << vrtxtocharmap[vid] << endl;
	for(auto mitr=edges.begin();mitr!=edges.end();mitr++){
		for(auto sitr=mitr->second.begin();sitr!=mitr->second.end();sitr++){
			datafile << "e " << mitr->first << ' ' << vrtxtocharmap[mitr->first] << ' ' << *sitr << ' ' << vrtxtocharmap[*sitr] <<endl;
		}
	}
}

void printGraph(unsigned gnumber,unsigned nv,unsigned ne,vector<unsigned>& vertex_ids,unordered_map<unsigned,unordered_set<unsigned> >& edges){ //printing the graph 
	cout << "g "<<gnumber << " vertices: " << nv << " edges: " << ne <<endl;
	for(auto vid:vertex_ids)
		cout << "v "<<vid<<endl;
	for(auto mitr=edges.begin();mitr!=edges.end();mitr++){
		for(auto sitr=mitr->second.begin();sitr!=mitr->second.end();sitr++){
			cout<<mitr->first<<" "<<*sitr<<endl;
		}
	}
}


void writeToGraphs(int i,vector<unsigned> &vertex_ids,unordered_map<unsigned,unordered_set<unsigned> > &edges,vector<Graph> &graph_dataset)//Writing it to Graph
{
	

	for(auto vid:vertex_ids)
	{
        	graph_dataset[i].vertices.push_back(vid);
        }
	
        for(auto mitr=edges.begin();mitr!=edges.end();mitr++){
		for(auto sitr=mitr->second.begin();sitr!=mitr->second.end();sitr++){
			graph_dataset[i].edgeo.push_back({mitr->first,*sitr});
		}
	}
	for(auto v:graph_dataset[i].vertices)
		cout<< v << " ";
	cout<<endl;

	for(auto e:graph_dataset[i].edgeo){
		
		cout << e.first << " " << e.second;
                cout << '\n';		
	}
	cout<<endl;
        cout << endl; 
  
}