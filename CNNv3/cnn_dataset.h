// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#pragma once
#include <fstream>
#include <string>
#include "array2d.h"

/*! \file cnn_dataset.h
	\brief Implementuje ³adowanie danych i ich walidacje
*/

namespace neural {

	/**
	* Funkcja zape³niaj¹ca macierze danymi wejœciowymi i wyjœciowymi z wektora
	* @param inputs WskaŸnik na dane wejœciowe
	* @param outputs WskaŸnik na dane wyjœciowe
	* @param mat_num Iloœæ weirszy danych
	* @param inputs_col Liczba kolumn w macierzy warstwy wejœciowej
	* @param outputs_col Liczba kolumn w macierzy warstwy wyjœciowej
	* @param training_data Wektor danych treningowych
	* @return void
	*/
	void populate_matrices(
		math::collections::Matrix * inputs,
		math::collections::Matrix * outputs,
		int mat_num,
		int inputs_col,
		int outputs_col,
		std::vector<double> & training_data
	);

	//! Klasa reprezentuj¹ca zbiór danych wejœciowych
	/*!
	Klasa reprezentuj¹ca zbiór danych wejœciowych w formacie csv oraz odpowiedzialna za ich walidacje
	*/
	class CNNDataset {
		public:
			/** Uchwyt do pliku z danymi */
			std::fstream file_handle;
			/** Œcie¿ka do pliku z danymi */
			std::string filepath;
			/** WskaŸnik na tablicê zawieraj¹c¹ nag³ówki danych */
			std::string * headers;
			/** Liczba za³adowanych linii danych */
			int datalines;
			/** Spodziewana liczba kolumn pliku csv */
			int expected_data_num;
			/**
			* Konstruktor
			* @param filepath Œcie¿ka do pliku
			* @param expected_data_num Oczekiwana iloœæ danych (kolumn pliku csv)
			* @return void
			*/
			CNNDataset(std::string filepath, int expected_data_num);
			/**
			* Destruktor klasy. Zwalnia pamiêæ.
			*/
			~CNNDataset();
			/**
			* Metoda odpowiedzialna za ³adowanie danych z pliku i ich walidacje
			* @param msg Wiadomoœæ wyœwietlana u¿ytkownikowi po zakoñczeniu dzia³ania funkcji
			* @param data_vector Wektor do którego zostan¹ za³adowane zwalidowane dane
			* @return Pradwa jeœli walidacja przebieg³a pomyœlnie. Fa³sz w przeciwnym razie.
			*/
			bool validate(std::string & msg, std::vector<double> & data_vector);
	};

} // namespace neural