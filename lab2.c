/**
 * This program reads a text file and prints the top 5 most frequent letters (upper or lower case)
 * and the top 5 most frequent digits that are in the file. If the file has less than 5 unique letters
 * and/or digits the top list is adjusted accordingly. The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Your Name Here {Runhao Huang}
 * @date Sept. 10, 2022
 * @assignment Lab 2
 * @course CSC 250
 **/

#include <stdio.h>
#include <ctype.h>
 
#define MAX_LIST_SIZE 26
#define MAX_PRINT_LIST_SIZE 5

/* a useful struct, if you choose to use it */  
struct CharFreq {
    char ch;
    int count;
};

/* function declarations go here */
int processCharacters(char filename[], struct CharFreq alphaList[], int *alphaNumPtr, 
     struct CharFreq digitList[], int *digitNumPtr);
int search(char ch, struct CharFreq list[], int num);
int printFunction(struct CharFreq alphaList[], struct CharFreq digitList[], int alphaNum, int digitNum);
int printMaxFive(struct CharFreq list[], int num);

/* This is the main function of the program that includes initialization, validation, process, and print result.*/
int main(int argc, char* argv[]){
    /* Initialization */
    struct CharFreq alphaList[MAX_LIST_SIZE];
    struct CharFreq digitList[MAX_LIST_SIZE];
    int alphaNum = 0;
    int digitNum = 0;
    int fileOK = 1;

    /* Check validation */
    if(argc < 2){
        printf("Usage: %s filename \n", argv[0]);
        return 1;
    }

    /* Read and process file */
    fileOK = processCharacters(argv[1], alphaList, &alphaNum, digitList, &digitNum);
    if(!fileOK){
        printf("%s could not be opened, and AJ is nice \n", argv[1]);
        return 1;
    }
    
    /* Print the results */
    printFunction(alphaList, digitList, alphaNum, digitNum);
    return 0;
}

/* A print function that prints the header of two sections and call the method of printing the details of two sections. */  
int printFunction(struct CharFreq alphaList[], struct CharFreq digitList[], int alphaNum, int digitNum) {
    int num = 0;

    if (alphaNum == 0) {
        printf("No letters were found \n--------------------- \n");
    }
    else {
        num = (alphaNum > 5) ? 5 : alphaNum;
        printf("alpha top %d out of %d \n--------------------- \n", num, alphaNum);
        printMaxFive(alphaList, alphaNum);
    }

    if (digitNum == 0) {
        printf("No digits were found \n--------------------- \n");
    }
    else {
        num = (digitNum > 5) ? 5 : digitNum;
        printf("digit top %d out of %d \n--------------------- \n", num, digitNum);
        printMaxFive(digitList, digitNum);
    }
    return 1;
}

/* A funtion that prints the elements of a struct CharFreq that have top five count.*/ 
/* It might prints the top n elements if the total number of element in the array is less than five.*/ 
int printMaxFive(struct CharFreq list[], int num) {
    int i = 0;
    int j = 0;
    struct CharFreq currCharFreq;

    /* Insertion sort that sort the elements with count from large to small. */
    for (i = 1; i < num; i++) { 
        currCharFreq = list[i];
        j = i - 1; 
        while (j >= 0 && list[j].count < currCharFreq.count) { 
            list[j + 1] = list[j]; 
            j = j - 1; 
        } 
        list[j + 1] = currCharFreq; 
    } 

    /* Print out the first 5 elements. */
    for(i = 0; i < 5 && i < num; i++) {
        printf("[%c] [%d]\n", list[i].ch, list[i].count);
    }
    return 1;
}

/* A funtion that checks the validity of the filename and stores all the letters, numbers, and their counts into the static arrays of struct CharFreq.*/
int processCharacters(char filename[], struct CharFreq alphaList[], int *alphaNumPtr,
 struct CharFreq digitList[], int *digitNumPtr) {
    char ch;
    FILE *filePtr = fopen(filename, "rie");
    int loc = 0;
    int alphaNum = *alphaNumPtr;  /* local copy of the alphaNum value */
    int digitNum = *digitNumPtr;  /* local copy of the digitNum value */

    /* If the file doesn't exist*/
    if(filePtr == 0){
        return 0;
    }

    printf("file name is %s \n", filename);

    ch = fgetc(filePtr);
    /* If don't reach an end of the file.*/
    while(ch != EOF) {
        /* If current character is a letter.*/
        if(isalpha(ch)){
            ch = tolower(ch);
            loc = search(ch, alphaList, alphaNum);
            if(loc != -1){
                alphaList[loc].count++;
            }
            else{
                alphaList[alphaNum].ch = ch;
                alphaList[alphaNum].count = 1;
                alphaNum++;
            }
        }
        /* If current character is a digit.*/
        else if(isdigit(ch)){
            loc = search(ch, digitList, digitNum);
            if(loc != -1){
                digitList[loc].count++;
            }
            else{
                digitList[digitNum].ch = ch;
                digitList[digitNum].count = 1;
                digitNum++;
            }
        }
        ch = fgetc(filePtr);
    }

    fclose(filePtr);

    /* let's copy our local copy back to main's copy */
    *alphaNumPtr = alphaNum;
    *digitNumPtr = digitNum;

    return 1;
}

/* A funtion that loop through the array to find the location of the letter or number. It will return -1 if not found.*/
int search(char ch, struct CharFreq list[], int num){
    int loc = -1;
    int i;
    for(i = 0; i < num && loc == -1; i++)
        if(list[i].ch == ch)
            loc = i;
    return loc;
}
