// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#include "activation.h"
#include <math.h>
#include <random>

double random_double(double a, double b) {
	static std::default_random_engine gen(std::random_device{}());
	static std::uniform_real_distribution<double> dist;
	return dist(gen, std::uniform_real_distribution<double>::param_type{ a, b });
}

double get_random_weight() {
	return random_double(0.01, 0.99);
}

double sigmoid(double value, int constant) {
	return 1.0 / ( 1.0 + exp(-value * constant) );
}
