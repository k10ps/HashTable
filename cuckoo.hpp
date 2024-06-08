    #pragma once

    template <typename E, typename P>
    class Dynamic;

    //2 tablice, 2 rozne funkcje, Cuckoo hashing
    template <typename E, typename P>
    class Cuckoo
    {
        public:
        Dynamic<E,P> unoTab;
        Dynamic<E,P> dosTab;

        public:
        friend class Dynamic<E, P>;
        double load_factor()    {return static_cast<double>(unoTab.size) / unoTab.capacity;;}     //wspolczynnik zajetosci
        bool over_load()        {return load_factor()>0.5;}                                       //przeladowanie -> prawda
        bool under_load()       {return load_factor()<0.1;}                                       //zbyt male zageszczenie -> prawda
        void get_size()  {std::cout<<std::endl<<"T1: "<<unoTab.size<<"   T2: "<<dosTab.size<<" SUM: "<<unoTab.size+dosTab.size<<std::endl;}   //wypisuje rozmiar obu tablic
        void wypisz() const;                        //wypisuje wszystkie elementy
        void insert(const E& val, const P& key);    //umieszanie, uzywa 2 funkcji hashujacych
        void resizeUP();                            //powiekszanie capacity*2
        void resizeDOWN();                          //pomnijeszanie capacity/2
        void resize(int grow);
        void remove(const P& key);                  //usuwanie elementu
        bool rehash(const E& val, const P& key, Dynamic<E, P>& newUno, Dynamic<E, P>& newDos);  //wyglada tak samo jak insert ale nie aktywuje resizeUP, pomocnicza dla remove

        //decyduje o indeksie w tablicy, miejscu do ktrego zostanie wrzucony element
        int unoHash(const P& key, int cap)  //dla unoTab
        {
            return key%(cap);
        }

        int dosHash(const P& key, int cap)  //dla dosTab
        {
            //MAD method
            int a=4;
            int b=17;
            return ((a*key+b)%(cap));
        }

    };

    template<typename E, typename P>
    bool Cuckoo<E,P>::rehash(const E& val, const P& key, Dynamic<E, P>& newUno, Dynamic<E, P>& newDos)
    {
        int cycle=0;
        int max_round=newUno.capacity;

        E tempval=val;
        P tempkey=key;

        while(cycle<max_round)
        {
            //tworzymy wezel z nowych wartosci
            typename Dynamic<E, P>::Node Nod1;
            Nod1.value = tempval;
            Nod1.key = tempkey;

            int dex1=unoHash(tempkey, newUno.capacity);  //znajdujemy index
            
            if(newUno.array[dex1].value==0)     //jezeli index w T1 wolny wstawiamy, mozemy zakonczyc petle
            {
                newUno.Dynamic<E,P>::insertCuckoo(Nod1, dex1); 
                return 1;
            }
            
            else            //miejsce w T1 zajete wiec realokujemy stary elemnt z T1 do dex2 w T2
            {
                //przypisujemy do temp stare wartosci
                tempval=newUno.array[dex1].value;
                tempkey=newUno.array[dex1].key;
                newUno.Dynamic<E,P>::removeCuckoo(dex1);
                newUno.Dynamic<E,P>::insertCuckoo(Nod1, dex1);  //wstawiamy do T1 nowy wezel
            }

            //nwoy wezel z poprzednimi wartosciami z T1
            typename Dynamic<E, P>::Node Nod2;
            Nod2.value = tempval;
            Nod2.key = tempkey;

            int dex2=dosHash(tempkey, newDos.capacity);
            if(newDos.array[dex2].value==0)        //jezeli jest miejsce w T2 wstawiamy, konczymy dzialanie
            {
                newDos.Dynamic<E,P>::insertCuckoo(Nod2, dex2);
                return 1;
            }
            
            else    //inczej nalezy ponownie przepisac elemnty i petla szukamy alternatywnego meijsca w T1
            {
                tempval=newDos.array[dex2].value;
                tempkey=newDos.array[dex2].key;
                newDos.Dynamic<E,P>::removeCuckoo(dex2);
                newDos.Dynamic<E,P>::insertCuckoo(Nod2, dex2);
            }

            cycle++;
        }
        //jezeli nie udalo sie znalezc miejsca mimo cykli to musi zrobic resize wiekszy
        std::cout<<"KUWRA"<<std::endl;
        return 0;
    }   

    template <typename E, typename P>
    void Cuckoo<E,P>::insert(const E& val, const P& key)
    {
        int cycle=0;
        int max_round=unoTab.capacity;

        E tempval=val;
        P tempkey=key;

        while(cycle<max_round)
        {
            //tworzymy wezel z nowych wartosci
            typename Dynamic<E, P>::Node Nod1;
            Nod1.value = tempval;
            Nod1.key = tempkey;
            int dex1=unoHash(tempkey, unoTab.capacity);  //znajdujemy index
            //jezeli index w T1 wolny wstawiamy, mozemy zakonczyc petle
            if(unoTab.array[dex1].value==0)     
            {
                unoTab.Dynamic<E,P>::insertCuckoo(Nod1, dex1); 
                if(over_load()) {resizeUP();}
                return;
            }

            //miejsce w T1 zajete wiec realokujemy stary elemnt z T1 do dex2 w T2
            else
            {
                //przypisujemy do temp stare wartosci
                tempval=unoTab.array[dex1].value;
                tempkey=unoTab.array[dex1].key;
                unoTab.Dynamic<E,P>::removeCuckoo(dex1);    //oczyszcamy stare miejsce po wyjeciu
                unoTab.Dynamic<E,P>::insertCuckoo(Nod1, dex1);  //wstawiamy do T1 nowy wezel
            }
            //nwoy wezel z poprzednimi wartosciami z T1
            typename Dynamic<E, P>::Node Nod2;
            Nod2.value = tempval;
            Nod2.key = tempkey;
            int dex2=dosHash(tempkey, dosTab.capacity);
            //jezeli jest miejsce w T2 wstawiamy, konczymy dzialanie

            if(dosTab.array[dex2].value==0)     
            {
                dosTab.Dynamic<E,P>::insertCuckoo(Nod2, dex2);
                if(over_load()) {resizeUP();}
                return;
            }
            //inczej nalezy ponownie przepisac elemnty i petla szukamy alternatywnego meijsca w T1
            else
            {
                tempval=dosTab.array[dex2].value;
                tempkey=dosTab.array[dex2].key;

                dosTab.Dynamic<E,P>::removeCuckoo(dex2);    //oczyszcamy stare miejsce po wyjeciu
                dosTab.Dynamic<E,P>::insertCuckoo(Nod2, dex2);
            }
        
            cycle++;
        }
        //while nie zakonczyl sie returnem wiec musimy rehashowac tablice aby moc dodac nowy element
        resizeUP();
        insert(tempval, tempkey);
        //rekurencyjnie wstawiamy z powrotem ostatni zapisany element ktorego nie udalo sie odpowiednio ulozyc
        
    }

    template <typename E, typename P>
    void Cuckoo<E, P>::resizeUP()
    {
        int grow=unoTab.capacity*2;
        resize(grow);
    }

    template <typename E, typename P>
    void Cuckoo<E, P>::resizeDOWN()
    {
        int degrow=unoTab.capacity/2;
        resize(degrow);
    }

    template <typename E, typename P>
    void Cuckoo<E, P>::resize(int grow)
    {
        int old = unoTab.capacity; 
        Dynamic<E, P> newUno(grow);
        Dynamic<E, P> newDos(grow);
        bool too_small=0;

        for (int i = 0; i < old; i++) 
        {
            if (unoTab.array[i].value != 0) 
            {
                if(!rehash(unoTab.array[i].value, unoTab.array[i].key, newUno, newDos))
                {
                    too_small=1;
                    break; //jeśli rehash nie powiodl sie, wyjdz
                }
            }

            if (dosTab.array[i].value != 0) 
            {
                if(!rehash(dosTab.array[i].value, dosTab.array[i].key, newUno, newDos))
                {
                    too_small=1;
                    break; 
                }
            }
        }
        //rehash sie nei powiodl probujemy drugi raz z 4x razy wieksza tablica
        if(too_small)
        {resize(grow*2); return;}

        unoTab.Dynamic<E, P>::resizeCuckoo(newUno);
        dosTab.Dynamic<E, P>::resizeCuckoo(newDos);
        //get_size();
    }


    template <typename E, typename P>
    void Cuckoo<E, P>::remove(const P& key)
    {
        //sprawdzamy 1 tablice i za pomoca jej funkjci wyznaczmy indeks
        int index=unoHash(key, unoTab.capacity);
        if(unoTab.array[index].key==key)    //jezeli klucz zostal odnaleizony
        {
            unoTab.Dynamic<E,P>::removeCuckoo(index);   //usuwamy
            if(under_load()){resizeDOWN();}
            return; //zakonczenie
        }
        //inczaej powtarzamy proces dla 2 tablicy
        index=dosHash(key, dosTab.capacity);
        if(dosTab.array[index].key==key)
        {
            dosTab.Dynamic<E,P>::removeCuckoo(index);
            if(under_load()){resizeDOWN();}
            return;
        }
    }

    template <typename E, typename P>
    void Cuckoo<E, P>::wypisz() const
    {
        std::cout<<"unoTab"<<std::endl;
        for (int i=0; i<unoTab.capacity; i++)
        {
            std::cout<<i<<"  ("<<unoTab.array[i].value<<", "<<unoTab.array[i].key<<")"<<std::endl;
        }
        std::cout<<"dosTab"<<std::endl;
        for (int i=0; i<dosTab.capacity; i++)
        {
            std::cout<<i<<"  ("<<dosTab.array[i].value<<", "<<dosTab.array[i].key<<")"<<std::endl;
        }
    }


