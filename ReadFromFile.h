#ifndef READ_FROM_FILE_H
#define READ_FROM_FILE_H

size_t num_of_symbols_in_file(const char* file_name);
size_t num_of_strings_in_file(const char* file_name);
void Read_file_to_buffer(const char* file_name, size_t file_size, char* buffer);
void Put_lineslen_for_all_lines(char* buffer, size_t file_size, size_t* lineslen);
void Put_pointers_to_lines(char* buffer, size_t file_size, size_t str_num, char** lines);

#endif
