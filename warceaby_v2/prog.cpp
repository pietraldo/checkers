#include <iostream>
#include <string>
#include "plansza.h"
#include "wspolrzedna.h"

using namespace std;

void ruch_gracza(plansza& p, int kolor_gracz, wspolrzedna = wspolrzedna(-1,-1));
void generuj_wszystkie_plansze(plansza&p, int kolor_gracz, vector<plansza>*);
void generuj_wszystkie_bicia(int i, int j, vector<plansza>* lista, plansza* p);
plansza ruch_komputera(plansza& p, int kolor_gracza, int max_glebokosc);
int oblicz_plansze(plansza* p, int kolor_gracza, int glebokosc, int max_glebokosc);

int main()
{
	plansza p;

	p.ustawienie_poczatkowe_planszy();

	for (int i = 0; i < 100; i++)
	{
		cout << p;
		p.resetuj_kolory();
		ruch_gracza(p, 1);
		p.resetuj_kolory();
		p=ruch_komputera(p,2,7);
		system("cls");
		
	}
	
	
}

int oblicz_plansze(plansza* p, int kolor_gracza, int glebokosc, int max_glebokosc)
{
	vector<plansza> plansze;
	generuj_wszystkie_plansze(*p, kolor_gracza, &plansze);

	int* wyniki = new int[plansze.size()];
	for (int i = 0; i < plansze.size(); i++)
	{
		if (glebokosc >= max_glebokosc)
			wyniki[i] = plansze[i].wynik();
		else
			wyniki[i] = oblicz_plansze(&plansze[i], (kolor_gracza+1)%2, glebokosc+1, max_glebokosc);
	}
	/*if (glebokosc == 1)
		cout <<endl<<endl<< "                          Glebokosc 1: ";
	if (glebokosc == 2)
		cout << endl << endl << "Glebokosc 2: ";*/
	int naj=wyniki[0];
	for (int i = 1; i < plansze.size(); i++)
	{
		/*if (glebokosc == 1|| glebokosc==2)
			cout << wyniki[i] << " ";*/
		if (kolor_gracza == 1)
		{
			if (wyniki[i] > naj)
				naj = wyniki[i];
		}
		else
		{
			if (wyniki[i] < naj)
				naj = wyniki[i];
		}		
	}

	int suma = naj;
	if (glebokosc == 1)
	{
		suma *= 1000;
		for (int i = 1; i < plansze.size(); i++)
		{
			suma += wyniki[i];
		}
	}
	/*if (glebokosc == 1 || glebokosc==2)
		cout << " -> " <<naj<<endl;*/
	
	return suma;
}

plansza ruch_komputera(plansza& p, int kolor_gracza, int max_glebokosc)
{
	vector<plansza> plansze;
	generuj_wszystkie_plansze(p, kolor_gracza, &plansze);

	
	int n = plansze.size();
	int* wyniki = new int[n];
	for (int i = 0; i < n; i++)
	{
		wyniki[i] = oblicz_plansze(&plansze[i], (kolor_gracza+1)%2, 1, max_glebokosc);
		/*system("cls");
		cout << (double)(i*100 / n) << "%";*/
		cout << "+";
	}

	

	int naj_indeks = 0;
	for (int i = 1; i < n; i++)
	{
		if (kolor_gracza == 1)
		{
			if (wyniki[i] > wyniki[naj_indeks])
				naj_indeks = i;
		}
		else
		{
			if (wyniki[i] < wyniki[naj_indeks])
				naj_indeks = i;
		}
	}

	// wyswietlanie 
	/*cout<<endl << "                 Glebokosc 0: ";
	for (int i = 0; i < n; i++)
		cout << wyniki[i] << " ";
	cout <<endl<<"Wybieram z wynikiem: "<< wyniki[naj_indeks];*/
	// koniec wyswietlania
	
	return plansze[naj_indeks];
}

void generuj_wszystkie_bicia(int i, int j, vector<plansza>* lista, plansza* p)
{
	vector<bicie> bicia;
	p->dodaj_bicie(i, j, &bicia);

	if (bicia.empty())
	{
		lista->push_back(*p);
		return;
	}
	
	for (int i = 0; i < bicia.size(); i++)
	{
		plansza p2 = *p;
		p2.wykonaj_bicie(bicia[i]);
		generuj_wszystkie_bicia(bicia[i].i2, bicia[i].j2, lista, &p2);
	}

}

void generuj_wszystkie_plansze(plansza& p, int kolor_gracza, vector<plansza>* plansze)
{
	vector<bicie> bicia;
	vector<ruch> ruchy;

	// sprawdzam czy sa jakies bicia
	bool sa_bicia;
	for (int i = 0; i < N; i++)
		for (int j = i % 2; j < N; j += 2)
			if (kolor_gracza % 2 == p[i][j] % 2 && p[i][j] != 0)
				p.dodaj_bicie(i, j, &bicia);
	sa_bicia = !bicia.empty();
	
	// nie ma bic -> dodaje ruchy
	if (!sa_bicia)
	{
		for (int i = 0; i < N; i++)
			for (int j = i % 2; j < N; j += 2)
				if (kolor_gracza % 2 == p[i][j] % 2 && p[i][j] != 0)
					p.dodaj_ruch(i, j, &ruchy);
	}
	
	// generujemy wszystkie mozliwe plansze
	if (sa_bicia)
	{
		for (int i = 0; i < bicia.size(); i++)
		{
			plansza p2 = p;
			p2.wykonaj_bicie(bicia[i]);
			generuj_wszystkie_bicia(bicia[i].i2, bicia[i].j2, plansze, &p2);
		}
	}
	else
	{
		for (int i = 0; i < ruchy.size(); i++)
		{
			plansza p2 = p;
			p2.wykonaj_ruch(ruchy[i]);
			plansze->push_back(p2);
		}
	}
}

void ruch_gracza(plansza &p, int kolor_gracza, wspolrzedna wsp_podana)
{
	vector<bicie> bicia;
	vector<bicie> bicia_z_pola;
	vector<ruch> ruchy;
	string w;
	bool sa_bicia;

	// sprawdzam czy sa bicia
	for (int i = 0; i < N; i++)
		for (int j = i % 2; j < N; j += 2)
			if (kolor_gracza % 2 == p[i][j] % 2 && p[i][j] != 0)
				p.dodaj_bicie(i, j, &bicia);
	sa_bicia = !bicia.empty();

	
	wspolrzedna wsp;
	if (wsp_podana.i == -1)
	{
		do
		{
			if (sa_bicia)
				cout << "Masz bicia";
			cout << endl << "Podaj pole: ";
			cin >> w;
			wsp = wspolrzedna(w);
			bicia_z_pola.clear();
			ruchy.clear();
			// musi byc dobry kolor i nie puste pole i jak jest bicie to musi byc pionek ktorym da sie cos zbic
			// a jak nie ma bicia to musi miec mozliwosc ruszenia sie tym pionkiem
		} while (kolor_gracza%2 != p[wsp.i][wsp.j] % 2 || p[wsp.i][wsp.j] == 0 || ((sa_bicia && !p.dodaj_bicie(wsp.i, wsp.j, &bicia_z_pola)) || (sa_bicia == false && !p.dodaj_ruch(wsp.i, wsp.j, &ruchy))));
	}
	else
	{
		wsp = wsp_podana;
		p.dodaj_bicie(wsp.i, wsp.j, &bicia_z_pola);
	}
	

	// wyswietlanie na czerwono mozliwosci z tego pola
	if (sa_bicia)
	{
		for (int i = 0; i < bicia_z_pola.size(); i++)
		{
			p.podswietl_ruch(bicia_z_pola[i]);
		}
	}
	else
	{
		for (int i = 0; i < ruchy.size(); i++)
		{
			p.podswietl_ruch(ruchy[i]);
		}
	}
	system("cls");
	cout << p;
	
	//drugie pole
	wspolrzedna wsp2;
	int indeks = -1;
	do
	{
		cout << "Podaj drugie pole: ";
		cin >> w;
		wsp2 = wspolrzedna(w);
		ruch r(wsp.i, wsp.j, wsp2.i, wsp2.j);
		if (sa_bicia)
		{
			for (int i = 0; i < bicia_z_pola.size(); i++)
				if (r == bicia_z_pola[i])
					indeks = i;
		}
		else
		{
			for (int i = 0; i < ruchy.size(); i++)
				if (r == ruchy[i])
					indeks = i;
		}
		
	} while (indeks==-1);
	system("cls");
	p.resetuj_kolory();
	if (sa_bicia)
	{
		p.wykonaj_bicie(bicia_z_pola[indeks]);
		if (p.dodaj_bicie(bicia_z_pola[indeks].i2, bicia_z_pola[indeks].j2, &bicia))
		{
			ruch_gracza(p,kolor_gracza, wspolrzedna(bicia_z_pola[indeks].i2, bicia_z_pola[indeks].j2));
		}
	}
		
	else
		p.wykonaj_ruch(ruchy[indeks]);
	cout << p;
}