#pragma once

//kubełki wypelnione wskaznikami na listy Metoda łańcuchowa (separate chaining).
template <typename E, typename P>
class Chaining
{
    public:
    int size;                     //liczba elementow
    int capacity;                 //liczba kubelkow
    TLinkedList<E, P> *array;     //wskaznik na tablice, kazdy element to lista, zaweirajaca (val, key)

    public:
    Chaining();
    ~Chaining();
    double load_factor()    {return static_cast<double>(size)/capacity;}    //wspolczynnik zajetosci
    bool over_load()        {return load_factor()>=1.5;}                    //przeladowanie -> prawda
    bool under_load()       {return load_factor()<=0.1;}                    //zbyt male zageszczenie -> prawda
    
    void insert(const E& val, const P& key);                //umieszamy nowy element
    void resize(int grow);      //powiekszanie i odpowidnia realkoacja elementow
    void resizeUP();            //zwiekszanie rozmiaru tablicy, podwajamy po capacity
    void resizeDOWN();          //zmniejszanie, capcaity/2
    void remove(const P& key);       //usuniecie elementu po kluczu
    void wypisz() const;             //wypisuje elementy
    int find(const P& key) const;    //wyszukwianie kluczem, zwroic wartosc
    void get_size()  {std::cout<<size<<std::endl;}   //wypisuje rozmiar

    //decyduje o indeksie w tablicy, miejscu do ktrego zostanie wrzucony element
    int hash_function(const P& key)
    {
        return key%capacity;
    }

};

//konstruktor tworzy nam pusta tablice
template <typename E, typename P>
Chaining<E, P>::Chaining(): capacity(10), size(0), array(new TLinkedList<E, P>[capacity]) { }
//destruktor
template <typename E, typename P>
Chaining<E, P>::~Chaining(){delete[] array;}

template <typename E, typename P>
void Chaining<E, P>::insert(const E& val, const P& key)
{
    int index=hash_function(key);               //determinujemy index
    array[index].TLinkedList<E, P>::addEnd(val, key); //dodajemy na koniec listy
    size++;
    if(over_load()) {resizeUP();}     //sprawdzamy czy doszlo do przeladowania, jezeli tak, powiekszamy
}

template <typename E, typename P>
void Chaining<E, P>::resizeUP()   
{
    int grow=capacity*2; //podwojne powiekszenie 
    resize(grow);
}

template <typename E, typename P>
void Chaining<E, P>::resizeDOWN()   
{
    int degrow=capacity/2;  //podwojne pomniejszenie
    resize(degrow);
}

template <typename E, typename P>
void Chaining<E, P>::resize(int grow)   
{
    int old=capacity;                            
    TLinkedList<E, P> *temp=new TLinkedList<E, P>[grow];      //nowa tablica w odpoweidnim rozmiarze i wskaznik na nia
    capacity=grow;      //usatwaimy capacity zeby funkcje mieszajace odpowiednio dzialaly
    
    for(int i=0; i<old; i++)
    {
        TailNode<E, P> *iterator = array[i].head;   //do przesuwania sie po koljenych elemenatch
        while(iterator!=NULL)
        {
            E fresh_value=iterator->value;
            P fresh_key=iterator->key;
            int index=hash_function(fresh_key);
            temp[index].TLinkedList<E, P>::addEnd(fresh_value, fresh_key);
            iterator=iterator->next;
        }
    }

    delete [] array;    //usuwamy stara tablice
    array=temp;     //oraz odpowiednio przypisujemy wskaznik
}

template <typename E, typename P>
void Chaining<E, P>::remove(const P& key)
{
    int index=hash_function(key);   //odnajdujemy index
    int where=0;
    TailNode<E, P> *iterator = array[index].head;
    while(iterator!=NULL)
    {
        //jezeli odnajdziemy taki sam klucz jak szukalismy usuwamy i wychodizmy
        if(iterator->key==key){array[index].TLinkedList<E, P>::removeMid(where); size--; break;}
        iterator=iterator->next;
        where++;
    }
    if(under_load()){resizeDOWN();}
}

template <typename E, typename P>
void Chaining<E, P>::wypisz() const
{
    
    for (int i=0; i<capacity; i++)
    {
        TailNode<E, P> *temp=array[i].head;
        std::cout<<i;
        while(temp!=NULL)
        {
            std::cout<<" -> ("<<temp->value<<", "<<temp->key<<")";
            temp=temp->next;
        }
        std::cout<<std::endl;
    }
}

template <typename E, typename P>
int Chaining<E, P>::find(const P& key) const
{
    int index=hash_function(key);
    TailNode<E, P> *temp=array[index].head;
    while(temp!=NULL)
    {
        if(temp->key==key)
        {
            //std::cout<<" -> ("<<temp->value<<", "<<temp->key<<")";
            return temp->value;
        }
        temp=temp->next;
    }
    return 0;
}