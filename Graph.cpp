// Student Name: Harsh Rana

#include <iostream>
#include <limits> // for declaration of 'numeric_limits' for in
#include <ios>    // for declaration of 'streamsize' for in
#include <queue>
#include <stack>
#include "Graph.h"

/**
 * Constructs an empty graph
 */
Graph::Graph() {
   array = nullptr;
   vertices = 0;
}

/**
 * POSTCONDITION: Returns the number of vertices in the graph. returns 0 if graph is empty.
 */
int Graph::numberOfVertices() const {
   return vertices;
}


/**
 * get stream out of the fail state and clear the buffer
 */
void Graph::clearClean(std::istream& in) {
   in.clear();
   in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   
}

/**
 * deletes the provided linked list.
 * POSTCONDITION: All the elements of the liost are deleted and a nullptr is returned.
 */
Graph::Node* Graph::deleteList(Node* list){
   if(list == nullptr){
      return nullptr;
   }
   Node* toBeRemoved = list;
   list = list->neighbour;
   delete toBeRemoved;
   toBeRemoved = nullptr;
   return deleteList(list);
}

/**
 * Deletes the provided graPH.
 * POSTCONDITION: Memory is returned to the free store. Graph becomes empty
 */
void Graph::deleteGraph(){
   
   if(array == nullptr){
      return;
   }
   for(int i = 0; i < vertices; i++){
      array[i] = deleteList(array[i]);
   }
   delete [] array;
   array = nullptr;
   vertices = 0;
}

/**
 * read the graph by first deleting the current graph
 * PRECONDITION: The number of ints input must be odd.
 * POSTCONDITION: A graph is created from the provided input stream.
 */
std::istream& operator >> (std::istream& in, Graph& graph) {
   
   graph.deleteGraph();


   in >> graph.vertices;
   const int CONST_VERTICES = graph.vertices;

   graph.array = new Graph::Node*[CONST_VERTICES];

   for(int i = 0; i < CONST_VERTICES; i++){
      graph.array[i] = nullptr;
   }// nullptr for no neighbours


   int vertex1 = 0;
   int neighbour1 = 0;

   while(in >> vertex1 && in >> neighbour1){// inserting at front because it doesn't matter.
   if(vertex1 != neighbour1){ // so that user doesn't do 0,0;
      graph.array[vertex1] = new Graph::Node{neighbour1, graph.array[vertex1]};
      graph.array[neighbour1] = new Graph::Node{vertex1, graph.array[neighbour1]};
   }
   }

   return in;
}

/**
 * outputs the graph
 * POSTCONDITION: An output stream is returned which contains data of the graph.
 */
std::ostream& operator << (std::ostream& out, const Graph& graph) {

   Graph::Node* nodeInHand;

   for(int i = 0; i< graph.vertices; i++){
      out << "vertex " << i << ": edges -";
      nodeInHand = graph.array[i];
      if(nodeInHand == nullptr){
         out << " none";
      }
      while(nodeInHand != nullptr){
         out << "( " << i <<" , " << nodeInHand->vertex << " ) ";
         nodeInHand = nodeInHand->neighbour;
      }
      out << "\n";
   }
   
   return out;
}

/**
 * Determines if all the vertices of a graph are connected or not.
 * POSTCONDITION: Returns true if all the vertices are connected, else returns false.
 */
bool Graph::isConnected() const {
   if(array == nullptr){
      return false;
   }
   const int CONST_VERTICES = vertices;
   bool* visited = new bool [CONST_VERTICES]; // array to keep track of visited nodes.

   for(int i = 0; i < CONST_VERTICES; i++){
      visited[i] = false;
   }// we have our array to keep track of our visited nodes.

   std::queue<int> q; // here is our q to traverse.

   q.push(0);// we will start from the beginning.
   visited[0] = true;
   // 0 can't be nullptr;
   Node* nodeInHand = array[0];

   while(!(q.empty())){
      // std::cout << q.front() << " pichu " << q.back() << "\n";
      
      if(nodeInHand == nullptr){// means no or no more neighbours
         q.pop();// restart the loop or exit.
         if(!(q.empty())){ // only when the q is not empty
            nodeInHand = array[q.front()]; // when we pop, we have a new node in hand
         }
      }
      else{
         if(!(visited[nodeInHand->vertex])){
            q.push(nodeInHand->vertex);
            visited[nodeInHand->vertex] = true;
         }
         nodeInHand = nodeInHand->neighbour;
      }
   }// out visited array is now filled correspondingly.

   for(int i = 0; i<CONST_VERTICES; i++){
      if(visited[i] == false){
         delete [] visited; // return memory
         return false;// if any element is not visited, return false
      }
   }// will exit when all elements are visited.

   delete [] visited;
   return true;
 
}

/**
 * Determines if the graph has verices forming a cycle or not.
 * POSTCONDITION: Returns true if there is atleast 1 cycle present, else returns false.
 */
bool Graph::hasCycle() const {
   // A cycle exists when we keep moving forward but somehow still endup revisiting a same vertex.
   // as if 1,0 are neighbours, array[0] will have 1. and array[1] will have 0; so we have to make sure that we don't keep going
   // back and forth.
   if(array == nullptr){
      return false;
   }

   const int CONST_VERTICES = vertices;
   int* cameFrom = new int [CONST_VERTICES];

   for(int i = 0; i < CONST_VERTICES; i++){
      cameFrom[i] = -2;
   } // we aren't traversing now so we didn't reach at any vertex

   Node* nodeInHand = nullptr;
   std::queue<int> q;

   for(int i = 0; i<CONST_VERTICES; i++){ // to check for each unvisited vertex;

      if(cameFrom[i] == -2){// we pick the element that hasn't been reached from anywhere.
         cameFrom[i] = -1; // we are starting from this vertex, so we have considered it
         nodeInHand = array[i]; //node in hand is the neighbour of the vertex we are starting from.
         q.push(i);

         while(!(q.empty())){

            if(nodeInHand == nullptr){

               q.pop();

               if(!(q.empty())){
                  nodeInHand = array[q.front()];
               }
            }
            else{

               if(cameFrom[nodeInHand->vertex] == -2){// never visited
                  cameFrom[nodeInHand->vertex] = q.front();
                  q.push(nodeInHand->vertex);
               }
               else if(cameFrom[nodeInHand->vertex] > -2 && cameFrom[q.front()] != nodeInHand->vertex){// This condition is satisfied 
               // if we reach at a visited element from a different path
                  delete [] cameFrom;
                  return true; // when we reach a visited elemrnt from a different path, means we went through a cycle
               }// if none of the conditions satisfy, it means the nodeinhand.vertex was the vertex we came from. just go fwd.
               nodeInHand = nodeInHand->neighbour;
            }
         }

      }

   }
   // means we checked all the vertices but still couldn't discover a cycle.
   delete [] cameFrom;
   return false;

}

/**
 * outputs the connected components in a line.
 * POSTCONDITION: Returns an output stream that has all the connected nodes together. Sets seperated by lines
 */
void Graph::listComponents(std::ostream& out) const {
   if(array == nullptr){
      out << "Oops! your graph is empty \n";
   }

   const int CONST_VERTICES = vertices;
   bool* visited = new bool [CONST_VERTICES]; // array to keep track of visited nodes.

   for(int i = 0; i < CONST_VERTICES; i++){
      visited[i] = false;
   }// we have our array to keep track of our visited nodes.

   Node* nodeInHand = nullptr;
   std::queue<int> q;

   for(int i = 0; i<CONST_VERTICES; i++){ // to check for each unvisited vertex;

      if(visited[i] == false){// we pick the element that hasn't been reached from anywhere.
         visited[i] = true; // we are starting from this vertex, so we have considered it
         nodeInHand = array[i]; //node in hand is the neighbour of the vertex we are starting from.
         q.push(i);

         while(!(q.empty())){

            if(nodeInHand == nullptr){
               out << q.front() << " ";
               q.pop();

               if(!(q.empty())){
                  nodeInHand = array[q.front()];
               }
            }
            else{
               if(!(visited[nodeInHand->vertex])){
               q.push(nodeInHand->vertex);
               visited[nodeInHand->vertex] = true;
               }
               nodeInHand = nodeInHand->neighbour;
            }
         }
         out << "\n";

      }
   }
   delete [] visited; // no longer in need.
}

/**
 * make a copy of the linked list p and returnS it
 * POSTCONDITION: returns a reference to the head of the copy created.
 */
Graph::Node* Graph::copyList(const Node* p){
    if(p == nullptr){
        return nullptr;
    }
    return new Node{p->vertex,copyList(p->neighbour)};
}

/**
 * POSTCONDITION:deep copy of the rtSide was made
 */    
Graph& Graph::operator = (const Graph& rtSide) {

   vertices = rtSide.vertices;
   const int CONST_VERTICES = vertices;

   array = new Graph::Node*[CONST_VERTICES];

   if(this != &rtSide){
      this->deleteGraph();
   }

   for(int i = 0; i < vertices; i++){
      array[i] = copyList(rtSide.array[i]);
   }

   vertices = rtSide.vertices;
   return *this;
}
  
/**
 * other is the Graph we are copying from
 * POSTCONDITION: the instance variables are initialized using other
 */
Graph::Graph(const Graph& other) {
   
   vertices = other.vertices;
   const int CONST_VERTICES = vertices;

   array = new Graph::Node*[CONST_VERTICES];

   for(int i = 0; i < vertices; i++){
      array[i] = copyList(other.array[i]);
   }

   vertices = other.vertices;
   
}


/**
 * destructor frees up the memory on the heap
 */
Graph::~Graph() {
   this->deleteGraph();
}

/**
 * Finds a path between two given vertices
 * POSTCONDITION: Returns an int that is equivalent to path length between 2 vertices. Returns -1 if no path exists.
 */
int Graph::pathLength(int from, int to) const {
// So, we keep moving forward, we go to an elemen't other neighbour if the one in hand is visited.
// if the one in hand is not visited, we go to its neighbours.
// if there are no neighbours or all the neighbours are visited, then we backtrack.
   if(array == nullptr || to >= vertices){
      return -1;
   }
   if(to == from){
      return 0;
   }
   const int CONST_VERTICES = vertices;
   bool* visited = new bool [CONST_VERTICES]; // array to keep track of visited nodes.

   for(int i = 0; i < CONST_VERTICES; i++){
      visited[i] = false;
   }// we have our array to keep track of our visited nodes.

   std::stack<int> q; // here is our q to traverse.

   q.push(from);// we will start from the beginning.
   visited[from] = true;
   // 0 can't be nullptr;
   Node* nodeInHand = array[0];

   while(!(q.empty())){
      // std::cout << q.front() << " pichu " << q.back() << "\n";
      
      if(nodeInHand == nullptr){// means no or no more neighbours
         q.pop();
         // restart the loop or exit.
         if(!(q.empty())){ // only when the q is not empty
            nodeInHand = array[q.top()]; // when we pop, we have a new node in hand
         }
      }
      else{
         if(nodeInHand->vertex == to){
            delete [] visited;
            return q.size();
         }
         if(!(visited[nodeInHand->vertex])){
            q.push(nodeInHand->vertex);
            visited[nodeInHand->vertex] = true;
            nodeInHand = array[nodeInHand->vertex];
         }
         else{
            nodeInHand = nodeInHand->neighbour;
         }
         
      }
   }// out visited array is now filled correspondingly.
   delete [] visited;
   return -1;
 
}
