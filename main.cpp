#include <iostream>
#include <string>
#include "Huffman.h"
#include "PriorityQueue.h"

// funkcja obslugujaca menu dla kolejki priorytetowej
// pozwala uzytkownikowi bawic sie kolejka
void runPriorityQueueDemo() {
    MinPriorityQueue<int> pq; // tworzymy nowa pusta kolejke na liczby
    int choice; // zmienna na wybor opcji
    int val, prio; // zmienne na wartosc i priorytet

    // nieskonczona petla menu
    while (true) {
        // wypisujemy dostepne opcje
        std::cout << "\n--- DEMO KOLEJKI PRIORYTETOWEJ ---\n";
        std::cout << "1. Dodaj (insert) - Wstaw element z priorytetem\n";
        std::cout << "2. Pobierz min (extract_min) - Usun i pokaz element o najmniejszym priorytecie\n";
        std::cout << "3. Zmniejsz priorytet (decrease_key) - Zmien priorytet istniejacego elementu\n";
        std::cout << "4. Sprawdz czy pusta (is_empty)\n";
        std::cout << "5. Wyswietl kolejke (stan wewnetrzny)\n";
        std::cout << "6. Zbuduj z tablicy (build) - Algorytm Floyda O(N)\n";
        std::cout << "0. Powrot\n";
        std::cout << "Wybierz: ";
        
        // pobieramy wybor i sprawdzamy czy to liczba
        if (!(std::cin >> choice)) {
            std::cin.clear(); // czyscimy bledy
            std::cin.ignore(10000, '\n'); // ignorujemy bledne wejscie
            continue; // wracamy do poczatku petli
        }

        // blok try catch zeby wylapac bledy np pusta kolejka
        try {
            switch (choice) {
                case 1: // dodawanie
                    std::cout << "Podaj wartosc (int): ";
                    std::cin >> val;
                    std::cout << "Podaj priorytet (int, nizszy = wazniejszy): ";
                    std::cin >> prio;
                    pq.insert(val, prio); // wstawiamy do kolejki
                    std::cout << "Dodano.\n";
                    break;
                case 2: // pobieranie min
                    if (pq.isEmpty()) std::cout << "Kolejka pusta.\n";
                    else std::cout << "Pobrano: " << pq.extractMin() << "\n"; // wyciagamy i wypisujemy
                    break;
                case 3: // zmiana priorytetu
                    std::cout << "Podaj wartosc elementu do zmiany: ";
                    std::cin >> val;
                    std::cout << "Podaj nowy (nizszy) priorytet: ";
                    std::cin >> prio;
                    // probujemy zmienic jak sie uda to info
                    if (pq.decreaseKey(val, prio)) std::cout << "Zmieniono priorytet.\n";
                    else std::cout << "Nie znaleziono elementu lub nowy priorytet jest wyzszy.\n";
                    break;
                case 4: // sprawdzanie pustosci
                    std::cout << (pq.isEmpty() ? "Pusta" : "Nie pusta") << "\n";
                    break;
                case 5: // wypisywanie calosci
                    pq.printQueue();
                    break;
                case 6: // budowanie z tablicy
                    {
                        std::cout << "Podaj ilosc elementow: ";
                        int n;
                        std::cin >> n;
                        if (n <= 0) break;
                        
                        // tworzymy tablice dynamiczne
                        int* vals = new int[n];
                        int* prios = new int[n];
                        
                        // wczytujemy elementy
                        for(int i=0; i<n; i++) {
                            std::cout << "Element " << i+1 << " (wartosc priorytet): ";
                            std::cin >> vals[i] >> prios[i];
                        }
                        
                        // odpalamy szybkie budowanie
                        pq.build(vals, prios, n);
                        std::cout << "Zbudowano kolejke metoda O(N).\n";
                        
                        // sprzatamy tablice tymczasowe
                        delete[] vals;
                        delete[] prios;
                    }
                    break;
                case 0: // wyjscie
                    return;
                default: // nieznana opcja
                    std::cout << "Nieznana opcja.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Blad: " << e.what() << "\n"; // wypisujemy blad
        }
    }
}

// funkcja pomocnicza do uruchamiania kompresji
void runHuffmanCompression() {
    std::string inFile, outFile;
    std::cout << "Podaj nazwe pliku do kompresji (np. dane.txt): ";
    std::cin >> inFile; // wczytujemy nazwy plikow
    std::cout << "Podaj nazwe pliku wyjsciowego (np. skompresowany.bin): ";
    std::cin >> outFile;
    
    // wywolujemy wlasciwa funkcje kompresujaca
    compressFile(inFile, outFile);
}

// funkcja pomocnicza do uruchamiania dekompresji
void runHuffmanDecompression() {
    std::string inFile, outFile;
    std::cout << "Podaj nazwe pliku do dekompresji (np. skompresowany.bin): ";
    std::cin >> inFile;
    std::cout << "Podaj nazwe pliku wyjsciowego (np. odzyskany.txt): ";
    std::cin >> outFile;

    // wywolujemy wlasciwa funkcje dekompresujaca
    decompressFile(inFile, outFile);
}

// glowna funkcja programu main
int main() {
    // glowna petla programu
    while (true) {
        // wyswietlamy glowne menu
        std::cout << "\n=== MENU GLOWNE ===\n";
        std::cout << "1. Testowanie Kolejki Priorytetowej (Min-Heap)\n";
        std::cout << "2. Kompresja pliku (Huffman)\n";
        std::cout << "3. Dekompresja pliku (Huffman)\n";
        std::cout << "0. Wyjscie\n";
        std::cout << "Wybierz opcje: ";

        int choice;
        // wczytujemy wybor i sprawdzamy poprawnosc
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        // obsluga wyboru uzytkownika
        switch (choice) {
            case 1:
                runPriorityQueueDemo(); // idziemy do demo kolejki
                break;
            case 2:
                runHuffmanCompression(); // idziemy do kompresji
                break;
            case 3:
                runHuffmanDecompression(); // idziemy do dekompresji
                break;
            case 0:
                std::cout << "Koniec programu.\n"; // konczymy
                return 0;
            default:
                std::cout << "Nieznana opcja.\n";
        }
    }
    return 0; // standardowe zakonczenie main
}
