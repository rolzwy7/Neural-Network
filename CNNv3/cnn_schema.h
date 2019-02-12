// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#pragma once
#include <string>
#include <vector>

/*! \file cnn_schema.h
	\brief Implementuje objekty obs³uguj¹ce pliki zawieraj¹ce schemat sieci
*/

namespace neural {

	//! Klasa reprezentuj¹ca plik schematu
	/*!
	Klasa reprezentuj¹ca plik schematu oraz implementuj¹ca jego ³adowanie i walidacjê.
	*/
	class CNNSchemaFile {
	public:
		/** Nazwa pliku schematu */
		std::string filename;
		/** Zawartoœæ pliku schematu */
		std::string content;
		/** Liczba wejœæ sieci */
		int inputs;
		/** Liczba wyjœæ sieci */
		int outputs;
		/**
		* Konstruktor
		* @param filename Œcie¿ka do pliku schematu
		*/
		CNNSchemaFile(const std::string & filename);
		/**
		* Metoda ³aduj¹ca zawartoœæ pliku schematu do pola content().
		* Wykonywana jest tak¿e wstêpna walidacja bêd¹ca wstêpem do walidacji szczegó³owej wykonywaniej przez metodê validate().
		* @param msg Wiadomoœæ wyœwietlana u¿ytkownikowi
		* @return Prawda jeœli plik zosta³ poprawnie za³adowany. Fa³sz w przeciwnym razie.
		*/
		bool load(std::string & msg);
		/**
		* Metoda wykonywuj¹ca szczegó³ow¹ walidacjê plik schematu i dystrybucje danych do wektorów wyjœciowych
		* @param reason Wiadomoœæ wyœwietlana u¿ytkownikowi
		* @param ref_layers Wczytana z pliku liczba warstw sieci przekazywana przez referencje
		* @param ref_hidden_r Wczytane z pliku informacje o wierszach warstw ukrytych
		* @param ref_hidden_c Wczytane z pliku informacje o kolumnach warstw ukrytych
		* @param hidden_weights Wczytane z pliku informacje o wagach warstw ukrytych
		* @param hidden_biases Wczytane z pliku informacje o obci¹¿eniach warstw ukrytych
		* @return Prawda jeœli walidacja przebieg³a pomyœlnie. Fa³sz w przeciwym razie.
		*/
		bool validate(
			std::string & reason,
			int & ref_layers,
			std::vector<int> & ref_hidden_r,
			std::vector<int> & ref_hidden_c,
			std::vector<double> & hidden_weights,
			std::vector<double> & hidden_biases
		);
	};

	//! Klasa generatora schematu
	/*!
	Klasa implementuj¹ca generator pliku schematu sieci o wymiarach podanych przez u¿ytkownika
	*/
	class CNNSchemaGenerator {
	public:
		/** Ci¹g tekstowy zawieraj¹cy w sobie kod schematu */
		std::string target_str;
		/**
		* Konstruktor
		* @param code Kod schematu
		*/
		CNNSchemaGenerator(std::string code);
		/**
		* Metoda generuj¹ca plik schematu i wyœwietlaj¹ca go w konsoli.
		* @return Prawda jeœli uda³o siê wygenerowaæ plik schematu. Fa³sz w przeciwnym razie.
		*/
		bool generate();
	};

} // namespace neural