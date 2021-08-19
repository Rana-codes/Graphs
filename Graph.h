// Student Name: Harsh Rana
//
// CPSC 2150 specifications by Dr. Gladys Monagan
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>

class Graph {
   public:
      // build an empty graph with no vertices and no edges
      // the only way to add edges or vertices to the graph is with 
      // the >> operator which deletes the current graph
      // and reads and builds a new one
      Graph();

      // read the number of vertices n
      // read pairs of integers corresponding to an edge in the graph
      // read until iStream fails (i.e. no more data or the data is not an integer)
      // reset iStream and clear the buffer of iStream
      // about the input expected
      //    => all the vertices vi will have values 0 <= vi < numberOfVertices()
      //    => if any of the vertices of the edge (i, j) are not
      //       0 <= i < numberOfVertices() and that 0 <= j < numberOfVertices(), 
      //       the edge is not added to the graph
      //    => every undirected edge may only be read ONCE
      //       if an undirected edge is read twice, it is ignored and not added
      //       to the graph
      // precondition: 
      //    in is an open stream not in a fail state
      // postcondition:
      //    the data read from a file forms the graph, allocating memory dynamically
      //    and setting the number of vertices internally so that the function
      //    numberOfVertices() returns the number of vertices in the graph
      friend std::istream& operator >> (std::istream& in, Graph& graph);

      // output the graph in a convenient format
      // a possibility is to output a vertex and then its neighbours
      // precondition:
      //    oStream is an open stream not in a fail state
      friend std::ostream& operator << (std::ostream& oStream, const Graph&);

      // return the number of vertices in the graph
      int numberOfVertices() const;

      // return true if the graph is connected, false otherwise
      // *****************************************************************
      // CPSC 2150
      // do a breadth first search to determine if the graph is connected
      // *****************************************************************
      bool isConnected() const;                                       

      // return true if the graph has at least one cycle, false otherwise
      bool hasCycle() const;

      // list all the connected components using the output stream oStream
      // each connected component has its vertices in the same line
      // e.g. the output could look like this
      //    0 
      //    1 2 4 5 3 
      //    6 8 7
      // meaning that there is one connected component with only one vertex 0
      // there is another connected component with the vertices 1 2 4 5 3
      // there is another connected component with the vertices 6 8 7
      // precondition:
      //    oStream is an open stream not in a fail state
      void listComponents(std::ostream& oStream) const;

      Graph& operator = (const Graph&);
      Graph(const Graph&);
      ~Graph();

      // determine the path length from the vertex 'from' to the vertex 'to'
      // if 'from' or 'to' are not valid 
      // (i.e. not in the range 0 <= from,to < numberOfVertices() ) return -1
      // return -1 also if there is no path 
      int pathLength(int from, int to) const;


   private:


      // *****************************************************************
      // CPSC 2150
      // use a list of neighbours' graph representation
      // use an array of linked lists

      struct Node {
         int vertex;
         Node *neighbour;
      };

      Node** array; // we need to use an array of pointers.
      // So now each element of an array is a dynamic array of pointers that point towards the node.
      // if we do array[x] we will get a pointer to the node, not the node itself.
      //If we just do Node* array, array[x] will be the node itself not the pointer to a node.
      int vertices;
      
      static Node* deleteList(Node* list);

      void deleteGraph();

      static Node* copyList(const Node* p);

      // take the stream out of the fail state and clean the input buffer
      static void clearClean(std::istream&);

      
   
};

#endif
