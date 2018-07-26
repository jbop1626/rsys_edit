#ifndef RSYS_EDIT_IO_HPP
#define RSYS_EDIT_IO_HPP

#include <string>
#include <cstdint>

#include "ninty-233/ninty-233.hpp"

// Set to 0 or 1 if on a big-endian or little-endian machine,
// respectively, in order to make I/O work correctly.
#define HOST_IS_LITTLE_ENDIAN 1

struct program_arguments {
	std::string rsys_filename;
	std::string v2_filename;
	bool edit;
};

void parse_args(program_arguments & prog_args, int argc, char * argv[]);

/*
	in_filename:	name of file to read
	start_pos:		position in file to start reading
	read_length:	number of bytes to read
	returns allocated buffer of the bytes read
*/
uint8_t * read_file(std::string in_filename, unsigned int start_pos, unsigned int read_length);

/*
	out_filename:	name of file to write to
	buffer:			buffer to write
	start_pos:		position in file to start writing
	write_length:	number of bytes to write
	trunc:			truncate the file?
*/
void write_file(std::string out_filename, uint8_t * buffer, unsigned int start_pos, unsigned int write_length, bool trunc);

int get_choice(int & input);

bool is_hex(std::string str);

bool hex_input_to_buffer(uint8_t * buffer, int length);

uint32_t byteswap(uint32_t input);

void byteswap_elem(element a);
#endif
