#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <fstream>
#include <QTemporaryDir>

using namespace std;

class Dictionary
{
public:
    Dictionary();
private:
    void find(string line);
};

#endif // DICTIONARY_H
