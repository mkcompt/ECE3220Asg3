/* Kyle Compton (mkcompt)
 * ECE 3220
 * Assignment 2
 * 04/14/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables
char data_blocks[5][3][4][17]; // used to store the input data into a 4D array. 5 disks, 3 stripes, 4 rows, 17 characters (16 + 1 null terminator)
char parity[4][5][3][17]; // used to store the parity data into a 4D array.
int failed_disk = -1; // used for when user specifies what disk fails

// Function that creates the raided 5 disks
void createRaid5(char *input, int num_disks, int num_stripes){
    // We first take the big string of data from the user and store it into 48 chunks of 16 + 1 characters
    char blocks[48][17];
    for (int i = 0; i < 48; i++) {
        strncpy(blocks[i], input + (i * 16), 16);
        blocks[i][16] = '\0';
    }

    // This loops through the number of stripes, disks, and rows to store the correct data into the data_blocks array
    int block_index = 0;
    for (int i = 0; i <num_stripes; i++){
        for (int j = 0; j < num_disks; j++){
            for(int z = 0; z < 4; z++){
                // if num_stripes = num_disks then the value when printed is just parity(x,y,z)
                if (j == i){
                    sprintf(data_blocks[j][i][z], "parity(%d,%d,%d)", z, j, i);
                }
                // else we iterate through the 48 chunks and store each chunk inside the correct datablock index
                else{
                    strncpy(data_blocks[j][i][z], blocks[block_index], 16);
                    data_blocks[j][i][z][16] = '\0';
                    block_index++;
                }
            }
        }
    }
}

// Function that calculates the parity values
void calculateParity(char* input){
    (void)input; // Dont need to pass in input since we already have the data in my global data_blocks variable
    for(int i = 0; i < 3; i++){ // loop stripes
        for (int j = 0; j<4; j++){ // loop rows
            for (int z = 0; z <16; z++){ // loop bytes
                char xor_result = 0;
                for (int d = 0; d < 5; d++){ // loop disks
                    // if the disk number is equal to stripe number then we dont include that in xor calculation since its parity were trying to calculate
                    if (d == i){
                        continue;
                    }
                    xor_result = xor_result ^ data_blocks[d][i][j][z]; // xor each character to the same character in the other disks
                }
                parity[j][i][i][z] = xor_result; // then store the result from the xor into our parity global array
            }
            parity[j][i][i][16] = '\0'; // end the parity for that row with a null terminator
        }
    }
}

// function to restore the data after disk failure
void restoreData(){
    int disk_to_restore = failed_disk;
    // similar to calculate parity, just looping through stripes, rows, bytes, and disks
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j<4; j++){
            for (int b = 0; b < 16; b++){
                char xor_result = 0; // this stores the value of the xor
                for (int d = 0; d < 5; d++){
                    // if the disk num is the same disk thats failed we skip that one since its what were trying to restore
                    if (d == disk_to_restore){
                        continue;
                    }
                    // if disk num = stripe num then we xor with the value in parity
                    if (d == i){
                        xor_result = xor_result ^ parity[j][d][i][b];
                    }
                    else{
                        xor_result = xor_result ^ data_blocks[d][i][j][b];
                    }
                }
                // here we store the calculated results into the correct arrays
                if (disk_to_restore == i){
                    parity[j][disk_to_restore][i][b] = xor_result;
                }
                else{
                    data_blocks[disk_to_restore][i][j][b] = xor_result;
                }
            }
            // here we add the null terminators to the correct arrays
            if (disk_to_restore == i){
                parity[j][disk_to_restore][i][16] = '\0';
                sprintf(data_blocks[disk_to_restore][i][j], "parity(%d,%d,%d)", j, disk_to_restore, i); // need to write back the parity (x,y,z) for printing in the table
            }
            else{
                data_blocks[disk_to_restore][i][j][16] = '\0';
            }
        }
    }
}

// function that prints the raided disks
void printRaid5Disks(void){
    printf("*****************************************************************************************************************************\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", "[Disk0]", "[Disk1]", "[Disk2]", "[Disk3]", "[Disk4]");
    printf("%-25s %-25s %-25s %-25s %-25s\n", "strip (0, 0)", "strip (1, 0)", "strip (2, 0)", "strip (3, 0)", "strip (4, 0)");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][0][0], data_blocks[1][0][0], data_blocks[2][0][0], data_blocks[3][0][0], data_blocks[4][0][0]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][0][1], data_blocks[1][0][1], data_blocks[2][0][1], data_blocks[3][0][1], data_blocks[4][0][1]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][0][2], data_blocks[1][0][2], data_blocks[2][0][2], data_blocks[3][0][2], data_blocks[4][0][2]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][0][3], data_blocks[1][0][3], data_blocks[2][0][3], data_blocks[3][0][3], data_blocks[4][0][3]);
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", "strip (0, 1)", "strip (1, 1)", "strip (2, 1)", "strip (3, 1)", "strip (4, 1)");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][1][0], data_blocks[1][1][0], data_blocks[2][1][0], data_blocks[3][1][0], data_blocks[4][1][0]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][1][1], data_blocks[1][1][1], data_blocks[2][1][1], data_blocks[3][1][1], data_blocks[4][1][1]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][1][2], data_blocks[1][1][2], data_blocks[2][1][2], data_blocks[3][1][2], data_blocks[4][1][2]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][1][3], data_blocks[1][1][3], data_blocks[2][1][3], data_blocks[3][1][3], data_blocks[4][1][3]);
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", "strip (0, 2)", "strip (1, 2)", "strip (2, 2)", "strip (3, 2)", "strip (4, 2)");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][2][0], data_blocks[1][2][0], data_blocks[2][2][0], data_blocks[3][2][0], data_blocks[4][2][0]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][2][1], data_blocks[1][2][1], data_blocks[2][2][1], data_blocks[3][2][1], data_blocks[4][2][1]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][2][2], data_blocks[1][2][2], data_blocks[2][2][2], data_blocks[3][2][2], data_blocks[4][2][2]);
    printf("%-25s %-25s %-25s %-25s %-25s\n", data_blocks[0][2][3], data_blocks[1][2][3], data_blocks[2][2][3], data_blocks[3][2][3], data_blocks[4][2][3]);
    printf("*****************************************************************************************************************************\n");
}

// function that simulates failure on a specific disk
void simulateFailure(int disk_num){
    // erases the data and replaces it with -failed- for the failed disk
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 4; j++){
            sprintf(data_blocks[disk_num][i][j], "-failed-");
        }
    }
    // erases the parity for that disk aswell
    for (int i = 0; i < 4; i++){
        sprintf(parity[i][disk_num][disk_num], "failed" );
    }
    
}

int main (){
    // creates a input buffer with some extra space for the input redirection
    char input[800] = {0}; // esnures that if input small, the remaining values are not garbage when printed
    char character;
    int index = 0;

    // scans the input redirection and stores the entire input one big string
    while (index < 768 && scanf("%c", &character) == 1){
        if (character == '\n'){
            character = ' ';
        }
        input[index++] = character;

    }
    input[index] = '\0'; // null terminates the input string

    createRaid5(input, 5, 3);
    printf("Creating RAID 5 reliable storage system\n");
    printRaid5Disks();
    calculateParity(input);

    // loops through the parity array and prints the binary values 
    for (int i = 0; i < 5; i++){
        for(int j = 0; j< 3; j++){
            for(int z = 0; z<4; z++){
                if(j == i){
                    printf("parity(%d, %d, %d): ", z, i ,j);
                    // Prints the values as binary
                    for (int byte = 0; byte < 16; byte++) {
                        for (int bit = 7; bit >= 0; bit--) {
                            printf("%d", (parity[z][i][i][byte] >> bit) & 1);
                        }
                    }  
                    printf("\n");         
                }
                else{
                    continue;
                }
            }
        }
    }

    // clears the stdin so can get the user input for failed disk
    freopen("/dev/tty", "r", stdin);
    printf("\n");
    printf("Enter Disk # to simulating failure: ");
    scanf("%d", &failed_disk);
    printf("Disk %d failed.\n", failed_disk);
    printf("\n");

    simulateFailure(failed_disk); // calls the simulateFailure function with the failed disk value the user specified
    printRaid5Disks();

    // prints the parity values again after disk failure
    for (int i = 0; i < 3; i++) {
        for (int z = 0; z < 4; z++) {
            printf("parity (%d, %d, %d): ", z, i, i);
            // if the stripe number is same as the disk that failed then we print the value inside of the parity array instead of the binary value
            if (i == failed_disk) {
                printf("%s\n", parity[z][i][i]);
            }
            else{
                for (int byte = 0; byte < 16; byte++) {
                    for (int bit = 7; bit >= 0; bit--) {
                        printf("%d", (parity[z][i][i][byte] >> bit) & 1);
                    }
                }
                printf("\n");
            }
            
        }
    }

    printf("\n\n");
    // Restoredata function call
    printf("Rebuilding the data... \n\n");
    restoreData();
    printRaid5Disks();

    // prints the parity binary values once again after the data has been restored
    for (int i = 0; i < 5; i++){
        for(int j = 0; j< 3; j++){
            for(int z = 0; z<4; z++){
                if(j == i){
                    printf("parity(%d, %d, %d): ", z, i ,j);
                    for (int byte = 0; byte < 16; byte++) {
                        for (int bit = 7; bit >= 0; bit--) {
                            printf("%d", (parity[z][i][i][byte] >> bit) & 1);
                        }
                    }  
                    printf("\n");         
                }
                else{
                    continue;
                }
            }
        }
    }

    return 0;
}