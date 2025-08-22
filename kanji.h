#ifndef KANJI_H
#define KANJI_H

#include <fstream>
#include <iostream>
#include <vector>
#include <QTemporaryDir>

using namespace std;

class Kanji {
public:
    string kanji;
    string strokes;
    string frequence;
    string nameOfIncludedRadical;
    string onAndKunReadings;
    string onReading;
    string onTranslation;
    string kunReading;
    string kunTranslation;
    string jukugo;
    string jukugoReading;
    string jukugoTranslation;
public:
    Kanji();
    bool setKanji(string kanji);
    bool setJukugo(string jukugo);
private:
    void readCharacter(int row);
    string readNextLinePart(ifstream &input);
};

#endif // KANJI_H



