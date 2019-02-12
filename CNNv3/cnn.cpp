// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#include <fstream>
#include <iostream>
#include "cnn.h"
#include "activation.h"

namespace neural {

	ConvolutionalNeuralNetwork::ConvolutionalNeuralNetwork(unsigned int layers, double learning_rate):
		layers(layers),
		total_error(0),
		learning_rate(learning_rate),
		bias_update(true)
	{
		this->weights    = new math::collections::Matrix[this->layers];
		this->biases     = new math::collections::Matrix[this->layers];
		this->out_values = new math::collections::Matrix[this->layers + 1];
	}

	ConvolutionalNeuralNetwork::~ConvolutionalNeuralNetwork() {
		delete[] this->weights;
		delete[] this->biases;
		delete[] this->out_values;
	}

	void ConvolutionalNeuralNetwork::feedforward(const math::collections::Matrix & input, math::collections::Matrix & output) {
		math::collections::Matrix tmp;
		output = input;
		this->out_values[0] = output;
		for (unsigned int it = 0; it < this->layers; ++it) {
			tmp = weights[it];
			tmp.dot(output);
			tmp.matrix_add(biases[it]);
			tmp.apply_actv_function(sigmoid, 1);
			this->out_values[it + 1] = tmp;
			output = tmp;
		}
	}

	void ConvolutionalNeuralNetwork::feedbackward(const math::collections::Matrix & ideal) {
		math::collections::Matrix * biases_adjust = new math::collections::Matrix[this->layers];
		math::collections::Matrix * weights_adjust = new math::collections::Matrix[this->layers];
		math::collections::Matrix * deltas = new math::collections::Matrix[this->layers];
		// For output layer
		math::collections::Matrix x3, t, pout, mout, unpack, tmp_1, tmp_2, weight_tmp;

		// (x3 - t)
		t = ideal;
		x3 = out_values[layers];
		x3.matrix_sub(t);

		// f'_3(W3 * x2)
		pout = out_values[layers]; // x3
		mout = out_values[layers]; // x3
		mout.mul(-1.0).add(1); // (1 - out)
		pout.hadamard(mout); // out * (1 - out) = f'_3(W3 * x2)
		unpack = pout;

		deltas[layers - 1] = x3.hadamard(unpack); // (x3 - t) o f'_3(W3 * x2)

		// output weight adjust
		tmp_1 = deltas[layers - 1]; // delta3
		tmp_2 = out_values[layers - 1]; // x2
		biases_adjust[layers - 1] = tmp_1;
		weights_adjust[layers - 1] = tmp_1.dot(tmp_2.transpose()); 
		
		// For other layers
		for (unsigned int it = layers - 1; it > 0; --it) {
			weight_tmp = weights[it];

			// f'_x(Wx+1 * x_x)
			pout = out_values[it]; // x3
			mout = out_values[it]; // x3
			mout.mul(-1.0).add(1); // (1 - out)
			pout.hadamard(mout); // out * (1 - out) = f'_3(W3 * x2)
			unpack = pout;
		
			deltas[it - 1] = weight_tmp.transpose().dot(deltas[it]).hadamard(unpack);

			tmp_1 = deltas[it - 1];
			tmp_2 = out_values[it - 1];
			biases_adjust[it - 1] = tmp_1;
			weights_adjust[it - 1] = tmp_1.dot(tmp_2.transpose());
		}

		for (unsigned int it = 0; it < layers; ++it) {
			if(bias_update)
				biases[it].matrix_sub(biases_adjust[it].mul(learning_rate));
			weights[it].matrix_sub(weights_adjust[it].mul(learning_rate));
		}

		delete[] weights_adjust;
		delete[] biases_adjust;
		delete[] deltas;
	}

	double ConvolutionalNeuralNetwork::train(const math::collections::Matrix & input, const math::collections::Matrix & ideal) {
		// Feedforward
		math::collections::Matrix feedforward_output, tmp;
		this->feedforward(input, feedforward_output);
		double total_error = 0;
		feedforward_output.matrix_sub(ideal).sqr().mul(1.0 / 2.0);
		for (unsigned int i = 0; i < feedforward_output.cols*feedforward_output.rows; ++i)
			total_error += feedforward_output.matrix[i];
		// Feedbackward
		this->feedbackward(ideal);

		return total_error;
	}

	void ConvolutionalNeuralNetwork::print_details() {
		for (unsigned int i = 0; i < this->layers; ++i) {
			std::cout << "[*] Layer = " << i << std::endl;
			std::cout << "- Weights:" << std::endl;
			std::cout << this->weights[i] << std::endl << std::endl;
			std::cout << "- Biases:" << std::endl;
			std::cout << this->biases[i] << std::endl << std::endl;
		}
		std::cout << std::endl;
	}

	bool ConvolutionalNeuralNetwork::save(std::string filepath, std::string & msg) {
		std::fstream schema;
		schema.open(filepath.c_str(), std::ios::out);
		if (schema.good()) {
			for (unsigned int it = 0; it < layers; ++it) {
				weights[it].transpose();
				biases[it].transpose();
				if (it == 0) {
					schema << "Inputs: " << weights[it].rows << std::endl << std::endl;
				}
				schema << "Hidden: " << weights[it].rows << "x" << weights[it].cols << std::endl;
				schema << "Weights: ";
				for (unsigned int elem_it = 0; elem_it < weights[it].rows*weights[it].cols; ++elem_it) {
					schema << weights[it].matrix[elem_it] << " ";
				}
				schema << std::endl;
				schema << "Biases: ";
				for (unsigned int elem_it = 0; elem_it < biases[it].cols; ++elem_it) {
					schema << biases[it].matrix[elem_it] << " ";
				}
				if (it != (layers - 1)) {
					schema << std::endl << std::endl;
				}
				else {
					schema << std::endl;
				}
			}
			msg = "[+] Schema saved successfully";
			schema.close();
			return true;
		}
		else {
			msg = "[-] Can't open file: " + filepath;
			return false;
		}
	}

}
