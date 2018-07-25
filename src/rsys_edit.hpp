#ifndef RSYS_EDIT_HPP
#define RSYS_EDIT_HPP

#include <string>
#include <cstdint>

uint8_t * open_list(std::string filename);
int view_list(uint8_t * recrypt_list, const uint8_t * virage2);
void print_entry(uint8_t * recrypt_list, int num_entries, int entry);
void edit_list(uint8_t * recrypt_list, const uint8_t * virage2, int data_length);
void edit_entry(uint8_t * recrypt_list, int entry);
void sign_list(uint8_t * recrypt_list, const uint8_t * virage2, int data_length);
void write_list(uint8_t * recrypt_list, const uint8_t * virage2, int data_length);
void crypt_entries(uint8_t * recrypt_list, const uint8_t * virage2, bool encrypt);
uint32_t get_BBID(const uint8_t * virage2);

#endif
