#pragma once

#ifndef WORDMATCH_H
#define WORDMATCH_H

int find_word(char* word);
void add_word(char* word, int insttype);
int remove_word(char* word);
void print_dict();
void free_all();

#endif
