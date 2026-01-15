#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <iostream>
#include <stdexcept>

// struktura reprezentujaca pojedynczy wezel w kopcu
// jest to szablon wiec mozna trzymac rozne typy danych
template <typename T>
struct HeapNode {
    T data;         // tutaj trzymamy wlasciwe dane
    int priority;   // a tutaj liczbe oznaczajaca waznosc czyli priorytet
};

// klasa kolejki priorytetowej typu min
// oznacza to ze najmniejsza liczba priorytetu jest najwazniejsza
template <typename T>
class MinPriorityQueue {
private:
    HeapNode<T>* heapArray; // wskaznik na tablice dynamiczna z wezlami
    int capacity;           // zmienna mowiaca ile max elementow sie zmiesci
    int currentSize;        // zmienna mowiaca ile aktualnie jest elementow

    // funkcja pomocnicza do zamiany dwoch elementow miejscami
    // uzywamy referencji zeby dzialac na oryginalach
    void swap(HeapNode<T>& a, HeapNode<T>& b) {
        HeapNode<T> temp = a; // tymczasowa zmienna
        a = b; // przypisujemy b do a
        b = temp; // przypisujemy stare a do b
    }

    // obliczanie indeksu rodzica w tablicy
    // wzor to (i - 1) / 2
    int parent(int i) { return (i - 1) / 2; }       
    
    // obliczanie indeksu lewego dziecka
    // wzor to 2 * i + 1
    int leftChild(int i) { return (2 * i + 1); }    
    
    // obliczanie indeksu prawego dziecka
    // wzor to 2 * i + 2
    int rightChild(int i) { return (2 * i + 2); }   

    // funkcja naprawiajaca kopiec w dol
    // sprawdza czy rodzic jest mniejszy od dzieci
    void heapifyDown(int i) {
        int smallest = i; // zakladamy ze rodzic jest najmniejszy
        int left = leftChild(i); // bierzemy indeks lewego
        int right = rightChild(i); // bierzemy indeks prawego

        // sprawdzamy czy lewe dziecko istnieje i czy jest mniejsze od rodzica
        if (left < currentSize && heapArray[left].priority < heapArray[smallest].priority)
            smallest = left; // jak tak to lewe jest nowym najmniejszym

        // sprawdzamy czy prawe dziecko istnieje i czy jest mniejsze od obecnego najmniejszego
        if (right < currentSize && heapArray[right].priority < heapArray[smallest].priority)
            smallest = right; // jak tak to prawe wygrywa

        // jesli najmniejszy to nie jest rodzic to trzeba zamienic
        if (smallest != i) {
            swap(heapArray[i], heapArray[smallest]); // zamieniamy miejscami
            heapifyDown(smallest); // i rekurencyjnie naprawiamy nizej
        }
    }

    // funkcja naprawiajaca kopiec w gore
    // uzywana po dodaniu elementu na koniec
    void heapifyUp(int i) {
        // dopoki nie jestesmy w korzeniu i rodzic jest wiekszy od dziecka
        while (i != 0 && heapArray[parent(i)].priority > heapArray[i].priority) {
            swap(heapArray[i], heapArray[parent(i)]); // zamieniamy z rodzicem
            i = parent(i); // idziemy pietro wyzej
        }
    }

    // funkcja do zmiany rozmiaru tablicy jak braknie miejsca
    void resize(int newCapacity) {
        HeapNode<T>* newArray = new HeapNode<T>[newCapacity]; // nowa wieksza tablica
        // przepisujemy stare elementy
        for (int i = 0; i < currentSize; i++) {
            newArray[i] = heapArray[i];
        }
        delete[] heapArray; // usuwamy stara tablice
        heapArray = newArray; // podmieniamy wskaznik
        capacity = newCapacity; // aktualizujemy pojemnosc
    }

public:
    // konstruktor domyslny ustawia startowa pojemnosc
    MinPriorityQueue(int initialCapacity = 10) {
        capacity = initialCapacity; // ustawiamy pojemnosc
        currentSize = 0; // na poczatku jest pusto
        heapArray = new HeapNode<T>[capacity]; // alokujemy pamiec
    }

    // destruktor czyszczacy pamiec
    ~MinPriorityQueue() {
        delete[] heapArray; // zwalniamy tablice
    }

    // sprawdza czy kolejka jest pusta
    bool isEmpty() const {
        return currentSize == 0; // zwraca true jak rozmiar 0
    }

    // zwraca aktualna liczbe elementow
    int size() const {
        return currentSize;
    }

    // funkcja dodajaca nowy element
    void insert(T data, int priority) {
        // jak brakuje miejsca to powiekszamy 2 razy
        if (currentSize == capacity) {
            resize(capacity * 2);
        }

        currentSize++; // zwiekszamy licznik elementow
        int i = currentSize - 1; // indeks nowego elementu
        heapArray[i] = {data, priority}; // wpisujemy dane na koniec
        heapifyUp(i); // naprawiamy strukture kopca w gore
    }

    // funkcja pobierajaca element o najmniejszym priorytecie
    T extractMin() {
        // jak pusto to rzucamy wyjatek
        if (currentSize <= 0) {
            throw std::runtime_error("Queue is empty");
        }
        // jak tylko jeden element to prosta sprawa
        if (currentSize == 1) {
            currentSize--; // zmniejszamy rozmiar
            return heapArray[0].data; // zwracamy jedyny element
        }

        T root = heapArray[0].data; // zapisujemy korzen zeby go zwrocic
        // bierzemy ostatni element i wstawiamy w miejsce korzenia
        heapArray[0] = heapArray[currentSize - 1];
        currentSize--; // zmniejszamy rozmiar
        heapifyDown(0); // naprawiamy kopiec w dol zeby przywrocic porzadek
        return root; // zwracamy stary korzen
    }

    // funkcja do podgladania co jest na wierzchu bez usuwania
    T peek() const {
        if (currentSize <= 0) throw std::runtime_error("Queue is empty");
        return heapArray[0].data; // zwracamy korzen
    }
    
    // pomocnicza funkcja sprawdzajaca czy sa elementy
    bool hasElements() const {
        return currentSize > 0;
    }

    // funkcja zmieniajaca priorytet istniejacego elementu
    // dziala tylko na zmniejszanie priorytetu czyli zwiekszanie waznosci
    bool decreaseKey(T targetData, int newPriority) {
        // szukamy elementu liniowo
        for (int i = 0; i < currentSize; i++) {
            if (heapArray[i].data == targetData) { // znalezlismy
                if (newPriority > heapArray[i].priority) {
                    return false; // blad bo nowy priorytet jest gorszy
                }
                heapArray[i].priority = newPriority; // zmieniamy priorytet
                heapifyUp(i); // naprawiamy w gore bo element stal sie wazniejszy
                return true; // sukces
            }
        }
        return false; // nie znalezlismy elementu
    }
    
    // funkcja budujaca kopiec z gotowej tablicy
    // jest szybsza niz dodawanie po jednym bo dziala w O n
    void build(T* data, int* priorities, int count) {
        // jak za malo miejsca to powiekszamy
        if (count > capacity) {
            resize(count);
        }
        currentSize = count; // ustawiamy nowy rozmiar
        // przepisujemy elementy do tablicy
        for (int i = 0; i < count; i++) {
            heapArray[i] = {data[i], priorities[i]};
        }
        
        // algorytm floyda zaczynamy od polowy i naprawiamy w dol
        for (int i = (currentSize / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    // funkcja wypisujaca zawartosc kolejki do konsoli
    void printQueue() {
        std::cout << "Kolejka (rozmiar " << currentSize << "): ";
        for (int i = 0; i < currentSize; i++) {
            // wypisujemy dane i priorytet
            std::cout << "[" << heapArray[i].data << ":" << heapArray[i].priority << "] ";
        }
        std::cout << "\n";
    }
};

#endif
