#include "patricia.h"
#include "automata.h"

main(){
  Patricia* tree = new Patricia;
  tree->insert("animal",0);
  tree->insert("anomalia",0);
  tree->insert("anibal",0);
  tree->insert("ana",0);

  SparseMatrix<char> M(3,3);
  State* a,*b,*c;
  a = new State(0,State::Initial);
  b = new State(1,State::Normal);
  c = new State(2,State::Final);
  M.add('w', a, b);
  M.add('c', b, c);
  M.print(std::cout);
}
