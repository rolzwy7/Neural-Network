// author: Bartosz Nowakowski (https://github.com/rolzwy7)
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <regex>
#include <sstream>
#include "argparse.h"

namespace argparse {

	void ArgumentParser::arg_convert(std::string str, Argument obj) {
		std::string tmp_val;
		std::istringstream ss(str);

		drop_positionals = (obj.arg_drop == dDrop) ? true:false ;

		if (obj.arg_type == tString)
			map_string[obj.name] = str;

		else if (obj.arg_type == tDouble) {
			double tmp; ss >> tmp;
			map_double[obj.name] = tmp;

		} else if (obj.arg_type == tInt) {
			int tmp; ss >> tmp;
			map_int[obj.name] = tmp;
		}

	}

	ArgumentParser::ArgumentParser(std::string description, int argc, char * argv[]):
		_arg_count(0),
		_arg_count_optional(0),
		_arg_count_positional(0),
		_arg_ptr(nullptr),
		description(description),
		exec_name(""),
		max_arg_name_len(0),
		drop_positionals(false)
	{
		std::string tmp = argv[0];
		size_t i = tmp.find_last_of('\\');
		this->exec_name = tmp.substr(++i);
	}

	ArgumentParser::~ArgumentParser() {
		if (this->_arg_ptr != nullptr)
			delete[] this->_arg_ptr;
	}

	bool ArgumentParser::parse_args(int argc, char * argv[]) {
		std::string tmp_str, tmp_argv_elem;
		// Check if help
		std::string help_arr[7] = { "h", "help", "-h", "-help" , "--h", "--help", "/?" };
		tmp_str = (argc == 2) ? argv[1] : "";
		if (tmp_str != "")
			for (int i = 0; i < 7; ++i)
				if (tmp_str == help_arr[i]) {
					this->print_help();
					return false;
				};
		// Parse store arguments
		for (int j = 0; j < this->_arg_count; ++j) {
			if (_arg_ptr[j].arg_type != tStoreTrue) continue;
			tmp_str = this->_arg_ptr[j].name;
			for (int i = 1; i < argc; ++i) {
				tmp_argv_elem = argv[i];
				map_bool[_arg_ptr[j].name] = (tmp_argv_elem == "-" + tmp_str) ? true :
					(tmp_argv_elem == "--" + tmp_str) ? true : false;
			}
		}
		// Parse optional arguments
		std::regex re_optional_eq("^[-]{1,2}[a-zA-z][a-zA-z+-9-]+[=][\\w\\W]+$");
		std::cmatch m;
		for (int j = 0; j < this->_arg_count; ++j) {
			if (_arg_ptr[j].arg_imp != iOptional || _arg_ptr[j].arg_type == tStoreTrue) continue;
			tmp_str = this->_arg_ptr[j].name;
			for (int i = 1; i < argc; ++i) {
				tmp_argv_elem = argv[i];
				if (tmp_argv_elem.find("-" + tmp_str) == 0 ||
					tmp_argv_elem.find("--" + tmp_str) == 0
					) {
					// ?Argument's value located at next element
					if ("-" + tmp_str == tmp_argv_elem || "--"+tmp_str == tmp_argv_elem) {
						if(i+1 >= argc) {
							std::cout << "[-] Brak wartoœci dla argumentu: '--" << tmp_str << "'" << std::endl;
							return false;
						} else {
							std::string tmp_val = argv[i + 1];
							this->arg_convert(tmp_val, this->_arg_ptr[j]);
						}
					}
					// ?Argument's value located after equal sign
					else if (std::regex_match(tmp_argv_elem.c_str(), m, re_optional_eq)) {
						std::string tmp_val = tmp_argv_elem;
						tmp_val = tmp_val.substr(tmp_val.find('=') + 1);
						this->arg_convert(tmp_val, this->_arg_ptr[j]);
					}
					// ?Possible user mistake
					else {
						std::cout << "[-] B³êdny argument: '" << tmp_argv_elem << "'" << std::endl;
						std::cout << "\t Czy chodzi³o ci o '--" << tmp_str << "' ?" << std::endl;
						return false;
					}
				}
			}
		}
		// Parse positional arguments
		if (drop_positionals) return true;
		// Positional arguments error (length check)
		if (!(argc >= _arg_count_positional + 1)) {
			this->print_positional_argument_err_msg();
			return false;
		} else {
			// Positional arguments parsing
			int pos = 1;
			for (int j = 0; j < this->_arg_count; ++j) {
				if (_arg_ptr[j].arg_imp != iPositional) continue;
				tmp_str = this->_arg_ptr[j].name;
				std::string tmp_val = argv[pos++];
				this->arg_convert(tmp_val, this->_arg_ptr[j]);
			}
		}
		return true;
	}

	void ArgumentParser::print_maps() {

		std::cout << std::setfill('*') << std::setw(64) << "*" << std::endl;
		std::cout << "- All arguments count: " << _arg_count << std::endl;
		std::cout << "- Positional arguments count: " << _arg_count_positional << std::endl;
		std::cout << "- Optional arguments count: " << _arg_count_optional<< std::endl;
		std::cout << "[*] String arguments:" << std::endl;
		for (auto i = map_string.begin(); i != map_string.end(); ++i)
			std::cout << "\t" << i->first << " = " << i->second << std::endl;
;
		std::cout << "[*] Int arguments:" << std::endl;
		for (auto i = map_int.begin(); i != map_int.end(); ++i)
			std::cout << "\t" << i->first << " = " << i->second << std::endl;

		std::cout << "[*] Double arguments:" << std::endl;
		for (auto i = map_double.begin(); i != map_double.end(); ++i)
			std::cout << "\t" << i->first << " = " << i->second << std::endl;

		std::cout << "[*] Bool arguments:" << std::endl;
		for (auto i = map_bool.begin(); i != map_bool.end(); ++i)
			std::cout << "\t" << i->first << " = " << i->second << std::endl;
	}

	void ArgumentParser::add_argument(std::string name, std::string help, ArgType tType, ArgImp tImp, ArgDrop tDrop) {
		(tImp == iPositional) ? _arg_count_positional++: _arg_count_optional++;

		for (int i = 0; i < this->_arg_count; ++i) {
			if (this->_arg_ptr[i].name == name) {
				std::cout << "[debug] Argument with name '" << name << "' already exists" << std::endl;
				return;
			}
		}
	
		if (tType == tStoreTrue && (tImp == iPositional)) {
			std::cout << "[debug] Positional arguments can't store boolean" << std::endl;
			return;
		}

		std::string rg = "^[a-zA-z]|[a-zA-z][a-zA-z0-9-]+$";
		std::regex re_arg_name(rg);
		std::cmatch m;
		if (!std::regex_match(name.c_str(), m, re_arg_name)) {
			std::cout << "[debug] Invalid argument name: regexp: " << rg << std::endl;
			return;
		}

		++this->_arg_count;
		Argument * tmp_arg_ptr = new Argument[_arg_count];
		for (int i = 0; i < this->_arg_count - 1; ++i)
			tmp_arg_ptr[i] = this->_arg_ptr[i];
		int position = (tImp == iPositional) ? _arg_count_positional:-1 ;
		Argument tmp_arg = Argument{ position, name, help, tType, tImp, tDrop };
		tmp_arg_ptr[_arg_count - 1] = tmp_arg;

		delete[] this->_arg_ptr;
		this->_arg_ptr = tmp_arg_ptr;

		int help_width = ((int)name.length()) * 2 + 7;
		if (help_width  > this->max_arg_name_len)
			this->max_arg_name_len = help_width;
	}

	void ArgumentParser::print_help(bool only_usage) {

		auto upper = [](Argument arg) {
			std::string upper = arg.name;
			if (arg.arg_type == tStoreTrue) { upper = ""; return upper; }
			for (auto it = upper.begin(); it != upper.end(); ++it) {
				*it = ::toupper(*it);
			}
			return upper;

		};

		std::string optional_args_usage = "";
		std::string positional_args_usage = "";

		for (int i = 0; i < this->_arg_count; ++i) {
			if (this->_arg_ptr[i].arg_imp == iOptional) {
				optional_args_usage += "[ --" + this->_arg_ptr[i].name + "] ";
				if (i != 0 && i % 4 == 0) {
					optional_args_usage += "\n";
					for (int j = 0; j < this->exec_name.length() + (7 + 1); ++j)
						optional_args_usage += " ";
				}
			}
			if (this->_arg_ptr[i].arg_imp == iPositional) {
				if (i == 0 || i % 4 == 0) {
					if (i != 0) positional_args_usage += "\n";
					for (int j = 0; j < this->exec_name.length() + (7 + 1); ++j)
						positional_args_usage += " ";
				}
				positional_args_usage += this->_arg_ptr[i].name + " ";
			}
		}

		std::cout << "usage: " << this->exec_name << " ";

		std::cout << optional_args_usage << std::endl;
		std::cout << positional_args_usage << std::endl;

		if (only_usage) return;

		std::cout << std::endl << this->description << std::endl;

		std::cout << std::endl << "positional arguments:" << std::endl;
		for (int i = 0; i < this->_arg_count; ++i) {
			if (this->_arg_ptr[i].arg_imp == iPositional) {
				std::cout << std::left << std::setfill(' ') << std::setw(this->max_arg_name_len) 
					<< "  " + this->_arg_ptr[i].name
					<< this->_arg_ptr[i].help << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << "optional arguments:" << std::endl;
		for (int i = 0; i < this->_arg_count; ++i) {
			if (this->_arg_ptr[i].arg_imp == iOptional) {
				optional_args_usage = this->_arg_ptr[i].name;
				
				std::cout << std::left << std::setfill(' ') << std::setw(this->max_arg_name_len) 
					<< "  --" + this->_arg_ptr[i].name + " " + upper(this->_arg_ptr[i])
					<< this->_arg_ptr[i].help << std::endl;

			}
		}

		// Modyfikacja dla projektu sieæ neuronowa - przyk³ady
		std::cout << std::endl << "[*] Examples: " << std::endl;
		std::cout << "- Generate 4x3 network schema" << std::endl;
		std::cout << "\t" << this->exec_name << " --generate-schema=4x3/3x3" << std::endl;
		std::cout << "- Train and eval network" << std::endl;
		std::cout << "\t" << this->exec_name << " schema.cnn --epochs=100 --learning-rate=0.5 --train-file=file1.csv --eval-file=file2.csv --train" << std::endl;
		std::cout << "- Eval network only" << std::endl;
		std::cout << "\t" << this->exec_name << " schema.cnn --eval-file=file.csv --eval-only" << std::endl;
		std::cout << std::endl;
	}

	void ArgumentParser::print_positional_argument_err_msg() {
		this->print_help(true);
		std::cout << std::endl;
		std::cout << this->exec_name << " error: the following arguments are required: " << std::endl << "  ";
		for (int j = 0; j < this->_arg_count; ++j) {
			if (this->_arg_ptr[j].arg_imp == iPositional) {
				if (j != 0 && j % 4 == 0)
					std::cout << std::endl << "  ";
				if (j == this->_arg_count_positional - 1)
					std::cout << this->_arg_ptr[j].name << std::endl;
				else
					std::cout << this->_arg_ptr[j].name << ", ";
			}
		}
	}

} // namespace argparse
