#pragma once

template <typename E, typename P>
class Cuckoo;

template <typename E, typename P>
class TwoChoice;

template <typename E, typename P>
class Dynamic
{
    public:
    struct Node
    {
        E value;
        P key;
        Node() : value(), key() {}
    };
    public:
    int size;       //liczba ele w tab
    int capacity;   //liczba miejsc
    Node *array;     //wskaznik na tablice dynamiczna
    
    
    public:
    Dynamic(): capacity(10), size(0), array(new Node[capacity]) {}              //konstruktor domyslny
    Dynamic(int capa): capacity(capa), size(0), array(new Node[capa]) {}    //konstruktor z deklaracja
    ~Dynamic() {delete[] array;}    //destruktor     

    bool notenough()const       {return (size==capacity);}    //true jesli zapelniona
    bool empty()const           {return (size==0);}           //true jesli pusta
    int return_size() const     {return size;}                //zwraca ilosc elementow  
    int return_capa() const     {return capacity;}            //zwraca ilosc miejsc
    void whatSize()const;                                     //wypisuje size i capacity
    
    void addEnd(const Node& node);                  //dodajemy elementy na koniec, od 0 wiec na index=size
    void addFront(const Node& val);                 //dodajemy elementy na początek
    void addMid(const Node& val, int index);        //dodajemy elementy do wybranego punktu
    void relocate(int index);         //przesuwa elementy w prawo
    void remove(int index);           //usuwa element
    void removeMid(int index);              
    void removeFront();
    void removeEnd();

    //zostala lekko zmodyfikowana
    void resize();                          //zwiekszamy rozmiar tablicy capacity*2

    friend class Cuckoo<E, P>;
    //funkjce pomocniczne do obslugi tablicy mieszajacej Cuckoo
    void insertCuckoo(const Node& val, int index);
    void removeCuckoo(int where);
    void resizeCuckoo(Dynamic<E, P>& newUno);

    friend class TwoChoice<E,P>;
    //funkcje pomocnicze do obslugi tablicy mieszajacej TwoChoice
    int return_key(int index)    {return array[index].key;}   //zwraca klucz
    void insertTwoChoice(const E& val, const P& key);         //umieszanie elementu w tablicy w lokazlizacji okreslonej przez BST
    int removeTwoChoice(const P& key);                    //usuwa element, zwraca 0 lub 1 by na podstawie tego moznabylo ocenic czy nalezy przesukwiac inna tabele
    void cleanerTwoChoice(int where);                     //pomoga remove, obsluguje 3 mozliwe przypadki rekurencyjnie
    int BST(const P& key);          //zwraca pozycje dla insertTwoChoice
    int findBST(const P& key);      //zwraca indeks szukanego klucza

};

template <typename E, typename P>
void Dynamic<E, P>::insertTwoChoice(const E& val, const P& key)
{
    //BST decyduje o lokazlizacji
    int index=BST(key);
    while(1)
    {
        if(index!=-1){break;}
        index=BST(key);
        resize();
    }//dopoki nie odnajdziemy poprawnego indeksu

    //tworzymy wezel i dodajemy do odpowiedniego miejsca wybranego przez BST
    Node Nod1;
    Nod1.value = val;
    Nod1.key = key;
    array[index]=Nod1;
    size++;
    if(notenough())  {resize();}    //jezeli po dodaniu okaze sie ze zapeliona zwiekszamy rozmiar
}

//klucze w priorytecie prawe > rodzic > lewe
template <typename E, typename P>
int Dynamic<E, P>::BST(const P& key)
{
    //jezeli drzewo jest puste wrzucamy w korzen
    if(return_size()==0){return 0;}

    int current=0;
    //wyrzucenie z while bedzie oznaczalo odnalezienie prawidlowego miejsca
    while(current<capacity)
    {
        //klucz wiekszy, idz w prawo
        if(key>return_key(current))
        {
            current=current*2+2;
        }
        //klucz mniejszy badź równy, idz w lewo
        else if(key<=return_key(current))
        {
            current=current*2+1;
        }
        //jezeli w obecnym wezele pusty to tam dodajemy
        if(array[current].value==0)     {return current;}
    }
    //jezeli nie odnalezniono wolnego miejsca w granicach
    return -1;
}

template <typename E, typename P>
int Dynamic<E, P>::removeTwoChoice(const P& key)
{
    int where=findBST(key);
    if(where==-1){return 0;}    //jezeli w kubelku nie bylo klucza zwraca false
    cleanerTwoChoice(where);    //usuwamy element w odpowiednio przesuwajac inne
    size--;
    return 1;                   //sukces zwracamy prawde
}

template <typename E, typename P>
void Dynamic<E, P>::cleanerTwoChoice(int where)
{
    //przypadek 1. nie ma synow, tylko tutaj rekrencja jest zatrzymywana
    if((array[where*2+1].value==0) && (array[where*2+2].value==0))
    {
        array[where].value=0;
        array[where].key=0;
        return;
    }

    //przypadek 2. mial obu synow, szukamy sukcesora, czyli liscia o najmniejszej wartosic dla naszego prawego dziecka
    else if((array[where*2+1].value!=0) && (array[where*2+2].value!=0))
    {
        int successor=where*2+2;    //ustawimy sie na prawym dziecku
        //nastepnie sprawdzamy jego lewe dzieci, schodzac jak najglebiej mozemy
        while (array[successor*2+1].value != 0) 
        {
            successor=successor*2+1;
        }
        //przypisujmy
        array[where].value=array[successor].value;
        array[where].key=array[successor].key;
        cleanerTwoChoice(successor);    //rekurencja dla sukcesora
    }

    //przypadek 3. ma 1 syna
    else
    {
        //jezeli ma lewego syna
        if (array[where * 2 + 1].value !=0) 
        {
            array[where] = array[where * 2 + 1];    //to na jego miejsce go wrzucamy
            cleanerTwoChoice(where*2+ 1);           //jezeli syn mial dzieci ponownie trzeba o nie odpowiednio zadbac
        }
        //jezeli a prawgo syna
        if (array[where*2+2].value !=0) 
        {
            array[where] = array[where*2+2];
            cleanerTwoChoice(where*2+2);
        }   
    }

}

template <typename E, typename P>
int Dynamic<E, P>::findBST(const P& key)
{
    int current=0;
    while(current<capacity)
    {
        //jezeli odnalezlismy klucz zwroc indeks na ktorym jest
        if(array[current].key==key)     {return current;}
        //klucz wiekszy, idz w prawo
        if(key>return_key(current))
        {
            current=current*2+2;
        }
        //klucz mniejszy idz w lewo
        else if(key<return_key(current))
        {
            current=current*2+1;
        }
    }
    //nie odnaleniono klucza w kubelku
    return -1;
}




template <typename E, typename P>
void Dynamic<E, P>::insertCuckoo(const Node& val, int index)
{
    array[index]=val;
    size++;
}

template <typename E, typename P>
void Dynamic<E, P>::removeCuckoo(int where)
{
    array[where].key=0;
    array[where].value=0;
    size--;
}

template <typename E, typename P>
void Dynamic<E, P>::resizeCuckoo(Dynamic<E, P>& newUno)
{
    capacity = newUno.capacity;
    size = newUno.size;
    delete[] array; // usuwamy starą tablicę

    array = new Node[capacity]; // tworzymy nową tablicę o odpowiednim rozmiarze

    // kopiujemy zawartość tablicy
    for (int i = 0; i < capacity; ++i) 
    {
        array[i] = newUno.array[i];
    }

    // usuwamy wskaźnik do tablicy z obiektu newUno
    delete[] newUno.array;
    newUno.array = nullptr;
}



template <typename E, typename P>
void Dynamic<E, P>::resize()
{
    int grow=capacity*2;
    Node *temp=new Node[grow];      //tymczasowy wskaznik
    for (int i=0; i<capacity; i++)      {temp[i]=array[i];}
    capacity=grow;
    delete [] array;
    array=temp; 
}



template <typename E, typename P>
void Dynamic<E, P>::whatSize()const{ std::cout<<"Ilosc elementow: "<<size<<std::endl;std::cout<<"Rozmiar: "<<capacity<<std::endl;}

template <typename E, typename P>
void Dynamic<E, P>::addEnd(const Node& node)
{  
    array[size]=node; 
    size++;
    if(notenough()){resize();} 
}

template <typename E, typename P>
void Dynamic<E, P>::addFront(const Node& val){

    if(size!=0){relocate(0);}
    array[0]=val; 
    size++;
    if(notenough()){resize();}
}

template <typename E, typename P>
void Dynamic<E, P>::addMid(const Node& val, int index)
{
    if(index>=size){addEnd(val);}
    else if(index==0){addFront(val);}
    else{
    relocate(index);
    array[index]=val; 
    size++;
    }
    if(notenough()) {resize();}
}

template <typename E, typename P>
void Dynamic<E, P>::relocate(int index){
    for(int i=size; i>index; i--)
    {
        array[i]=array[i-1];
    }
}

template <typename E, typename P>
void Dynamic<E, P>::remove(int index)
{
    int a=array[index]; //zapamietujemyw wartosc by wypisac ja na koncu
    //przesuwamy elementy w lewo
    if(size>0)
    {
    for(int i=index; i<size-1; i++)
    {
        array[i]=array[i+1];
    }
    size--;
    //std::cout << "Z " << index << " usunieto: " << a << std::endl;
    }
}

template <typename E, typename P>
void Dynamic<E, P>::removeMid(int index){
    if(size>0){remove(index);}
}

template <typename E, typename P>
void Dynamic<E, P>::removeEnd(){
    if(size>0){array[size-1]=array[size]; size--;}
}

template <typename E, typename P>
void Dynamic<E, P>::removeFront(){
    remove(0);
}



