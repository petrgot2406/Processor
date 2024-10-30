#ifndef READ_FROM_FILE_H
#define READ_FROM_FILE_H

size_t num_of_symbols_in_file(File_t file);
size_t num_of_strings_in_file(File_t file);
void Init_file(File_t* file);
void Read_file_to_buffer(File_t* file);
void Put_lineslen_for_all_lines(File_t* file);
void Put_pointers_to_lines(File_t* file);
void Put_file_to_structure(File_t* file);

#endif
