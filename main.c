/*-------------------------------------------
Program: Food Web Analysis with Dynamic Memory
Course: CS 211, Fall 2024, UIC
Author: Guillermo Ramirez III
Date: 9/28/2024
UIN:664042145

This C code simulates a food web, Users can add organisms, add and remove relationships between organisms,
This code also analyizes and outputs each apex preditor, producers, flexible eaters, tastiest food, heights, and vore types.

------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices  
    int numPrey;
} Org;


void addOrgToWeb(Org** ppWeb, int* pNumOrg, char* newOrgName) {

    //Makes memory for new animal to come in and join the party (in all seriousness, a new organism has entered the food web)
    Org* web = (Org*)malloc(sizeof(Org) * (*pNumOrg+1));
    
    for(int i = 0; i < (*pNumOrg); i++){
        web[i]= (*ppWeb)[i];
    }
    //free the old array
    free(*ppWeb);
    
    //rewire the array name to the new array
    (*ppWeb) = web;
    //update the size parameter for the array
    strcpy((*ppWeb)[*pNumOrg].name, newOrgName);
    (*ppWeb)[*pNumOrg].numPrey = 0;
    (*ppWeb)[*pNumOrg].prey = NULL;
    (*pNumOrg)++;

}


bool addRelationToWeb(Org* pWeb, int numOrg, int predInd, int preyInd) {

    // Check if the predator or prey index is invalid
    if (predInd < 0 || predInd >= numOrg || preyInd < 0 || preyInd >= numOrg) {
        printf("Invalid predator and/or prey index. No relation added to the food web.\n");
        return false;
    }

    // Check for duplicate prey relationship
    for (int i = 0; i < pWeb[predInd].numPrey; i++) {
        if (pWeb[predInd].prey[i] == preyInd) {
            printf("Duplicate predator/prey relation. No relation added to the food web.\n");
            return false;
        }
    }

    // Allocate new space for the prey array of the predator (if it's NULL, initialize it)
    int* newPrey = (int*)malloc((pWeb[predInd].numPrey + 1) * sizeof(int));

    

    // Copy existing prey indices to the new array
    for (int i = 0; i < pWeb[predInd].numPrey; i++) {
        newPrey[i] = pWeb[predInd].prey[i];
    }

    // Add the new prey to the predator's prey array
    newPrey[pWeb[predInd].numPrey] = preyInd;

    // Free the old prey array if it exists
    if (pWeb[predInd].prey != NULL) {
        free(pWeb[predInd].prey);
    }

    // Update the predator's prey array and number of prey
    pWeb[predInd].prey = newPrey;
    pWeb[predInd].numPrey++;

    return true;

}
    
bool removeOrgFromWeb(Org** ppWeb, int* pNumOrg, int index) {

    //Checking if the index is valid
    if(index < 0 || index >= *pNumOrg){ 
        printf("Invalid extinction index. No organism removed from the food web.\n");
        return false;
    }

    // If there's only one animal left, free the web and decrement the size
    if((*pNumOrg) == 1){ 
        free((*ppWeb)[0].prey); // Free the only animal
        free((*ppWeb)); //Free the whole web
        (*ppWeb) = NULL; //Set web to NULL
        (*pNumOrg)--; // Decrements the number of animals
        return true; //Get out of function and confirm that we have changed the web      
    }

    //Remove the specified animal from every organism's prey array
    for(int animalIndex = 0; animalIndex < *pNumOrg; animalIndex++){ //Index of all animals in pWeb
        bool need_change = false;
        if((*ppWeb)[animalIndex].numPrey > 0){ //If the current animal we are on does not have a numPrey of 0, then we continue
            
            for(int preyIndex = 0; preyIndex < (*ppWeb)[animalIndex].numPrey; preyIndex++){ //Loop through animal's orginal prey array
                if((*ppWeb)[animalIndex].prey[preyIndex] == index || (*ppWeb)[animalIndex].prey[preyIndex] > index ){ // If the animal we don't want is found, then we need to change animalIndex's prey array
                    need_change = true;
                    break;
                }
            }


            if(need_change){
                int* preyArr = (int*) malloc(sizeof(int) * (*ppWeb)[animalIndex].numPrey); //Creates new Prey Array for specifically animal's prey index
                int preyArrIndex = 0; //keep track of the index were in for prey Array
            
                for(int preyIndex = 0; preyIndex < (*ppWeb)[animalIndex].numPrey; preyIndex++){ //Loop through animal's orginal prey array
                    
                    if ((*ppWeb)[animalIndex].prey[preyIndex] == index){
                        continue;
                    }
                    else if((*ppWeb)[animalIndex].prey[preyIndex] > index){ //If the current prey has a higher number value as the animal we wanted out... 
                        preyArr[preyArrIndex++] = (*ppWeb)[animalIndex].prey[preyIndex] - 1; //We skip over the animal we don't want but we minus the prey so we don't go out of range and keep the animals we want
                    }
                    else if ((*ppWeb)[animalIndex].prey[preyIndex] < index){ //If we're not on the animal we want out, we don't skip over any animals... yet
                        preyArr[preyArrIndex++] = (*ppWeb)[animalIndex].prey[preyIndex];
                    }
                }
                //Say goodbye to the old prey array
                free((*ppWeb)[animalIndex].prey);

                //And say hello to the new prey array!
                if(preyArrIndex == 0){
                    (*ppWeb)[animalIndex].prey = NULL;
                }
                else{
                    (*ppWeb)[animalIndex].prey = preyArr;
                }

                //Update numPrey
                (*ppWeb)[animalIndex].numPrey = preyArrIndex;

            }
        }
        
    }
    Org* web = (Org*)malloc(sizeof(Org) * (*pNumOrg - 1)); //New web without the animal specified
    
    for(int i = 0; i < index; i++){ //Loop through index until the index of animal we want out
        web[i]= (*ppWeb)[i];
    }
    free((*ppWeb)[index].prey);
    for(int i = index + 1; i < *pNumOrg; i++){ //Loop after the index we want out and continue until end of web
        web[i - 1]= (*ppWeb)[i];
    }
    
    //free the old array
    free(*ppWeb);
    
    //rewire the array name to the new array
    *ppWeb = web;

    //update the size parameter for the array
    (*pNumOrg)--;
    
    return true;

}


void freeWeb(Org* pWeb, int numOrg) {

    //Free every prey array of every animal
    for(int i = 0; i < numOrg; i++){
        free(pWeb[i].prey);
    }
    //Free the Web
    free(pWeb);
}

void printWeb(Org* pWeb, int numOrg) {

    //Go through every animal in food web
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey > 0){
            printf("  (%d) %s eats ", i, pWeb[i].name);
            for(int k = 0; k < pWeb[i].numPrey; k++){ //This for loops access all of the current animal's prey list and prints each prey
                if(k != pWeb[i].numPrey - 1){
                    printf("%s, ", pWeb[pWeb[i].prey[k]].name);
                }
                else{
                    printf("%s\n", pWeb[pWeb[i].prey[k]].name);
                }
            }
        }
        else if(pWeb[i].numPrey == 0){
            printf("  (%d) %s\n", i, pWeb[i].name);
        }
    }

}

void printApexPredators(Org* pWeb, int numOrg){
    int animalCount = 0;
    for(int currAnimal = 0; currAnimal < numOrg; currAnimal++){ // Outer for loop to compare to current animal
        animalCount = 0; // Count how many times the animal pops up as a prey
        
        for(int i = 0; i < numOrg; i++){ // for loop goes through each organism to compare  to the current animal
            for(int k = 0; k < pWeb[i].numPrey; k++){ // for loop goes through each animals prey's index
                if( pWeb[pWeb[i].prey[k]].name == pWeb[currAnimal].name){ // compares the prey index with current animal index
                    animalCount++; // If the name matche up in the list of prey, we will add by 1 in animalCount to count the times the current animal has been detected as a prey
                }
            }
        }
        if(animalCount == 0){ // If the animal has not been detected as a prey, it is an apex animal
            printf("  %s\n", pWeb[currAnimal].name); // Print out the current apex animal
        }
        
    }
}

void printProducers(Org* pWeb, int numOrg){

    //Goes through each animal and if their numPrey is 0, then its a producer!
    for(int i = 0; i < numOrg; i++){
        if(pWeb[i].numPrey == 0){
            printf("  %s\n", pWeb[i].name);
        }
    }
}

void printFlexibleEaters(Org* pWeb, int numOrg){

    //Big back is a joke for the animal who eats the most prey which by definition, is a flexible eater
    int bigBackArray[numOrg] = {};
    int arrayIndex = 0;
    int bigBackCount = 0;
    for(int i = 0; i < numOrg; i++){ // loop goes through each animal
        if(pWeb[i].numPrey > bigBackCount){ // If the number of prey is the highest, we know who's the big back

            bigBackCount = pWeb[i].numPrey; // Sets the highest bigBackCount with the highest current num of prey
            bigBackArray[arrayIndex] = i; //i represents the organism's index
            
            arrayIndex++;
        }
        else if(pWeb[i].numPrey == bigBackCount){
            bigBackArray[arrayIndex] = i; 
            arrayIndex++;
        }
    }

    int finalArray[numOrg] = {};
    int finalIndex = 0;
    for (int g = 0 ; g < arrayIndex; g++){
        if(pWeb[bigBackArray[g]].numPrey > bigBackCount){ // If the number of prey is the highest, we know who's the big back
            finalArray[finalIndex] = bigBackArray[g]; 
            finalIndex++;
        }
        else if(pWeb[bigBackArray[g]].numPrey == bigBackCount){
            finalArray[finalIndex] = bigBackArray[g];  
            finalIndex++;
        }
    }

    for (int h = 0; h < finalIndex; h++){
        printf("  %s\n", pWeb[finalArray[h]].name);
    }
}

void printTastiestFood(Org* pWeb, int numOrg){
    int highestPreyCount = 0; // Keep track of highest prey count
    int nameOfTastiestArray[numOrg] = {}; // Stores tasty animals index array
    int tastiestAppearanceCountArray[numOrg] = {};
    int arrayIndex = 0;

    for(int currAnimal = 0; currAnimal < numOrg; currAnimal++){ // Outer for loop to compare to current animal
        int animalCount = 0; // Count how many times the animal pops up as a prey
        
        for(int i = 0; i < numOrg; i++){ // for loop goes through each organism to compare  to the current animal
            for(int k = 0; k < pWeb[i].numPrey; k++){ // for loop goes through each animals prey's index
                if( pWeb[pWeb[i].prey[k]].name == pWeb[currAnimal].name){ // compares the prey index with current animal index
                    animalCount++; // If the name match up in the list of prey, we will add by 1 in animalCount to count the times the current animal has been detected as a prey
                }
            }
        }
        if(animalCount > highestPreyCount){ // If the number of prey is the highest, we know who's the big back

            highestPreyCount = animalCount; // Sets the highest number of animal with the highest current num of appearnces as a prey
            nameOfTastiestArray[arrayIndex] = currAnimal; //arrayIndex represents the organism's index
            tastiestAppearanceCountArray[arrayIndex] = animalCount;
            
            arrayIndex++;
        }
        else if(animalCount == highestPreyCount){
            nameOfTastiestArray[arrayIndex] = currAnimal; //arrayIndex represents the organism's index
            tastiestAppearanceCountArray[arrayIndex] = animalCount;

            arrayIndex++;
        }
        
    }

    for(int animalIndex = 0; animalIndex < arrayIndex; animalIndex++){
        if((tastiestAppearanceCountArray[animalIndex] == highestPreyCount)){
            printf("  %s\n", pWeb[nameOfTastiestArray[animalIndex]].name);
        }
    }
}

void printHeights(Org* pWeb, int numOrg){
    //Int array to store height numbers of each animal
    int heights[numOrg];

    //Sets every number to 0
    for(int animalIndex = 0; animalIndex < numOrg; animalIndex++){
        heights[animalIndex] = 0;
    }

    bool change = true;

    //While there's a change from a new max height, then keep going 
    while(change){
        change = false;

        //For loop goes through each animal in web
        for(int animalIndex = 0; animalIndex < numOrg; animalIndex++){
            int max_height = 0;

            //Goes through current animal's prey index
            for(int preyIndex = 0; preyIndex < pWeb[animalIndex].numPrey; preyIndex++){
                //If the number from the animal prey's height number is heigher than the max height, then set the max as the number that beat max
                if(heights[pWeb[animalIndex].prey[preyIndex]] > max_height){
                    max_height = heights[pWeb[animalIndex].prey[preyIndex]];
                }
            }

            //if the animal's height number is less than the max height, then change the animal's new height as the max plus 1
            if(heights[animalIndex] < max_height + 1){
                heights[animalIndex] = max_height + 1;
                change = true;
            }

        }
    }

    //For loop that prints each animal's height number
    for(int animalIndex = 0; animalIndex < numOrg; animalIndex++){
        printf("  %s: %d\n", pWeb[animalIndex].name, heights[animalIndex] - 1);
    }
}

void printVoreTypes(Org* pWeb, int numOrg){
    bool producerBoolArray[numOrg]; 
    int producerIndexArray[numOrg];
    int prodCount = 0;

    int herbivoreIndexArray[numOrg];
    int herbCount = 0;

    int omnivoreIndexArray[numOrg];
    int omniCount = 0;

    int carnivoreIndexArray[numOrg];
    int carnCount = 0;

    //Checks if the current animal has any prey, if not, then its a producer and the index is shoved into a producer array
    for(int animalIndex = 0; animalIndex < numOrg; animalIndex++){
        if(pWeb[animalIndex].numPrey == 0){
            producerBoolArray[animalIndex] = true;
            producerIndexArray[prodCount] = animalIndex;
            prodCount++;
        }
        else{
            producerBoolArray[animalIndex] = false;
        }
    }

    for(int animalIndex = 0; animalIndex < numOrg; animalIndex++){
        bool eatPlant = false;
        bool eatMeat = false;
        for(int preyIndex = 0; preyIndex < pWeb[animalIndex].numPrey; preyIndex++){
            //Checks if the current animal eats plants by checking it's prey if its a producer or not
            if(producerBoolArray[pWeb[animalIndex].prey[preyIndex]] ){
                eatPlant = true;
            }
            //Checks if the current animal eats meat
            else{
                eatMeat = true;
            }
        }

        //If the current animal sits the criteria of any of these if statement, then that is the animal's vore type
        if(eatPlant && !eatMeat){
            herbivoreIndexArray[herbCount] = animalIndex;
            herbCount++;
        }
        if(eatPlant && eatMeat){
            omnivoreIndexArray[omniCount] = animalIndex;
            omniCount++;
        }
        if (!eatPlant && eatMeat){
            carnivoreIndexArray[carnCount] = animalIndex;
            carnCount++;
        }
    }

    //Print statments that goes through each vore type's array of index and prints them out
    printf("  Producers:\n");
    
    for(int animalIndex = 0; animalIndex < prodCount; animalIndex++){
        printf("  %s\n", pWeb[producerIndexArray[animalIndex]].name);
    }

    printf("  Herbivores:\n");
    for(int animalIndex = 0; animalIndex < herbCount; animalIndex++){
        printf("  %s\n", pWeb[herbivoreIndexArray[animalIndex]].name);
    }
  
    printf("  Omnivores:\n");
    for(int animalIndex = 0; animalIndex < omniCount; animalIndex++){
        printf("  %s\n", pWeb[omnivoreIndexArray[animalIndex]].name);
    }

    
    printf("  Carnivores:\n");
    for(int animalIndex = 0; animalIndex < carnCount; animalIndex++){
        printf("  %s\n", pWeb[carnivoreIndexArray[animalIndex]].name);
    }
}

void displayAll(Org* pWeb, int numOrg, bool modified) {

    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(pWeb,numOrg); 
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
    //identify and print the organisms not eaten by any others
    printApexPredators(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    //identify and print the organisms that eat no other organisms
    printProducers(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");
    //identity and print the organism(s) with the most prey
    printFlexibleEaters(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    //identity and print organism(s) eaten by the most other organisms
    printTastiestFood(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    //calculate and print the length of the longest chain from any producer to each organism
    printHeights(pWeb, numOrg);
    printf("\n");

    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    //classified all organisms and print each group (producers, herbivores, omnivores, & carnivores)
    printVoreTypes(pWeb, numOrg);
    printf("\n");

}

bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {  

    int basicCall = 0;
    int debugCall = 0;
    int quietCall = 0;
    int invalidCall = 0;

    //If argc arguments is over 4, this means we have an invalid input, so return false
    if(argc > 4){
        return false;
    }
    else{
        if(argc == 1){ //Default
            *pBasicMode = false;
            *pDebugMode = false;
            *pQuietMode = false;
            return true;
        }
        else{
            //Set the settings of each mode if the mode was called once
            for(int i = 0; i < argc; i++){
                if(argv[i][1] == 'b'){
                    basicCall++;
                }
                else if(argv[i][1] == 'd'){
                    debugCall++;
                }
                else if(argv[i][1] == 'q'){
                    quietCall++;
                }
                else{
                    invalidCall++;
                }

            } 
            if(basicCall == 2 || debugCall == 2 || quietCall == 2 || invalidCall > 1){ //Checks invalid inputs
                return false;
            }
            //Officially sets modes based on user's input
            if(basicCall == 1){
                *pBasicMode = true;
            }
            if(debugCall == 1){
                *pDebugMode = true;
            }
            if(quietCall == 1){
                *pQuietMode = true;
            }
            return true;
        }
    }
    return true;

}


void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}


int main(int argc, char* argv[]) {  
    
    //If statements are placed all around main in order to know what are the next actions to take based on these bools
    bool basicMode = false;
    bool debugMode = false;
    bool quietMode = false;

    //Termination of program
    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1;
    }

    //Prints program settings based on basicMode bool
    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode);
    printf("  debug mode = ");
    printONorOFF(debugMode);
    printf("  quiet mode = ");
    printONorOFF(quietMode);
    printf("\n");

    
    int numOrgs = 0;
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* pWeb = NULL;

    //Builds the initial food web by asking the user for organism name until they input "DONE"
    printf("Building the initial food web...\n");
    
    //If quietMode is not true, then the if statement will run and take user's input of organism
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = "";
    scanf("%s",tempName); 
    if (!quietMode) printf("\n");
    //Seems as if we have a while loop until statisifed with number of organisms
    while (strcmp(tempName,"DONE") != 0) {
        addOrgToWeb(&pWeb,&numOrgs,tempName);
        if (debugMode) {
            printf("DEBUG MODE - added an organism:\n");
            printWeb(pWeb,numOrgs);
            printf("\n");
        }
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName); 
        if (!quietMode) printf("\n");
    }
    if (!quietMode) printf("\n");
    //Identifying where organism belongs in Food web until an invalid index has been entered
    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
    int predInd, preyInd;
    scanf("%d %d",&predInd, &preyInd);
    if (!quietMode) printf("\n");

    //While the organism does not have an invalid input and in range of num of organisms, while loop will run
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(pWeb,numOrgs,predInd,preyInd);
        if (debugMode) {
            printf("DEBUG MODE - added a relation:\n");
            printWeb(pWeb,numOrgs);
            printf("\n");
        }
        //Add another organism to food web
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
        scanf("%d %d",&predInd, &preyInd);  
        if (!quietMode) printf("\n");
    }
    printf("\n");

    printf("--------------------------------\n\n");
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");
    //Display food web
    displayAll(pWeb,numOrgs,false);

    //If we are not in basic mode, then we are modifying the food web
    if (!basicMode) {
        printf("--------------------------------\n\n");
        printf("Modifying the food web...\n\n");
        char opt = '?';
        //We have a bunch of commands to choose from and most commands calls out the functions I need to fill out in order for the program to work as intended
        while (opt != 'q') {
            if (!quietMode) {
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");
            }
            scanf(" %c", &opt);
            if (!quietMode) printf("\n\n");

            if (opt == 'o') { //Add a new organism
                char newName[20];
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName);
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&pWeb,&numOrgs,newName);
                printf("\n");

                if (debugMode) {
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'x') { //Remove organism
                int extInd;
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd);
                if (!quietMode) printf("\n");
                if (extInd >= 0 && extInd < numOrgs) {
                    printf("Species Extinction: %s\n", pWeb[extInd].name);
                    removeOrgFromWeb(&pWeb,&numOrgs,extInd);
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");
                
                if (debugMode) {
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'r') { // Add new relation
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd);
                if (!quietMode) printf("\n");

                if (addRelationToWeb(pWeb,numOrgs,predInd,preyInd)) {
                    printf("New Food Source: %s eats %s\n", pWeb[predInd].name, pWeb[preyInd].name);
                };
                printf("\n");
                if (debugMode) {
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(pWeb,numOrgs);
                    printf("\n");
                }

            } else if (opt == 'p') { // Print updated food web
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(pWeb,numOrgs);
                printf("\n");
                
            } else if (opt == 'd') { //Display all of the web's characteristics
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(pWeb,numOrgs,true);

            }
            printf("--------------------------------\n\n");
        
        }
        
    }
    //Funtion to free memory to not have a memory leak
    freeWeb(pWeb,numOrgs);

    return 0;
}

