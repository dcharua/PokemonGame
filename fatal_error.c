/*
  PokemonServer.c
  FinalProyect
  24/11/18
  Daniel Charua - Joan Andoni
  Copyright (c) 2018. All rights reserved.
*/


#include "fatal_error.h"

void fatalError(const char * message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
