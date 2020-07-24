#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "dictionary.h"
#include "redBlackTree.h"

int Compare_States(State* s1, State* s2);

class Transductor{
private:

  // Abreviatures //
  typedef State::typeState            TypeState;
  typedef RedBlackTree<State*>        Dictionary;
  typedef std::vector<State*>         Stack_State;

  // Private class-members //
  State*                              InitialState;
  Dictionary*                         MinimalAutomatonTransitionsDictionary;


public:
  static std::string                  alphabet;
  // Constructor //
  Transductor(): InitialState(nullptr), MinimalAutomatonTransitionsDictionary(nullptr) {}

  // Public class-methods //
  State* Get_InitialState(){ return this->InitialState; }

  //void Print_Automaton(){}
  void Create_Minimal_FS_Automaton_for_given_list(std::string filename){
    MinimalAutomatonTransitionsDictionary = new Dictionary;
    std::string CurrentWord, PreviousWord, CurrentOutput, WordSuffix, CommonPrefix;
    std::string TempString;
    Input c;
    size_t i,PrefixLengthPlus;
    size_t LengthCurrentWord, LengthPreviousWord;
    size_t MAX_WORD_SIZE = Find_Max_Word_Size(filename);
    typename State::mapTransition::iterator TransitionEnd;
    Stack_State TempStates(++MAX_WORD_SIZE);
    std::ifstream file(filename);
    //Initializing the vector of states
    for (i = 0; i < MAX_WORD_SIZE; ++i)
      TempStates[i] = Transductor::New_State();
    Transductor::Clear_State(TempStates[0]);
    PreviousWord = "";
    while(file>>CurrentWord>>CurrentOutput){
      LengthCurrentWord = CurrentWord.size();
      LengthPreviousWord = PreviousWord.size();
      i = 1;
      while((i < LengthCurrentWord) && (i < LengthPreviousWord) && (PreviousWord[i-1] == CurrentWord[i-1]))
        ++i;
      PrefixLengthPlus = i;
      std::cout << "Sizes: " << PrefixLengthPlus << " " << LengthPreviousWord << " " << LengthCurrentWord << '\n';
      for(i = LengthPreviousWord; i >= PrefixLengthPlus; --i){
        Transductor::Set_Transition(TempStates[i-1], PreviousWord[i-1], FindMinimazed(TempStates[i]));
        std::cout << PreviousWord[i-1] << " ";
      }
      for(i = PrefixLengthPlus; i <= LengthCurrentWord; ++i){
        Transductor::Clear_State(TempStates[i]);
        Transductor::Set_Transition(TempStates[i-1], CurrentWord[i-1], TempStates[i]);
      }
      std::cout << '\n';
      Transductor::Set_Final(TempStates[LengthCurrentWord], true);
      PreviousWord = CurrentWord;
    }
    for(i = LengthCurrentWord; i >= 1; --i){
      Set_Transition(TempStates[i-1], PreviousWord[i-1], FindMinimazed(TempStates[i]));
      std::cout << PreviousWord[i-1] << " ";
    }
    InitialState = FindMinimazed(TempStates[0]);
    std::cout << "finish" << '\n';
    file.close();
    Print_Automaton(InitialState, std::string("  "));
  }


private:

  static std::string init_alphabet(){
    std::string cad = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
    //char c = -61;
    //cad += c;
    return cad;
  }
  // Private class-methods for Transitions //
  static State* New_State(){ return new State; }
  static TypeState isFinal(State *&s){ return s->Final; }
  static void Set_Final(State *s, TypeState type){ s->Final = type; }
  static State* Transition(State *&s, Input &c){
    auto it = s->Transitions.find(c);
    if(it != s->Transitions.end())
      return it->second.Next;
    return nullptr;
  }
  static void Set_Transition(State *&s1, const Input c, State *s2){ s1->Transitions[c].Next = s2; }
  static State* Copy_State(State *s){
    State* r = New_State();
    Set_Final(r, s->Final);
    for(auto it = s->Transitions.begin(); it != s->Transitions.end(); ++it)
      r->Transitions[it->first] = it->second;
    return r;
  }
  static void Clear_State(State *s){
    Set_Final(s,false);
    auto end = s->Transitions.end();
    for(auto c = s->Transitions.begin(); c != end; ++c)
      s->Transitions.erase(c);
  }
  /*static SetOutput* State_Output(State *&s){
    if(s->Final)
      return s->FinalOutputs;
    return nullptr;
  }
  static void Set_State_Output(State *&s, SetOutput *&o){ s->FinalOutputs = o; }*/
  static Output Output_Transition(State *&s, Input i){
    auto ptr = s->Transitions.find(i);
    if(ptr != s->Transitions.end())
      return ptr->second.Equivalent;
    else
      return "";
  }
  static void Set_Output(State *&s, Input i, Output o){
    auto ptr = s->Transitions.find(i);
    if(ptr != s->Transitions.end())
      ptr->second.Equivalent = o;
  }
  static int Compare_States(State* s1, State* s2){
    if (Transductor::isFinal(s1) < Transductor::isFinal(s2)) return 0;
    else if (Transductor::isFinal(s1) > Transductor::isFinal(s2)) return 1;
    auto end = Transductor::alphabet.end();
    for(auto c = Transductor::alphabet.begin(); c != end; ++c){
      if(Transductor::Transition(s1,*c) < Transductor::Transition(s2,*c)) return 0;
      else if(Transductor::Transition(s1,*c) > Transductor::Transition(s2,*c)) return 1;
    }
    return -1;
  }
  /*static void Set_Output(State *&s, Output o){
    if(isFinal(s))
      s->FinalOutputs->insert(o);
  }*/
  // Private class-methods for Dictionary //
  void Print_Automaton(State *s, std::string tab){
    if(s == nullptr)
      return;
    for(auto it = s->Transitions.begin(); it != s->Transitions.end(); ++it){
      if(it->second.Next != nullptr){
        std::cout << tab;
        std::cout << it->first << '\n';
        Print_Automaton(it->second.Next, tab+"   ");
      }
    }
  }
  void Insert(State *s){ MinimalAutomatonTransitionsDictionary->addNode(s); }
  State* Member(State *s){
    RBNode<State*> *node;
    node = MinimalAutomatonTransitionsDictionary->root;
    if(node){
      int side = Transductor::Compare_States(s,node->data);
      std::cout << side << " ";
      if(side == -1){
        std::cout << "minimized ";
        return node->data;
      }
      node = node->child[side];
    }
    return nullptr;
  }
  State* FindMinimazed(State *s){
    State* node = Member(s);
    if(!node){
      std::cout << "new ";
      node = Copy_State(s);
      Insert(node);
    }
    return node;
  }

  // Private methods for operation between states
  Output Concat(const Output &word1, const Output &word2){ return word2 + word1; }
  Output RightSuffix(const Output &word1, const Output &word2){
    size_t i = 0;
    while(word1[i] != word2[i]) ++i;
    Output suffix = word2.substr(i);
    return suffix;
  }
  Output MaxPrefix(const Output &word1, const Output &word2){
    Output prefix;
    size_t i = 0;
    while(word1[i] == word2[i]) ++i;
    prefix = word1.substr(0,i);
    return prefix;
  }
  size_t Find_Max_Word_Size(std::string filename){
    std::string word,trash;
    size_t MAX_WORD_SIZE = 0;
    std::ifstream file(filename);
    while(file>>word>>trash)
      if(word.size() > MAX_WORD_SIZE)
        MAX_WORD_SIZE = word.size();
    return MAX_WORD_SIZE;
  }
};
std::string Transductor::alphabet = Transductor::init_alphabet();

/*if(CurrentWord != PreviousWord){
  Set_Final(TempStates[LengthCurrentWord],true);
  Set_Output(TempStates[LengthCurrentWord],"");
}
for(i = 1; i < PrefixLengthPlus; ++i){
  CommonPrefix = MaxPrefix(Output_Transition(TempStates[i-1],CurrentWord[i]),CurrentOutput);
  WordSuffix = RightSuffix(CommonPrefix,Output_Transition(TempStates[i-1],CurrentWord[i]));
  Set_Output(TempStates[i-1],CurrentWord[i],CommonPrefix);
  TransitionEnd = TempStates[i]->Transitions.end();
  for(auto ptr = TempStates[i]->Transitions.begin(); ptr != TransitionEnd;++ptr){
    c = ptr->first;
    State::Set_Output(TempStates[i],c,Concat(WordSuffix,Output_Transition(TempStates[i],c)));
  }
  if(isFinal(TempStates[i])){
    typename State::SetOutput* tempSet = new typename State::SetOutput;
    for(auto tempOutput : *(TempStates[i-1]->FinalOutputs))
      tempOutput += WordSuffix;
  }
  CurrentOutput = RightSuffix(CommonPrefix,CurrentOutput);
}
if(CurrentWord == PreviousWord)
  TempStates[LengthCurrentWord]->FinalOutputs->insert(CurrentOutput);
else
  Set_Output(TempStates[LengthCurrentWord],CurrentWord[LengthCurrentWord-1],CurrentOutput);

}*/
