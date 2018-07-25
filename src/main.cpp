#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "io.hpp"
#include "rsys_edit.hpp"

const std::string ver = "1.0.0";

int main(int argc, char * argv[]) {

	std::cout << std::endl << "rsys_edit v" << ver << std::endl;
	std::cout << "Copyright 2018 Jbop (https://github.com/jbop1626)" << std::endl << std::endl;

	program_arguments prog_args;
	parse_args(prog_args, argc, argv);

	uint8_t * recrypt_list = open_list(prog_args.rsys_filename);
	uint8_t * virage2 = read_file(prog_args.v2_filename, 0, 256);

	int data_length = view_list(recrypt_list, virage2);
	if (prog_args.edit) {
		edit_list(recrypt_list, virage2, data_length);
	}

	delete[] recrypt_list;
	delete[] virage2;
	return 0;
}
