//
// Created by ganit on 20/11/2025.
//
#include"Pokemon.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Pokemon_type *createType(char *name) {
    Pokemon_type *ppokemon_type = (Pokemon_type *) malloc(sizeof(Pokemon_type));
    if (ppokemon_type == NULL)
        return NULL;
    ppokemon_type->name = (char *) malloc(strlen(name) + 1);
    if (ppokemon_type->name == NULL)
    //We will check that the memory allocation was successful, if not we will return null.
    {
        free(ppokemon_type);
        return NULL;
    }
    strcpy(ppokemon_type->name, name);
    ppokemon_type->type_num = 0;
    ppokemon_type->effective_against_me = NULL;
    ppokemon_type->effective_against_others = NULL;
    ppokemon_type->num_Against_me = 0;
    //Since the array is initially empty, we initialize the number of elements in it to 0.
    ppokemon_type->num_Against_others = 0;
    //Since the array is initially empty, we initialize the number of elements in it to 0.
    return ppokemon_type;
}

Bio_information *createBioInformation(double height, double weight, int attack) {
    Bio_information *pbioinformation = (Bio_information *) malloc(sizeof(Bio_information));
    //A pointer from the stack to a Bio_information structure on the heap.
    if (pbioinformation == NULL) //We will check that the memory allocation was successful, if not we will return null.
        return NULL;
    pbioinformation->height = height;
    pbioinformation->weight = weight;
    pbioinformation->attack = attack;
    return pbioinformation;
}

Pokemon *createPokemon(char *name, char *species, Pokemon_type *pokemon_type, double height, double weight,int attack) {
    Pokemon *ppokemon = (Pokemon *) malloc(sizeof(Pokemon));
    //A pointer from the stack to a Pokémon structure on the heap.
    if (ppokemon == NULL) //We will check that the memory allocation was successful, if not we will return null.
        return NULL;
    ppokemon->name = (char *) malloc(sizeof(char) * strlen(name) + 1);
    //Using the pointer from the stack, we will access the Pokemon and create a pointer from the name to a dynamic array that will contain the name.
    if (ppokemon->name == NULL) {
        //We will check that the memory allocation was successful, if not we will return null.
        free(ppokemon);
        return NULL;
    }
    strcpy(ppokemon->name, name); //We will enter the name we received in the input into the place we created.
    ppokemon->species = (char *) malloc(sizeof(char) * strlen(species) + 1);
    //Using the pointer from the stack, we will access the Pokemon and create a pointer from the name to a dynamic array that will contain the name.
    //We will check that the memory allocation was successful, if not we will return null.
    if (ppokemon->species == NULL) {
        free(ppokemon->name);
        free(ppokemon);
        return NULL;
    }
    strcpy(ppokemon->species, species); //We will enter the species we received in the input into the place we created.
    ppokemon->type = pokemon_type; //The Pokemon type will be the content of the pointer to the Pokemon type structure.
    pokemon_type->type_num++;
    //Once we have created a Pokémon of a certain type, we will increase the number of Pokémon within that type by 1.
    ppokemon->bio_information = createBioInformation(height, weight, attack);
    //The bio_information type will be the content of the pointer to the Bio_information structure.
    if (ppokemon->bio_information == NULL) {
        free(ppokemon->name);
        free(ppokemon->species);
        free(ppokemon);
        return NULL;
    }
    return ppokemon; //Finally, we return the pointer to the structure we created in the stack.
}

//free functions
void freePokemonType(Pokemon_type *ppokemon_type) {
    if (ppokemon_type == NULL)
        return;
    free(ppokemon_type->name);
    free(ppokemon_type->effective_against_me);
    free(ppokemon_type->effective_against_others);
    free(ppokemon_type);
}

void freeBioInfo(Bio_information *pbioinformation) {
    if (pbioinformation == NULL)
        return;
    free(pbioinformation);
}

void freePokemon(Pokemon *ppokemon) {
    if (ppokemon == NULL)
        return;
    free(ppokemon->name);
    free(ppokemon->species);
    freeBioInfo(ppokemon->bio_information);
    free(ppokemon);
}

status addEffectiveAgainstMe(Pokemon_type *pokemonA, Pokemon_type *pokemonB) {
    if (pokemonA == NULL || pokemonB == NULL) {
        return failure;
    }
    for (int i = 0; i < pokemonA->num_Against_me; i++) {
        if (strcmp(pokemonA->effective_against_me[i]->name, pokemonB->name) == 0) {
            return already_exist;
        }
    }
    int new_size = pokemonA->num_Against_me + 1;
    Pokemon_type **temporary = realloc(pokemonA->effective_against_me, sizeof(Pokemon_type *) * new_size);
    //A pointer to the original location so that if the memory allocation fails, we won't lose it.
    if (temporary == NULL)
        return failure; //If the memory allocation was unsuccessful, we will return failure.
    pokemonA->num_Against_me++;
    pokemonA->effective_against_me = temporary;
    //If the memory allocation was successful, we will assign the pointer to the temporary variable we saved for the original pointer.
    pokemonA->effective_against_me[pokemonA->num_Against_me - 1] = pokemonB;
    //We will insert the new Pokémon type into the last position in the array.
    return success;
}

status addEffectiveAgainstOthers(Pokemon_type *pokemonA, Pokemon_type *pokemonB) {
    if (pokemonA == NULL || pokemonB == NULL) {
        return failure;
    }
    for (int i = 0; i < pokemonA->num_Against_others; i++) {
        if (strcmp(pokemonA->effective_against_others[i]->name, pokemonB->name) == 0) {
            return already_exist;
        }
    }
    int new_size = pokemonA->num_Against_others + 1;
    //We will want to increase the pointer array because we are adding a Pokemon type to the list, so accordingly we will need to add a pointer.
    Pokemon_type **temporary = realloc(pokemonA->effective_against_others, sizeof(Pokemon_type *) * new_size);
    //We will save the address after allocating the new location in a variable so that if the allocation fails we do not lose the original allocation.
    if (temporary == NULL)
        return failure; //If the memory allocation was unsuccessful, we will return failure.
    pokemonA->num_Against_others++;
    pokemonA->effective_against_others = temporary;
    //If the memory allocation was successful, we will assign the pointer to the temporary variable we saved for the original pointer.
    pokemonA->effective_against_others[pokemonA->num_Against_others - 1] = pokemonB;
    //We will insert the new Pokémon type into the last position in the array.
    return success;
}

//delete functions
status deletePokemonAgainstMe(Pokemon_type *pokemonA, Pokemon_type *pokemonB) {
    if (pokemonA == NULL || pokemonB == NULL) {
        return failure;
    }
    //We will loop over the types of Pokémon that are on the list of Pokémon that are more effective than me, and look for Pokémon type B.
    for (int i = 0; i < pokemonA->num_Against_me; i++) {
        if (strcmp(pokemonA->effective_against_me[i]->name, pokemonB->name) == 0) {
            //If we found type B, We will run a function that will free the dynamic arrays in the element we want to overwrite so that there is no memory leak.
            for (int j = i; j < pokemonA->num_Against_me - 1; j++) {
                //we will run another loop and overwrite it using pointers found to its right in the dynamic array.
                pokemonA->effective_against_me[j] = pokemonA->effective_against_me[j + 1];
            }
            pokemonA->num_Against_me--;
            //After moving the pointers, we will reduce the number of types in the relevant list by 1.
            if (pokemonA->num_Against_me == 0) {
                //If after deletion there are no more appositive types left, we will want to free up the pointers array and hold null in the pointer.
                free(pokemonA->effective_against_me);
                pokemonA->effective_against_me = NULL;
                return success;
            }
            //We will reduce the lecture space after deleting the type
            Pokemon_type **temporary = (Pokemon_type **) realloc(pokemonA->effective_against_me,
                                                                 pokemonA->num_Against_me * sizeof(Pokemon_type *));
            //We will save the address after allocating the new location in a variable so that if the allocation fails we do not lose the original allocation.
            if (temporary == NULL)
                return failure;
            pokemonA->effective_against_me = temporary;
            //If the assignment is successful, we will place it at the original address.
            return success;
        }
    }
    return not_exist;
}

status deletePokemonAgainstOthers(Pokemon_type *pokemonA, Pokemon_type *pokemonB) {
    //We will loop over the types of Pokémon that are on the list of Pokémon that are less effective than me, and look for Pokémon type B.
    if (pokemonA == NULL || pokemonB == NULL) {
        return failure;
    }
    for (int i = 0; i < pokemonA->num_Against_others; i++) {
        if (strcmp(pokemonA->effective_against_others[i]->name, pokemonB->name) == 0) {
            //If we found type B, we will run another loop and overwrite it using values found to its right in the dynamic array.
            for (int j = i; j < pokemonA->num_Against_others - 1; j++) {
                pokemonA->effective_against_others[j] = pokemonA->effective_against_others[j + 1];
            }
            pokemonA->num_Against_others--;
            //After moving the values, we will reduce the number of types in the relevant list by 1.
            if (pokemonA->num_Against_others == 0) {
                //If after deletion there are no more appositive types left, we will want to free up the space and hold null in the pointer.
                free(pokemonA->effective_against_others);
                pokemonA->effective_against_others = NULL;
            }
            //We will reduce the lecture space after deleting the type
            Pokemon_type **temporary = (Pokemon_type **) realloc(pokemonA->effective_against_others,
                                                                 pokemonA->num_Against_others * sizeof(Pokemon_type *));
            //We will save the address after allocating the new location in a variable so that if the allocation fails we do not lose the original allocation.
            if (temporary == NULL)
                return failure;
            pokemonA->effective_against_others = temporary;
            //If the assignment is successful, we will place it at the original address.
            return success;
        }
    }
    return not_exist;
}

status printPokemon(Pokemon *ppokemon) {
    if (ppokemon == NULL|| !ppokemon->bio_information || !ppokemon->type )
        return failure;

    printf("%s :\n", ppokemon->name);
    printf("%s, %s Type.\n", ppokemon->species, ppokemon->type->name);
    printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n\n", ppokemon->bio_information->height,
           ppokemon->bio_information->weight, ppokemon->bio_information->attack);
    return success;
}

status printPokemonType(Pokemon_type *ppokemonType) {
    if (ppokemonType == NULL)
        return failure;
    printf("Type %s -- %d pokemons\n", ppokemonType->name, ppokemonType->type_num);
    if (ppokemonType->num_Against_me != 0) {
        printf("\tThese types are super-effective against %s:", ppokemonType->name);
        for (int i = 0; i < ppokemonType->num_Against_me; i++) {
            char *name_to_print = ppokemonType->effective_against_me[i]->name;
            if (i != ppokemonType->num_Against_me - 1) {
                printf("%s ,", name_to_print);
            } else {
                printf("%s\n", name_to_print);
            }
        }
    }
    if (ppokemonType->num_Against_others != 0) {
        printf("\t%s moves are super-effective against:", ppokemonType->name);
        for (int i = 0; i < ppokemonType->num_Against_others; i++) {
            char *name_to_print = ppokemonType->effective_against_others[i]->name;
            if (i != ppokemonType->num_Against_others - 1) {
                printf("%s ,", name_to_print);
            } else {
                printf("%s\n\n", name_to_print);
            }
        }
    } else {
        printf("\n");
    }
    return success;
}
