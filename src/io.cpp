#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <cctype> // for isxdigit
#include <algorithm>

#include "io.hpp"
#include "ninty-233/ninty-233.hpp"

void parse_args(program_arguments & prog_args, int argc, char * argv[]) {
	prog_args.rsys_filename = "";
	prog_args.v2_filename = "";
	prog_args.edit = false;

	for (int a = 1; a < argc; ++a) {
		std::string arg = std::string(argv[a]);
		if (arg == "-i" && a < argc - 1) {
			prog_args.rsys_filename = argv[a + 1];
		}
		else if (arg == "-v2" && a < argc - 1) {
			prog_args.v2_filename = argv[a + 1];
		}
		else if (arg == "-edit") {
			prog_args.edit = true;
		}
	}
	if (prog_args.v2_filename == "" || prog_args.rsys_filename == "") {
		std::cerr << "Invalid arguments." << std::endl;
		std::cerr << "Usage: rsys_edit -i [recrypt.sys] -v2 [Virage2 dump] (-edit)" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

uint8_t * read_file(std::string in_filename, unsigned int start_pos, unsigned int read_length) {
	std::ifstream fin;
	fin.open(in_filename, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
	if (!fin.is_open()) {
		fin.clear();
		std::cerr << in_filename << " could not be opened for reading." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	size_t file_length = fin.tellg();
	if (read_length == 0) {
		read_length = file_length - start_pos;
	}
	if (read_length + start_pos > file_length) {
		fin.clear();
		fin.close();
		std::cerr << "Intended read length and file size are not compatible." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	fin.seekg(start_pos);
	uint8_t * buffer = new uint8_t[read_length];
	fin.read((char *)buffer, read_length);
	
	fin.clear();
	fin.close();
	return buffer;
}

void write_file(std::string out_filename, uint8_t * buffer, unsigned int start_pos, unsigned int write_length, bool trunc) {
	auto open_mode = std::ios_base::out | std::ios_base::binary;
	if (trunc) {
		open_mode |= std::ios_base::trunc;
	}
	else {
		open_mode |= std::ios_base::app;
	}
	
	std::ofstream fout(out_filename, open_mode);
	if (!fout.is_open()) {
		std::cerr << "ERROR: Could not open " << out_filename << " to write." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	fout.seekp(start_pos);
	fout.write((char *)buffer, write_length);
	fout.close();
}

int get_choice(int & input) {
	std::cout << std::endl << "Enter the number of the entry you wish to edit." << std::endl;
	std::cout << "Enter 0 to stop editing. " << std::endl << "> ";
	std::cin >> input;
	if (!std::cin) {
		input = -1;
	}
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return input;
}

bool is_hex(std::string str) {
	for (char s : str) {
		if (!isxdigit(s)) {
			return false;
		}
	}
	return true;
}

bool hex_input_to_buffer(uint8_t * buffer, int length) {
	std::string line_str;
	std::getline(std::cin, line_str);

	// Strip spaces, check length and hex, insert/replace spaces
	line_str.erase(std::remove(line_str.begin(), line_str.end(), ' '), line_str.end());
	if (line_str.length() != length * 2) {
		return false;
	}
	if (!is_hex(line_str)) {
		return false;
	}
	for (int p = 2; p <= length * 3; p += 3) {
		line_str.insert(p, 1, ' ');
	}

	std::istringstream iss{ line_str };
	unsigned int input_byte = 0;

	for (int i = 0; i < length; ++i) {
		if (iss >> std::hex >> input_byte) {
			if (input_byte < 0x100)
				buffer[i] = input_byte;
			else
				return false;
		}
		else {
			return false;
		}
	}
	return true;
}

uint32_t byteswap(uint32_t input) {
	uint32_t output = input;
#if defined(HOST_IS_LITTLE_ENDIAN) && (HOST_IS_LITTLE_ENDIAN == 1)
	output = (input & 0xFF) << 24;
	output |= (input & 0xFF00) << 8;
	output |= (input & 0xFF0000) >> 8;
	output |= (input & 0xFF000000) >> 24;
#endif
	return output;
}

void byteswap_elem(element a) {
#if defined(HOST_IS_LITTLE_ENDIAN) && (HOST_IS_LITTLE_ENDIAN == 1)
	for (int i = 0; i < 8; ++i) {
		a[i] = byteswap(a[i]);
	}
#endif
}
