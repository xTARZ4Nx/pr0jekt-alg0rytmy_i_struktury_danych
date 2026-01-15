# Dokumentacja Projektu: Implementacja Kodowania Huffmana

## Informacje o projekcie
**Temat:** Algorytmy i Struktury Danych - Kodowanie Huffmana
**Język programowania:** C++ (bez użycia bibliotek STL takich jak `vector`, `map`, `priority_queue`)

---

## 1. Wstęp i Analiza Wymagań

Celem projektu było stworzenie aplikacji konsolowej umożliwiającej kompresję i dekompresję plików tekstowych przy użyciu algorytmu Huffmana. Zgodnie z wymaganiami projektowymi, kluczowym elementem zadania była samodzielna implementacja struktur danych, w szczególności Kolejki Priorytetowej.

**Zrealizowane wymagania:**
1.  **Język C++**: Projekt napisany w standardowym C++, bez użycia niedozwolonych kontenerów STL. Pamięć zarządzana jest dynamicznie.
2.  **Kolejka Priorytetowa (Min-Heap)**: Zaimplementowana od podstaw jako szablon (`template`). Obsługuje operacje:
    - `insert`: Dodawanie elementu.
    - `extractMin`: Pobieranie elementu o najmniejszym priorytecie.
    - `decreaseKey`: Zmiana priorytetu elementu.
    - `build`: Budowanie kopca w czasie liniowym O(N).
3.  **Algorytm Huffmana**: Pełna implementacja procesu budowania drzewa, generowania kodów oraz zapisu/odczytu binarnego.
4.  **Obsługa plików**: Program umożliwia użytkownikowi wskazanie plików wejściowych i wyjściowych.
5.  **Format wyjściowy**: Plik skompresowany zawiera nagłówek ze słownikiem (zgodnie z formatem omawianym na wykładzie) oraz właściwe dane binarne.

---

## 2. Opis Implementacji (Struktura Plików)

Projekt został podzielony na moduły w celu zachowania czytelności i porządku w kodzie.

### `PriorityQueue.h`
Plik nagłówkowy zawierający implementację szablonu klasy `MinPriorityQueue`.
- Wykorzystuje strukturę kopca binarnego (Min-Heap) opartego na dynamicznej tablicy.
- Implementuje algorytmy `heapifyUp` i `heapifyDown` do utrzymania własności kopca.
- Zarządza pamięcią poprzez dynamiczną realokację tablicy w przypadku jej zapełnienia.

### `Huffman.h`
Definicje struktur danych specyficznych dla algorytmu Huffmana:
- `HuffmanNode`: Struktura węzła drzewa binarnego (liście przechowują znaki, węzły wewnętrzne sumę częstości).
- `SimpleMap`: Autorska, prosta implementacja mapy asocjacyjnej (słownika) oparta na tablicy dynamicznej, zastępująca `std::map`.
- `BitWriter` / `BitReader`: Klasy narzędziowe buforujące operacje wejścia/wyjścia, umożliwiające zapis i odczyt pojedynczych bitów do strumieni bajtowych.

### `Huffman.cpp`
Implementacja logiki biznesowej:
- **Kompresja**: Analiza częstości znaków -> Budowa kolejki -> Konstrukcja drzewa Huffmana -> Generowanie kodów -> Zapis pliku wynikowego.
- **Dekompresja**: Odczyt słownika -> Odbudowa struktury drzewa -> Dekodowanie strumienia bitów do postaci tekstu jawnego.

### `main.cpp`
Interfejs użytkownika (Menu Konsolowe).
- Pozwala na wybór trybu pracy (Testowanie kolejki, Kompresja, Dekompresja).
- Prezentuje działanie zaimplementowanej kolejki priorytetowej w izolacji (zgodnie z wymogiem demonstracji operacji na kolejce).

---

## 3. Format Pliku Wynikowego (.bin)

Plik po kompresji posiada specyficzną strukturę, umożliwiającą jego późniejsze odtworzenie. Składa się z trzech sekcji:

1.  **Nagłówek rozmiaru**: Liczba całkowita określająca liczbę wszystkich znaków w oryginalnym pliku (niezbędne do precyzyjnego zakończenia dekompresji).
2.  **Słownik kodów**: Lista par `KOD_ASCII CIĄG_BITÓW` dla każdego unikalnego znaku.
    - *Dodatkowo:* W pliku, jako komentarz po znakach `//`, zapisana jest reprezentacja znakowa danego kodu ASCII. Służy to jedynie celom poglądowym przy ręcznej analizie pliku i jest ignorowane przez dekompresor.
3.  **Dane binarne**: Ciągła sekwencja bitów reprezentująca skompresowaną treść. W edytorach tekstowych sekcja ta widoczna jest jako zestaw znaków nieczytelnych (tzw. "krzaczki"), co jest naturalnym efektem interpretacji losowych bajtów jako znaków ASCII.

---

## 4. Uwagi Techniczne

### Obsługa Polskich Znaków (Kodowanie)
Program operuje na bajtach, co czyni go niezależnym od kodowania (UTF-8, ANSI, etc.). Jednakże, sposób wyświetlania polskich znaków w plikach wynikowych (w sekcji słownika) zależy od kodowania pliku wejściowego oraz edytora, w którym plik jest otwierany.

- Jeśli plik wejściowy jest w kodowaniu **ANSI (Windows-1250)** (jeden bajt na znak), słownik w pliku wynikowym pokaże poprawny pojedynczy kod dla znaku (np. `185` dla `ą`).
- Jeśli plik wejściowy jest w kodowaniu **UTF-8** (dwa bajty na znak), program poprawnie przetworzy go jako dwa niezależne bajty. W słowniku pojawią się dwa wpisy składowe.

Program działa poprawnie w obu przypadkach, wiernie odtwarzając treść pliku po dekompresji.

---

## 5. Instrukcja Uruchomienia

Aby skompilować i uruchomić projekt, należy wykonać poniższe polecenia w terminalu:

**Kompilacja:**
```bash
g++ main.cpp Huffman.cpp -o huffman.exe
```

**Uruchomienie:**
```bash
./huffman.exe
```

Program posiada intuicyjne menu tekstowe, które poprowadzi przez proces testowania kolejki oraz kompresji/dekompresji plików.

## Mój program stosuje format zapisu zgodny z tym, co zrozumiałem z wykładu (Słownik tekstowy + Dane binarne). Ponieważ algorytm Huffmana ## nie definiuje standardu nagłówka pliku, mój dekompresor obsługuje pliki stworzone w tym konkretnym formacie. Aby obsłużyć pliki z innych ## programów, musiałbym znać ich dokładną strukturę nagłówka.


Łukasz Sasin nr albumu: 288511
