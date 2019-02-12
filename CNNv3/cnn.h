// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#pragma once
#include <string>
#include "array2d.h"

/*! \file cnn.h
	\brief Plik implementuje splotow¹ sieæ neuronow¹
*/

namespace neural {

	//! Klasa reprezentuj¹ca splotow¹ sieæ neuronow¹
	/*!
		Klasa reprezentuj¹ca splotow¹ sieæ neuronow¹ oraz metody umo¿liwiaj¹ce jej wytrenowanie.
	*/
	class ConvolutionalNeuralNetwork {
		
	public:
		/** Czy aktualizowaæ obci¹¿enia */
		bool bias_update;
		/** Iloœæ warstw sieci */
		unsigned int layers;
		/** Ca³kowity b³¹d sieci */
		double total_error;
		/** Szybkoœæ uczenia siê */
		double learning_rate;
		/** WskaŸnik na tablicê wag */
		math::collections::Matrix * weights;
		/** WskaŸnik na tablicê obci¹¿eñ */
		math::collections::Matrix * biases;
		/** WskaŸnik na tablicê wartoœci wyjœciowych neuronów */
		math::collections::Matrix * out_values;

		/**
		* Konstruktor
		* @param layers Iloœæ warstw sieci
		* @param learning_rate Szybkoœæ uczenia siê
		*/
		ConvolutionalNeuralNetwork(unsigned int layers, double learning_rate);
		/**
		* Deksonstruktor. Zwalnia alokowan¹ pamiêæ.
		*/
		~ConvolutionalNeuralNetwork();
		/**
		* Algorytm "feedforward" (karmienie sieci do przodu)
		* @param input Dane wejœciowe
		* @param output Dane wynikowe
		* @return void
		*/
		void feedforward(const math::collections::Matrix & input, math::collections::Matrix & output);
		/**
		* Algorytm "feedbackward" (karmienie sieci od ty³u)
		* @param ideal Dane "idealne"
		* @return void
		*/
		void feedbackward(const math::collections::Matrix & ideal);
		/**
		* Trenuje sieæ na podstawie pojedyñczego rekordu danych
		* @param input Dane wejœciowe
		* @param ideal Dane "idealne"
		* @return Ca³kowity b³¹d sieci
		* @see feedforward()
		* @see feedbackward()
		*/
		double train(const math::collections::Matrix & input, const math::collections::Matrix & ideal);
		/**
		* Wyœwietla na ekran budowê sieci
		* @return void
		*/
		void print_details();
		/**
		* Zapisuje plik schematu sieci do pliku
		* @param filepath Œcie¿ka do pliku
		* @param msg Wiadomoœæ wyœwietlana u¿ytkownikowi
		* @return Prawda jeœli uda³o siê zapisaæ plik schematu. Fa³sz w przeciwnym razie.
		*/
		bool save(std::string filepath, std::string & msg);
	};
}
