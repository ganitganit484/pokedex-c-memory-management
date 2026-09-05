//
// Created by ganit on 20/11/2025.
//
// Struct definitions

#include <stdlib.h>
#include <stdio.h>
#include "Pokemon.h"
#include <string.h>

//A function that removes hidden characters and spaces from a string
void help_func(char *s) {
    if (s == NULL) return;
    while (*s == ' ' || *s == '\t') s++;
    char *end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t')) {
        *end = '\0';
        end--;
    }
}

//Function declarations
static Pokemon_type **createTypesArray(char *pbuffer, int size, FILE *file_pointer, int numberOfTypes);

static Pokemon **createPokemonsArray(char *pbuffer, int size, FILE *file_pointer, int numberOfPokemons,
                                     int numberOfTypes, Pokemon_type **ptypes_array);

//Function implementations
static Pokemon_type **createTypesArray(char *pbuffer, int size, FILE *file_pointer, int numberOfTypes) {
    //Extracting Pokémon type names and type inference
    if (file_pointer == NULL) {
        return NULL;
    }
    Pokemon_type **temp = (Pokemon_type **) malloc(numberOfTypes * sizeof(Pokemon_type *));
    if (temp == NULL) {
        return NULL;
    }

    Pokemon_type **pArrayToPokemonTypes = temp;
    while (fgets(pbuffer, size, file_pointer)) {
        //Read the lines until we find the line that begins with the word Types
        if (strncmp(pbuffer, "Types", 5) == 0)
            break;
    }
    help_func(fgets(pbuffer, size, file_pointer)); //Let's go down another line to get to the type names.
    int i = 0;
    char buffer_copy[300];
    strcpy(buffer_copy, pbuffer);
    char *types_names[numberOfTypes];
    //We will define an array with the size of the number of types that will contain the type names.
    char *name = strtok(buffer_copy, ",");
    while (name != NULL) {
        help_func(name);
        types_names[i++] = name; //We will place the separated word in the i position and then increase the i.
        name = strtok(NULL, ",");
        //We call the separation function again this time with NULL so that it continues where it left off.
    }

    for (i = 0; i < numberOfTypes; i++) {
        //We will go through all the names and insert into the pointers array a pointer to a new type that we will create with the name from the names array.
        pArrayToPokemonTypes[i] = createType(types_names[i]);
        if (pArrayToPokemonTypes[i] == NULL) {
            //If one of the type creations is unsuccessful, we will want to free the slots of all previously created Pokemon types and the pointer to the pointer array.
            for (int j = 0; j < i + 1; j++) {
                freePokemonType(pArrayToPokemonTypes[j]);
            }
            free(pArrayToPokemonTypes);
            return NULL;
        }
    }

    //Updating the lists in the types we created
    while (fgets(pbuffer, size, file_pointer)) {
        //We will go down a line to get to the details that need to be updated about the type.
        if (strncmp(pbuffer, "Pokemons", 8) == 0) {
            //As long as we haven't reached the line whose first word is Pokemon, we'll want to update the relevant type and move down a line.
            break;
        }

        char temporoary[300];
        strcpy(temporoary, pbuffer);
        char *ptype_to_edit = temporoary;
        while (*ptype_to_edit == ' ' || *ptype_to_edit == '\t') {
            //skipping the spaces at the beginning
            ptype_to_edit++;
        }

        char *type_name_to_edit = strtok(ptype_to_edit, " ");
        //After skipping the spaces at the beginning, we will take the first word that will be the name of our first Pokemon.
        help_func(type_name_to_edit);
        char *find = strchr(pbuffer, ':');
        if (find == NULL) {
            continue;
        }
        int is_me = 0;
        if (strncmp(find - 2, "me", 2) == 0) {
            //Check if two characters before the : contain me
            is_me = 1;
        }
        if (is_me == 1) {
            char *names_of_affectiveAgainstMe = find + 1;
            char *name_against_me = strtok(names_of_affectiveAgainstMe, ",");
            //We will take the words separated by commas, and these will be the types we will need to add to the list of affective Against Me.
            while (name_against_me != NULL) {
                //We will place the separated word in the i position and then increase the i.
                help_func(name_against_me);
                for (int k = 0; k < numberOfTypes; k++) {
                    if (strcmp(pArrayToPokemonTypes[k]->name, type_name_to_edit) == 0) {
                        //We will go through each of the types we created and look for the type that matches the name to which we want to add types that are more effective than it.
                        for (int m = 0; m < numberOfTypes; m++) {
                            if (strcmp(pArrayToPokemonTypes[m]->name, name_against_me) == 0) {
                                //We will go through each of the types we created and look for the type that matches the name we want to add to one of the lists.
                                if (addEffectiveAgainstMe(pArrayToPokemonTypes[k], pArrayToPokemonTypes[m]) ==failure) {
                                    //We will update the list with the types we found.
                                    for (int j = 0; j < i; j++) {
                                        freePokemonType(pArrayToPokemonTypes[j]);
                                    }
                                    free(pArrayToPokemonTypes);
                                    return NULL;
                                }
                            }
                        }
                    }
                }

                name_against_me = strtok(NULL, ",");
                //We call the separation function again this time with NULL so that it continues where it left off.
            }
        }
        //If the word before is not I will act similarly only for the list of effective against others
        else {
            char *names_of_affectiveAgainstOthers = find + 1;
            char *name_against_others = strtok(names_of_affectiveAgainstOthers, ",");
            while (name_against_others != NULL) {
                //We will place the separated word in the i position and then increase the i.
                help_func(name_against_others);
                for (int k = 0; k < numberOfTypes; k++) {
                    if (strcmp(pArrayToPokemonTypes[k]->name, type_name_to_edit) == 0) {
                        for (int m = 0; m < numberOfTypes; m++) {
                            if (strcmp(pArrayToPokemonTypes[m]->name, name_against_others) == 0) {
                                if (addEffectiveAgainstOthers(pArrayToPokemonTypes[k], pArrayToPokemonTypes[m]) ==failure) {
                                    for (int j = 0; j < i; j++) {
                                        freePokemonType(pArrayToPokemonTypes[j]);
                                    }
                                    free(pArrayToPokemonTypes);
                                    return NULL;
                                }
                            }
                        }
                    }
                }
                name_against_others = strtok(NULL, ",");
                //We call the separation function again this time with NULL so that it continues where it left off.
            }
        }
    }
    return pArrayToPokemonTypes;
}

static Pokemon **createPokemonsArray(char *pbuffer, int size, FILE *file_pointer, int numberOfPokemons,
                                     int numberOfTypes, Pokemon_type **ptypes_array) {
    if (file_pointer == NULL) {
        return NULL;
    }
    if (strncmp(pbuffer, "Pokemons", 8) == 0) {
        Pokemon **temp = (Pokemon **) malloc(numberOfPokemons * sizeof(Pokemon *));
        if (temp == NULL) {
            return NULL;
        }
        Pokemon **pArrayToPokemon = temp;
        for (int i = 0; i < numberOfPokemons; i++) {
            pArrayToPokemon[i] = NULL;
        }
        for (int i = 0; i < numberOfPokemons; i++) {
            if (fgets(pbuffer, size, file_pointer) == NULL) {
                //We will go down a row to reach the data of the next Pokémon we want to create.
                break;
            }
            //We will extract the details of the Pokemon we want to create.
            char *pokemon_name = strtok(pbuffer, ",");
            char *pokemon_species = strtok(NULL, ",");
            double height = strtod(strtok(NULL, ","),NULL);
            double weight = strtod(strtok(NULL, ","),NULL);
            int attack = strtol(strtok(NULL, ","),NULL, 10);
            char *type_name = strtok(NULL, "\n");
            help_func(type_name);
            Pokemon_type *pMyPokemonType = NULL;
            //After we have found the name of the Pokemon type, we will run through the array of pointers to the different types to find the pointer to our type.
            for (int j = 0; j < numberOfTypes; j++) {
                if (strcmp(ptypes_array[j]->name, type_name) == 0) {
                    pMyPokemonType = ptypes_array[j];
                    break;
                }
            }
            //After we find the type pointer, we will create our Pokemon and link it to the pointer array.
            if (pMyPokemonType != NULL) {
                pArrayToPokemon[i] = createPokemon(pokemon_name, pokemon_species, pMyPokemonType, height, weight,
                                                   attack);
                if (pArrayToPokemon[i] == NULL) {
                    for (int j = 0; j < i; j++) {
                        freePokemon(pArrayToPokemon[j]);
                    }
                    free(pArrayToPokemon);
                    return NULL;
                }
            } else {
                for (int j = 0; j < i; j++) {
                    freePokemon(pArrayToPokemon[j]);
                }
                free(pArrayToPokemon);
                return NULL;
            }
        }
        //We will return the pointer to the array of Pokemon pointers
        return pArrayToPokemon;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int numberOfTypes = strtol(argv[1],NULL, 10);
    int numberOfPokemons = strtol(argv[2],NULL, 10);
    char *pconfigurationFile = argv[3];
    char buffer[300];
    char *pbuffer = buffer;
    FILE *file_pointer = fopen(pconfigurationFile, "r");
    if (file_pointer == NULL) {
        return failure;
    }
    //Creating a Pokedex
    Pokemon_type **ptypes_array = createTypesArray(pbuffer, 300, file_pointer, numberOfTypes);
    if (ptypes_array == NULL) {
        printf("Memory Problem\n");
        fclose(file_pointer);
        return failure;
    }

    Pokemon **pArrayToPokemon = createPokemonsArray(pbuffer, 300, file_pointer, numberOfPokemons, numberOfTypes,ptypes_array);
    if (pArrayToPokemon == NULL) {
        printf("Memory Problem\n");
        fclose(file_pointer);
        for (int i = 0; i < numberOfTypes; i++) {
            freePokemonType(ptypes_array[i]);
        }
        free(ptypes_array);
        return failure;
    }

    //Printing a menu for the user
    const char *menu =
            "Please choose one of the following numbers:\n"
            "1 : Print all Pokemons\n"
            "2 : Print all Pokemons types\n"
            "3 : Add type to effective against me list\n"
            "4 : Add type to effective against others list\n"
            "5 : Remove type from effective against me list\n"
            "6 : Remove type from effective against others list\n"
            "7 : Print Pokemon by name\n"
            "8 : Print Pokemons by type\n"
            "9 : Exit\n";
    while (1) {
        printf("%s", menu);
        char input_from_user;
        scanf(" %c", &input_from_user);
        switch (input_from_user) {
            case '1':
                for (int i = 0; i < numberOfPokemons; i++) {
                    if (pArrayToPokemon[i] == NULL) {
                        break;
                    }
                    printPokemon(pArrayToPokemon[i]);
                }
                break;
            case '2':
                for (int i = 0; i < numberOfTypes; i++) {
                    printPokemonType(ptypes_array[i]);
                }
                break;

            case '3': {
                Pokemon_type *ptypeToChange = NULL;
                printf("Please enter type name:\n");
                char type_name[300];
                scanf("%s", type_name);
                for (int i = 0; i < numberOfTypes; i++) {
                    if (strcmp(ptypes_array[i]->name, type_name) == 0) {
                        ptypeToChange = ptypes_array[i];
                        break;
                    }
                }
                if (ptypeToChange == NULL) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                printf("Please enter type name to add to %s effective against me list:\n", type_name);
                Pokemon_type *ptypeToAdd = NULL;
                char name_to_add[300];
                scanf("%s", name_to_add);
                for (int j = 0; j < numberOfTypes; j++) {
                    if (strcmp(ptypes_array[j]->name, name_to_add) == 0) {
                        ptypeToAdd = ptypes_array[j];
                        break;
                    }
                }
                if (ptypeToAdd == NULL) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                if (addEffectiveAgainstMe(ptypeToChange, ptypeToAdd) != already_exist) {
                    printPokemonType(ptypeToChange);
                } else {
                    printf("This type already exist in the list.\n");
                }
                break;
            }
            case '4': {
                Pokemon_type *ptypeToChange = NULL;
                printf("Please enter type name:\n");
                char type_name[300];
                scanf("%s", type_name);
                for (int i = 0; i < numberOfTypes; i++) {
                    if (strcmp(ptypes_array[i]->name, type_name) == 0) {
                        ptypeToChange = ptypes_array[i];
                        break;
                    }
                }
                if (ptypeToChange == NULL) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                printf("Please enter type name to add to %s effective against others list:\n", type_name);
                Pokemon_type *ptypeToAdd = NULL;
                char name_to_add[300];
                scanf("%s", name_to_add);
                for (int j = 0; j < numberOfTypes; j++) {
                    if (strcmp(ptypes_array[j]->name, name_to_add) == 0) {
                        ptypeToAdd = ptypes_array[j];
                        break;
                    }
                }
                if (ptypeToAdd == NULL) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                if (addEffectiveAgainstOthers(ptypeToChange, ptypeToAdd) != already_exist) {
                    printPokemonType(ptypeToChange);
                } else {
                    printf("This type already exist in the list.\n");
                }
                break;
            }
            case '5': {
                printf("Please enter type name:\n");
                char type_name_3[300];
                scanf("%s", type_name_3);
                Pokemon_type *ptypeToRemoveFrom = NULL;
                for (int i = 0; i < numberOfTypes; i++) {
                    if (strcmp(ptypes_array[i]->name, type_name_3) == 0) {
                        ptypeToRemoveFrom = ptypes_array[i];
                        break;
                    }
                }
                if (ptypeToRemoveFrom == NULL) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                printf("Please enter type name to remove from %s effective against me list:\n", type_name_3);
                Pokemon_type *ptypeToRemove = NULL;
                char name_to_remove[300];
                scanf("%s", name_to_remove);
                for (int j = 0; j < ptypeToRemoveFrom->num_Against_me; j++) {
                    if (strcmp(ptypeToRemoveFrom->effective_against_me[j]->name, name_to_remove) == 0) {
                        ptypeToRemove = ptypeToRemoveFrom->effective_against_me[j];
                        break;
                    }
                }
                if (ptypeToRemove == NULL) {
                    printf("Type name doesn't exist in the list.\n");
                    continue;
                }
                if (deletePokemonAgainstMe(ptypeToRemoveFrom, ptypeToRemove) != not_exist) {
                    printPokemonType(ptypeToRemoveFrom);
                }
                break;
            }
            case '6': {
                printf("Please enter type name:\n");
                char type_name_4[300];
                scanf("%s", type_name_4);
                Pokemon_type *ptypeToRemoveFrom_2 = NULL;
                for (int i = 0; i < numberOfTypes; i++) {
                    if (strcmp(ptypes_array[i]->name, type_name_4) == 0) {
                        ptypeToRemoveFrom_2 = ptypes_array[i];
                        break;
                    }
                }
                if (ptypeToRemoveFrom_2 == NULL) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                printf("Please enter type name to remove from %s effective against others list:\n", type_name_4);
                Pokemon_type *ptypeToRemove_2 = NULL;
                char name_to_remove_2[300];
                scanf("%s", name_to_remove_2);
                for (int j = 0; j < ptypeToRemoveFrom_2->num_Against_others; j++) {
                    if (strcmp(ptypeToRemoveFrom_2->effective_against_others[j]->name, name_to_remove_2) == 0) {
                        ptypeToRemove_2 = ptypeToRemoveFrom_2->effective_against_others[j];
                        break;
                    }
                }
                if (ptypeToRemove_2 == NULL) {
                    printf("Type name doesn't exist in the list.\n");
                    continue;
                }
                if (deletePokemonAgainstOthers(ptypeToRemoveFrom_2, ptypeToRemove_2) != not_exist) {
                    printPokemonType(ptypeToRemoveFrom_2);
                    continue;
                }
                break;
            }
            case '7': {
                printf("Please enter Pokemon name:\n");
                char name[300];
                scanf("%s", name);
                Pokemon *ppokemonToPrint_2 = NULL;
                for (int i = 0; i < numberOfPokemons; i++) {
                    if (pArrayToPokemon[i] == NULL) {
                        break;
                    }
                    if (strcmp(pArrayToPokemon[i]->name, name) == 0) {
                        ppokemonToPrint_2 = pArrayToPokemon[i];
                        break;
                    }
                }
                if (ppokemonToPrint_2 == NULL) {
                    printf("The Pokemon doesn't exist.\n");
                } else {
                    printPokemon(ppokemonToPrint_2);
                }
                break;
            }
            case '8': {
                printf("Please enter type name:\n");
                char type_name_5[300];
                scanf("%s", type_name_5);
                int type_counter = 0;
                int type_exist = 0;
                for (int j = 0; j < numberOfTypes; j++) {
                    if (strcmp(ptypes_array[j]->name, type_name_5) == 0) {
                        type_exist = 1;
                    }
                }
                if (type_exist == 0) {
                    printf("Type name doesn't exist.\n");
                    continue;
                }
                Pokemon *array_to_return[numberOfPokemons];
                for (int i = 0; i < numberOfPokemons; i++) {
                    if (pArrayToPokemon[i] == NULL) {
                        break;
                    }
                    if (strcmp(pArrayToPokemon[i]->type->name, type_name_5) == 0) {
                        array_to_return[type_counter] = pArrayToPokemon[i];
                        type_counter++;
                    }
                }
                if (type_counter == 0) {
                    printf("There are no Pokemons with this type.\n");
                    continue;
                }
                printf("There are %d Pokemons with this type:\n", type_counter);
                for (int i = 0; i < type_counter; i++) {
                    printPokemon(array_to_return[i]);
                }
                break;
            }
            case '9':
                printf("All the memory cleaned and the program is safely closed.\n");
                fclose(file_pointer);
                //Frees each of the types and the array of pointers
                for (int i = 0; i < numberOfTypes; i++) {
                    freePokemonType(ptypes_array[i]);
                }
                free(ptypes_array);

                //Releases each of the Pokémon and the array of pointers
                for (int j = 0; j < numberOfPokemons; j++) {
                    if (pArrayToPokemon[j] == NULL) {
                        break;
                    }
                    freePokemon(pArrayToPokemon[j]);
                }
                free(pArrayToPokemon);

                return 0;
            default:
                printf("Please choose a valid number.\n");
        }
    }
}