#include <iostream>
#include <Windows.h>
#include <time.h>
#include <list>
#include <cassert>
#include <iomanip>
#include <cmath>

#define N 3
#define M 3

using namespace std;

template<typename T>
struct lista {
	T pole;
	lista* nast{ NULL };
};

template<typename T>
class Konten
{
	lista<T>* glowa{ NULL };
	lista<T>* ogon{ NULL };
	int top;
public:
	Konten() : top(0) {}
	void push(T i) {
		if (glowa == NULL) {
			glowa = new lista<T>;
			glowa->pole = i;
			glowa->nast = NULL;
			ogon = glowa;
		}
		else {
			ogon->nast = new lista<T>;
			ogon = ogon->nast;
			ogon->pole = i;
			ogon->nast = NULL;
		}
		top++;
	};
	void pop() {
		if (glowa == NULL) return;
		if (glowa->nast == NULL) {
			T i = glowa->pole;
			delete glowa;
			top--;
			return;
		}
		lista<T>* tmp = glowa;
		while (tmp->nast != ogon)
			tmp = tmp->nast;
		ogon = tmp;
		delete ogon->nast;
		ogon->nast = NULL;
		top--;
	};
	int rozmiar() { return top; };

	void draw() {
		if (glowa == NULL) return;
		lista<T>* tmp = glowa;
		while (tmp != NULL) {
			cout << setw(10) << tmp->pole << " ";
			tmp = tmp->nast;
		}
		cout << endl;
	}


	class iterator {
		Konten<T> s;
		lista<T>* suwak;
	public:
		iterator(Konten& is) : s(is), suwak(s.glowa) {};
		iterator(Konten& is, bool) : s(is), suwak(s.ogon->nast) {};
		lista<T>* operator++() { // Prefix
			suwak = suwak->nast;
			return suwak;
		};
		lista<T>* operator++(int) { // Postfix
			lista<T>* tmp2 = suwak;
			suwak = suwak->nast;
			return tmp2;
		};
		T& operator*() const { return suwak->pole; };
		iterator& operator=(const iterator& rv) {
			suwak = rv.suwak;
			s = rv.s;
			return *this;
		}
		bool operator!=(const iterator& obj) {
			if (suwak != obj.suwak) {
				return true;
			}
			return false;
		}
		bool operator>(const iterator& obj) {
			if (suwak->pole > obj.suwak->pole) return true;
			return false;
		}
	};
	iterator begin() { return iterator(*this); };
	iterator end() { return iterator(*this, true); };

	friend class iterator;
};

template <typename T>
void Roznicuj1(typename T::iterator i1, typename T::iterator i2, typename T::iterator i3) {
	while (i1 != i2) {
		*i1 = fabs(*i3 - *i1);
		i1++;
		i3++;
	}
}
template <typename T>
void Roznicuj2(typename T::iterator i1, typename T::iterator i2, double D) {
	while (i1 != i2) {
		*i1 = *i1 - D;
		i1++;
	}
}
template <typename T>
void DoKwadratu(typename T::iterator i1, typename T::iterator i2) {
	while (i1 != i2) {
		*i1 = *i1 * (*i1);
		i1++;
	}
}
template<typename U>
double obliczT(typename Konten<U> pierwszy,typename Konten<U> q) {
	Konten<U> p;
	for (auto it{ pierwszy.begin() }; it != pierwszy.end(); it++) {
		p.push(*it);
	}

	Roznicuj1<Konten<double>>(p.begin(), p.end(), q.begin());
	double D = 0;
	for (auto it_p = p.begin(); it_p != p.end(); it_p++) {
		D += *it_p / N;
	}
	Roznicuj2<Konten<double>>(p.begin(), p.end(), D);
	DoKwadratu<Konten<double>>(p.begin(), p.end());
	double S = 0;
	for (auto it_p = p.begin(); it_p != p.end(); it_p++) {
		S += *it_p / (N - 1.0);
	}
	S = sqrt(S);
	double T = D / (S / sqrt(N));
	return T;
}

double randdouble()
{
	return rand() / (double(RAND_MAX) + 1) * 100;
}

template <typename T>
class pomiary {
public:
	Konten<T> przed;
	Konten<T> po;
};
template<typename T>
class Kalkulator {
public:
	template <typename T>
	static void testtStudenta(typename Konten<pomiary<T>>::iterator pierwszy,
		typename Konten<pomiary<T>>::iterator drugi) {
		auto tmp = pierwszy;
		pomiary<T> stala = *tmp;
		pomiary<T> a = *tmp;
		double najmT = obliczT<double>(a.przed, a.po);
		tmp++;
		while (tmp != drugi) {
			a = *tmp;
			stala = *tmp;
			if (najmT > obliczT<double>(a.przed, a.po)) {
				najmT = obliczT<double>(a.przed, a.po);
				stala = *tmp;
			}
			tmp++;
		}
		*pierwszy = stala;
	}
};


int main()
{
	srand(time(NULL));

	Konten<pomiary<double>> p;
	auto it_p{ p.begin() };

	pomiary<double> a;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			a.po.push(randdouble());
			a.przed.push(randdouble());
		}
		p.push(a);
	}
	Kalkulator<double> kalk;
	
	kalk.testtStudenta<double>(p.begin(), p.end());

	a = *p.begin();
	cout << "Najmniejsza wartosc T wystepuje dla pary:\n";
	a.przed.draw();
	a.po.draw();
}