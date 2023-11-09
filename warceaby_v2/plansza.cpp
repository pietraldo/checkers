#include "plansza.h"
#include <utility>
#include <functional>

plansza::plansza()
{
	resetuj_kolory();
}

plansza::plansza(const plansza& p)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			pola[i][j] = p.pola[i][j];

	if (kolor == nullptr)
	{
		kolor = new int* [N];
		for (int i = 0; i < N; i++)
			kolor[i] = new int[N];
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			kolor[i][j] = p.kolor[i][j];
	
}

void plansza::resetuj_kolory()
{
	if (kolor == nullptr)
	{
		kolor = new int*[N];
		for (int i = 0; i < N; i++)
			kolor[i] = new int[N];
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			kolor[i][j] = ((i + j) % 2==0)?kolory_e::kolor_pola_czarnego:kolory_e::kolor_pola_bialego;
}

int& plansza::operator[](string wsp)
{
	wspolrzedna w(wsp);
	return pola[w.i][w.j];
}

int& plansza::operator[](wspolrzedna w)
{
	return pola[w.i][w.j];
}

int* plansza::operator[](int i)
{
	return pola[i];
}

bool plansza::czy_czarne(int i, int j) 
{
	return(i + j) % 2 == 0;
};

int plansza::wynik()
{
	int odp = 0;
	for (int i = 0; i < N; i++)
		for (int j = i%2; j < N; j+=2)
		{
			int pionek = pola[i][j];
			if(pionek!=0)
			{
				switch (pionek)
				{
				case 1:
					odp += 1;
					break;
				case 2:
					odp -= 1;
					break;
				case 3:
					odp += 5;
					break;
				case 4:
					odp -= 5;
					break;
				}
			}
		}
	return odp;
}

ostream& operator<<(ostream& out, plansza& p)
{
	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << endl<<endl;
	for (int i =(p.przekrec_plansze)? 0:N-1; i >= 0 && i<N; i+= (p.przekrec_plansze) ? 1:-1)
	{
		SetConsoleTextAttribute(hconsole, 15);//resetuje ustwaienia
		cout << "  " << fixed<<setw(3)<<i + 1 << " ";
		for (int j = (p.przekrec_plansze) ? N-1:0; j < N && j>=0; j+= (p.przekrec_plansze) ? -1:1)
		{
			//ustwaiamy kolor tla, czcionki
			SetConsoleTextAttribute(hconsole, ((p.czy_czarne(i, j) && (p[i][j] == 1 || p[i][j] == 3)) ? kolory_e::kolor_bialych : kolory_e::kolor_czarnych) +
				(p.kolor[i][j] << 4));

			if (p.czy_czarne(i, j))
			{
				if (p[i][j] > 2)
					cout << " O ";
				else if (p[i][j] > 0)
					cout << " o ";
				else
					cout << "   ";
			}
			else
				cout << "   ";
			SetConsoleTextAttribute(hconsole, 15);//resetuje ustwaienia
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hconsole, 15);//resetuje ustwaienia
	cout << "      ";
	for (int i = (p.przekrec_plansze) ? N-1 : 0; i >= 0 && i < N; i += (p.przekrec_plansze) ? -1 : 1)
		cout << " " << (char)('a' + i) << " ";
	

	SetConsoleTextAttribute(hconsole, 15);//resetuje ustwaienia
	cout << endl;
	return out;
}

void plansza::ustawienie_poczatkowe_planszy()
{
	if (N < 3)
		return;
	for (int i = 0; i < 3; i++)
		for (int j = i%2; j < N; j+=2)
			pola[i][j] = 1;
	for (int i = N-3; i < N; i++)
		for (int j = i%2; j < N; j+=2)
			pola[i][j] = 2;
	
}

bool plansza::dodaj_ruch(int i, int j, vector<ruch>* ruchy)
{
	if (pola[i][j] == 0)
		return false;
	bool dodano = false;
	if (pola[i][j] < 3) // pionki
	{
		int plus = (pola[i][j] == 1) ? 1 : -1;
		
		if (i + plus < N && i + plus >= 0) // czy nie wychodzi za tablice
		{
			if (j + 1 < N && pola[i + plus][j + 1] == 0)
			{
				ruchy->push_back(*new ruch(i, j, i + plus, j + 1));
				dodano = true;
			}
			if (j - 1 >= 0 && pola[i + plus][j - 1] == 0)
			{
				ruchy->push_back(*new ruch(i, j, i + plus, j - 1));
				dodano = true;
			}
		}
	}
	else // damki
	{
		int i1 = i+1, j1 = j+1;
		while (i1<N && j1<N)
		{
			if (pola[i1][j1] != 0)
				break;
			ruchy->push_back(*new ruch(i, j, i1, j1));
			i1++;
			j1++;
			dodano = true;
		}
		i1 = i + 1;
		j1 = j - 1;
		while (i1 < N && j1 >=0)
		{
			if (pola[i1][j1] != 0)
				break;
			ruchy->push_back(*new ruch(i, j, i1, j1));
			i1++;
			j1--;
			dodano = true;
		}
		i1 = i - 1;
		j1 = j - 1;
		while (i1 >=0 && j1 >= 0)
		{
			if (pola[i1][j1] != 0)
				break;
			ruchy->push_back(*new ruch(i, j, i1, j1));
			i1--;
			j1--;
			dodano = true;
		}
		i1 = i - 1;
		j1 = j + 1;
		while (i1 >=0 && j1 <N)
		{
			if (pola[i1][j1] != 0)
				break;
			ruchy->push_back(*new ruch(i, j, i1, j1));
			i1--;
			j1++;
			dodano = true;
		}
	}
	return dodano;
}

void plansza::wykonaj_ruch(ruch r)
{
	if (pola[r.i1][r.j1] == 1 && r.i2 == N - 1)
		pola[r.i1][r.j1] = 3;
	else if(pola[r.i1][r.j1] == 2 && r.i2 == 0)
		pola[r.i1][r.j1] = 4;
	pola[r.i2][r.j2] = pola[r.i1][r.j1];
	pola[r.i1][r.j1] = 0;
	kolor[r.i1][r.j1] = kolory_e::kolor_podswietlenia_ruchu;
	kolor[r.i2][r.j2] = kolory_e::kolor_podswietlenia_ruchu;
}
void plansza::podswietl_ruch(ruch r)
{
	kolor[r.i1][r.j1] = kolory_e::kolor_podswietlenia_mozliwosci;
	kolor[r.i2][r.j2] = kolory_e::kolor_podswietlenia_mozliwosci;
}

bool plansza::czy_jest_bicie_damka(int i,int j)
{
	if (pola[i][j] < 3) return false;

	int i1 = i + 1, j1 = j + 1;
	while (i1 < N-1 && j1 < N-1)
	{
		if (pola[i1][j1] != 0)
		{
			if (pola[i1][j1] != pola[i][j] && pola[i1][j1] != pola[i][j] - 2 && pola[i1 + 1][j1 + 1] == 0) return true;
			else break;
		}
		i1++;
		j1++;
	}

	i1 = i + 1;
	j1 = j - 1;
	while (i1 < N-1 && j1 >= 1)
	{
		if (pola[i1][j1] != 0)
		{
			if (pola[i1][j1] != pola[i][j] && pola[i1][j1] != pola[i][j] - 2 && pola[i1 + 1][j1 - 1] == 0) return true;
			else break;
		}
		i1++;
		j1--;
	}
	i1 = i - 1;
	j1 = j - 1;
	while (i1 >= 1 && j1 >= 1)
	{
		if (pola[i1][j1] != 0)
		{
			if (pola[i1][j1] != pola[i][j] && pola[i1][j1] != pola[i][j] - 2 && pola[i1 - 1][j1 - 1] == 0) return true;
			else break;
		}
		i1--;
		j1--;
	}
	i1 = i - 1;
	j1 = j + 1;
	while (i1 >= 1 && j1 < N-1)
	{
		if (pola[i1][j1] != 0)
		{
			if (pola[i1][j1] != pola[i][j] && pola[i1][j1] != pola[i][j] - 2 && pola[i1 - 1][j1 + 1] == 0) return true;
			else break;
		}
		i1--;
		j1++;
	}
	return false;
}

void plansza::wykonaj_bicie(bicie b)
{
	kolor[b.i1][b.j1] = kolory_e::kolor_podswietlenia_ruchu;
	kolor[b.i2][b.j2] = kolory_e::kolor_podswietlenia_ruchu;
	for (int i = 0; i < b.zbite.size(); i++)
	{
		(*this)[b.zbite[i]] = 0;
	}
	if (pola[b.i1][b.j1] == 1 && b.i2 + 1 == N)
		pola[b.i1][b.j1] = 3;
	else if(pola[b.i1][b.j1] == 2 && b.i2 == 0)
		pola[b.i1][b.j1] = 4;
	pola[b.i2][b.j2] = pola[b.i1][b.j1];
	pola[b.i1][b.j1] = 0;
	
}

bool plansza::dodaj_bicie(int i, int j, vector<bicie>* bicia)
{
	bool zbite=false;
	int liczba_zbic_przed_dodaniem = bicia->size();
	int kolor_do_bicia1 = (pola[i][j] == 1 || pola[i][j] == 3) ? 2 : 1;
	int kolor_do_bicia2 = (pola[i][j] == 1 || pola[i][j] == 3) ? 4 : 3;
	if (pola[i][j] <= 2) // bicie pionkiem
	{
		// gora prawo
		if (i + 2 < N && j + 2 < N && pola[i + 2][j + 2] == 0 && (pola[i + 1][j + 1] == kolor_do_bicia1 || pola[i + 1][j + 1] == kolor_do_bicia2)) // czy nie wychodzi za zakres, czy jest wolne, czy nie zbija swojego koloru
		{
			bicie b(i, j, i + 2, j + 2);
			b.zbite.push_back(wspolrzedna(i + 1, j + 1));
			bicia->push_back(b);
			zbite = true;
		}
		// gora lewo
		if (i + 2 < N && j - 2 >= 0 && pola[i + 2][j - 2] == 0 && (pola[i + 1][j - 1] == kolor_do_bicia1 || pola[i + 1][j - 1] == kolor_do_bicia2)) // czy nie wychodzi za zakres, czy jest wolne, czy nie zbija swojego koloru
		{
			bicie b(i, j, i + 2, j - 2);
			b.zbite.push_back(wspolrzedna(i + 1, j - 1));
			bicia->push_back(b);
			zbite = true;
		}
		// dol lewo
		if (i - 2 >=0 && j -2 >=0 && pola[i - 2][j - 2] == 0 && (pola[i - 1][j - 1] == kolor_do_bicia1 || pola[i - 1][j - 1] == kolor_do_bicia2)) // czy nie wychodzi za zakres, czy jest wolne, czy nie zbija swojego koloru
		{
			bicie b(i, j, i - 2, j - 2);
			b.zbite.push_back(wspolrzedna(i - 1, j - 1));
			bicia->push_back(b);
			zbite = true;
		}
		// dol prawo
		if (i - 2 >=0 && j + 2 < N && pola[i - 2][j + 2] == 0 && (pola[i - 1][j + 1] == kolor_do_bicia1 || pola[i - 1][j + 1] == kolor_do_bicia2)) // czy nie wychodzi za zakres, czy jest wolne, czy nie zbija swojego koloru
		{
			bicie b(i, j, i - 2, j + 2);
			b.zbite.push_back(wspolrzedna(i - 1, j + 1));
			bicia->push_back(b);
			zbite = true;
		}
		
	}
	else // bicie damka
	{
		auto pomoc1 = [this,i,j,bicia,zbite](int dod_i, int dod_j) {
			int i1 = i + dod_i;
			int j1 = j + dod_j;
			while (i1 >= 1 && i1 < N - 1 && j1 >= 1 && j1 < N - 1)
			{

				if (pola[i1][j1] != 0) // damka idzie po skosie az nie dojdzie do jakiegos pionka
				{
					// jezeli to nie jest taki sam kolor i pole za jest puste
					if (pola[i1][j1] != pola[i][j] && pola[i1][j1] != pola[i][j] - 2 && pola[i1 + dod_i][j1 + dod_j] == 0)
					{
						vector<pair<bicie, bool>> bicia2;
						wspolrzedna pionek_bity(i1, j1);
						while (pola[i1 + dod_i][j1 + dod_j] == 0 && i1 + dod_i >= 0 && i1 + dod_i < N && j1 + dod_j >= 0 && j1 + dod_j < N)
						{
							bicie b(i, j, i1 + dod_i, j1 + dod_j);
							b.zbite.push_back(pionek_bity);
							plansza p2 = *this;
							p2.wykonaj_bicie(b);
							pair<bicie, bool> p(b, p2.czy_jest_bicie_damka(i1 + dod_i, j1 + dod_j));
							bicia2.push_back(p);

							i1 += dod_i;
							j1 += dod_j;
						}
						bool jest = false;
						for (int i = 0; i < bicia2.size(); i++)
						{
							if (bicia2[i].second)
								jest = true;
						}
						for (int i = 0; i < bicia2.size(); i++)
						{
							if (jest)
							{
								if (bicia2[i].second)
									bicia->push_back(bicia2[i].first);
							}
							else
							{
								bicia->push_back(bicia2[i].first);
							}
						}

						
					}
					break;
				}
				i1 += dod_i;
				j1 += dod_j;
			}
		};
		pomoc1( 1, 1);
		pomoc1(1, -1);
		pomoc1(-1, -1);
		pomoc1(-1, 1);
	}

	
	return liczba_zbic_przed_dodaniem != bicia->size();
}

