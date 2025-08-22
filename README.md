# Japanese Songs

<p align="center">
   <img src="/screenshot.png" alt="Japanese songs" width="400"/>
</p>

A desktop application displaying selected Japanese songs lyrics with inbuild Kanji translator. Built with C++ and Qt.

## Features

- Browse and search a collection of Japanese songs
- View lyrics, kanji, and translations
- Play associated media files (audio/video)
- Manage a dictionary and kanji database

## Project Structure

- `main.cpp`, `mainwindow.cpp`, `mainwindow.h`, `mainwindow.ui`: Main application window and logic
- `dictionary.cpp`, `dictionary.h`: Dictionary management
- `kanji.cpp`, `kanji.h`: Kanji database management
- `song.cpp`, `song.h`: Song data structures
- `Database/`: Contains dictionary, kanji, and source data files
- `Songs/`: Contains song lyrics, media, and images
- `Textures/`: (Optional) Contains UI textures or images

## Requirements

- Qt 5 or 6 (Qt Widgets)
- C++ compiler (GCC, Clang, MSVC, etc.)

## Building

1. Open `JapaneseSongs.pro` in Qt Creator, or run:
   ```bash
   qmake JapaneseSongs.pro
   make
   ```
2. Run the generated executable.

## License

See [LICENCE.md](LICENCE.md) for license information.
