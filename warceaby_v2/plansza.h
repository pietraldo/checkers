#pragma once
#include<iostream>
#include<string>
#include <Windows.h>
#include <iomanip>
#include <vector>

#include "wspolrzedna.h"
#include "ruch.h"

using namespace std;


const int N = 8;

enum kolory_e {
	kolor_pola_bialego = 9,
	kolor_pola_czarnego = 1,
	kolor_czarnych = 0,
	kolor_bialych = 15,
	kolor_podswietlenia_ruchu=13,
	kolor_podswietlenia_mozliwosci=4
};

class plansza
{
	int pola[N][N] = {0};
	
public:
	int** kolor=nullptr;
	bool przekrec_plansze = false;

	plansza();
	plansza(const plansza&);
	
	void resetuj_kolory();
	int wynik();
	bool czy_czarne(int i, int j);
	void ustawienie_poczatkowe_planszy();
	bool dodaj_ruch(int i, int j,vector<ruch>* ruchy);
	void wykonaj_ruch(ruch);
	void podswietl_ruch(ruch);
	void wykonaj_bicie(bicie);
	bool dodaj_bicie(int i, int j, vector<bicie>* );
	bool czy_jest_bicie_damka(int i, int j);

	int& operator[](string);
	int& operator[](wspolrzedna);
	int* operator[](int);

	friend ostream& operator<<(ostream&, plansza&);
	
};

