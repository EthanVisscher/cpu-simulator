#ifndef _PARSE_CLIENT_H
#define _PARSE_CLIENT_H

using namespace std;

class ParseClient
{
    public:
        // parse commands from given file
        virtual void parse(ifstream& infile) = 0; 
};

#endif