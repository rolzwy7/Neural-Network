// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#pragma once
#include <string>

/*! \file representable.h
	\brief Zawiera klasê czysto wirtualn¹ wspomagaj¹c¹ wyœwietlanie obiektów innych klas w konsoli.
*/

//! Klasa wirtualna implementuj¹ca wyœwietlanie macierzy
/*!
Klasa odpowiadaj¹ca za wyœwietlanie macierzy w czytelny sposób.
*/
class Representable {
public:
	/**
	* Czysto wirtualny dekonstruktor
	*/
	virtual ~Representable() {}
	/**
	* Wirtualna metoda zwracaj¹ca macierz jako tekst
	* @return Tekst reprezentuj¹cy macierz w czytelnym formacie.
	*/
	virtual std::string asText() const = 0;
};
/**
* Przeci¹zenie operatora wyjœcia strumienia dla klasy dziedzicz¹cej po klasie Representable
* @param stream Strumieñ wyjœciowy.
* @param printable Sta³a referencja na objekt klasy Representable
* @return Strumieñ wyjœciowy.
*/
std::ostream & operator<<(std::ostream & stream, const Representable & printable);