#pragma once

//chaining ale kubelki wypelnnione tablica z BST dodatkowo 2 funckje hashujace
template <typename E, typename P>
class TwoChoice
{
    public:
    int size;                     //liczba elementow
    int capacity;                 //liczba kubelkow
    Dynamic<E,P> *HashTab;        //tablica wskaznikow na kubelki bedace tablicami dynamicznymi
    
    public:
    friend class Dynamic<E,P>;
    TwoChoice();            //konstruktor
    ~TwoChoice();           //destruktor
    double load_factor()    {return static_cast<double>(size)/capacity;}    //wspolczynnik zajetosci
    bool over_load()        {return load_factor()>=2;}                    //przeladowanie -> prawda
    bool under_load()       {return load_factor()<=0.1;}                    //zbyt male zageszczenie -> prawda

    void insert(const E& val, const P& key);
    void remove(const P& key);
    void resizeUP();
    void resizeDOWN();
    void resize(int grow);
    void wypisz() const;
    void get_size()  {std::cout<<"HT: "<<size<<std::endl;}   //wypisuje rozmiar

    //funkcje mieszjace/hashujace
    int uno(const P& key, int cap)
    {
        return key%(cap);
    }

    int dos(const P& key, int cap) 
    {
       //MAD method
        int a=4;
        int b=17;
        return ((a*key+b)%(cap));
    }

};

//tworzy nam tablice wskaznikow
template <typename E, typename P>
TwoChoice<E, P>::TwoChoice(): capacity(10), size(0), HashTab(new Dynamic<E, P>[capacity]) { }
//destruktor
template <typename E, typename P>
TwoChoice<E, P>::~TwoChoice()
{   
    delete[] HashTab;
}

template <typename E, typename P>
void TwoChoice<E, P>::insert(const E& val, const P& key)
{
    //sparwdzamy mijesce za pomoca funkcji hash
    int unodex=uno(key, capacity);
    int dosdex=dos(key, capacity);

    //jezeli 1hash kublek pusty lub 1hash ma mnije/tyle samo elementow niz kubelek 2 to wrzucamy do niego
    if((HashTab[unodex].Dynamic<E,P>::empty()) || (HashTab[unodex].Dynamic<E,P>::return_size()<=HashTab[dosdex].Dynamic<E,P>::return_size()))
    {
        HashTab[unodex].Dynamic<E,P>::insertTwoChoice(val, key);    //tutaj zwiekszamy rozmiar odpoiwedniego kubelka
    }

    else    //inaczej laduje w kubelku 2 wyboru
    {
        HashTab[dosdex].Dynamic<E,P>::insertTwoChoice(val, key);
    }
   
    size++; //tutaj ogolna ilosc elementow w tablicy
    if(over_load()) {resizeUP();}
}

template <typename E, typename P>
void TwoChoice<E, P>::remove(const P& key)
{
    int unodex=uno(key, capacity);
    //jezeli nie udalo sie usuanc dostaniemy false
    if(!(HashTab[unodex].Dynamic<E,P>::removeTwoChoice(key)))
    {
        //sprawdzamy indeks z 2 fukncji hashujacej
        int dosdex=uno(key, capacity);
        if(!(HashTab[dosdex].Dynamic<E,P>::removeTwoChoice(key)))
        {
            //nie udalo sie odnalezc klucza
            return;
        }
    }
    size--;
    if(under_load()){resizeDOWN();}
}

template <typename E, typename P>
void TwoChoice<E, P>::resizeUP()
{
    int grow=capacity*2;
    resize(grow);
}

template <typename E, typename P>
void TwoChoice<E, P>::resizeDOWN()
{
    int grow=capacity/2;
    resize(grow);
}

template <typename E, typename P>
void TwoChoice<E, P>::resize(int grow)
{
    Dynamic<E,P> *temp=new Dynamic<E,P>[grow];
    int old=capacity;
    
    for (int i=0; i<old; i++)
    {
        if(!(HashTab[i].Dynamic<E,P>::empty()))      //jezeli kubelek nie jest pusty
        {
            for(int j=0; j<HashTab[i].Dynamic<E,P>::return_capa(); j++)
            {
                int val=HashTab[i].array[j].value;
                if(val!=0)
                {
                    int key=HashTab[i].array[j].key;
    
                    int unodex=uno(key, grow);
                    int dosdex=dos(key, grow);

                    //jezeli 1hash kublek pusty lub 1hash ma mnije/tyle samo elementow niz kubelek 2 to wrzucamy do niego
                    if(temp[unodex].Dynamic<E,P>::empty() || temp[unodex].Dynamic<E,P>::return_size()<=temp[dosdex].Dynamic<E,P>::return_size())   
                    {
                        temp[unodex].Dynamic<E,P>::insertTwoChoice(val, key);
                    }

                    else    //inaczej laduje w naszej opcji kubelka numer 2
                    {
                        temp[dosdex].Dynamic<E,P>::insertTwoChoice(val, key);
                    }
                }
            }   
        }
    }
    //usuwamy stara tablice i przypisujemy wskaznik
    capacity=grow;
    delete [] HashTab;
    HashTab=temp;
    
}

template <typename E, typename P>
void TwoChoice<E, P>::wypisz() const
{
    for (int i=0; i<capacity; i++)
    {
        std::cout<<i;
        for(int j=0; j<HashTab[i].Dynamic<E,P>::return_capa(); j++)
        {
            if(HashTab[i].array[j].value!=0)
            {std::cout<<" ->  ("<<HashTab[i].array[j].value<<", "<<HashTab[i].array[j].key<<")";}
        }
        std::cout<<std::endl;
    }
}

