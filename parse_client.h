#ifndef _PARSE_CLIENT_H
#define _PARSE_CLIENT_H

using namespace std;

class ParseClient
{
    public:
        virtual void parse(ifstream& infile, string command) = 0; 
        virtual void reset() = 0;
        virtual void dump(ifstream& infile) = 0;
};

#endif