struct State{
  unsigned int ID;
  enum TypeState {
    Final,
    Initial,
    Normal
  };
  TypeState Type;
  State(unsigned int id): ID(id){(id == 0) ? Type = Initial : Type = Normal;}
  State(unsigned int id, TypeState t): ID(id), Type(t){}
  bool operator<(const State &o){return this->ID < o.ID;}
  bool operator>(const State &o){return this->ID > o.ID;}
  bool operator==(const State &o){return this->ID == o.ID;}
  bool operator!=(const State &o){return this->ID != o.ID;}
};
