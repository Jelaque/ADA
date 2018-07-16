#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "redBlackTree.h"

typedef char symbol;
typedef unsigned int c_size;
typedef std::string ustring;

class State{
public:
  typedef bool typeState;
  typedef std::map<symbol,State*> set_State;
public:
  typeState Final;
  set_State* States;
public:
  State(): States(new set_State){}
  //class Iterator{};
  static State* New_State(){ return new State; }
  static typeState isFinal(State *&s){ return s->Final; }
  static void Set_Final(State *s, typeState type){ s->Final = type; }
  static State* Transition(State *&s, symbol &c){
    auto it = s->States->find(c);
    if(it != s->States->end())
      return it->second;
    return nullptr;
  }
  static void Set_Transition(State *&s1, const symbol &c, State *s2){ (*s1->States)[c] = s2; }
  static State* Copy_State(State *s){
    State* r = New_State();
    Set_Final(s, s->Final);
    for(auto it = s->States->begin(); it != s->States->end(); ++it)
      (*r->States)[it->first] = it->second;
    return r;
  }

};

class DFSA{
public:
  typedef RedBlackTree<State*> dictionary;
  typedef std::vector<State*> Stack_State;
  static ustring alphabet;
  dictionary* MinimalAutomatonStatesDictionary;
  State* InitialState;
public:
  DFSA():MinimalAutomatonStatesDictionary(nullptr), InitialState(nullptr) {}
  void Print_Automaton(State *s, std::string tab){
    if(s == nullptr)
      return;
    for(auto it = s->States->begin(); it != s->States->end(); ++it){
      if(it->second != nullptr){
        std::cout<< tab;
        std::cout << it->first << '\n';
        Print_Automaton(it->second, tab+"   ");
      }
    }
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
      std::cout << "new ";
      node = State::Copy_State(s);
      Insert(node);
    }
    return node;
  }
  void Create_Minimal_FS_Automaton_for_given_list(std::string filename){
    ustring CurrentWord, PreviousWord, trash;
    MinimalAutomatonStatesDictionary = new dictionary;
    c_size MAX_WORD_SIZE = Find_Max_Word_Size(filename);
    std::ifstream file(filename);
    State* TempStates[++MAX_WORD_SIZE];
    for (c_size i = 0; i < MAX_WORD_SIZE; ++i){
      TempStates[i] = State::New_State();
    }
    std::cout << std::endl;
    Clear_State(TempStates[0]);
    PreviousWord = "";
    c_size PrefixLenghtPlus, it;
    c_size LenghtPreviousWord, LenghtCurrentWord;
    while(file >> CurrentWord >> trash){
      it = 1;
      LenghtPreviousWord = PreviousWord.size();
      LenghtCurrentWord = CurrentWord.size();
      while(it < LenghtPreviousWord && it < LenghtCurrentWord && CurrentWord[it-1] == PreviousWord[it-1])
        ++it;
      PrefixLenghtPlus = it;
      for(c_size i = LenghtPreviousWord; i >= PrefixLenghtPlus; --i){
        State::Set_Transition(TempStates[i-1], PreviousWord[i-1], FindMinimazed(TempStates[i]));
        std::cout << CurrentWord[i-1] << ' ';
      }
      for(c_size i = PrefixLenghtPlus; i <= LenghtCurrentWord; ++i){
        Clear_State(TempStates[i]);
        State::Set_Transition(TempStates[i-1], CurrentWord[i-1], TempStates[i]);

      }
      std::cout << std::endl;
      State::Set_Final(TempStates[LenghtCurrentWord], true);
      PreviousWord = CurrentWord;
    }
    for(c_size i = LenghtCurrentWord; i >= 1; --i)
      State::Set_Transition(TempStates[i-1], PreviousWord[i-1], FindMinimazed(TempStates[i]));
    InitialState = FindMinimazed(TempStates[0]);
    std::cout << "finish" << '\n';
    file.close();
    Print_Automaton(InitialState, std::string("  "));
    //MinimalAutomatonStatesDictionary->print();
  }
private:
  bool Find_Forward_Word(Stack_State &S, std::string word, int &sp){
    symbol c = 'a';
    while((c <= 'z') && (State::Transition(S[sp],c) == nullptr)) ++c;
    if(c > 'z') return false;
    S[sp+1] = State::Transition(S[sp++],c);
    word += c;
    while(!State::isFinal(S[sp])){
      c = 'a';
      while(State::Transition(S[sp],c) == nullptr) ++c;
      S[sp+1] = State::Transition(S[sp++],c);
      word += c;
    }
    return true;
  }
  bool Next_Autom_Word(Stack_State &S, std::string word){
    int sp = word.size() - 1;
    symbol c;
    if(Find_Forward_Word(S,word,sp)) return true;
    do{
      if(sp-- == 0) return false;
      c = word[word.size()];
      word = word.substr(0,word.size()-1);
      while((c <= 'z') && (State::Transition(S[sp],c) == nullptr)) ++c;
    }while(c > 'z');
    S[sp+1] = State::Transition(S[sp++],c);
    word += c;
    if(!State::isFinal(S[sp])) return Find_Forward_Word(S,word,sp);
    else return true;
  }
  void Clear_State(State *s){
    State::Set_Final(s,false);
    for(auto c = alphabet.begin(); c <= alphabet.end(); ++c){
      State::Set_Transition(s,*c,nullptr);
    }
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
std::string DFSA::alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ`^´~";
