#include <fstream>
#include <iostream>
#include <string>
#include "states.h"
typedef unsigned int IndexE;
//struct State
template<class T>
class NodeE{
  public:
    NodeE<T>* p_row;
    NodeE<T>* p_column;
    T data;
    State* row;
    State* column;
    NodeE(T d, State* r, State* c): data(d), p_row(0), p_column(0) {row = r; column = c;}
};
template<class T>
class SparseMatrix{
  private:
    NodeE<T>** array_row;
    NodeE<T>** array_column;
    IndexE rows;
    IndexE columns;
  public:
    SparseMatrix(IndexE r, IndexE c): rows(r), columns(c){
      array_row = new NodeE<T>*[rows];
      array_column = new NodeE<T>*[columns];
      for(IndexE j = 0; j < rows; ++j)
        array_row[j] = 0;
      for(IndexE i = 0; i < columns; ++i)
        array_column[i] = 0;
    }
    /*SparseMatrix(char * filename, IndexE r, IndexE c):rows(r), columns(c){ //constructor para construir la matriz esparsa de el archivo txt
      array_row = new NodeE<T>*[rows];
      array_column = new NodeE<T>*[columns];
      for(IndexE j = 0; j < rows; ++j)
        array_row[j] = 0;
      for(IndexE i = 0; i < columns; ++i)
        array_column[i] = 0;
      IndexE i,j;
      std::string value;
      std::ifstream file(filename);
      for(i = 0; i < rows; ++i){
        for(j = 0; j < columns; ++j){
          file >> value;
          if(value == "1")
            this->add(1,i,j);
        }
      }
    }*/
    void add(T, State*, State*);
    bool findRow(IndexE, IndexE, NodeE<T>**&);
    bool findColumn(IndexE, IndexE, NodeE<T>**&);
    void print(std::ostream &);
};
template<class T>
bool SparseMatrix<T>::findRow(IndexE r, IndexE c, NodeE<T>**& p){
  p = &(array_column[c]);
  while(*p){
    if ((*(*p)->row) == r) return true;
    if ((*(*p)->row) > r) return false;
    p = &((*p)->p_row);
  }
  return false;
}
template<class T>
bool SparseMatrix<T>::findColumn(IndexE r, IndexE c, NodeE<T>**& p){
  p = &(array_row[r]);
  while(*p){
    if ((*(*p)->column) == c) return true;
    if ((*(*p)->column) > c) return false;
    p = &((*p)->p_column);
  }
  return false;
}
template<class T>
void SparseMatrix<T>::add(T d, State* r, State* c){
  NodeE<T>** p_c;
  bool ifc= findColumn(r->ID, c->ID , p_c);
  NodeE<T>** p_r;
  bool ifr = findRow(r->ID, c->ID , p_r);
  if(!ifc){
    NodeE<T>* new_node = new NodeE<T>(d, r, c);
    new_node->p_column = *p_c;
    new_node->p_row = *p_r;
    *p_c = *p_r = new_node;
  }
}
template<class T>
void SparseMatrix<T>::print(std::ostream &o){
  NodeE<T> ** ptr;
  int i,j;
  for (i = 0; i < rows; ++i){
    o << '\n';
    for(j = 0; j < columns; ++j)
      if(findRow(i,j,ptr))
        o << (*ptr)->data << " ";
      else
        o << "0 ";
  }
}
