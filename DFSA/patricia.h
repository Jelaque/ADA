#include<fstream>
#include<list>
#include<string>
#include<iostream>
using namespace std;
typedef int posicion;
class Patricia{
  class PatriciaNode{
  public:
    typedef PatriciaNode Pnode;
    Pnode* right;
    Pnode* left;
    Pnode* middle;
    string word;
    bool end_word;
    list<streampos> directions;
    PatriciaNode(string w, int d, bool mark = true): word(w), end_word(mark) {directions.push_back(d);}
    PatriciaNode(string w, list<streampos>& d, bool mark = true): word(w), directions(d) ,end_word(mark) {}
    void set_mark(bool value) {end_word = value;}
    bool is_marked() {return end_word;}
  };
  typedef PatriciaNode Pnode;
private:
  Pnode* root;
  char* file;
  char* output;
public:
  Patricia(): root(0), output("extract.txt") {}
  Patricia(char* textfile): root(0), file(textfile), output("extract.txt") {indexFile(textfile);}
  void indexFile(char * textfile){
    ifstream file(textfile);
    string word;
    posicion pos = 0;//comienza desde la primera palabra
    while(file >> word){
      insert(word, pos);
      //cout << word << " " << pos << endl;
      pos = file.tellg()+1;//se le suma el espacio
    }
  }
  void insert(string newword, posicion dir){
    Pnode** p = &root;
    string auxiliar;
    while(*p){
      if(newword.empty())
        return;
      if (comparison((*p)->word, newword, auxiliar)){
        if(auxiliar.empty()){//si solo hubo sobrante de la nueva palabra, sera insertada en el medio
          if(newword.empty()){//en caso sea otra aparicion de una palabra, pues en comparison no hubo sobrantes
            if (!((*p)->is_marked()))
              (*p)->set_mark(true);
            (*p)->directions.push_back(dir);
            return;
          }
        }
        else{//se debe crear un nuevo nodo entre el nodo actual y su hijo, para luego redirigir la porcion que sobra
          Pnode* child = (*p)->middle;
          list<streampos> list_copy = (*p)->directions;
          Pnode* newchild = new Pnode(auxiliar, list_copy);
          (*p)->directions.clear(); //make p node unmarked
          (*p)->set_mark(false);    //make p node unmarked
          (*p)->middle = newchild;
          newchild->middle = child;
          auxiliar.clear();//limpia las instancias en el nodo no marcado
        }
        //cout << (*p)->word << endl;
        p = &(*p)->middle;
      }
      else if ((*p)->word[0] > newword[0])
        p = &(*p)->left;
      else
        p = &(*p)->right;
    }
    (*p) = new Pnode(newword, dir);
  }
  bool comparison(string& a, string& b, string& c){
    int pos = 0;
    for (; pos < a.size(); ++pos){
      if(a[pos] != b[pos])
        break;
    }
    if (pos == 0)// si no hubo un match
      return false;
    string sub = a;
    //repartir los sobrantes
    b = b.substr(pos);
    c = sub.substr(pos);
    a = sub.substr(0,pos);
    return true;
  }
};
