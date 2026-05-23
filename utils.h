#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define KEY_SIZE 16

typedef struct {
    char service[50];
    char password[50];
    char key[KEY_SIZE];
    size_t pass_len;
} Credential;

typedef struct{
    Credential* entries;
    size_t count;
    size_t capacity;
} Vault;

void xor_cipher(char* data,size_t data_len,const char* key);
void add_credential(Vault* v,const char* service,const char* raw_password);
void save_vault(const Vault* v, const char* filename);
bool load_vault(Vault* v, const char* filename);
void generate_random_key(char* buffer, size_t length);
void list_passwords(Vault* v);
void add_password(Vault* v);

#endif