// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#pragma once
#include <string>
#include <sstream>
#include <map>

/*! \file argparse.h
	\brief Obs³uga argumentów konsolowych

	Plik implementuje "parser" argumentów konsolowych.
*/

namespace argparse {

	/** Typ wyliczeniowy.
	*  Przechowywuje kody typów danych dodawanych argumentów.
	*/
	enum ArgType {
		/** Wskazuje na to, ¿e argument jest typu string */
		tString = 1,
		/** Wskazuje na to, ¿e argument jest typu int */
		tInt = 2,
		/** Wskazuje na to, ¿e argument jest typu double */
		tDouble = 3, 
		/** Wskazuje na to, ¿e argument jest typu bool (domyœlnie prawda) */
		tStoreTrue = 4 
	};
	/** Typ wyliczeniowy.
	*  Przechowywuje kody typów wa¿noœci dodawanych argumentów.
	*/
	enum ArgImp {
		/** Wskazuje na to, ¿e argument jest pozycyjny */
		iPositional = 21,
		/** Wskazuje na to, ¿e argument jest opcjonalny */
		iOptional = 22 
	};
	/** Typ wyliczeniowy.
	*  Przechowywuje kody inforumuj¹ce o tym czy argument powoduje zaprzestanie wczytywania argumentów pozycyjnych.
	*/
	enum ArgDrop {
		/** Wskazuje na to, ¿e argument powoduje zaprzestanie wczytywania argumentów pozycyjnych. */
		dDrop = 31,
		/** Wskazuje na to, ¿e argument NIE powoduje zaprzestania wczytywania argumentów pozycyjnych. */
		dNone = 32 
	};
	//! Klasa przechowywuj¹ca argument konsolowy.
	/*!
	Klasa odpowiadaj¹ca za przechowywanie argumentów przekazanych przez konsolê wraz z dodatkowymi informacjami.
	*/
	struct Argument {
		/** Pozycja argumentu w wektorze. */
		int position;
		/** Nazwa nadana argumentowi. */
		std::string name;
		/** Tekst pomocy dla argumentu.  */
		std::string help;
		/** Typ danych argumentu. */
		ArgType arg_type;
		/** Typ wa¿noœci argumentu. */
		ArgImp arg_imp;
		/** Element typu wyliczeniowego ArgDrop */
		ArgDrop arg_drop;
	};

	//! Klasa obs³uguj¹ca argumenty konsolowe.
	/*!
	Klasa odpowiadaj¹ca za obs³ugê i interpretacje argumentów przekazanych przez konsolê.
	*/
	class ArgumentParser {
	private:
		/** Liczba dodanych argumentów. */
		int _arg_count; 
		/** Liczba dodanych argumentów opcjonalnych. */
		int _arg_count_optional;
		/** Liczba dodanych argumentów pozycyjnych (obowi¹zkowych). */
		int _arg_count_positional; 
		/** Maksymalna dozwolona d³ugoœæ nazwy argumentu. */
		int max_arg_name_len;
		/** Ignoruj argumenty pozycyjne. */
		bool drop_positionals;
		/** WskaŸnik na tablicê objektów typu Argument. */
		Argument * _arg_ptr;
		/** Opis programu. */
		std::string description;
		/** Obecna nazwa pliku wykonywalnego. */
		std::string exec_name;
		/** Mapa przechowywuj¹ca argumenty konsolowe typu string. */
		std::map<std::string, std::string> map_string;
		/** Mapa przechowywuj¹ca argumenty konsolowe typu int. */
		std::map<std::string, int> map_int;
		/** Mapa przechowywuj¹ca argumenty konsolowe typu double. */
		std::map<std::string, double> map_double;
		/** Mapa przechowywuj¹ca argumenty konsolowe typu bool. */
		std::map<std::string, bool> map_bool;
	public:

		/**
		* Metoda mapuj¹ca argument do odpowiedniego pola klasy.
		* @param str Nazwa argumentu.
		* @param obj Objekt klasy Argument.
		* @see Argument
		* @see map_string()
		* @see map_int()
		* @see map_double()
		* @see map_bool()
		* @return void
		*/
		void arg_convert(std::string str, Argument obj);
		/**
		* Konstruktor klasy ArgumentParser.
		* @param description opis programu.
		* @param argc Liczba argumentów przekazanych do konsoli.
		* @param argv Wektor zawieraj¹cy argumenty przekazane do konsoli.
		*/
		ArgumentParser(std::string description, int argc, char * argv[]);
		/**
		* Dekonstruktor klasy ArgumentParser. Zwalnia pamiêæ na któr¹ wskazuje _arg_ptr()
		*/
		~ArgumentParser();
		/**
		* Metoda mapuj¹ca argument do odpowiedniego pola klasy.
		* @param argc Liczba argumentów przekazanych przez konsole.
		* @param argv Wektor przechowywuj¹cy argumenty przekazane przez konsole.
		* @return Prawda gdy argumenty zosta³y wczytane poprawnie. Fa³sz w przeciwnym razie.
		*/
		bool parse_args(int argc, char * argv[]);
		/**
		* Wyœwietla zawartoœæ map na ekran.
		* @return void
		*/
		void print_maps();
		/**
		* Wyœwietla pomoc.
		* @param only_usage Wyœwietla tylko sposób uzycia.
		* @return void
		*/
		void print_help(bool only_usage = false);
		/**
		* Wyœwietla wiadomoœci o b³êdach zwi¹zane z argumentami pozycyjnymi.
		* @return void
		*/
		void print_positional_argument_err_msg();
		/**
		* Dodaje nowy argument.
		* @param name Nazwa argumentu.
		* @param help Tekst pomocy dla argumentu.
		* @param tType Typ danych argumentu.
		* @param tImp Typ wa¿noœci argumentu.
		* @param tDrop Zobacz ArgDrop().
		* @return void
		*/
		void add_argument(std::string name, std::string help, ArgType tType = tString, ArgImp tImp = iOptional, ArgDrop tDrop = dNone);
		
		/**
		* £aduje wartoœæ argumentu do zmiennej. (przeci¹¿enie dla typu string)
		* @param name Nazwa argumentu.
		* @param target Zmienna do której ma zostaæ wczytana wartoœæ argumentu.
		* @return Prawda jeœli argument istnieje. Fa³sz w przeciwnym razie.
		*/
		inline bool get_arg(std::string name, std::string & target);
		/**
		* £aduje wartoœæ argumentu do zmiennej. (przeci¹¿enie dla typu int)
		* @param name Nazwa argumentu.
		* @param target Zmienna do której ma zostaæ wczytana wartoœæ argumentu.
		* @return Prawda jeœli argument istnieje. Fa³sz w przeciwnym razie.
		*/
		inline bool get_arg(std::string name, int & target);
		/**
		* £aduje wartoœæ argumentu do zmiennej. (przeci¹¿enie dla typu double)
		* @param name Nazwa argumentu.
		* @param target Zmienna do której ma zostaæ wczytana wartoœæ argumentu.
		* @return Prawda jeœli argument istnieje. Fa³sz w przeciwnym razie.
		*/
		inline bool get_arg(std::string name, double & target);
		/**
		* £aduje wartoœæ argumentu do zmiennej. (przeci¹¿enie dla typu bool)
		* @param name Nazwa argumentu.
		* @param target Zmienna do której ma zostaæ wczytana wartoœæ argumentu.
		* @return Prawda jeœli argument istnieje. Fa³sz w przeciwnym razie.
		*/
		inline bool get_arg(std::string name, bool & target);
	};

	inline bool ArgumentParser::get_arg(std::string name, std::string & target) {
		target = (map_string.count(name)) ? map_string[name] : "";
		return (target != "") ? true : false;
	}

	inline bool ArgumentParser::get_arg(std::string name, int & target) {
		target = (map_int.count(name)) ? map_int[name] : 0;
		return (target != 0) ? true : false;
	}

	inline bool ArgumentParser::get_arg(std::string name, double & target) {
		target = (map_double.count(name)) ? map_double[name] : .0;
		return (target != .0) ? true : false;
	}

	inline bool ArgumentParser::get_arg(std::string name, bool & target) {
		if (map_bool.count(name)) {
			target = map_bool[name];
			return true;
		} 
		return false;
	}

} // namespace argparse
