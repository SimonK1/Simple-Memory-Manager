//
//  Vlastník Simon Kokavec
//  Dátové štruktúry a algoritmy
//
//  Cvičiaci: Ing. František Horvát, PhD.
//  Cvičenie: Pondelok 18:00 - 19:50
//
//  Projekt "Zadanie 1 - Správca pamäti" na predmet s názvom dátové štruktúry a algoritmy.
//
//  Tento Projekt sa zaoberá správou pamäte a to konkrétne vytvorením a implementáciou vlastnej alokácie pamäte.
//  Pri tvorení programu nesmeli byť požité funkcie "malloc()" a "free()".
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define velkost_hlavicky (sizeof(memory))

// Definovanie štruktúry spájaného zoznamu.

typedef struct Memory
{
    unsigned int size;
    char used;
    struct Memory* next;
    
} memory;

memory* root;

int arrInt[4];

// Definovanie prototypov funkcii

void memory_init(void *ptr, unsigned int size);
void *memory_alloc(unsigned int size);
int memory_free(void *valid_ptr);
int memory_check(void *ptr);
int percentage (int wholeBlock, int arrNum[]);

// Funkcia memory_init predstavuje inicializáciu pamäte, ktorá sa bude využívať.

void memory_init(void *ptr, unsigned int size)
{
    root = ptr;
    root -> size = size;
    root -> used = 0;
    root -> next = NULL;
}

// Funkcia memory_alloc alokuje miesto v pamäti o zvolenej veľkosti. Nahrádza funkciu malloc().

void *memory_alloc(unsigned int size)
{

    // Ošetrenie prípadu kedy užívateľ zadal zlú veľkosť
    
    if (size == 0)
        return NULL;
    
    // Zarovnenie blokov na najvyšší násobok 2
    if ((size%2) == 1)
        size++;
    
    // Nájdenie prvého vhodného dostupného bloku na alokovanie. Podmienený veľkosťou a tým či už sa na danom mieste nenachádza iný alokovaná blok. Prechádzame spájaný zoznam na jeho nájdenie.
    
    memory *allocatedblock = root;
    
    while (allocatedblock != NULL)
    {
        if (allocatedblock -> used != 1)
            if (allocatedblock -> size >= (size + velkost_hlavicky))
            {
                // Rozdelenie bloku na dve časti. Na alokovanú časť a zvyšok pamäte (volné/dostupné na ďaľšie alokovanie). Zvyšok prázdnej pamäte sa vytvorí ako samostnaný nový blok ktorý sa následne prepojí.
                
                // Vytvorenie nového prázdneho bloku
                memory* freeblock =  ((void *) allocatedblock + velkost_hlavicky + size);
                
                // Zistenie či nový blok môže byť alokovaný
                if (allocatedblock -> size < (size + velkost_hlavicky*2 + 1))
                {
                    allocatedblock -> used = 1;
                    allocatedblock -> next = NULL;
                }
                // Napojenie nového prazdneho bloku na blok, ktoý bude alokovaný.
                else {
                    
                    freeblock -> size = (allocatedblock -> size - size - velkost_hlavicky);
                    freeblock -> used = 0;
                    freeblock -> next = allocatedblock -> next;
                    
                    // Úprava alokovaného bloku a jeho napojenie na nový prázdy blok.
                    
                    allocatedblock -> size = size + velkost_hlavicky;
                    allocatedblock -> used = 1;
                    allocatedblock -> next = freeblock;
                    
                    return (void*)allocatedblock + velkost_hlavicky;
                }
                
            }
                
         if (allocatedblock -> next == NULL)
                   return NULL;
        
        // Posunutie na nový blok
        allocatedblock = allocatedblock -> next;
    }
    return NULL;
}

// Funkcia memory_free uvoľní alokované miesto v pamäti. Nahrádza funkciu free().

int memory_free(void *valid_ptr)
{
    // Skontrolovanie či daný pointer je platný alebo nie
    if ((memory_check(valid_ptr) == 0) || (valid_ptr == NULL)){
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("!!!!!! Invalid Pointer Found !!!!!!\n");
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return 1;
    }
    
    memory *movingblock = valid_ptr;
    
    movingblock = (void*)movingblock - velkost_hlavicky;
    movingblock->used = 0;
    
    // Vyhľadanie a spájanie susedných voľných blokov
    
    movingblock = root;
    
    while ((movingblock->next) != NULL) {
        if (movingblock == NULL)
            break;
        if (movingblock->used == 0)
        {
            if (movingblock->next->used == 0)
            {
                // Spojenie susedných voľných blokov
                
                movingblock->size += (movingblock->next)->size;
                movingblock->next = (movingblock->next)->next;
                continue;
            }
        }
        // Posunutie na nový blok
        
        movingblock = movingblock->next;
    }
    return 0;
}

// Funkcia memory_check skontroluje či zadaný pointer je platný - či daný pointer bol vrátený funkciou memory_alloc(). Je potrebné prechádzať spájaný zoznam a porovnávať adresy.

int memory_check(void *ptr)
{
    if (ptr == NULL)
        return 0;
    else
        return 1;
}

// Funkcia pre výpočet %

int percentage(int wholeBlock, int arrNum[])
{
    memory* perc = root;
    int numberAll = 0;
    int numberIdeal = 0;
    
    // zrátanie počtu alokovaných blokov
    
    while (perc != NULL) {
        if (perc->used == 1){
            numberAll++;
        }
        perc = perc->next;
    }
    
    // Zrátanie počtu blokov pri ideálnom riešení
    while (wholeBlock > 0){
        for (int i = 0; i < 5; i++){
            wholeBlock -= arrNum[i];
            if (wholeBlock <= 0){
                break;
            }
            numberIdeal++;
        }
    }
    
    // Vypočítanie percentuálnej zložitosti
    int per = numberAll*100/numberIdeal;
    return per;
}


// Funkcia pre zorzumiteľné formátovanie výstupu

void formattedOutput()
{
    memory* print = root;
    int counter = 1;
    
    // Ohlásanie výpisu blokov
    printf("Formatted output for Memory checking\n");
    printf("___________________________\n");
    printf("\n");
    
    // Výpis jednotlivých blokov s ich informáciami
    while (print != NULL) {
        printf("\t%d. Block of memory:\n",counter++);
        printf("\n");
        printf("\t\t\t  Allocated:    \t\t\t\t%d \n", print->used);
        printf("\t\t\t  Size: \t\t\t\t\t\t%d \n", print->size);
        printf("\t\t\t  Address to header of block:\t%p \n",print);
        printf("\t\t\t  Address to data of block:\t\t%p \n", (void*)print + velkost_hlavicky);
        printf("\t\t\t  Address of the next block:\t%p \n", print->next);
        printf("\n");
        printf("\n");
        print = print->next;
    }
    printf("___________________________\n");
}

// Otestovanie programu

// Najjednoduchší test o 100B. Alokovanie 4 blokov po 8B plus hlavička. Uvoľnenie bloku 1 a 2 a ich spojenie do jedného.

void test1()
{
    clock_t t;
    t = clock();
    char region[100];
    memory_init(region, 100);
    
    printf("__________________________________________________________\n\n\t\t\t\t\t\tTest 1\n__________________________________________________________\n\n");
    printf("\t\t\t\t!!!!!ALL STEPS BELOW!!!!!!\n\n\nStep1 : Allocation of 4 blocks\n");
    printf("\n");
    
    // Alokovanie jednotlivých blokov
    
    char *test = memory_alloc(8);
    char *test2 = memory_alloc(8);
    char *test3 = memory_alloc(8);
    char *test4 = memory_alloc(8);
    formattedOutput();
    
    // Uvoľnenie bloku 2
    printf("Step2: Freeing of block 2\n");
    memory_free(test2);
    formattedOutput();
    
    
    // Uvoľnenie bloku 1
    printf("Step3: Freeing of block 1 + merging two blocks\n");
    memory_free(test);
    formattedOutput();
    
    printf("Statistics:\n");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
    
    // Vypočítanie percentuálnej úspešnosti testu
    printf("Success rate: %d", 4/4*100);
}

// Test2 o 100B. Alokovanie 4 blokov po 8B plus hlavička. Postupné uvoľnenie všetkých blokov a ich spojenie do jedného veľkého.

void test2()
{
    clock_t t;
    t = clock();
    char region[100];
    memory_init(region, 100);
    
    printf("__________________________________________________________\n\n\t\t\t\t\t\tTest 2\n__________________________________________________________\n\n");
    printf("\t\t\t\t!!!!!ALL STEPS BELOW!!!!!!\n\n\nStep1 : Allocation of 4 blocks\n");
    printf("\n");
    
    // Alokovanie jednotlivých blokov
    char *test = memory_alloc(8);
    char *test2 = memory_alloc(8);
    char *test3 = memory_alloc(8);
    char *test4 = memory_alloc(8);
    formattedOutput();
    
    
    // Postupné uvoľnovanie a spájanie všetkých blokov
    printf("Step2: Freeing of block 1\n");
    memory_free(test);
    formattedOutput();
    
    printf("Step3: Freeing of block 2 + merging two blocks\n");
    memory_free(test2);
    formattedOutput();
    
    printf("Step4: Freeing of block 3 + merging two blocks\n");
    memory_free(test3);
    formattedOutput();
    
    printf("Step5: Freeing of block 4 + merging two blocks\n");
    memory_free(test4);
    formattedOutput();
    
    printf("Statistics:\n");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
    
    // Vypočítanie percentuálnej úspešnosti testu
    printf("Success rate: %d", 4/4*100);
}

// Test pri ktorom skúsime alokovať blok ktorý sa už do pamäte nezmestí. Alokujú sa dva bloky z troch.

void test3()
{
    clock_t t;
    t = clock();
    char region[100];
    memory_init(region, 100);
    
    printf("__________________________________________________________\n\n\t\t\t\t\t\tTest 3\n__________________________________________________________\n\n");
    printf("\t\t\t\t!!!!!ALL STEPS BELOW!!!!!!\n\n\nStep1 : Allocation of 2 blocks (Last one does not allocate)\n");
    printf("\n");
    
    // Alokovanie jednotlivých blokov
    
    char *test = memory_alloc(32);
    char *test2 = memory_alloc(32);
    char *test3 = memory_alloc(32);
    formattedOutput();
    
    printf("Statistics:");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
    
    // Vypočítanie percentuálnej úspešnosti testu
    printf("Success rate: %d", 2/2*100);
}


// Test pri ktorom skúsime alokovať blok ktorý sa už do pamäte nezmestí + skúsime ho uvoľniť. Program oznámi neplatný pointer a nič neuvoľní.

void test4()
{
    clock_t t;
    t = clock();
    char region[100];
    memory_init(region, 100);
    
    printf("__________________________________________________________\n\n\t\t\t\t\t\tTest 4\n__________________________________________________________\n\n");
    printf("\t\t\t\t!!!!!ALL STEPS BELOW!!!!!!\n\n\nStep1 : Allocation of 2 blocks (Last one does not allocate)\n");
    printf("\n");
    
    // Alokovanie jednotlivých blokov
    
    char *test = memory_alloc(32);
    char *test2 = memory_alloc(32);
    char *test3 = memory_alloc(32);
    formattedOutput();
    
    // Uvoľňovanie nealokovaného bloku
    printf("Step2: Freeing of not allocated block 3\n");
    memory_free(test3);
    formattedOutput();
    
    printf("Statistics:\n");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
    
    // Vypočítanie percentuálnej úspešnosti testu
    printf("Success rate: %d", 2/2*100);
}

void test51()
{
    
    // Aktivácia hodín
    clock_t t;
    t = clock();
    srand(time(NULL));
    
    // Inicializovanie pamäte
    
    char region[200];
    memory_init(region, 200);
    
    // Vytvorenie poľa 5 náhodných premenných
    char *test = memory_alloc(8);
    char *test2 = memory_alloc(8);
    char *test3 = memory_alloc(8);
    char *test4 = memory_alloc(8);
    
    // Formátovaný výstup
    
    formattedOutput();
    
    printf("Statistics:\n");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
}
void test52()
{
    
    // Aktivácia hodín
    clock_t t;
    t = clock();
    srand(time(NULL));
    
    // Inicializovanie pamäte
    
    char region[200];
    memory_init(region, 200);
    
    // Vytvorenie poľa 5 náhodných premenných
    char *test = memory_alloc(15);
    
    // Formátovaný výstup
    
    formattedOutput();
    
    printf("Statistics:\n");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
}

void test53(){
    // Aktivácia hodín
    clock_t t;
    t = clock();
    srand(time(NULL));
    
    // Inicializovanie pamäte
    
    char region[200];
    memory_init(region, 200);
    
    // Vytvorenie poľa 5 náhodných premenných
    char *test = memory_alloc(24);
    
    // Formátovaný výstup
    
    formattedOutput();
    
    printf("Statistics:\n");
    
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
}

void test6()
{
    
    // Aktivácia hodín
    clock_t t;
    t = clock();
    srand(time(NULL));
    
    // Nastavenie potrebných premenných
    int answer = 0;
    int lower = 8, upper = 24;
    int arrNum[5];
    char* tests[5];
    
    // Inicializovanie pamäte
    
    char region[200];
    memory_init(region, 200);
    
    // Vytvorenie poľa 5 náhodných premenných
    
    for (int i = 0; i < 5; i++)
    {
          int randNumber = (rand() % (upper - lower + 1)) + lower;
          arrNum[i] = randNumber;
    }
    
    // Cyklické napĺňanie pamäte
    while(1) {
        
        for (int i = 0; i < 5; i++){
            char *test = memory_alloc(arrNum[i]);
            tests[i] = test;
        }
        
        for (int i = 0; i < 5; i++){
            if (tests[i]){
                answer = 1;
            }
        }
        if (answer == 1) {
            answer = 0;
            continue;
        }
        break;
    }
    
    // Formátovaný výstup
    printf("Random number1: %d\nRandom number2: %d\nRandom number3: %d\nRandom number4: %d\nRandom number5: %d\n", arrNum[0],arrNum[1],arrNum[2],arrNum[3],arrNum[4]);
    
    formattedOutput();
    
    printf("Statistics:\n");
    
    printf("Success rate: %d\n", percentage(200,arrNum));
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
}

void test7()
{
    
    // Aktivácia hodín
    clock_t t;
    t = clock();
    srand(time(NULL));
    
    // Nastavenie potrebných premenných
    int answer = 0;
    int lower = 500, upper = 5000;
    int arrNum[5];
    char* tests[5];
    
    // Inicializovanie pamäte
    
    char region[10000];
    memory_init(region, 10000);
    
    // Vytvorenie poľa 5 náhodných premenných
    
    for (int i = 0; i < 5; i++)
    {
          int randNumber = (rand() % (upper - lower + 1)) + lower;
          arrNum[i] = randNumber;
    }
    
    // Cyklické napĺňanie pamäte
    while(1) {
        for (int j = 0; j < 5; j++){
            char *test = memory_alloc(arrNum[j]);
            tests[j] = test;
        }
        for (int k = 0; k < 5; k++){
            if (tests[k]){
                answer = 1;
            }
        }
        if (answer == 1) {
            answer = 0;
            continue;
        }
        break;
        
    }
    
    printf("Random number1: %d\nRandom number2: %d\nRandom number3: %d\nRandom number4: %d\nRandom number5: %d\n", arrNum[0],arrNum[1],arrNum[2],arrNum[3],arrNum[4]);
    // Formátovaný výstup
    formattedOutput();
    
    printf("Statistics:\n");
    
    printf("Success rate: %d\n",percentage(10000,arrNum));
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
}

void test8()
{
    // Aktivácia hodín
    clock_t t;
    t = clock();
    srand(time(NULL));
    
    // Nastavenie potrebných premenných
    int answer = 0;
    int lower = 8, upper = 50000;
    int arrNum[5];
    char* tests[5];
    
    // Inicializovanie pamäte
    
    char region[100000];
    memory_init(region, 100000);
    
    // Vytvorenie poľa 5 náhodných premenných
    
    
    for (int i = 0; i < 5; i++)
    {
          int randNumber = (rand() % (upper - lower + 1)) + lower;
          arrNum[i] = randNumber;

    }
    
    // Cyklické napĺňanie pamäte
    while(1) {
        
        for (int i = 0; i < 5; i++){
            char *test = memory_alloc(arrNum[i]);
            tests[i] = test;
        }
        
        for (int i = 0; i < 5; i++){
            if (tests[i]){
                answer = 1;
            }
        }
        if (answer == 1) {
            answer = 0;
            continue;
        }
        break;
    }
    
    // Formátovaný výstup
    printf("Random number1: %d\nRandom number2: %d\nRandom number3: %d\nRandom number4: %d\nRandom number5: %d\n", arrNum[0],arrNum[1],arrNum[2],arrNum[3],arrNum[4]);
    
    formattedOutput();
    
    printf("Statistics:\n");
    
    printf("Success rate: %d\n", percentage(100000, arrNum));
    // Vypočítanie času na vykonanie testu
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took seconds to execute: %f\n", time_taken);
}


// Main programu - Tu sa používateľovi zobrazia všetky dostupné testy aj s ich stručnými popismi. Používateľ si z nich môže vybrať a vypíše sa potrebný výstup pre skontrolovanie.

int main()
{
    int chosentest;
    
    // Výpis dostupných testov
    printf("Here are all available tests\n");
    printf("__________________\n");
    printf("My Tests:\n");
    printf("1 for Test1 - Merging two blocks\n");
    printf("2 for Test2 - Freeing whole memory with merging\n");
    printf("3 for Test3 - Trying to alocate block that does not fit in memory\n");
    printf("4 for Test4 - Test 3 + trying to free not alocated block\n");
    
    // Scenáre z doplnkového zadania
    printf("Tests from assignment:\n");
    printf("51 for Scenar1.1 \n");
    printf("52 for Scenar1.2 \n");
    printf("53 for Scenar1.3 \n");
    printf("6 for Scenar2 \n");
    printf("7 for Scenar3 \n");
    printf("8 for Scenar4 \n");
    printf("__________________\n");
    printf("Please choose test:\n");
    
    // Spracovanie vstupu od používateľa
    scanf("%d", &chosentest);
    
    // Spustenie vybraného testu
    if (chosentest == 1)
        test1();
    if (chosentest == 2)
        test2();
    if (chosentest == 3)
        test3();
    if (chosentest == 4)
        test4();
    if (chosentest == 51)
        test51();
    if (chosentest == 52)
        test52();
    if (chosentest == 53)
        test53();
    if (chosentest == 6)
        test6();
    if (chosentest == 7)
        test7();
    if (chosentest == 8)
        test8();
    return 0;
    
}
