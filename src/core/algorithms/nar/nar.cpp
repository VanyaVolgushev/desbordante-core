#include "nar.h"

namespace model {

NAR::NAR() {
    // TODO Auto-generated constructor stub

}

NAR::~NAR() {
    // TODO Auto-generated destructor stub
}

/**
 * Print a particular association rule.
 *
 * @param no input parameters.
 * @return no return code.
 */
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

/**
 * Write the association rule into output file in specific format.
 *
 * @param the output file.
 * @return no return code.
 */
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

} // namespace model