#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    srand(time(NULL));
    Vault vault = {NULL,0,0};
    char filepath[512];
    get_vault_path(filepath, sizeof(filepath));
    if(load_vault(&vault,filepath)){
        printf("Succesfully loaded the vault\n");
    }
    else{
        printf("New vault created\n\n");
    }
    bool new_pas = false;
    bool running = true;
    while(running){
        printf(" 1.List Passwords\n 2.Add Password\n 3.Exit\n");
        int choice;
        scanf("%d",&choice);
        switch (choice) {
            case(1):
            list_passwords(&vault);
            break;
            case(2):
            add_password(&vault);
            new_pas = true;
            break;
            case(3) :
            running = false;
            break;
            default :
            printf("Invalid Option\n");
            break;
        }
    }
    if(new_pas) {
        save_vault(&vault,filepath);
    }
    free(vault.entries);
    
    return 0;

}