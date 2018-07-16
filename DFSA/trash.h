#include <vector>
#include <set>
#include <fstream>
#include "redBlackTree.h"
#include "graph.hpp"

using namespace std;

enum Tongue {Portuguese, Spanish, English};

template<Tongue Type>
class Languages{
private:
  string Initializer(){
    switch (Type) {
      case Portuguese:
        return "aáâãàAÁÂÃÀbBcCdDeéêEÉÊfFgGhHiíIÍjJkKlLmMnNoóôõOÓÔÕpPqQrRsStTuúUÚvVwWxXyYzZ";
      case English:
        return "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";
      case Spanish:
        return "aáAÁbBcCdDeéEÉfFgGhHiíIÍjJkKlLmMnNñÑoóOÓpPqQrRsStTuúUÚvVwWxXyYzZ";
    }
  }
public:
  const string Language;
  Languages(): Language(Initializer()) {}
};

//template<Tongue Type>
class Transductor{
public:
  typedef char Input;
  typedef string Output;
  typedef Graph<Input,Output>::Vertex State;
  typedef Graph<Input,Output>::Edge Transition;
  typedef bool TypeState;
private:

public:
  typedef RedBlackTree<State*> Dictionary;
  typedef vector<State*> Stack_State;

  Transductor(){}
  void Print_Automaton(State *s, std::string tab){
    if(s == nullptr)
      return;
    for(auto it = s->Transitions->begin(); it != s->Transitions->end(); ++it){
      if(it->second._state != nullptr){
        cout<< tab;
        cout << it->first << '\n';
        Print_Automaton(it->second._state, tab+"   ");
      }
    }
  }
  void Create_Minimal_FS_Automaton_for_given_list(string filename){
    MinimalAutomatonStatesDictionary = new Dictionary;
    string CurrentWord, PreviousWord, CurrentOutput, WordSuffix, CommonPrefix;
    string TempString;
    Input c;
    size_t i,PrefixLengthPlus;
    size_t LengthCurrentWord, LengthPreviousWord;
    size_t MAX_WORD_SIZE = Find_Max_Word_Size(filename);
    typename State::MapTransition::iterator TransitionEnd;
    Stack_State TempStates(++MAX_WORD_SIZE);
    ifstream file(filename);
    //Initializing the vector of states
    for (i = 0; i < MAX_WORD_SIZE; ++i)
      TempStates[i] = State::New_State();
    State::Clear_State(TempStates[0]);
    PreviousWord = "";
    while(file>>CurrentWord>>CurrentOutput){
      cout << CurrentWord << " " << CurrentOutput << endl;
      LengthCurrentWord = CurrentWord.size();
      LengthPreviousWord = PreviousWord.size();
      i = 1;
      while((i < LengthCurrentWord) && (i < LengthPreviousWord) && (PreviousWord[i] == CurrentWord[i]))
        ++i;
      PrefixLengthPlus = i;
      for(i = LengthPreviousWord; i >= PrefixLengthPlus; --i)
        State::Set_Transition(TempStates[i-1], PreviousWord[i-1], FindMinimazed(TempStates[i]));
      for(i = PrefixLengthPlus; i <= LengthCurrentWord; ++i){
        State::Clear_State(TempStates[i]);
        State::Set_Transition(TempStates[i-1], CurrentWord[i-1], TempStates[i]);
      }
      if(CurrentWord != PreviousWord){
        State::Set_Final(TempStates[LengthCurrentWord],true);
        State::Set_Output(TempStates[LengthCurrentWord],"");
      }
      for(i = 1; i < PrefixLengthPlus; ++i){
        CommonPrefix = MaxPrefix(State::Output_Transition(TempStates[i-1],CurrentWord[i]),CurrentOutput);
        WordSuffix = RightSuffix(CommonPrefix,State::Output_Transition(TempStates[i-1],CurrentWord[i]));
        State::Set_Output(TempStates[i-1],CurrentWord[i],CommonPrefix);
        TransitionEnd = TempStates[i]->Transitions->end();
        for(auto ptr = TempStates[i]->Transitions->begin(); ptr != TransitionEnd;++ptr){
          c = ptr->first;
          State::Set_Output(TempStates[i],c,Concat(WordSuffix,State::Output_Transition(TempStates[i],c)));
        }
        if(State::isFinal(TempStates[i])){
          typename State::SetOutput* tempSet = new typename State::SetOutput;
          for(auto tempOutput : *(TempStates[i-1]->FinalOutputs))
            tempOutput += WordSuffix;
        }
        CurrentOutput = RightSuffix(CommonPrefix,CurrentOutput);
      }
      if(CurrentWord == PreviousWord)
        TempStates[LengthCurrentWord]->FinalOutputs->insert(CurrentOutput);
      else
        State::Set_Output(TempStates[LengthCurrentWord],CurrentWord[LengthCurrentWord-1],CurrentOutput);
      PreviousWord = CurrentWord;
    }
    for( i = LengthCurrentWord; i >= 1; --i)
      State::Set_Transition(TempStates[i-1], PreviousWord[i-1], FindMinimazed(TempStates[i]));
    InitialState = FindMinimazed(TempStates[0]);
    std::cout << "finish" << '\n';
    file.close();
    Print_Automaton(InitialState, std::string("  "));
  }
private:
  // Variables Members
  /*Languages<Type> Alphabet;*/
  Dictionary* MinimalAutomatonStatesDictionary;
  State* InitialState;
  // Internal Methods Members
  size_t Find_Max_Word_Size(string filename){
    string word,trash;
    size_t MAX_WORD_SIZE = 0;
    ifstream file(filename);
    while(file>>word>>trash)
      if(word.size() > MAX_WORD_SIZE)
        MAX_WORD_SIZE = word.size();
    return MAX_WORD_SIZE;
  }
  void Insert(State *s){ MinimalAutomatonStatesDictionary->addNode(s); }
  State* Member(State *s){
    RBNode<State*> **node;
    bool found = MinimalAutomatonStatesDictionary->findNode(s, node);
    if(found){
      return (*node)->data;
    }
    return nullptr;
  }
  State* FindMinimazed(State *s){
    State* node = Member(s);
    if(!node){
      node = State::Copy_State(s);
      Insert(node);
    }
    return node;
  }
  string Concat(const string &word1, const string &word2){ return word2 + word1; }
  string RightSuffix(const string &word1, const string &word2){
    size_t i = 0;
    while(word1[i] != word2[i]) ++i;
    string suffix = word2.substr(i);
    return suffix;
  }
  string MaxPrefix(const string &word1, const string &word2){
    string prefix;
    size_t i = 0;
    while(word1[i] == word2[i]) ++i;
    prefix = word1.substr(0,i);
    return prefix;
  }
  static State* New_State(){ return new State; }
  static TypeState isFinal(State *&s){ return s->Final; }
  static void Set_Final(State *s, TypeState type){
    if(type == true && s->FinalOutputs == nullptr)
      s->FinalOutputs = new SetOutput;
  }
  static State* Transition(State *&s, Input &c){
    auto it = s->Transitions->find(c);
    if(it != s->Transitions->end())
      return it->second._state;
    return nullptr;
  }
  static void Set_Transition(State *&s1, Input &c, State *s2){ (*s1->Transitions)[c] = Transition("",s2); }
  static State* Copy_State(State *s){
    State* copy = New_State();
    Set_Final(s, s->Final);
    for(auto it = s->Transitions->begin(); it != s->Transitions->end(); ++it)
      (*copy->Transitions)[it->first] = it->second;
    return copy;
  }
  static SetOutput* State_Output(State *&s){
    if(s->Final)
      return s->FinalOutputs;
    return nullptr;
  }
  static void Set_State_Output(State *&s, SetOutput *&o){ s->FinalOutputs = o; }
  static Output Output_Transition(State *&s, Input i){
    auto ptr = s->Transitions->find(i);
    if(ptr != s->Transitions->end())
      return ptr->second._output;
    else
      return "";
  }
  static void Set_Output(State *&s, Input i, Output o){
    auto ptr = s->Transitions->find(i);
    if(ptr != s->Transitions->end())
      ptr->second._output = o;
  }
  static void Set_Output(State *&s, Output o){
    if(isFinal(s))
      s->FinalOutputs->insert(o);
  }
  static void Clear_State(State *s){
    Set_Final(s,false);
    auto TransitionsEnd = s->Transitions->end();
    for(auto it = s->Transitions->begin(); it != TransitionsEnd; ++it){
      Input c = it->first;
      Set_Transition(s,c,nullptr);
    }
  }
};

/*struct Arrival;

class State{
public:
 typedef bool TypeState;
 typedef set<Output> SetOutput;
 typedef map<Input,Arrival> MapTransition;
 SetOutput *FinalOutputs;
 MapTransition *Transitions;
 TypeState Final;
 State(): Final(false), Transitions(new MapTransition), FinalOutputs(nullptr) {}
 State(TypeState boolean): Final(boolean), Transitions(new MapTransition) { Final ? FinalOutputs = new SetOutput : FinalOutputs = nullptr; }
 static State* New_State(){ return new State; }
 static TypeState isFinal(State *&s){ return s->Final; }
 static void Set_Final(State *s, TypeState type){
   if(type == true && s->FinalOutputs == nullptr)
     s->FinalOutputs = new SetOutput;
 }
 static State* Transition(State *&s, Input &c){
   auto it = s->Transitions->find(c);
   if(it != s->Transitions->end())
     return it->second._state;
   return nullptr;
 }
 static void Set_Transition(State *&s1, Input &c, State *s2){ (*s1->Transitions)[c] = Arrival("",s2); }
 static State* Copy_State(State *s){
   State* copy = New_State();
   Set_Final(s, s->Final);
   for(auto it = s->Transitions->begin(); it != s->Transitions->end(); ++it)
     (*copy->Transitions)[it->first] = it->second;
   return copy;
 }
 static SetOutput* State_Output(State *&s){
   if(s->Final)
     return s->FinalOutputs;
   return nullptr;
 }
 static void Set_State_Output(State *&s, SetOutput *&o){ s->FinalOutputs = o; }
 static Output Output_Transition(State *&s, Input i){
   auto ptr = s->Transitions->find(i);
   if(ptr != s->Transitions->end())
     return ptr->second._output;
   else
     return "";
 }
 static void Set_Output(State *&s, Input i, Output o){
   auto ptr = s->Transitions->find(i);
   if(ptr != s->Transitions->end())
     ptr->second._output = o;
 }
 static void Set_Output(State *&s, Output o){
   if(State::isFinal(s))
     s->FinalOutputs->insert(o);
 }
 static void Clear_State(State *s){
   State::Set_Final(s,false);
   auto TransitionsEnd = s->Transitions->end();
   for(auto it = s->Transitions->begin(); it != TransitionsEnd; ++it){
     Input c = it->first;
     State::Set_Transition(s,c,nullptr);
   }
 }
};

struct Arrival{
public:
 State* _state;
 Output _output;
 Arrival(): _state(nullptr), _output("") {}
 Arrival(Output o, State* s): _output(o), _state(s) {}
};*/
