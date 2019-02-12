// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#include <sstream>
#include <vector>
#include <regex>
#include "cnn_dataset.h"


namespace neural {

	CNNDataset::CNNDataset(std::string filepath, int expected_data_num):
		filepath(filepath),
		expected_data_num(expected_data_num),
		headers(nullptr),
		datalines(0)
	{}

	CNNDataset::~CNNDataset()
	{
		if (headers)
			delete[] headers;
	}

	bool CNNDataset::validate(std::string & msg, std::vector<double> & data_vector) {
		std::regex re_dataline("^[ ]{0,}(([0-9]+[.]{1}[0-9]+,){"+std::to_string(expected_data_num-1)+"}([0-9]+[.]{1}[0-9]+){1})[ ]{0,}$");
		std::regex re_headers("^[ ]{0,}(([a-zA-Z_]+,){"+std::to_string(expected_data_num-1)+"}([a-zA-Z_]+){1})[ ]{0,}$");
		file_handle.open(filepath.c_str(), std::ios::in);
		bool seek_headers = true;
		if (file_handle.good()) {
			std::string tmp_line;
			while (!file_handle.eof()) {
				std::cmatch m;
				std::getline(file_handle, tmp_line);
				if (seek_headers) {
					// Save headers
					if (std::regex_match(tmp_line.c_str(), m, re_headers)) {
						headers = new std::string[expected_data_num];
						tmp_line += ",";
						int c = 0;
						while (tmp_line.find(',') != std::string::npos) {
							headers[c] = tmp_line.substr(0, tmp_line.find(','));
							tmp_line.erase(0, tmp_line.find(',')+1);
							++c;
						}
						seek_headers = false;
					}
				} else {
					// Save data
					if (std::regex_match(tmp_line.c_str(), m, re_dataline)) {
						tmp_line += ",";
						std::istringstream iss(tmp_line);
						double tmp_double;
						char tmp_char;
						while (iss >> tmp_double >> tmp_char) {
							data_vector.push_back(tmp_double);
						}
						++datalines;
					}
				}
			}
		} else {
			std::ostringstream oss;
			oss << "[-] Couldn't open file: " << filepath;
			msg = oss.str();
			return false;
		}
		file_handle.close();

		std::ostringstream oss;
		oss << "[+] Read file successfully: " << filepath;
		msg = oss.str();
		return true;
	}

	void populate_matrices(
		math::collections::Matrix * inputs,
		math::collections::Matrix * outputs,
		int mat_num,
		int inputs_col,
		int outputs_col,
		std::vector<double> & training_data
	) {
		int expected_data_num = inputs_col + outputs_col;

		for (int it = 0; it < mat_num; ++it) {
			inputs[it].resize(1, inputs_col, 0);
			outputs[it].resize(1, outputs_col, 0);
		}

		int training_data_vec_it = 0;
		int inputs_range = 0;
		int it_tmp = 0;
		int outputs_range = inputs_range + inputs_col;
		int matrices_couter = 0;
		while (training_data_vec_it < training_data.size()) {
			// inputs
			it_tmp = 0;
			for (int it = inputs_range; it < outputs_range; ++it) {
				inputs[matrices_couter].matrix[0 * inputs_col + it_tmp] = training_data.at(training_data_vec_it);
				++training_data_vec_it;
				++it_tmp;
			}
			inputs_range += expected_data_num;
			// outputs
			it_tmp = 0;
			for (int it = outputs_range; it < inputs_range; ++it) {
				outputs[matrices_couter].matrix[0 * outputs_col + it_tmp] = training_data.at(training_data_vec_it);
				++training_data_vec_it;
				++it_tmp;
			}
			outputs_range += expected_data_num;

			matrices_couter++;
		}

		// Transpose
		for (int it = 0; it < mat_num; ++it) {
			inputs[it].transpose();
			outputs[it].transpose();
		}
		
	}

} // namespace neural