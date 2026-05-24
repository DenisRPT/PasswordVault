#include "utils.h"



void generate_random_key(char* buffer, size_t length){
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t charset_size = strlen(charset);
    for(size_t i=0;i<length-1;i++){
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[length-1]='\0';
}

void xor_cipher(char* data,size_t data_len,const char* key){
    for(size_t i=0;i<data_len;i++){
        data[i]^=key[i%KEY_SIZE];
    }
}

void add_credential(Vault* v,const char* service,const char* raw_password){
    if(v->capacity==0){
        v->capacity++;
        v->entries = malloc(sizeof(Credential));
        if(v->entries==NULL) return;
    }
    else if(v->count==v->capacity) {
        v->capacity*=2;
        Credential* temp = realloc(v->entries,sizeof(Credential)*v->capacity);
        if(temp==NULL) return;
        v->entries = temp;
    }
    Credential* new_cred = &v->entries[v->count];
    strcpy(new_cred->service,service);
    new_cred->pass_len = strlen(raw_password);
    strcpy(new_cred->password,raw_password);
    generate_random_key(new_cred->key, 16);
    xor_cipher(new_cred->password, new_cred->pass_len, new_cred->key);
    v->count++;

}

void save_vault(const Vault* v, const char* filename){
    FILE* f = fopen(filename,"wb");
    if(f==NULL) return;
    fwrite(&v->count, sizeof(size_t), 1, f);
    fwrite(v->entries, sizeof(Credential), v->count, f);
    fclose(f);
}

bool load_vault(Vault* v, const char* filename){
    FILE* f = fopen(filename,"rb");
    if(f==NULL) return false;
    size_t new_count;
    fread(&new_count, sizeof(size_t), 1, f);
    v->count = new_count;
    v->capacity = new_count;
    v->entries=malloc(sizeof(Credential)*new_count);
    if(v->entries==NULL) return false;
    fread(v->entries,sizeof(Credential),new_count,f);
    fclose(f);
    return true;
}

void list_passwords(Vault* v){
    printf("\n");
    for(size_t i = 0; i < v->count; i++){
        Credential* c = &v->entries[i];
        char temp_password[50];
        memcpy(temp_password, c->password, 50); 
        xor_cipher(temp_password, c->pass_len, c->key);
        temp_password[c->pass_len] = '\0';
        printf("%u. %s : %s\n",i+1, c->service, temp_password);
    }
    printf("\n");
}

void add_password(Vault *v){
    char password[50];
    char service[50];
    printf("Enter the service(no spaces): ");
    scanf("%s",service);
    printf("Enter your password(no spaces): ");
    scanf("%s",password);
    printf("\n");
    add_credential(v, service, password);
}

void rm_password(Vault*v){
    list_passwords(v);
    printf("Select password ID to remove: ");
    int choice;
    while(1){
        scanf("%d",&choice);
        while ((getchar()) != '\n');
        if(choice>=1 && choice <=v->count){
            break;
        }
        printf("Invalid choice\n");
    }
    if(choice==v->count){
        v->count--;
    }
    else {
        for(int i=choice-1;i<v->count-1;i++){
            v->entries[i] = v->entries[i+1];
        }
        v->count--;
    }
    printf("Password succesfully removed\n");
    if(v->capacity==0){
        free(v->entries);
        v->entries = NULL;
        v->capacity = 0;
        return;
    }
    v->capacity = v->count;
    Credential* temp = realloc(v->entries,sizeof(Credential)*v->capacity);
    if(temp==NULL) return;
    v->entries = temp;
}

void get_vault_path(char* filepath, size_t max_len){
    const char* home_dir =NULL;
    #ifdef _WIN32
        home_dir = getenv("USERPROFILE");
    #else
        home_dir = getenv("HOME");
    #endif
    if(home_dir!=NULL){
        snprintf(filepath, max_len, "%s/.vault.bin",home_dir);

    }
    else{
        strcpy(filepath,"vault.bin");
    }
}
