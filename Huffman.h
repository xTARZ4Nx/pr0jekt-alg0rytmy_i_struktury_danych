#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <string>
#include "PriorityQueue.h"

// struktura wezla uzywana w drzewie huffmana
struct HuffmanNode {
    unsigned char character;    // znak jaki przechowuje wezel jesli jest lisciem
    int frequency;              // liczba wystapien znaku w tekscie
    HuffmanNode *left, *right;  // wskazniki na lewe i prawe dziecko

    // konstruktor dla liscia czyli wezla koncowego ze znakiem
    HuffmanNode(unsigned char c, int f) : character(c), frequency(f), left(nullptr), right(nullptr) {}
    
    // konstruktor dla wezla wewnetrznego ktory laczy dwa inne
    // nie ma znaku ale ma sume ich czestosci
    HuffmanNode(int f, HuffmanNode* l, HuffmanNode* r) : character(0), frequency(f), left(l), right(r) {}
    
    // funkcja sprawdzajaca czy wezel jest lisciem
    // czyli czy nie ma zadnych dzieci
    bool isLeaf() const {
        return !left && !right;
    }
};

// mala struktura pomocnicza do mapy
// trzyma pary znak i jego kod binarny jako tekst
struct CodeMap {
    unsigned char character; // znak
    std::string code;        // kod np 01010
};

// wlasna prosta implementacja mapy
// zastepuje std map zeby nie uzywac biblioteki standardowej
struct SimpleMap {
    CodeMap* entries; // dynamiczna tablica wpisow
    int size;         // ile aktualnie mamy wpisow
    int capacity;     // jaka jest pojemnosc tablicy

    // konstruktor inicjalizujacy pusta mape
    SimpleMap() {
        capacity = 256; // 256 bo tyle jest znakow ascii
        size = 0; // na start 0 elementow
        entries = new CodeMap[capacity]; // alokacja pamieci
    }

    // destruktor zwalniajacy pamiec
    ~SimpleMap() {
        delete[] entries;
    }

    // funkcja dodajaca lub aktualizujaca wpis
    void add(unsigned char c, std::string code) {
        // szukamy czy znak juz jest
        for(int i=0; i<size; i++) {
            if(entries[i].character == c) {
                entries[i].code = code; // jak jest to podmieniamy kod
                return;
            }
        }
        // jak nie ma i jest miejsce to dodajemy na koniec
        if (size < capacity) {
            entries[size++] = {c, code};
        }
    }

    // funkcja pobierajaca kod dla danego znaku
    std::string get(unsigned char c) {
        // przeszukujemy tablice
        for(int i=0; i<size; i++) {
            if(entries[i].character == c) return entries[i].code; // zwracamy kod
        }
        return ""; // jak nie ma to pusty string
    }
};

// zapowiedzi funkcji ktore sa zaimplementowane w pliku cpp
void deleteTree(HuffmanNode* root);
void generateCodes(HuffmanNode* root, std::string currentCode, SimpleMap& map);

// glowne funkcje sterujace
void compressFile(const std::string& inputFile, const std::string& outputFile);
void decompressFile(const std::string& inputFile, const std::string& outputFile);

// klasa pomocnicza do zapisu bitowego
// normalnie mozna zapisywac tylko bajty czyli 8 bitow
// ta klasa buforuje bity i zapisuje caly bajt jak sie uzbiera
class BitWriter {
    std::ofstream& out; // referencja do pliku
    unsigned char buffer; // bufor na 8 bitow
    int bitCount;         // licznik ile bitow juz mamy

public:
    // konstruktor przypisuje strumien
    BitWriter(std::ofstream& stream) : out(stream), buffer(0), bitCount(0) {}

    // funkcja dodajaca jeden bit do bufora
    void writeBit(int bit) {
        buffer = buffer << 1;   // przesuwamy istniejace bity w lewo
        if (bit) buffer |= 1;   // jesli nowy bit to 1 to ustawiamy najmlodszy bit
        bitCount++; // zwiekszamy licznik

        // jak uzbieralismy 8 bitow to wysylamy do pliku
        if (bitCount == 8) {
            out.put(buffer); // zapis bajtu
            buffer = 0; // zerujemy bufor
            bitCount = 0; // zerujemy licznik
        }
    }

    // funkcja wypychajaca reszte bitow na koncu
    // bo moze zostac np 3 bity w buforze
    void flush() {
        if (bitCount > 0) {
            buffer = buffer << (8 - bitCount); // przesuwamy zeby wyrownac do lewej
            out.put(buffer); // zapisujemy niepelny bajt
        }
    }
};

// klasa pomocnicza do odczytu bitowego
// dziala odwrotnie pobiera bajt z pliku i wydaje po jednym bicie
class BitReader {
    std::ifstream& in; // strumien wejsciowy
    unsigned char buffer; // bufor na aktualny bajt
    int bitCount; // ile bitow jeszcze zostalo w buforze

public:
    // konstruktor
    BitReader(std::ifstream& stream) : in(stream), buffer(0), bitCount(0) {}

    // funkcja zwracajaca kolejny bit 0 lub 1
    int readBit() {
        // jak bufor pusty to czytamy nowy bajt z pliku
        if (bitCount == 0) {
            // probujemy czytac jak sie nie uda to koniec pliku
            if (!in.get(reinterpret_cast<char&>(buffer))) return -1; 
            bitCount = 8; // mamy nowych 8 bitow
        }

        // wyciagamy bit z bufora przesuwajac go
        int bit = (buffer >> (bitCount - 1)) & 1;
        bitCount--; // zmniejszamy licznik
        return bit; // zwracamy wartosc bitu
    }
};

#endif
