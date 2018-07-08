#include"fibonacci_heap.hpp"
#include<set>
#include<typeinfo>
#include<list>

using namespace std;

enum TypeGraph {DIRECTED};//debido a que la funcion de busqueda en Insert_connected_nodes y Have_cicle no es lo mismo para grafos dirigidos //, UNDIRECTED};

template<class V, class E, TypeGraph T>
class Graph{
  private:
    class Edge;
    //
    class Vertex{
      public:
        mutable V data;
        mutable set<Edge> axes;
        Vertex(V d): data(d) {}
        V getData() const {return data;}
        friend bool operator <(const Vertex& a, const Vertex& b){ return a.data < b.data; }
        friend bool operator >(const Vertex& a, const Vertex& b){ return a.data > b.data; }
        friend bool operator ==(const Vertex& a, const Vertex& b){ return a.data == b.data; }
        friend bool operator <=(const Vertex& a, const Vertex& b){ return a.data <= b.data; }
        friend bool operator >=(const Vertex& a, const Vertex& b){ return a.data >= b.data; }
        friend bool operator !=(const Vertex& a, const Vertex& b){ return a.data != b.data; }
        set<Edge>& getEdges() const {return axes;}
    };
    class Edge{
      public:
        mutable E distance;
        typename set<Vertex>::iterator v_arrival;
        mutable bool undirected;
        Edge(E d, typename set<Vertex>::iterator dir): distance(d), v_arrival(dir), undirected(0) {}
        Edge(E d, typename set<Vertex>::iterator dir, bool b): distance(d), v_arrival(dir), undirected(b) {}
        friend bool operator <(const Edge& a, const Edge& b){ return a.distance < b.distance; }
        friend bool operator >(const Edge& a, const Edge& b){ return a.distance > b.distance; }
        friend bool operator ==(const Edge& a, const Edge& b){ return a.distance == b.distance; }
        friend bool operator <=(const Edge& a, const Edge& b){ return a.distance <= b.distance; }
        friend bool operator >=(const Edge& a, const Edge& b){ return a.distance >= b.distance; }
        friend bool operator !=(const Edge& a, const Edge& b){ return a.distance != b.distance; }
    };
  public:
    typedef typename set<Vertex>::iterator ItVertex;
    typedef set<Vertex> SetVertex;
    typedef set<Edge> SetEdge;
    typedef typename set<Edge>::iterator ItEdge;
    class CompleteEdge{
      private:
        mutable ItVertex v_going;
        mutable ItVertex v_arrival;
        E distance;
      public:
        const Vertex& getVertexGoing() const {return *v_going;}
        const Vertex& getVertexArrival() const {return *v_arrival;}
        ItVertex& getItrGoing() const {return v_arrival;}
        ItVertex& getItrArrival() const {return v_arrival;}
        const E getDistance() {return distance;}
        CompleteEdge(): v_going(0), v_arrival(0) {}
        CompleteEdge(E d, ItVertex dir_g, ItVertex dir_a): v_going(dir_g), v_arrival(dir_a), distance(d) {}
        friend bool operator <(const CompleteEdge& a, const CompleteEdge& b){ return a.distance < b.distance; }
        friend bool operator >(const CompleteEdge& a, const CompleteEdge& b){ return a.distance > b.distance; }
        friend bool operator ==(const CompleteEdge& a, const CompleteEdge& b){ return a.distance == b.distance; }
        friend bool operator <=(const CompleteEdge& a, const CompleteEdge& b){ return a.distance <= b.distance; }
        friend bool operator >=(const CompleteEdge& a, const CompleteEdge& b){ return a.distance >= b.distance; }
        friend bool operator !=(const CompleteEdge& a, const CompleteEdge& b){ return a.distance != b.distance; }
    };
  //Graph
  private:
    size_t n_vertexs;
    size_t n_edges;
  public:
    Graph(): n_vertexs(0), n_edges(0) {}
    void addEdge(const E& d, const V& d1, const V& d2) {
      ItVertex it1 = this->Nodes.find(Vertex(d1));
      ItVertex it2 = this->Nodes.find(Vertex(d2));
      if (it1 != this->Nodes.end() && it2 != this->Nodes.end()){
        it1->getEdges().insert(Edge(d,it2));
        ++n_edges;
      }
      if (typeid(T) == typeid(DIRECTED))
        it2->getEdges().insert(Edge(d,it1,1));
    }
    void addEdge(const E& d, const Vertex& d1, const Vertex& d2) {
      ItVertex it1 = this->Nodes.find(d1);
      ItVertex it2 = this->Nodes.find(d2);
      if (it1 != this->Nodes.end() && it2 != this->Nodes.end()){
        it1->getEdges().insert(Edge(d,it2));
        ++n_edges;
      }
      if (typeid(T) == typeid(DIRECTED))
        it2->getEdges().insert(Edge(d,it1,1));
    }
    void addVertex(const V& value){
      this->Nodes.insert(Vertex(value));
      ++n_vertexs;
    }
    void addVertex(const Vertex& v){
      this->Nodes.insert(v);
      ++n_vertexs;
    }
    size_t sizeEdges() {return n_edges;}
    void deleteVertex();
    void deleteEdge();
    bool findVertex();
    bool findEdge();
    void printVertex(){
      for(ItVertex it = this->Nodes.begin(); it != this->Nodes.end(); ++it)
        cout << "Vertice: " << (*it).data << endl;
    }
    Graph<V, E ,T>* dijkstra();
    Graph<V, E, T>* kruskal(){
      Fibonacci_Heap<CompleteEdge> heap;
      for(ItVertex it = this->Nodes.begin(); it != this->Nodes.end(); ++it)
        Heap_Edges(heap, it);
      Graph<V, E, T>* mst = new Graph<V, E, T>();
      list< set<V> > setConnectedVertex;
      CompleteEdge min_edge;
      while(n_vertexs > mst->sizeEdges() + 1 && heap.size() > 0){
        min_edge = heap.Extract_Min();
        if (!(Have_cicle(min_edge, setConnectedVertex))){
          Insert_connected_nodes(min_edge, setConnectedVertex);
          mst->addVertex(min_edge.getVertexGoing().getData());
          mst->addVertex(min_edge.getVertexArrival().getData());
          mst->addEdge(min_edge.getDistance(), min_edge.getVertexGoing(), min_edge.getVertexArrival());
        }
      }
      return mst;
    }
    Graph<V, E, T>* prim(){
      Graph<V, E, T>* mst = new Graph<V, E, T>();
      list< set<V> > setConnectedVertex;
      Fibonacci_Heap<CompleteEdge> heap;
      CompleteEdge min_edge;
      ItVertex it = this->Nodes.begin();
      Heap_Edges(heap, it);
      if(n_vertexs > mst->sizeEdges() + 1 && heap.size() > 0){
        min_edge = heap.Extract_Min();
        if (!(Have_cicle(min_edge, setConnectedVertex))){
          Insert_connected_nodes(min_edge, setConnectedVertex);
          mst->addVertex(min_edge.getVertexGoing().getData());
          mst->addVertex(min_edge.getVertexArrival().getData());
          mst->addEdge(min_edge.getDistance(), min_edge.getVertexGoing(), min_edge.getVertexArrival());
          Heap_Edges(heap, min_edge.getItrArrival());
        }
      }
    }
  private:
    set<Vertex> Nodes;
    void Insert_connected_nodes(CompleteEdge &c_edge, list< set<V> >& s_set){
      typename list< set<V> >::iterator it1 = s_set.end();
      typename list< set<V> >::iterator it2 = s_set.end();
      for (typename list< set<V> >::iterator it = s_set.begin(); it != s_set.end(); ++it){
        if(it->find(c_edge.getVertexGoing().getData()) != it->end()) it1 = it;
        if(it->find(c_edge.getVertexArrival().getData()) != it->end()) it2 = it;
      }
      if (it1 != s_set.end() && it2 != s_set.end()){
        typename set<V>::iterator itS = it2->begin();
        for(;itS != (*it2).end() ;++itS)
          it1->insert(*itS);
        s_set.erase(it2);
      }
      else if (it1 != s_set.end())
        it1->insert(c_edge.getVertexArrival().getData());
      else if (it2 != s_set.end())
        it2->insert(c_edge.getVertexGoing().getData());
      else{
        s_set.push_back(set<V>());
        s_set.back().insert(c_edge.getVertexGoing().getData());
        s_set.back().insert(c_edge.getVertexArrival().getData());
      }
    }
    bool Have_cicle(CompleteEdge &c_edge, list< set<V> >& s_set){
      for (typename list< set<V> >::iterator it = s_set.begin(); it != s_set.end(); ++it){
        if(it->find(c_edge.getVertexGoing().getData()) != it->end() && it->find(c_edge.getVertexArrival().getData()) != it->end())
          return true;
      }
      return false;
    }
    void Heap_Edges(Fibonacci_Heap<CompleteEdge>& m_heap, ItVertex& itr){
      ItEdge itE;
        for (itE = itr->getEdges().begin(); itE != itr->getEdges().end(); ++itE)
          if (!(itE->undirected))
            m_heap.Insert(CompleteEdge(itE->distance, itr, itE->v_arrival));
    }
};
