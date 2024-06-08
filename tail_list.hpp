#pragma once

template <typename E, typename P>
class TLinkedList;

template <typename E, typename P>
class Chaining;

template <typename E, typename P>
class TailNode {
public:
    E value;                 //wartsoc elementu
    P key;                   //klucz
    TailNode<E, P>* next;    //wskaznik na nastepny wezel/element

    friend class TLinkedList<E, P>;
    friend class Chaining<E, P>;
};

template <typename E, typename P>
class TLinkedList {
public:
    int size;
    TailNode<E, P>* head; //wskaznik na 1 element
    TailNode<E, P>* tail; //wskaznik na ostatni element, nie null

public:
    friend class Chaining<E, P>;
    TLinkedList();          //konstruktor
    ~TLinkedList();         //destruktor

    bool empty() const { return head == NULL; }     //true jezeli pusta
    int return_size() const { return size; }           //zwraca ilosc elementow
    const E& front() const { return head->value; }    //zwraca wartosc pierwszego ele
    const E& end() const { return tail->value; }    //zwraca wartosc ostatniego ele

    void whatSize()const;                //wypisuje size i capacity
    void addFront(const E& val, const P& key);                    //dodaj na poczatek
    void addEnd(const E& val, const P& key);                      //dodaj na koniec
    void addMid(const E& val, const P& key, int where);           //dodaj do srodka
    void removeFront();             //usun z poczatku
    void removeEnd();               //usun z konca
    void removeMid(int where);      //usun z wybranego miejsca
    void wypisz() const;            //wypisz wszystkie elementy

};


template <typename E, typename P>
TLinkedList<E, P>::TLinkedList() : head(NULL), tail(NULL), size(0) { }

template <typename E, typename P>
TLinkedList<E, P>::~TLinkedList() { while (!empty()) removeFront(); delete head; delete tail; }


template <typename E, typename P>
void TLinkedList<E, P>::whatSize()const { std::cout << "Ilosc elementow: " << return_size() << std::endl; }

template <typename E, typename P>
void TLinkedList<E, P>::wypisz() const {
    TailNode<E, P>* temp = head;
    while (temp != NULL) {
        std::cout << temp->value << std::endl;
        temp = temp->next;
    }
}

template <typename E, typename P>
void TLinkedList<E, P>::addFront(const E& val, const P& key) {
    TailNode<E, P>* nowy = new TailNode<E, P>;
    nowy->value = val;
    nowy->key = key;
    nowy->next = head;
    head = nowy;
    size++;
    if (tail == NULL) tail = head;
}

template <typename E, typename P>
void TLinkedList<E, P>::removeFront() {
    if (empty()) { return; }
    TailNode<E, P>* temp = head;
    head = temp->next;
    delete temp;
    size--;
    if (head == NULL) { tail = NULL; };
}

template <typename E, typename P>
void TLinkedList<E, P>::addEnd(const E& val, const P& key) {
    TailNode<E, P>* nowy = new TailNode<E, P>;
    nowy->value = val;
    nowy->key = key;
    nowy->next = NULL;
    if (empty()) { head = nowy; tail = nowy; }
    else {
        tail->next = nowy;
        tail = nowy;
    }
    size++;
}

template <typename E, typename P>
void TLinkedList<E, P>::addMid(const E& val, const P& key, int where) {

    if (where < 0 || where > return_size()) { std::cout << "Lokalizacja poza zakresem" << std::endl; return; }
    else if (where == 0 || empty()) { addFront(val, key); return; }

    TailNode<E, P>* nowy = new TailNode<E, P>;
    nowy->value = val;
    nowy->key = key;
    TailNode<E, P>* temp1 = head;
    for (int i = 0; i < where - 1; i++)
    {
        temp1 = temp1->next;
        if (temp1 == NULL) {
            addEnd(val, key);
            return;
        }
    }
    TailNode<E, P>* temp2 = temp1->next;
    temp1->next = nowy;
    nowy->next = temp2;

    size++;
}

template <typename E, typename P>
void TLinkedList<E, P>::removeEnd() {
    if (empty()) { std::cout << "Brak elementow" << std::endl;return; }
    else if (return_size() == 1) { removeFront();return; }
    else {
        TailNode<E, P>* temp1 = head; //musi znalezc przedostatni element zeby moc przekierowac tail na niego, a jego next na null
        while (1)
        {
            if (temp1->next == tail) { break; }
            temp1 = temp1->next;
        }
        delete tail;
        tail = temp1;
        temp1->next = NULL;
        size--;
    }
}

template <typename E, typename P>
void TLinkedList<E, P>::removeMid(int where) {
    if (empty()) { std::cout << "Brak elementow" << std::endl;return; }
    else if (where < 0 || where>return_size()) { std::cout << "Lokalizacja poza zakresem" << std::endl;return; }
    else if (where == 0) { removeFront();return; }
    else if (where == return_size() - 1) { removeEnd();return; }
    else
    {
        TailNode<E, P>* temp1 = head;    //element przed wybranym miejscu  
        TailNode<E, P>* temp2 = head;    //element usuwany wybranym miejscu
        TailNode<E, P>* temp3 = head;    //element po wybranym miejscu  
        for (int i = 0; i < where - 1; i++)
        {
            temp1 = temp1->next;
            if (temp1 == NULL)
            {
                removeEnd();
                return;
            }
        }
        temp2 = temp1->next;
        temp3 = temp2->next;
        temp1->next = temp3;
        //std::cout<<"Usunieto wartosc: "<<temp2->value<<std::endl;
        delete temp2;
        size--;
    }
}




