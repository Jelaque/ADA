#include<iostream>
#define black 0
#define red 1
template<class T>
class RBNode{
  public:
    T data;
    bool color;
    RBNode<T>* child[2];
  public:
    RBNode(){child[0] = nullptr; child[1] = nullptr; color = 1;}
    RBNode(T d) {data = d; child[0] = nullptr; child[1] = nullptr; color = 1;}
};
template<class T>
class RedBlackTree{
  public:
    RBNode<T>* root;
    void printRecursive(RBNode<T>*node);
  public:
    RedBlackTree(): root(0) {}
    bool findNode(T d, RBNode<T>**&p);
    bool addNode(T d);
    bool deleteNode(T d);
    RBNode<T>* getFather(T d);
    RBNode<T>* getBrother(T d);
    RBNode<T>* getUncle(T d);
    RBNode<T>* getGrandparent(T d);
    void repair_tree(RBNode<T> *&c_node);
    void print();
    void case_1(RBNode<T> *&c_node);
    void case_2();
    void case_3(RBNode<T> *&c_node);
    void case_4(RBNode<T> *&c_node);
    void case_4step2(RBNode<T> *&c_node);
    void rotate1(RBNode<T> *&node, RBNode<T> *&grandparent, bool side);
    void rotate2(RBNode<T> *&node, RBNode<T> *&grandparent, bool side);
};
template<class T>
bool RedBlackTree<T>::findNode(T d, RBNode<T>**&p){
  p=&root;
  while(*p){
    if((*p)->data==d) return true;
    if((*p)->data>d) p=&((*p)->child[0]);
    else p=&((*p)->child[1]);
  }
  return false;
}
template<class T>
bool RedBlackTree<T>::addNode(T d){
  RBNode<T>**p;
  if(findNode(d,p)) return false;
  *p = new RBNode<T>(d);
  repair_tree(*p);
  //std::cout << "repair_tree" << '\n';
  return true;
}
template<class T>
RBNode<T>* RedBlackTree<T>::getFather(T d){
  RBNode<T>**p=&root;
  RBNode<T>* dad=*p;
  if(*p)
    if((*p)->data==d)
      return 0;
  while(*p){
    if((*p)->data==d){
      return dad;
    }
    dad=*p;
    if((*p)->data>d)
      p=&((*p)->child[0]);
    else
      p=&((*p)->child[1]);
  }
  return NULL;
}
template<class T>
RBNode<T>* RedBlackTree<T>::getBrother(T d){
  RBNode<T>* node=getFather(d);
  if(node){
    if (node->data>d)
      node=node->child[1];
    else
      node=node->child[0];
  }
  return node;
}
template<class T>
RBNode<T>* RedBlackTree<T>::getUncle(T d){
  RBNode<T>* node=getFather(d);
  if (node){
    return getBrother(node->data);
  }
}
template<class T>
RBNode<T>* RedBlackTree<T>::getGrandparent(T d){
  RBNode<T>* node=getFather(d);
  if (node){
    return getFather(node->data);
  }
}
template<class T>
void RedBlackTree<T>::print(){
  RBNode<T>* cur=this->root;
  printRecursive(cur);
}
template<class T>
void RedBlackTree<T>::printRecursive(RBNode<T>*node){
  if(node){
    printRecursive(node->child[0]);
    std::cout<<node->data<<" color:"<<node->color<<" ";
    printRecursive(node->child[1]);
  }
}
template<class T>
void RedBlackTree<T>::repair_tree(RBNode<T> *&node){
  RBNode<T>* parent = getUncle(node->data);
  if (getFather(node->data)==NULL){
    //std::cout<<"case1"<<std::endl;
    case_1(node);
  }
  else if(getFather(node->data)->color==black){
    //std::cout<<"case2"<<std::endl;
    case_2();
  }
  else if(parent){
    if(getUncle(node->data)->color==red){
      //std::cout<<"case3"<<std::endl;
      case_3(node);
    }
    else{
      //std::cout<<"case4"<<std::endl;
      case_4(node);
    }
  }
  else{
    //std::cout<<"case4"<<std::endl;
    case_4(node);
  }
}
template<class T>
void RedBlackTree<T>::case_1(RBNode<T> *&node){
  node->color=black;
}
template<class T>
void RedBlackTree<T>::case_2(){
  return;
}
template<class T>
void RedBlackTree<T>::case_3(RBNode<T> *&node){
  getFather(node->data)->color=black;
  getUncle(node->data)->color=black;
  RBNode<T>* grandparent = getGrandparent(node->data);
  grandparent->color=red;
  repair_tree(grandparent);
}
template<class T>
void RedBlackTree<T>::rotate1(RBNode<T> *&node, RBNode<T> *&grandparent, bool side){
  RBNode<T>* parent = grandparent->child[side];
  grandparent->child[side] = node;
  parent->child[side] = node->child[!side];
  grandparent->child[side]->child[side] = parent;
}
template<class T>
void RedBlackTree<T>::case_4(RBNode<T> *&node){
  RBNode<T>* grandparent = getGrandparent(node->data);
  if(grandparent->child[0])
    if(grandparent->child[0]->child[1] == node){
      rotate1(node,grandparent,0);
      node=node->child[0];
    }
  else if(grandparent->child[1])
    if (grandparent->child[1]->child[0] == node){
      rotate1(node,grandparent,1);
      node=node->child[1];
    }
  case_4step2(node);
}
template<class T>
void RedBlackTree<T>::rotate2(RBNode<T> *&parent, RBNode<T> *&grandparent, bool side){
  //std::cout << "rotate2" << '\n';
  RBNode<T> *p = grandparent->child[!side];
  std::swap(parent->data,grandparent->data);
  grandparent->child[!side] = grandparent->child[side];
  grandparent->child[side] = parent->child[side];
  parent->child[side] = parent->child[!side];
  parent->child[!side] = p;
}
template<class T>
void RedBlackTree<T>::case_4step2(RBNode<T>*&node){
  //std::cout << "case4_step2" << '\n';
  RBNode<T>* grandparent = getGrandparent(node->data);
  RBNode<T>* parent = getFather(node->data);
  if (parent->child[0] == node)
    rotate2(parent,grandparent,0);
  else
    rotate2(parent,grandparent,1);
  grandparent->color=black;
  parent->color=red;
}
