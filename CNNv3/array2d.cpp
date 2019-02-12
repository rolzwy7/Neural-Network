// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#include "array2d.h"
#include <string>

namespace math {
	namespace collections {
		Matrix::Matrix() :Array2d<double>()
		{}

		Matrix::Matrix(const unsigned int & rows, const unsigned int & cols) :
			Array2d<double>(rows, cols)
		{}

		Matrix::Matrix(const unsigned int & rows, const unsigned int & cols, const double & fill) :
			Array2d<double>(rows, cols, fill)
		{}


	} // namespace collections
} // namespace math
