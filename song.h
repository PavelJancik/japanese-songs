#ifndef SONG_H
#define SONG_H

#include <fstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <vector>
#include <QTemporaryDir>
#include "kanji.h"

using namespace std;

class Song
{
    string artist;
    string title;
    vector<string> japanese;
public:
    Song(string songArtist, string songTitle, string fileBaseName);
    string getTitle();
    string getLyrics();
    vector<Kanji*> getTranslation(string input);
private:
    void setLyrics(string fileBaseName);
    vector<string> parseString(string line);
};

#endif // SONG_H


