//
// Created by ganit on 20/11/2025.
//

#ifndef HW2_POKEMON_H
#define HW2_POKEMON_H
#include "Defs.h"

//A structure that holds a Pokémon type and data about it.
typedef struct poke_type{
    char *name;
    int type_num;
    struct poke_type **effective_against_me;
    struct poke_type **effective_against_others;
    int num_Against_me;
    int num_Against_others;
} Pokemon_type;

//A structure that stores biological information about a Pokémon
typedef struct bio_info{
    double height;
    double weight;
    int attack;
} Bio_information;

//A structure that stores information about Pokémon
typedef struct {
    char *name;
    char *species;
    Pokemon_type *type;
    Bio_information *bio_information;
} Pokemon;

//Function declarations
//A function that creates a Pokémon
Pokemon *createPokemon(char *name,char *species,Pokemon_type *pokemon_type,double height, double weight,int attack);

//A function that creates a Pokémon type
Pokemon_type *createType(char *name);

//A function that generates biological information about a Pokémon
Bio_information *createBioInformation(double height,double weight,int attack);

//Add/delete functions from lists of more/less effective Pokémon
status addEffectiveAgainstMe(Pokemon_type *pokemonA , Pokemon_type *pokemonB);
status addEffectiveAgainstOthers(Pokemon_type *pokemonA , Pokemon_type *pokemonB);
status deletePokemonAgainstMe(Pokemon_type *pokemonA , Pokemon_type *pokemonB);
status deletePokemonAgainstOthers(Pokemon_type *pokemonA , Pokemon_type *pokemonB);

//free functions
void freePokemonType(Pokemon_type *ppokemon_type);
void freeBioInfo(Bio_information *pbioinformation);
void freePokemon(Pokemon *ppokemon);

//Print functions
status printPokemon(Pokemon *ppokemon);
status printPokemonType(Pokemon_type *ppokemonType);

#endif //HW2_POKEMON_H
