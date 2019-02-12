// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "activation.h"
#include "cnn_schema.h"

namespace neural {
	CNNSchemaFile::CNNSchemaFile(const std::string & filename):
		filename(filename),
		content(""),
		inputs(0)
	{}

	bool CNNSchemaFile::load(std::string & msg) {
		std::fstream file(filename, std::ios::in);
		std::string tmp;
		if (!file.good()) {
			msg = "[-] Can't open schema file";
			return false;
		}
		int line_count = 0;
		while (!file.eof()) {
			std::getline(file, tmp);
			++line_count;
			std::regex validate_line(
			"^([ ]+|)((Input[s]{0,1}:[ ]+[1-9]+)|(Bias(es){0,1}:|Weight[s]{0,1}:)([ ]+[-]{0,}[0-9]+.[0-9]+)+|(Hidden:[ ]+[1-9]+x[1-9]+))([ ]+|)$"
			);
			std::cmatch m;
			if (std::regex_match(tmp.c_str(), m, validate_line)) {
				content += tmp + "\n";
			} else {
				std::istringstream iss(tmp);
				std::string tmp_str;
				iss >> tmp_str;
				if ( 
					(tmp_str == "Inputs:")  || 
					(tmp_str == "Input:")   ||
					(tmp_str == "Bias:")    ||
					(tmp_str == "Biases:")  ||
					(tmp_str == "Weight:")  ||
					(tmp_str == "Weights:") ||
					(tmp_str == "Hidden:")
				) {
					std::ostringstream oss;
					oss << "[*] Potential problem in line " << line_count << " of schema file:"
						<< std::endl << tmp << std::endl << std::endl;
					msg = oss.str();
					return false;
				}

			}
		}
		content += "Hidden: 1x1\n";
		return true;
	}

	bool CNNSchemaFile::validate(
		std::string & reason,
		int & ref_layers,
		std::vector<int> & ref_hidden_r,
		std::vector<int> & ref_hidden_c,
		std::vector<double> & hidden_weights,
		std::vector<double> & hidden_biases
	) {
		ref_layers = 0;
		std::istringstream iss(content);
		std::string tmp;
		char state = 'S', prev_state, check = 'N';

		int current_hidden_dim = 0;
		int current_inputs_col_count = 0;
		int current_weights_count = 0;
		int current_biases_count = 0;
		int current_hidden_rows = 0;
		int current_hidden_cols = 0;

		int inputs_declarations = 0;
		int biases_declarations = 0;
		int weights_declarations = 0;
		int hidden_declarations = -1;

		// Check if first hidden layer is compatible with inputs number
		bool checked_first_hidden = false;
		bool checked_hidden_break = false;

		while (iss >> tmp) {
			if (checked_hidden_break) {
				reason = "inputs rows != hidden rows";
				return false;
			}
			/// Order -> return invalid
			if (check == 'Y') {
				if (prev_state == 'S' && state != 'I') {
					reason = "'Inputs' needs to be declared on the top of the schema file";
					return false;
				}
				if (prev_state == 'I' && state != 'H') {
					reason = "'Hidden' declaration needs to follow 'Inputs' declaration";
					return false;
				}
				if (prev_state == 'H' && state != 'W') {
					reason = "'Weights' declaration needs to follow 'Hidden' declaration";
					return false;
				}
				if (prev_state == 'W' && state != 'B') {
					reason = "'Biases' declaration needs to follow 'Weights' declaration";
					return false;
				}
			}
			/// Number of declarations -> return invalid
			if (inputs_declarations > 1) {
				reason = "Too many 'Inputs' declarations";
				return false;
			}
			/// Headers
			if ((tmp == "Inputs:") || (tmp == "Input:")) {
				prev_state = state;
				check = 'Y';
				state = 'I';
				++inputs_declarations;
			} else if ((tmp == "Bias:") || (tmp == "Biases:")) {
				prev_state = state;
				check = 'Y';
				state = 'B';
				++biases_declarations;
			} else if ((tmp == "Weight:") || (tmp == "Weights:")) {
				prev_state = state;
				check = 'Y';
				state = 'W';
				++weights_declarations;
			} else if (tmp == "Hidden:") {
				prev_state = state;
				check = 'Y';
				state = 'H';
				++hidden_declarations;
			} else {
				check = 'N';
				/// Values
				if(state == 'I') {
					std::istringstream tmp_iss(tmp);
					tmp_iss >> inputs;
					current_inputs_col_count = inputs;
				} else if(state == 'B') {
					std::istringstream tmp_iss(tmp);
					double tmp_biase;
					tmp_iss >> tmp_biase;
					hidden_biases.push_back(tmp_biase);
					++current_biases_count;
				} else if (state == 'W') {
					std::istringstream tmp_iss(tmp);
					double tmp_weight;
					tmp_iss >> tmp_weight;
					hidden_weights.push_back(tmp_weight);
					++current_weights_count;
				} else if (state == 'H') {
					ref_layers++;
					if (current_hidden_cols != current_biases_count) {
						reason = "current_hidden_cols != current_biases_count";
						return false;
					}
					if (current_hidden_dim != current_weights_count) {
						reason = "weights num != hidden dim";
						return false;
					}
					std::istringstream tmp_iss(tmp);
					char ch;
					int x, y;
					tmp_iss >> x >> ch >> y;

					ref_hidden_r.push_back(x);
					ref_hidden_c.push_back(y);
					if(ref_hidden_c.size() >= 2)
						outputs = ref_hidden_c[ref_hidden_c.size()-2];

					current_hidden_rows = x;
					current_hidden_cols = y;
					current_hidden_dim = x * y;

					if (!checked_first_hidden) {
						checked_first_hidden = true;
						checked_hidden_break = (x != current_inputs_col_count) ? true:false ;
						current_inputs_col_count = y;
					} else {
						checked_hidden_break = (x != current_inputs_col_count) ? true : false;
						current_inputs_col_count = y;
					}
					current_weights_count = 0;
					current_biases_count = 0;
				}
			}
		}
		if ((weights_declarations != biases_declarations) |
			(hidden_declarations != weights_declarations) |
			(hidden_declarations != biases_declarations)
		){
			reason = "Not enough biases/weights/hidden declarations";
			return false;
		}

		ref_layers--;
		return true;
	}



	CNNSchemaGenerator::CNNSchemaGenerator(std::string code): target_str(code)
	{}

	bool CNNSchemaGenerator::generate() {
		std::regex validate_code("^(([1-9]{1}[0-9]{0,1}x[1-9]{1}[0-9]{0,1}/{0,1})+(?!/)){2,}$");
		std::cmatch m;
		if (!std::regex_match(target_str.c_str(), m, validate_code)) {
			std::cout << "[-] CNNSchemaGenerator: regex error" << std::endl;
			return false;
		}
		std::istringstream iss(target_str);
		std::ostringstream oss;

		oss << "# Schema code: " << target_str << std::endl;
		int tmp_rows, tmp_cols;
		int prev_tmp_rows=0, prev_tmp_cols=0;
		char tmp_char;
		bool got_inputs = false;
		while (iss) {
			iss >> tmp_rows >> tmp_char >> tmp_cols;
			if (!got_inputs) {
				got_inputs = true;
				oss << "Inputs: " << tmp_rows << std::endl;
			}
			if ((tmp_rows == 0) || (tmp_cols == 0)) {
				std::cout << "[-] Layer can't have 0 neurons" << std::endl;
				return false;
			}
			if ((prev_tmp_rows) != 0 && (prev_tmp_cols != 0)) {
				if (tmp_rows != prev_tmp_cols) {
					std::cout << "[-] Can't perform dot product on provided network" << std::endl;
					return false;
				}
			}

			oss << std::endl << "Hidden: " << tmp_rows << "x" << tmp_cols << std::endl;

			oss << "Weights: ";
			for (int it = 0; it < tmp_rows*tmp_cols; ++it) {
				oss << get_random_weight() << " ";
			}
			oss << std::endl;

			oss << "Biases: ";
			for (int it = 0; it < tmp_cols; ++it) {
				oss << get_random_weight() << " ";
			}
			oss << std::endl;

			iss >> tmp_char;
			prev_tmp_rows = tmp_rows;
			prev_tmp_cols = tmp_cols;
		}

		std::cout << oss.str() << std::endl;

		return true;
	}

} // namespace neural