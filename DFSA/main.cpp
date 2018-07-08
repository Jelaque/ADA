//#include "dictionary.h"
#include "Transductor.h"

main(){
  /*DFSA *A = new DFSA;
  A->InitialState;
  A->Create_Minimal_FS_Automaton_for_given_list("words.txt");
  */
  Transductor *A = new Transductor;
  A->Create_Minimal_FS_Automaton_for_given_list("words.txt");
  std::cout << "hello world" << '\n';
}
