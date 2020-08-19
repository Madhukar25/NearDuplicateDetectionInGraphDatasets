

To Generate the Results Run the Following Commands - 

controlled.cpp contains a naive way of implementation where we are selecting edges based on 2 vertex_ids generated each time randomly.
 
controlled_approach3 contains a modified way where we generate all the edges and select an edge at random using edge index and including it in the graph.

1.  g++ controlled.cpp(or corresponding file) -o control

2.  ./control NumberofGraphstoBeGenerated maximumnoofvertexices minimumnoofverices maxedgepercentage minedgepercentage noofallowedlabels datafile
    
     For Example ./control 10 8 3 75 20 5 data_file
