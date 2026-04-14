/* Kyle Compton (mkcompt)
 * ECE 3220
 * Assignment 2
 * 04/14/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char data_blocks[5][3][4][17];
char parity[4][5][3][17];
int failed_disk = -1;

void createRaid5(char *input, int num_disks, int num_stripes){
    char blocks[48][17];
    for (int i = 0; i < 48; i++) {
        strncpy(blocks[i], input + (i * 16), 16);
        blocks[i][16] = '\0';
    }

    int block_index = 0;
    for (int i = 0; i <num_stripes; i++){
        for (int j = 0; j < num_disks; j++){
            for(int z = 0; z < 4; z++){
                if (j == i){
                    sprintf(data_blocks[j][i][z], "parity(%d,%d,%d)", z, j, i);
                }
                else{
                    strncpy(data_blocks[j][i][z], blocks[block_index], 16);
                    data_blocks[j][i][z][16] = '\0';
                    block_index++;
                }
            }
        }
    }
}

// make this more human
void calculateParity(char* input){
    for (int stripe = 0; stripe < 3; stripe++) {
        for (int row = 0; row < 4; row++) {
            for (int byte = 0; byte < 16; byte++) {
                char xor_result = 0;
                for (int disk = 0; disk < 5; disk++) {
                    if (disk == stripe){
                        continue;
                    }
                    xor_result = xor_result ^ data_blocks[disk][stripe][row][byte];
                }
                parity[row][stripe][stripe][byte] = xor_result;
            }
            parity[row][stripe][stripe][16] = '\0';
        }
    }
}

void printRaid5Disks(void){
    printf("*****************************************************************************************************************************\n");
    printf("%-25s %-25s %-25s %-25s %-25s\n", "Disk[0]", "Disk[1]", "Disk[2]", "Disk[3]", "Disk[4]");
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

void simulateFailure(int disk_num){
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 4; j++){
            sprintf(data_blocks[disk_num][i][j], "-failed-");
        }
    }
    for (int i = 0; i < 4; i++){
        sprintf(parity[i][disk_num][disk_num], "failed" );
    }
    
}

int main (){

    char input[800];
    char character;
    int index = 0;

    while (index < 768 && scanf("%c", &character) == 1){
        if (character == '\n'){
            character = ' ';
        }
        input[index++] = character;

    }

    createRaid5(input, 5, 3);
    printf("Creating RAID 5 reliable storage system\n");
    printRaid5Disks();
    calculateParity(input);

    for (int i = 0; i < 5; i++){
        for(int j = 0; j< 3; j++){
            for(int z = 0; z<4; z++){
                if(j == i){
                    printf("Parity(%d, %d, %d): ", z, i ,j);
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


    freopen("/dev/tty", "r", stdin);
    printf("Enter Disk # to simulating failure: ");
    scanf("%d", &failed_disk);
    printf("Disk %d failed.\n", failed_disk);
    simulateFailure(failed_disk);
    
    printRaid5Disks();


    return 0;
}