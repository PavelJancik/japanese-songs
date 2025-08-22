#include "kanji.h"

const int KANJICHARSUM = 2137;
const int JUKUGOSUM = 52792;

Kanji::Kanji(){
     kanji="";
     strokes="";
     frequence="";
     nameOfIncludedRadical="";
     onAndKunReadings="";
     onReading="";
     onTranslation="";
     kunReading="";
     kunTranslation="";
     jukugo="";
     jukugoReading="";
     jukugoTranslation="";
}

void Kanji::readCharacter(int row) {
    // ifstream nevidi resources, proto je musime prekopirovat do docasne slozky z <QTemporaryDir>
    QTemporaryDir temporaryDir;
    QFile::copy(":/kanji_database/Database/Kanji.csv", temporaryDir.path() + "Kanji.csv");
    std::ifstream input(QString(temporaryDir.path() + "Kanji.csv").toLatin1().data());

    if (!input.is_open()) throw invalid_argument("nepodarilo se otevri soubor Kanji.csv v readCharacter (Kanji)");

    string line;

    for(int i=1; i<row; i++) getline(input, line); // nastavi cteni na prislusny radek

    char byte;

    if(row>KANJICHARSUM) invalid_argument("Presah pres velikost souboru ve funkci readCharacter (Kanji)");

    try {
        // id (nikam neukladam)
        readNextLinePart(input);
        // kanji
        this->kanji = readNextLinePart(input);
        // strokes
        this->strokes = readNextLinePart(input);
        // radical name
        this->nameOfIncludedRadical = readNextLinePart(input);
        // radical frequency (nikam neukladam)
        readNextLinePart(input);
        // On a Kun readings (v ramci Joyo)
        this->onAndKunReadings = readNextLinePart(input);
        // On a Kun readings (mimo Joyo)
        this->onAndKunReadings += readNextLinePart(input);
        // On reading
        this->onReading = readNextLinePart(input);
        // On translation
        this->onTranslation = readNextLinePart(input);
        // Kun reading
        this->kunReading = readNextLinePart(input);
        // Kun translation
        this->kunTranslation = readNextLinePart(input);
        // frekvence vyskytu - nekonci '|', proto musime precist zvlast
        byte = input.get();
        while (byte != '\n' and byte != -1) { // '\n' pro konec radku, -1 pro konec souboru
            this->frequence += byte;
            byte = input.get();
        }
    } catch (exception &e){
        throw invalid_argument("Vyjimka pri zpracovani Kanji znaku z databaze v readCharacter (Kanji): ");
    }
}

string Kanji::readNextLinePart(ifstream &input) {
    string partOfTheLine;
    char byte;
    byte = input.get();
    while (byte != '|') {
        if (byte != 9) partOfTheLine += byte; // if() odstrani tabulatory (v ascii kodu 9)
        byte = input.get();
    }

    return partOfTheLine;
}


bool Kanji::setKanji(string kanji) {
    // ifstream nevidi resources, proto je musime prekopirovat do docasne slozky z <QTemporaryDir>
    QTemporaryDir temporaryDir;
    QFile::copy(":/kanji_database/Database/Kanji.csv", temporaryDir.path() + "Kanji.csv");
    std::ifstream input(QString(temporaryDir.path() + "Kanji.csv").toLatin1().data());

    if (!input.is_open()) throw invalid_argument("nepodarilo se otevri soubor Kanji.csv v getKanji (Kanji)");

    int row=1;
    string databaseKanji;
    // prohleda soubor s Kanji a najde shodu
    while (kanji != databaseKanji and row<KANJICHARSUM){
        row++;
        getline(input, databaseKanji); // 1. cuklus - precte prvni radek (zbavi se zahlavi), dalsi cykly - precte zbytek radku z predchoziho cyklu
        readNextLinePart(input); // prvni sloupec
        databaseKanji = readNextLinePart(input); // druhou sloupec, zbytek nepotrebuju
    }

    if (row==KANJICHARSUM) return false; // kanji NOT FOUND
    else readCharacter(row); // bylo nalezeno
    return true;
}

bool Kanji::setJukugo(string jukugo) {
    // ifstream nevidi resources, proto je musime prekopirovat do docasne slozky z <QTemporaryDir>
    QTemporaryDir temporaryDir;
    QFile::copy(":/kanji_database/Database/Jukugo.csv", temporaryDir.path() + "Jukugo.csv");
    std::ifstream input(QString(temporaryDir.path() + "Jukugo.csv").toLatin1().data());

    if (!input.is_open()) throw invalid_argument("nepodarilo se otevri soubor Jukugo.csv v getJukugo (Kanji)");
    string databaseJukugo;
    // prohleda soubor s Jukugo a najde shodu
    int i=1;
    while (jukugo != databaseJukugo and i<JUKUGOSUM){
        getline(input, databaseJukugo); // 1. cuklus - precte prvni radek (zbavi se zahlavi), dalsi cykly - precte zbytek radku z predchoziho cyklu
        readNextLinePart(input); // prvni sloupec
        databaseJukugo = readNextLinePart(input); // druhou sloupec, zbytek nepotrebuju
        i++;
    }
    if (i==JUKUGOSUM) return false; // jukugo se nenaslo (protoze se nejednalo o jukugo ale o dva samostatne kaji)
    else {
        readNextLinePart(input); // 3. sloupec nepotrebuju
        readNextLinePart(input); // 4. sloupec nepotrebuju
        this->jukugo = databaseJukugo;
        this->jukugoReading = readNextLinePart(input);
        this->jukugoTranslation = readNextLinePart(input);
    }
    return true;
}
