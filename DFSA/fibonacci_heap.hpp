#include<iostream>
#include<math.h>
#define white 0
#define black 1
template<class T>
class FNode{
  typedef FNode<T>* f_node;
  public:
    T data;
    bool marked;
    int degree;
    f_node next;
    f_node back;
    f_node father;
    f_node child;
  public:
    FNode(T _data): degree(0), data(_data), next(0), back(0), father(0), child(0), marked(white) {}
};
typedef size_t len_list;
template<class T>
class Fibonacci_Heap{
  typedef FNode<T>* f_node;
  private:
    f_node min;
    len_list len;
    void Link_nodes(f_node, f_node);
    void Exchange(f_node, f_node);
    void Cut(f_node, f_node);
    void Cascading_Cut(f_node);
    int root_list();
  public:
    Fibonacci_Heap(): min(0), len(0) {}
    void Insert(T);
    f_node Minimun();
    T& Extract_Min();
    void Decrease_Key(f_node, T);
    void Consolidate();
    size_t size();
    void Delete_Key(f_node, T);
};
template<class T>
size_t Fibonacci_Heap<T>::size(){return len;}
template<class T>
int Fibonacci_Heap<T>::root_list(){
  int c = 0;
  FNode<T>* temp = min;
  if(temp){
    ++c;
    temp = temp->next;
    while(temp!=min){
      ++c;
      temp = temp->next;
    }
  }
  return c;
}
template<class T>
T& Fibonacci_Heap<T>::Extract_Min(){
  FNode<T>* s_min = min;
  if(min->next != min){
    if (min->child){
      min->child->back->next = min->next;
      min->child->back = min->back;
      min->back->next = min->child;
      min->next->back = min->child->back;
    }
    else{
      min->back->next = min->next;
      min->next->back = min->back;
    }
    min = min->next;
  }
  else
    min = min->child;
  if (min)
    min->father = NULL;
  if(len>0)
    --len;
  Consolidate();
  return s_min->data;
}
template<class T>
FNode<T>* Fibonacci_Heap<T>::Minimun() {return min;}
template<class T>
void Fibonacci_Heap<T>::Insert(T x){
  FNode<T>* new_node = new FNode<T>(x);
  if(!min){
    min = new_node;
    min->next = min->back = min;
  }
  else{
    new_node->next = min;
    new_node->back = min->back;
    min->back->next = new_node;
    min->back = new_node;
    if (new_node->data < min->data)
      min = new_node;
  }
  ++len;
}
template<class T>
void Fibonacci_Heap<T>::Link_nodes(FNode<T>* k_min, FNode<T>* k_max){
  k_max->back->next = k_min;
  k_min->back = k_max->back;
  if (!k_min->child)
    k_max->back = k_max->next = k_max;
  else{
    k_max->next = k_min->child;
    k_min->child->back->next = k_max;
    k_max->back = k_min->child->back;
    k_min->child->back = k_max;
  }
  k_min->child = k_max;
  k_max->father = k_min;
  k_max->marked = white;
}
template<class T>
void Fibonacci_Heap<T>::Exchange(FNode<T>* n1, FNode<T>* n2){
  if (n1 == min)
    min = n2;
  std::swap(n1->data, n2->data);
  FNode<T>* temp = n1->child;
  n1->child = n2->child;
  if(n1->child)
    n1->child->father = n1;
  n2->child = temp;
  if(n2->child)
    n2->child->father = n2;
}
template<class T>
void Fibonacci_Heap<T>::Consolidate(){
  len_list size = 0;
  if (len!=0)
    size = int(ceil(log2(len)))+1;
  FNode<T>* container[size];
  int i = 0;
  for(; i < size; ++i)
    container[i] = NULL;
  FNode<T>* cur = min;
  FNode<T>* s_cur = NULL;
  int d;
  int it = root_list();
  for(i = 0; i < it; ++i){
    d = cur->degree;
    while(container[d]){
      s_cur = container[d];
      if (s_cur->data < cur->data)
        Exchange(s_cur, cur);
      Link_nodes(cur, s_cur);
      ++cur->degree;
      container[d] = NULL;
      ++d;
    }
    container[d] = cur;
    cur = cur->next;
  }
  min = NULL;
  for(i = 0, cur = container[i]; i < size; ++i){
    if (container[i])
      if (min == NULL || container[i]->data < min->data)
        min = container[i];
  }
}
template<class T>
void Fibonacci_Heap<T>::Decrease_Key(FNode<T>* node, T x){
  if (x <= node->data)
    node->data = x;
  else
    return;
  FNode<T>* parent = node->father;
  if (parent != NULL && node->data < parent->data){
    Cut(node,parent);
    Cascading_Cut(parent);
  }
  if(x < min->data)
    min = node;
}
template<class T>
void Fibonacci_Heap<T>::Cut(FNode<T>* cnode, FNode<T>* pnode){
  if (cnode->next == cnode){
    pnode->child = NULL;
    pnode->degree -= (cnode->degree+1);
  }
  else{
    cnode->next->back = cnode->back;
    cnode->back->next = cnode->next;
    if (cnode == pnode->child){
      pnode->child = cnode->next;
    }
  }
  cnode->next = min;
  cnode->back = min->back;
  min->back->next = cnode;
  min->back = cnode;
  cnode->father = NULL;
  cnode->marked = white;
}
template<class T>
void Fibonacci_Heap<T>::Cascading_Cut(FNode<T>* node){
  FNode<T>* parent = node->father;
  if (parent){
    if (node->marked == white)
      node->marked = black;
    else{
      Cut(node,parent);
      Cascading_Cut(parent);
    }
  }
}
template<class T>
void Fibonacci_Heap<T>::Delete_Key(FNode<T>* node, T){
  Decrease_Key(node, Minimun()->data-1);
  Extract_Min();
}
