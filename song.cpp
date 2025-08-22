#include "song.h"


Song::Song(string songArtist, string songTitle, string fileBaseName) {
    artist = songArtist;
    title = songTitle;
    setLyrics(fileBaseName);
}

//// nacteni ze souboru do vektoru
void Song::setLyrics(string fileBaseName) {
    // ifstream nevidi resources, proto je musime prekopirovat do docasne slozky z <QTemporaryDir>
    QTemporaryDir temporaryDir;
    QFile::copy(":/lyrics/Songs/" + QString::fromStdString(fileBaseName) + ".txt", temporaryDir.path() + QString::fromStdString(fileBaseName) + ".txt");
    std::ifstream input(QString(temporaryDir.path() + QString::fromStdString(fileBaseName) + ".txt").toLatin1().data());

    if(!input.is_open()) throw invalid_argument("nepodarilo se otevri soubor s lyrics ve funkci setLyrics (Song)");

    string line="";
    while(getline(input, line)) japanese.push_back(line);
}

string Song::getTitle() {
    return artist + " - " + title;
}

vector<Kanji*> Song::getTranslation(string input){
    vector<Kanji*> kanjiVector;
    vector<string> charArray = parseString(input);
    for (unsigned long int i=0; i<charArray.size(); i++){
        if (charArray[i] >= "一") { // tato podminka zahodi vse co neni kanji nebo jukugo
            Kanji *kanji = new Kanji();
            if (i+1 != charArray.size() and charArray[i+1] >= "一") { // 2 kanji znaky po sobe -> jukugo (u posledniho znaku netestujeme)
                string jukugo = charArray[i] + charArray[i+1];
                // nasledujici if: pokud jde o jukogo tak se nasetuje do instance kanji (pri vyhodnoceni podminky),
                // i++ preskoci nasledujici znak protoze ten jsme zahrnuli v jukugo
                // pokud nejde o jukugo (v textu jsou 3 kanji znaky po sobe a prvni 2 z nich nejsou jukugo) tak nasetuju jednoznakove kanji
                // a vycountruju i++ pomoci i--
                if (!kanji->setJukugo(jukugo)){
                    kanji->setKanji(charArray[i]);
                    i--;
                }
                i++;
            }
            else kanji->setKanji(charArray[i]);
            kanjiVector.push_back(kanji);
        }
    }
    return kanjiVector;
}

string Song::getLyrics(){
    string updatedLyrics; // lyrics doplnene o html znaky, ktere vyznacuji Kanji znaky + jejich style
    for (unsigned long int i=0; i<japanese.size(); i++){
        string updatedLine="";
        // konverze jednoho radku po znacich do pole (1 JAP znak = 3 Byte; proto je vektor stringu)
        vector<string> charArray = parseString(japanese[i]);
        // vyznaceni Kanji
        for (unsigned long int i=0; i<charArray.size(); i++){
            if (charArray[i] >= "一") { // je to Kanji (znak '一' je prvni Kanji dle unicode (0x4e00))
              updatedLine = updatedLine + "<span style=\"color: red\">" + charArray[i] + "</span>";
            } else updatedLine += charArray[i]; // je to kana nebo anglictina
        }
        updatedLine += "<br>";
        updatedLyrics += updatedLine;
    }
    return updatedLyrics;
}

//// konverze string na vektor po jednotlivych znacich
vector<string> Song::parseString(string line) {
    vector<string> parsedString;
    string character;
    unsigned long int shift;
    for (unsigned long int i=0; i<line.length(); i+=shift){
        // vsechny japonske znaky maji v kazdem bajtu bit s nejvyssi hodnotou 1
        if (line.substr(i,1)<"z") { // znaky po 'z' maji v ascii nejvyssi bit roven 0
            character = line.substr(i,1); // je to z anglicke abecedy (1 Byte)
            shift = 1;
        }
        else {
            character = line.substr(i,3); // je to japonsky znak (3 Bytes)
            shift = 3; // 3 byty protoze zpracovavam soubory v UTF8
            // japonske znaky jsou maximalne na 2 bytech (max. ffef)
            // takze v UTF-8 je jeden byte s informaci na kolika bytech je znak ulozen, a 2 byty ktere nesou samotny znak
            // napriklad znak hito "一" je hexadecimalne 0xff10, tedy v UTF-8 binarne 1110-0000 1111-1111 0001-0000
            // update: tak prej ne, prej to takhle nefunguje
        }        
        parsedString.push_back(character);
    }
    return parsedString;
}

//  Japanese Unicode:
//    Punctuation 3000 - 303f
//    Hiragana 3040 - 309f
//    Katakana 30a0 - 30ff
//    Kanji 4e00 - 9faf
//    Full-width roman characters and half-width katakana ff00 - ffef
//
//    zdroj: http://www.rikai.com/library/kanjitables/kanji_codes.unicode.shtml

