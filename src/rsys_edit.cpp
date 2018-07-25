#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

#include "aes/aes.hpp"
#include "ninty-233/ninty-233.hpp"
#include "rsys_edit.hpp"
#include "io.hpp"

const int HEADER_SIZE = 0x44; // sizeof(ECC_sig) + sizeof(num_entries) = 0x40 + 0x04
const int ENTRY_SIZE = 0x20;
const bool ENCRYPT = true;
const bool DECRYPT = false;

uint8_t * open_list(std::string filename) {
	uint8_t * num_entries = read_file(filename, 0x43, 1);
	int data_length = ((*num_entries) * ENTRY_SIZE) + HEADER_SIZE;
	delete[] num_entries;
	return read_file(filename, 0, data_length);
}

int view_list(uint8_t * recrypt_list, const uint8_t * virage2) {
	crypt_entries(recrypt_list, virage2, DECRYPT);
	
	int num_entries = recrypt_list[0x43];
	std::cout << std::endl << "There are " << num_entries << " entries in the recrypt list:" << std::endl;
	for(int entry = 1; entry <= num_entries; ++entry) {
		print_entry(recrypt_list, num_entries, entry);
	}
	
	return HEADER_SIZE + (num_entries * ENTRY_SIZE);
}

void print_entry(uint8_t * recrypt_list, int num_entries, int entry) {
	int offset = HEADER_SIZE + (ENTRY_SIZE * (entry - 1));
	std::cout << "Entry " << entry << ":" << std::endl;
	std::cout << "Content ID:  ";
	for (int j = 0; j < 4; ++j) {
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int)recrypt_list[offset];
		offset++;
	}
	std::cout << std::endl;
	std::cout << "Recrypt key: ";
	for (int j = 0; j < 16; ++j) {
		std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int)recrypt_list[offset];
		offset++;
	}
	offset += 12; // Skip past unknown 12-byte field
	std::cout << std::endl << std::endl;
}

void edit_list(uint8_t * recrypt_list, const uint8_t * virage2, int data_length) {
	bool edited = false;
	int num_entries = recrypt_list[0x43];
	int entry = -1;

	while (get_choice(entry)) {
		if (entry < 0 || entry > num_entries) {
			std::cout << "This is not a valid entry number. Try again." << std::endl;
		}
		else {
			edit_entry(recrypt_list, entry);
			edited = true;
		}
	}

	if (!edited) {
		return;
	}
	else {
		write_list(recrypt_list, virage2, data_length);
		std::cout << "Changes saved." << std::endl;
	}
}

void edit_entry(uint8_t * recrypt_list, int entry) {
	uint8_t content_id[4] = { 0 };
	uint8_t recrypt_key[16] = { 0 };
	uint8_t * entry_ptr = recrypt_list + HEADER_SIZE + (ENTRY_SIZE * (entry - 1));

	std::cout << std::endl << "Editing entry #" << entry << ":" << std::endl;

	std::cout << "Enter the new content ID - 4 bytes (8 digits, hexadecimal): ";
	std::cout << std::endl << "> ";
	while (!hex_input_to_buffer(content_id, 4)) {
		std::cout << "Input invalid. Try again." << std::endl;
		std::cout << "> ";
	}
	std::memcpy(entry_ptr, content_id, 4);

	std::cout << "Enter the new recrypt key - 16 bytes (32 digits, hexadecimal): ";
	std::cout << std::endl << "> ";
	while (!hex_input_to_buffer(recrypt_key, 16)) {
		std::cout << "Input invalid. Try again." << std::endl;
		std::cout << "> ";
	}
	std::memcpy(entry_ptr + 4, recrypt_key, 16);
}

void sign_list(uint8_t * recrypt_list, const uint8_t * virage2, int data_length) {
	BigUnsigned hash = sha1(recrypt_list + 0x40, data_length - 0x40);
	element r = { 0 };
	element s = { 0 };
	ecdsa_sign(hash, virage2 + 0x98, r, s);
	byteswap_elem(r);
	byteswap_elem(s);
	std::memcpy(recrypt_list, r, 32);
	std::memcpy(recrypt_list + 32, s, 32);
}

void write_list(uint8_t * recrypt_list, const uint8_t * virage2, int data_length) {
	crypt_entries(recrypt_list, virage2, ENCRYPT);

	sign_list(recrypt_list, virage2, data_length);

	int null_data_length = 0x4000 - data_length;
	uint8_t * null_data = new uint8_t[null_data_length];
	std::memset(null_data, 0, null_data_length);
	write_file("new_recrypt.sys", recrypt_list, 0, data_length, true);
	write_file("new_recrypt.sys", null_data, data_length, null_data_length, false);
	delete[] null_data;
}

void crypt_entries(uint8_t * recrypt_list, const uint8_t * virage2, bool encrypt) {
	uint8_t recrypt_list_key[16] = { 0 };
	std::memcpy(recrypt_list_key, virage2 + 0xC8, 16);

	uint32_t BBID = get_BBID(virage2);
	uint32_t recrypt_list_iv[4] = { byteswap(BBID), byteswap(BBID + 1), byteswap(BBID + 2), byteswap(BBID + 3) };

	uint8_t num_entries = recrypt_list[0x43];
	uint8_t * entry = recrypt_list + HEADER_SIZE;
	for (int n = 0; n < num_entries; ++n) {
		struct AES_ctx ctx;
		AES_init_ctx_iv(&ctx, recrypt_list_key, (uint8_t*)recrypt_list_iv);
		if (encrypt) {
			AES_CBC_encrypt_buffer(&ctx, entry, ENTRY_SIZE);
		}
		else {
			AES_CBC_decrypt_buffer(&ctx, entry, ENTRY_SIZE);
		}
		entry += ENTRY_SIZE;
	}
}

uint32_t get_BBID(const uint8_t * virage2) {
	uint32_t BBID = *(uint32_t *)(virage2 + 0x94);
	return byteswap(BBID);
}
