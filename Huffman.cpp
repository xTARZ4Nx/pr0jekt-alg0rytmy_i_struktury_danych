#include "Huffman.h"
#include <iostream>

// funkcja do usuwania drzewa z pamieci
// zeby nie bylo wyciekow pamieci jak juz nie potrzebujemy drzewa
void deleteTree(HuffmanNode* root) {
    if (!root) return; // jak wezel jest pusty to nic nie robimy i wracamy
    deleteTree(root->left); // idziemy rekurencyjnie usunac lewe poddrzewo
    deleteTree(root->right); // idziemy rekurencyjnie usunac prawe poddrzewo
    delete root; // na koncu usuwamy sam wezel z pamieci
}

// funkcja generujaca kody binarne dla znakow
// przechodzimy cale drzewo i zapisujemy sciezke jako zera i jedynki
void generateCodes(HuffmanNode* root, std::string currentCode, SimpleMap& map) {
    if (!root) return; // jak wezel pusty to wracamy

    if (root->isLeaf()) { // sprawdzamy czy to lisc czyli koniec galezi
        // jak tak to znalezlismy znak i przypisujemy mu wygenerowany kod
        map.add(root->character, currentCode); // dodajemy do naszej mapy
    }

    // wywolujemy rekurencyjnie dla lewego dziecka dopisujac 0 do kodu
    generateCodes(root->left, currentCode + "0", map);
    // wywolujemy rekurencyjnie dla prawego dziecka dopisujac 1 do kodu
    generateCodes(root->right, currentCode + "1", map);
}

// glowna funkcja do kompresji pliku
// bierze plik wejsciowy i zapisuje skompresowany do wyjsciowego
void compressFile(const std::string& inputFile, const std::string& outputFile) {
    // otwieramy plik do odczytu w trybie binarnym
    std::ifstream in(inputFile, std::ios::binary);
    // sprawdzamy czy udalo sie otworzyc
    if (!in.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wejsciowego: " << inputFile << "\n"; // wypisujemy blad
        return; // konczymy dzialanie funkcji
    }

    // tablica do zliczania czestosci znakow ascii jest ich 256
    int frequencies[256] = {0}; // zerujemy tablice na start
    unsigned char c; // zmienna na wczytywany znak
    long long totalChars = 0; // licznik wszystkich znakow w pliku
    
    char readBuf; // bufor do czytania z pliku
    // petla czytajaca plik znak po znaku
    while (in.get(readBuf)) {
        frequencies[(unsigned char)readBuf]++; // zwiekszamy licznik dla danego znaku
        totalChars++; // zwiekszamy ogolny licznik znakow
    }
    
    // czyscimy flagi bledow strumienia bo doszlismy do konca pliku
    in.clear(); 
    // cofamy sie na poczatek pliku zeby go pozniej znowu przeczytac
    in.seekg(0); 

    // sprawdzamy czy plik nie byl pusty
    if (totalChars == 0) {
        std::cout << "Plik jest pusty.\n"; // informujemy uzytkownika
        return; // konczymy
    }

    std::cout << "Wczytano " << totalChars << " znakow. Budowanie drzewa...\n"; // info dla usera

    // tworzymy kolejke priorytetowa na wskazniki do wezlow
    MinPriorityQueue<HuffmanNode*> pq(256);
    // przelatujemy przez wszystkie mozliwe znaki ascii
    for (int i = 0; i < 256; i++) {
        // jesli znak wystapil chociaz raz
        if (frequencies[i] > 0) {
            // tworzymy nowy wezel lisc i dodajemy go do kolejki
            // priorytetem jest czestosc wystepowania
            pq.insert(new HuffmanNode((unsigned char)i, frequencies[i]), frequencies[i]);
        }
    }

    // budujemy drzewo huffmana laczac wezly
    // robimy to dopoki w kolejce nie zostanie tylko jeden element czyli korzen
    while (pq.size() > 1) {
        HuffmanNode* left = pq.extractMin(); // pobieramy wezel o najmniejszej czestosci
        HuffmanNode* right = pq.extractMin(); // pobieramy drugi najmniejszy
        
        // tworzymy nowy wezel rodzica ktory laczy te dwa
        // jego czestosc to suma czestosci dzieci
        HuffmanNode* parent = new HuffmanNode(left->frequency + right->frequency, left, right);
        // wrzucamy rodzica z powrotem do kolejki
        pq.insert(parent, parent->frequency);
    }

    // wyciagamy ostatni element ktory jest korzeniem calego drzewa
    HuffmanNode* root = pq.extractMin();

    // tworzymy mape do przechowywania kodow
    SimpleMap codes;
    // generujemy kody przechodzac przez drzewo
    generateCodes(root, "", codes);

    std::cout << "Wygenerowano kody. Zapisywanie...\n"; // info

    // otwieramy plik wyjsciowy do zapisu w trybie binarnym
    std::ofstream out(outputFile, std::ios::binary);
    
    // zapisujemy calkowita liczbe znakow w naglowku
    // zeby przy dekompresji wiedziec ile bitow czytac
    out << totalChars << "\n";
    
    // zapisujemy rozmiar slownika czyli ile mamy wpisow
    out << codes.size << "\n";
    // petla zapisujaca slownik do pliku
    for(int i=0; i<codes.size; i++) {
        // zapisujemy kod ascii znaku jako liczbe
        out << (int)codes.entries[i].character << " " << codes.entries[i].code;
        
        // dodatek zeby wyswietlac litery w pliku jak czlowiek
        // sprawdzam czy znak nie jest bialym znakiem (jak spacja, enter)
        // wypisuje wszystko co ma kod wiekszy niz 32
        // dzieki temu polskie znaki tez beda widoczne jako komentarz
        if (codes.entries[i].character > 32) {
            // dopisuje komentarz z ta litera
            out << " //" << (char)codes.entries[i].character;
        }
        
        out << "\n"; // nowa linia
    }

    // tworzymy obiekt bitwriter do zapisywania bitow
    BitWriter bw(out);
    
    // czytamy plik wejsciowy jeszcze raz od poczatku
    while(in.get(readBuf)) {
        unsigned char uc = (unsigned char)readBuf; // rzutujemy na unsigned char
        std::string code = codes.get(uc); // pobieramy kod binarny dla znaku
        // przelatujemy przez stringa z zerami i jedynkami
        for(char bitChar : code) {
            // konwertujemy znak '0' lub '1' na liczbe i zapisujemy
            bw.writeBit(bitChar - '0');
        }
    }
    // zapisujemy to co zostalo w buforze na koniec
    bw.flush();

    std::cout << "Kompresja zakonczona. Zapisano do " << outputFile << "\n"; // sukces
    
    // sprzatamy pamiec usuwajac drzewo
    deleteTree(root);
}

// funkcja do dekompresji pliku
void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    // otwieramy plik skompresowany
    std::ifstream in(inputFile, std::ios::binary);
    // sprawdzamy czy istnieje
    if (!in.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku: " << inputFile << "\n";
        return;
    }

    std::cout << "Otwieranie pliku " << inputFile << "...\n";

    // czytamy z naglowka ile ma byc wszystkich znakow po odkodowaniu
    long long totalChars;
    if (!(in >> totalChars)) {
        std::cerr << "Blad odczytu naglowka (totalChars).\n"; // blad jak sie nie da
        return;
    }
    
    // czytamy ile wpisow ma slownik
    int dictSize;
    if (!(in >> dictSize)) {
        std::cerr << "Blad odczytu naglowka (dictSize).\n"; // blad
        return;
    }
    
    // musimy pominac znak nowej linii ktory zostal po wczytaniu liczby
    char temp; 
    in.get(temp);

    std::cout << "Odtwarzanie drzewa (" << dictSize << " wpisow)...\n"; // info

    // tworzymy korzen nowego drzewa
    HuffmanNode* root = new HuffmanNode(0, 0); 

    // petla wczytujaca slownik i budujaca drzewo
    for(int i=0; i<dictSize; i++) {
        int charCode; // zmienna na kod ascii
        std::string codeStr; // zmienna na kod binarny
        
        in >> charCode >> codeStr; // wczytujemy pare z pliku
        
        // wazne musimy zignorowac reszte linii bo moga byc tam komentarze z literami
        // uzywam getline zeby wczytac smieci do konca linii i przejsc do nowej
        std::string dummy;
        std::getline(in, dummy);
        
        unsigned char c = (unsigned char)charCode; // zamieniamy liczbe na znak
        
        // zaczynamy od korzenia i idziemy w dol
        HuffmanNode* curr = root;
        // dla kazdego znaku w kodzie binarnym
        for(char bit : codeStr) {
            if (bit == '0') { // jak 0 to idziemy w lewo
                if (!curr->left) curr->left = new HuffmanNode(0, 0); // tworzymy wezel jak nie ma
                curr = curr->left; // przechodzimy
            } else { // jak 1 to idziemy w prawo
                if (!curr->right) curr->right = new HuffmanNode(0, 0); // tworzymy
                curr = curr->right; // przechodzimy
            }
        }
        // jak doszlismy do konca kodu to zapisujemy znak w lisciu
        curr->character = c; 
    }
    
    // tutaj usunalem in.get(temp) bo getline w petli wyzej juz zjada enter
    // wiec jestesmy gotowi do czytania danych binarnych
 

    // otwieramy plik wyjsciowy do zapisu odzyskanego tekstu
    std::ofstream out(outputFile, std::ios::binary);
    // tworzymy bitreader do czytania bitow
    BitReader br(in);
    
    HuffmanNode* curr = root; // wskaznik do chodzenia po drzewie
    long long charsDecoded = 0; // licznik odkodowanych znakow

    std::cout << "Dekodowanie tresci...\n";

    // petla dziala dopoki nie odzyskamy wszystkich znakow
    while (charsDecoded < totalChars) {
        int bit = br.readBit(); // czytamy jeden bit
        if (bit == -1) { // jak koniec pliku to przerywamy
            std::cerr << "Nieoczekiwany koniec pliku! Odczytano " << charsDecoded << " z " << totalChars << " znakow.\n";
            break; 
        }

        // idziemy w lewo lub prawo zaleznie od bitu
        if (bit == 0) curr = curr->left;
        else curr = curr->right;

        // zabezpieczenie jakby drzewo bylo uszkodzone
        if (!curr) {
             std::cerr << "Blad struktury drzewa/sciezki!\n";
             break;
        }

        // sprawdzamy czy to lisc
        if (curr->isLeaf()) {
            out.put(curr->character); // zapisujemy odzyskany znak
            charsDecoded++; // zwiekszamy licznik
            curr = root; // wracamy do korzenia zeby szukac nastepnego znaku
        }
    }

    std::cout << "Dekompresja zakonczona. Zapisano do " << outputFile << "\n"; // sukces
    // sprzatamy pamiec
    deleteTree(root);
}
