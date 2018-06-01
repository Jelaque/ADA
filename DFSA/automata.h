#include "sparse_matrix.h"
#include <vector>
#include <string>
typedef char symbol;

////////////////////////////
//File Automata           //
//A = a0, a1, a2, ... ,an //
//q0                      //
//F = f0, f1, ... , fn    //
//f(S,A) = S              //
// .                      //
// .                      //
//f(S,A) = fn             //
////////////////////////////

class Automata{
private:
  SparseMatrix<symbol>* Matrix;// contain the functions
  std::vector<char> Alphabet;//alphabet of the automata
  // Index 0 is always initial state
  // [0..n], for n x n matrix is the set of states
  std::vector<State*> States; //set for states
  //final states set in constructor
  //tupla - 5
public:
  Automata(std::vector<char> a, IndexE n, std::vector<IndexE> f){
    Alphabet = a;
    Matrix = new SparseMatrix<symbol>(n,n);
    States.push_back(new State(0,State::Initial));
    for(unsigned int i = 1; i < n ;++i)
      States.push_back(new State(i,State::Normal));
    for(auto it = f.begin(); it != f.end(); ++it)
      States[*it]->Type = State::Final;
  }
  void addFuncion(IndexE from, symbol character, IndexE to){
    this->Matrix->add(character, States[from], States[to]);
  }
  bool ReadWord(std::string word){
    
  }
  ~Automata(){delete this->Matrix;}
};
