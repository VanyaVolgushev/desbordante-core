#include "nar.h"

namespace algos {

/*
void NAR::print()
{
    cout << "NAR printout: fitness= " << fitness << ", support= " << support <<
         ", confidence= " << confidence  << ", inclusion= " << inclusion << ", ante={";
    for(set<string>::iterator it=ante.begin();it != ante.end();++it) {
        cout << *it << ",";
    }
    cout << "}, cons={";
    for(set<string>::iterator it=cons.begin();it != cons.end();++it) {
        cout << *it << ",";
    }
    cout << "}" << endl;
}

void NAR::write(ofstream &out)
{
    out << fitness << " [";
    for(set<string>::iterator it=ante.begin();it != ante.end();++it) {
        if(it != ante.begin())
            out << ",";
        out << "\'" << *it << "\'";
    }
    out << "]===>[";
    for(set<string>::iterator it=cons.begin();it != cons.end();++it) {
        if(it != cons.begin())
            out << ",";
        out << "\'" << *it << "\'";
    }
    out << "] " << support << " " << confidence << endl;
}
*/
} // namespace model