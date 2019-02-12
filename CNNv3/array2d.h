// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#pragma once
#include "representable.h"

/*! \file array2d.h
	\brief Implementuje macierz dwuwymiarow¹ oraz operacje na macierzy dwuwymiarowej.
*/

namespace math {
	namespace collections {
		//! Klasa szablonowa reprezentuj¹ca tablicê dwuwymiarow¹.
		/*!
		Klasa szablonowa reprezentuj¹ca tablicê dwuwymiarow¹ przechowywuj¹c¹ dowolny typ.
		*/
		template <class T>
		class Array2d: public Representable {
		private:
			/** Wskazuje na to czy macierz jest transponowana. */
			bool transposed;
		public:
			/** Liczba wierszy tablicy. */
			unsigned int rows;
			/** Liczba kolumn tablicy. */
			unsigned int cols;
			/** WskaŸnik na alokowany dynamicznie wektor. */
			T * matrix;

			/**
			* Metoda implementuj¹ca wyœwietlanie macierzy w sposób czytelny.
			* @return Tekst reprezentuj¹cy macierz w czytelnym formacie.
			*/
			std::string asText() const override;

			/**
			* Konstruktor alokuj¹cy macierz 1x1 (domyœlny).
			*/
			Array2d();
			/**
			* Konstruktor alokuj¹cy macierz o wybranym wymiarze.
			* @param rows Liczba wierszy.
			* @param cols Liczba kolumn.
			*/
			Array2d(const unsigned int & rows, const unsigned int & cols);
			/**
			* Konstruktor alokuj¹cy macierz o wybranym wymiarze z zape³nieniem wartoœci¹ domyœln¹.
			* @param rows Liczba wierszy.
			* @param cols Liczba kolumn.
			* @param fill Wartoœæ domyœlna.
			*/
			Array2d(const unsigned int & rows, const unsigned int & cols, const T & fill);
			/**
			* Dekonstruktor zwalniaj¹cy alokowan¹ pamiêæ.
			*/
			~Array2d();
			/**
			* Metoda implementuj¹ca dodawanie do macierzy sta³ej.
			* @param value Sta³a
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & add(T value);
			/**
			* Metoda implementuj¹ca mno¿enie macierzy przez sta³¹
			* @param value Sta³a
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & mul(T value);
			/**
			* Metoda implementuj¹ca podniesienie ka¿dego elementu macierzy do kwadratu
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & sqr();
			/**
			* Metoda implementuj¹ca transponowanie macierzy
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & transpose();
			/**
			* Metoda implementuj¹ca mno¿enie macierzowe
			* @param other Obiekt typu Array2d
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & dot(Array2d<T> & other);
			/**
			* Metoda implementuj¹ca iloczyn Hadamarda https://en.wikipedia.org/wiki/Hadamard_product_(matrices)
			* @param other Obiekt typu Array2d
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & hadamard(Array2d<T> & other);
			/**
			* Metoda implementuj¹ca dodawanie dwóch macierzy element po elemencie.
			* @param other Obiekt typu Array2d
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & matrix_add(Array2d<T> & other);
			/**
			* Metoda implementuj¹ca odejmowanie dwóch macierzy element po elemencie.
			* @param other Obiekt typu Array2d
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & matrix_sub(const Array2d<T> & other);
			/**
			* Metoda implementuj¹ca nak³adanie na ka¿dy element macierzy wybran¹ funkcji (aktywacyjnej)
			* @param func - funkcja nak³adana na elementy
			* @param constant - sta³a
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & apply_actv_function(T(*func)(T value, int constant), int constant);
			/**
			* Metoda implementuj¹ca zmianê wymiaru macierzy
			* @param rows Nowa liczba wierszy
			* @param cols Nowa liczba kolumn
			* @param fill Wartoœæ któr¹ ma byæ zape³niona macierz
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & resize(int rows,  int cols, const T & fill);


			// Operatory
			/**
			* Operator przypisania macierzy
			* @param other Obiekt typu Array2d
			* @return Referencja na obecny objekt
			*/
			Array2d<T> & operator=(const Array2d & other);
			/**
			* Operator umo¿liwiaj¹cy wydobycie elementu z macierzy
			* @param idx_row indeks wiersza
			* @param idx_col indeks kolumny
			* @return Wybrany element macierzy
			*/
			T operator()(int idx_row, int idx_col);
		};

		template<class T>
		inline std::string Array2d<T>::asText() const {

			std::string ret;
			unsigned int idx_tmp;
			for (unsigned int r = 0; r< this->rows; ++r) {
				ret += "| ";
				for (unsigned int c = 0; c < this->cols; ++c) {
					idx_tmp = (this->transposed) ? c * this->rows + r : r * this->cols + c;
					ret += std::to_string(this->matrix[idx_tmp]) + " ";
				}

				if(r != this->rows-1)
					ret += "|\n";
				else
					ret += "|";
			}
			return ret;
		}
		
		template<class T>
		inline Array2d<T>::Array2d():
			rows(1),
			cols(1),
			transposed(false)
		{
			this->matrix = new T[1];
		}

		template<class T>
		inline Array2d<T>::Array2d(const unsigned int & rows, const unsigned int & cols) :
			rows(rows),
			cols(cols),
			transposed(false)
		{
			this->matrix = new T[rows*cols];
		}

		template<class T>
		inline Array2d<T>::Array2d(const unsigned int & rows, const unsigned int & cols, const T & fill) :
			rows(rows),
			cols(cols),
			transposed(false)
		{
			this->matrix = new T[rows*cols];
			for (unsigned int it = 0; it < rows*cols; ++it)
				this->matrix[it] = fill;
		}

		template<class T>
		inline Array2d<T>::~Array2d() {
			delete[] this->matrix;
		}

		template<class T>
		inline Array2d<T> & Array2d<T>::add(T value) {
			for (unsigned int it = 0; it < rows*cols; ++it)
				this->matrix[it] += value;
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::mul(T value) {
			for (unsigned int it = 0; it < rows*cols; ++it)
				this->matrix[it] *= value;
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::sqr() {
			for (unsigned int it = 0; it < this->rows*this->cols; ++it)
				this->matrix[it] *= this->matrix[it];
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::transpose() {
			this->transposed = (transposed) ? false:true;
			unsigned int tmp;
			tmp = this->cols;
			this->cols = this->rows;
			this->rows = tmp;
			return *this;
		}

		template<class T>
		inline Array2d<T> & Array2d<T>::dot(Array2d<T> & other) {
			if (this->cols != other.rows) {
				throw std::length_error(
					"Dot product error: shapes("
					+std::to_string(this->rows)+
					", "
					+ std::to_string(this->cols)+
					") and ("
					+ std::to_string(other.rows)+
					", "
					+ std::to_string(other.cols)+
					") not aligned: "
					+ std::to_string(this->cols)+
					" (dim 1) != "
					+ std::to_string(other.rows)+
					" (dim 0)"
				);
			}

			unsigned int idx_tmp_this, idx_tmp_other;
			T * tmp_mat = new T[this->rows*other.cols];
			unsigned int c = 0;
			for (unsigned int i = 0; i < this->rows; ++i) {
				for (unsigned int j = 0; j < other.cols; ++j) {
					T tmp_sum = 0;
					for (unsigned int k = 0; k < this->cols; ++k) {
						idx_tmp_this = (this->transposed) ? k * this->rows + i : i * this->cols + k;
						idx_tmp_other = (other.transposed) ? j * other.rows + k : k * other.cols + j;
						tmp_sum += this->matrix[idx_tmp_this] * other.matrix[idx_tmp_other];
					}
					tmp_mat[c] = tmp_sum;
					++c;
				}
			}
				
			this->cols = other.cols;
			delete[] this->matrix;
			this->matrix = tmp_mat;

			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::hadamard(Array2d<T>& other) {
			if ((this->rows != other.rows) || (this->cols != other.cols)) {
				throw std::length_error(
					"Hadamard product error: shapes("
					+ std::to_string(this->rows) +
					", "
					+ std::to_string(this->cols) +
					") and ("
					+ std::to_string(other.rows) +
					", "
					+ std::to_string(other.cols) +
					") are not the same"
				);
			}
				
			unsigned int idx_tmp_this, idx_tmp_other;
			for (unsigned int r = 0; r < this->rows; ++r) {
				for (unsigned int c = 0; c < this->cols; ++c) {
					idx_tmp_this = (this->transposed) ? c * this->rows + r : r * this->cols + c;
					idx_tmp_other = (other.transposed) ? c * other.rows + r : r * other.cols + c;
					this->matrix[idx_tmp_this] *= other.matrix[idx_tmp_other];
				}
			}
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::matrix_add(Array2d<T>& other) {
			if ((this->rows != other.rows) || (this->cols != other.cols)) {
				throw std::length_error(
					"Matrix addition error: shapes("
					+ std::to_string(this->rows) +
					", "
					+ std::to_string(this->cols) +
					") and ("
					+ std::to_string(other.rows) +
					", "
					+ std::to_string(other.cols) +
					") are not the same"
				);
			}

			unsigned int idx_tmp_this, idx_tmp_other;
			for (unsigned int r = 0; r < this->rows; ++r) {
				for (unsigned int c = 0; c < this->cols; ++c) {
					idx_tmp_this = (this->transposed) ? c * this->rows + r : r * this->cols + c;
					idx_tmp_other = (other.transposed) ? c * other.rows + r : r * other.cols + c;
					this->matrix[idx_tmp_this] += other.matrix[idx_tmp_other];
				}
			}
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::matrix_sub(const Array2d<T>& other) {
			if ((this->rows != other.rows) || (this->cols != other.cols)) {
				throw std::length_error(
					"Matrix subtraction error: shapes("
					+ std::to_string(this->rows) +
					", "
					+ std::to_string(this->cols) +
					") and ("
					+ std::to_string(other.rows) +
					", "
					+ std::to_string(other.cols) +
					") are not the same"
				);
			}

			unsigned int idx_tmp_this, idx_tmp_other;
			for (unsigned int r = 0; r < this->rows; ++r) {
				for (unsigned int c = 0; c < this->cols; ++c) {
					idx_tmp_this = (this->transposed) ? c * this->rows + r : r * this->cols + c;
					idx_tmp_other = (other.transposed) ? c * other.rows + r : r * other.cols + c;
					this->matrix[idx_tmp_this] -= other.matrix[idx_tmp_other];
				}
			}
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::apply_actv_function(T(*func)(T value, int constant), int constant) {
			for (unsigned int it = 0; it < this->rows*this->cols; ++it) {
				this->matrix[it] = func(this->matrix[it], constant);
			}
			return *this;
		}

		template<class T>
		inline Array2d<T>& Array2d<T>::resize(int rows, int cols, const T & fill) {
			delete[] this->matrix;
			this->matrix = new T[rows*cols];
			this->rows = rows;
			this->cols = cols;
			this->transposed = false;
			for (int it = 0; it < rows*cols; ++it)
				this->matrix[it] = fill;
			return *this;
		}

		template<class T>
		inline Array2d<T> & Array2d<T>::operator=(const Array2d & other) {
			if (&other == this)
				return *this;
			this->cols = other.cols;
			this->rows = other.rows;
			this->transposed = other.transposed;
			delete[] this->matrix;
			this->matrix = new T[other.cols*other.rows];
			for (unsigned int it = 0; it < other.cols*other.rows; ++it)
				this->matrix[it] = other.matrix[it];
			return *this;
		}

		template<class T>
		inline T Array2d<T>::operator()(int idx_row, int idx_col) {
			return this->matrix[idx_row * this->cols + idx_col];
		}

		//! Klasa specjalizowana dla elementów typu double
		/*!
		Klasa szablonowa reprezentuj¹ca tablicê dwuwymiarow¹ przechowywuj¹c¹ typ double.
		*/
		class Matrix : public Array2d<double> {
		public:
			using Array2d::operator=;
			/**
			* Konstruktor alokuj¹cy macierz 1x1 (domyœlny).
			*/
			Matrix();
			/**
			* Konstruktor alokuj¹cy macierz o wybranym wymiarze.
			* @param rows Liczba wierszy.
			* @param cols Liczba kolumn.
			*/
			Matrix(const unsigned int & rows, const unsigned int & cols);
			/**
			* Konstruktor alokuj¹cy macierz o wybranym wymiarze z zape³nieniem wartoœci¹ domyœln¹.
			* @param rows Liczba wierszy.
			* @param cols Liczba kolumn.
			* @param fill Wartoœæ domyœlna.
			*/
			Matrix(const unsigned int & rows, const unsigned int & cols, const double & fill);
		};


} // namespace math
} // namespace collections