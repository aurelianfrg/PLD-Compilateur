#pragma once

#include <string>

using namespace std;

class Variable {
    public:
        Variable(string name, int line) : name(name), line(line), _used(false) {}

        string getName() {return this->name;}
        int getLine(){return this -> line;}
        bool used() {return _used;}

        void use() {
            _used = true;
        }

        friend ostream & operator << (ostream & ofs, const Variable & s){
            ofs << "Variable (name=" << s.name << ", used=" << s._used ;
            return ofs;
        }

    protected:

        string name;
        int line;
        bool _used;
};

