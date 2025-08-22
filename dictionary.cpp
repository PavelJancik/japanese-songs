#include "dictionary.h"

Dictionary::Dictionary()
{

}

void Dictionary::find(string line){
    // ifstream nevidi resources, proto je musime prekopirovat do docasne slozky z <QTemporaryDir>
    // todo ten soubor ma 13 MB, budes to muset vyresit nejak jak ty videa
//    QTemporaryDir temporaryDir;
//    QFile::copy(":/kanji_database/Database/Dictionary.txt", temporaryDir.path() + "Dictionary.txt");
//    std::ifstream input(QString(temporaryDir.path() + "Dictionary.txt").toLatin1().data());

    ifstream input("C:\\Users\\jancp\\Desktop\\JapaneseSongs\\JapaneseSongs\\JapaneseSongs\\Database\\Dictionary.txt");
    if (!input.is_open()) throw invalid_argument("nepodarilo se otevri soubor Dictionary.txt ve tride Dictionary");



}
