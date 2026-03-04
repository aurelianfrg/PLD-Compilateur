#pragma once

#include <string>

using namespace std;

typedef enum Location {
    REGISTER,
    MEMORY,
    CONSTANT
} Location;

class Address {
    public:
        Address(Location location, string address) : type(location), address(address) {} 
        Address() {} 
        Location type;
        string address;
};

class Symbol {
    public:
        Symbol(string name, int offset, bool affected) : name(name), offset(offset), _affected(affected), _used(false) {}

        string getName() {return this->name;}
        string getType() {return type;}
        int getOffset() {return offset;}
        bool affected() {return _affected;}
        bool used() {return _used;}

        void use() {
            _used = true;
        }
        void affect() {
            _affected = true;
            _used = false;
        }

        string getAdress() {
            return to_string(offset) + "(%rbp)";
        }

        friend ostream & operator << (ostream & ofs, const Symbol & s){
            ofs << "Symbol (name=" << s.name << ", offset=" << s.offset << ", affected=" << s._affected << ", used=" << s._used ;
            return ofs;
        }

    protected:

        string name;
        string type;
        int offset;
        bool _affected;
        bool _used;
};

