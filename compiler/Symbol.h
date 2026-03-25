#pragma once

#include <string>
#include <iostream>
#include "Type.h"

using namespace std;

class Symbol
{
public:
  // Symbol(string name, int offset, bool affected) : name(name), offset(offset), _affected(affected), _used(false) {}
  Symbol(Type type, string name, int offset) : type(type), name(name), offset(offset), _affected(false), _used(false) {}

  string getName() const { return this->name; }
  Type getType() const { return type; }
  int getOffset() const { return offset; }
  bool affected() const { return _affected; }
  bool used() const { return _used; }

  void use() { _used = true; }
  void affect()
  {
    _affected = true;
    _used = false;
  }

  string getAdressx86() const
  {
    return to_string(offset) + "(%rbp)";
  }

  string getAdressAarch64() const
  {
    return "[sp, "+to_string(offset)+"]";
  }

  friend ostream &operator<<(ostream &ofs, const Symbol &s)
  {
    ofs << "Symbol (name=" << s.name << ", offset=" << s.offset << ", affected=" << s._affected << ", used=" << s._used;
    return ofs;
  }

protected:
  string name;
  Type type;
  int offset;
  bool _affected;
  bool _used;
};
