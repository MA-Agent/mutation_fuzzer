#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>

//function that randomizes a hex array 
/*params
percentage = the % of bytes in the hexarray to randomize
hexarray = the hexarray that we want to be randomized
arraysize = the size of the array
*/
void randomizeHexArray(float percentage,char hexArray[], int arraySize){   
    int i;
    //calculating the amount of bytes we need to randomize in the array
    float nbBytesToRandomize =  (float)arraySize * percentage;
    roundf(nbBytesToRandomize);
    int randomArrayIndex = 0;
    //for loop for every byte that we need to randomize
    for(i=0;i<nbBytesToRandomize;i++){

        //calculate that index of the array that needs to be randomized by excluding the 2 first bytes
        randomArrayIndex = (rand() % (arraySize - 2)) + 2;
        
        //excluding version errors
        if(randomArrayIndex == 2 || randomArrayIndex == 3){
            hexArray[3] = 0x00;
            hexArray[2] = 0x64;
            
            
            
        }
        //avoiding numcolor errors
        else if (randomArrayIndex == 16 || 
        randomArrayIndex == 17 ||
        randomArrayIndex == 18 ||
        randomArrayIndex == 19){

            hexArray[17]= 0x00;
            hexArray[18]= 0x00;
            hexArray[19]= 0x00;
            char hexv    = rand()%257 & 0xff;
            if(hexv > '\xff'){
                hexArray[16] = 0xff;
                hexArray[17] = 0x01;
            }else{
                hexArray[16] = hexv;

            }
            
        }
        //avoiding width errors
        else if(randomArrayIndex == 8||
        randomArrayIndex == 9 ||
        randomArrayIndex == 10|| 
        randomArrayIndex == 11){
            hexArray[9] = 0x00;
            if(randomArrayIndex != 9){
                hexArray[randomArrayIndex] = rand()%256 & 0xff ;
                             
            }

        }
        //avoiding height errors
        else if(randomArrayIndex == 12 ||
        randomArrayIndex == 13 ||
        randomArrayIndex == 14 ||
        randomArrayIndex == 15){
            hexArray[13] = 0x00;
            if(randomArrayIndex != 13){
                
                if(randomArrayIndex !=12){
                    hexArray[randomArrayIndex] = rand()%256 & 0xff;
                    
                }else{
                    hexArray[12] = rand()%2 & 0xff;                   
                }
            }
                             
        }
        else{
            hexArray[randomArrayIndex] = rand()%256 & 0xff;

        }
    }
}

//function that reads a hexfile
/*params
path = the path of the hexfile that we want to read
hexarray = the hexarray that we want to be initiliazed with the value of the hexfile
*/
void readHexFile(char* path,char hexArray[]) {
    //file declaration
    FILE *inputseed;
    //opening the file binary
    if ((inputseed = fopen (path, "rb")) != NULL) {
        struct stat st;
        stat(path, &st);                

        int i;
        int ch;
        //reading the hex values of the file
        for (i = 0; i < st.st_size; i++) {
            ch = fgetc(inputseed); 
            //initializing the array
            hexArray[i]= ch;            
            
        }
        //closing the file
        fclose(inputseed);
        
    }
    else {
        //return NULL;
    }
}
//function that writes in a hexfile
/*params
path = the path of the file
hexarrayToWrite = the hex array that we want to write in the file
arraySize = the size of the hex array that we want to write
*/
void writeHexFile(char* path,char hexArrayToWrite[],int arraySize){
    //initializing the outputfile
    FILE *outputfile;
    int i;
    char* test = "abcd";
    
    //opening the binary of the file
    outputfile = fopen(path,"wb");
    //writing the binary inside the file
    fwrite(hexArrayToWrite, 1, arraySize, outputfile);
    //closing the outputfile
    fclose(outputfile);
    
}
//function that deletes a hexfile
/*params7
path =  the path of the hexfile that we want to delete
*/
void deleteHexFile(char* path){
      int status;

    status = remove(path);
    if (status == 0){
        //printf("%s file deleted successfully.\n", path);

    }
    else
    {
        printf("Unable to delete the file\n");
        perror("Following error occurred");
    }
}

int main(int argc, char* argv[])
{
    //giving a time seed to the srand() function
    srand(time(NULL));
    char buf[BUFSIZ]={ '\0' },c;
    char *inputfs = argv[1];
    int i;
    void readHexFile(char* path,char hexArray[]);
    void randomizeHexArray(float percentage,char hexArray[], int arraySize);
    void writeHexFile(char* path,char hexArrayToWrite[],int arraySize);
    /*Spawn a child to run the converter program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        FILE *pf;
        
        char data[BUFSIZ];
        
        unsigned char hexArray[294];
        
        //for loop using the argument given to the program
        for(i=0;i<atoi(argv[2]);i++){
            if(i==0){
                //caling the readhexFile function to initialize our hexArray
                readHexFile(inputfs,hexArray);
            }
            //char that will be the name of our randomized input files
            char randomInputFile[4];
            
            sprintf(randomInputFile, "%d", i+1);
            strcat(randomInputFile,"randomizedInput.img");

            //calling the randomizeHexArray function to randomize our hexarray       
            randomizeHexArray(atof(argv[3]),hexArray,(int)( sizeof(hexArray) / sizeof(hexArray[0]) ));
            //calling the writeHexFile to write a new hexFile with our hexArray values
            writeHexFile(randomInputFile,hexArray,(int)( sizeof(hexArray) / sizeof(hexArray[0]) ));

            char *argv[]={"converter",randomInputFile,"testoutput.img",NULL};
            char result[4];
            sprintf(result, "%d", i);
            
        
            char converter[] = "./converter ";
            char outputf[]= " testoutput.img";
            char dest[500];
            strcat(dest,converter);
            strcat(dest,randomInputFile);
            strcat(dest,outputf);
            //opening the converter program with the needed arguments
            pf = popen(dest,"r");
            if(!pf){
                fprintf(stderr, "Could not open pipe for output.\n");
            }


            fgets(data, 512 , pf);
            //if statement that catches the crash of the converter program
            if (pclose(pf) != 0 ){
                fprintf(stderr,"input %d: Converter program has crashed \n",i);
                readHexFile(inputfs,hexArray);

            }else{
                
                fprintf(stderr,"==========================================================\n");
                fprintf(stderr,"inputf %s: No Crash \n",randomInputFile,i);
                readHexFile(inputfs,hexArray);
                //if the converter program didn't crash we delete the hexfile
                deleteHexFile(randomInputFile);     
            }
            
            strcpy(dest,"");            
        }
        
    }
    else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */        
        puts("Press any key to continue ... ");
        getchar();
    }
    return 0;
}