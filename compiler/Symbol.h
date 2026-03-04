#include <string>

using namespace std;

typedef enum TYPE {
    INT // int32
} TYPE;


class Symbol {
    public:
        Symbol(string name, int offset, bool affected) : name(name), offset(offset), _affected(affected), _used(false) {}

        string getName() {return this->name;}
        TYPE getType() {return type;}
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

        friend ostream & operator << (ostream & ofs, const Symbol & s){
            ofs << "Symbol (name=" << s.name << ", offset=" << s.offset << ", affected=" << s._affected << ", used=" << s._used ;
            return ofs;
        }

    protected:

        string name;
        TYPE type;
        int offset;
        bool _affected;
        bool _used;
};

