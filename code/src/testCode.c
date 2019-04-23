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


void randomizeHexArray(float percentage,char hexArray[], int arraySize){   
    int i;
    float nbBytesToRandomize =  (float)arraySize * percentage;
    roundf(nbBytesToRandomize);
    int randomArrayIndex = 0;
    for(i=0;i<nbBytesToRandomize;i++){
        randomArrayIndex = (rand() % (arraySize - 2)) + 2;
        //printf("randarr: %d",randomArrayIndex);
        if(randomArrayIndex == 2 || randomArrayIndex == 3){
            hexArray[3] = 0x00;
            hexArray[2] = 0x64;
            //hexArray[2] = rand()%101 & 0xff;
            
            
            
        //printf("2-3\n");
        }else if (randomArrayIndex == 16 || 
        randomArrayIndex == 17 ||
        randomArrayIndex == 18 ||
        randomArrayIndex == 19){

            //hexArray[16]= 0x00;
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
            //printf("hex16: %x\n",hexArray[16]);
        //printf("16-19\n");
        }else if(randomArrayIndex == 8||
        randomArrayIndex == 9 ||
        randomArrayIndex == 10|| 
        randomArrayIndex == 11){
            //converter behaves strangely when the 9th byte is changed
            hexArray[9] = 0x00;
            if(randomArrayIndex != 9){
                hexArray[randomArrayIndex] = rand()%256 & 0xff ;
                /*hexArray[8] = rand()%256 & 0xff;           
                hexArray[10] = rand()%256 & 0xff;
                hexArray[11] = rand()%256 & 0xff;  */              
            }
        //printf("8-11\n");

        }else if(randomArrayIndex == 12 ||
        randomArrayIndex == 13 ||
        randomArrayIndex == 14 ||
        randomArrayIndex == 15){
            hexArray[13] = 0x00;
            if(randomArrayIndex != 13){
                /*hexArray[12] = rand()%2 & 0xff;
                hexArray[14] = rand()%256 & 0xff;
                hexArray[15] = rand()%256 & 0xff;*/
                if(randomArrayIndex !=12){
                    hexArray[randomArrayIndex] = rand()%256 & 0xff;
                    /*hexArray[14] = rand()%256 & 0xff;
                    hexArray[15] = rand()%256 & 0xff;*/
                }else{
                    hexArray[12] = rand()%2 & 0xff;                   
                }
            }
        //printf("12-15\n");
                             
        }
        else{
             //printf("OTHERS \n");           
            hexArray[randomArrayIndex] = rand()%256 & 0xff;

        }
    }
    //return hexArray;
}

void readHexFile(char* path,char hexArray[]) {
    FILE *inputseed;
    
    if ((inputseed = fopen (path, "rb")) != NULL) {
        struct stat st;
        stat(path, &st);                

        int i;
        int ch;
        
        for (i = 0; i < st.st_size; i++) {
            ch = fgetc(inputseed); 
            hexArray[i]= ch;            
            
        }
        fclose(inputseed);
        
    }
    else {
        //return NULL;
    }
}

void writeHexFile(char* path,char hexArrayToWrite[],int arraySize){
    FILE *outputfile;
    int i;
    char* test = "abcd";
    
    outputfile = fopen(path,"wb");
    //fwrite(test, 4, 1, outputfile);
    fwrite(hexArrayToWrite, 1, arraySize, outputfile);
    /*for(i=0;i<arraySize;i++){
        fprintf(outputfile,"%x ",hexArrayToWrite[i]);
    }*/
    
    fclose(outputfile);
    
}
void deleteHexFile(char* path){
      int status;

    status = remove(path);
    //printf("path to delete: %s\n",path);
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
    srand(time(NULL));
    char buf[BUFSIZ]={ '\0' },c;
    char *inputfs = argv[1];
    int i;
    void readHexFile(char* path,char hexArray[]);
    void randomizeHexArray(float percentage,char hexArray[], int arraySize);
    void writeHexFile(char* path,char hexArrayToWrite[],int arraySize);
    /*Spawn a child to run the program.*/
    pid_t pid=fork();
    if (pid==0) { /* child process */
        FILE *pf;
        
        char data[BUFSIZ];
        
        unsigned char hexArray[294];
        

        for(i=0;i<atoi(argv[2]);i++){
            if(i==0){
                readHexFile(inputfs,hexArray);
            }

            //printf("close %d\n",i);
            char randomInputFile[4];
            
            sprintf(randomInputFile, "%d", i+1);
            strcat(randomInputFile,"randomizedInput.img");
        
            randomizeHexArray(atof(argv[3]),hexArray,(int)( sizeof(hexArray) / sizeof(hexArray[0]) ));
            
            writeHexFile(randomInputFile,hexArray,(int)( sizeof(hexArray) / sizeof(hexArray[0]) ));

            char *argv[]={"converter",randomInputFile,"testoutput.img",NULL};
            //setbuf(stdin, buf);
            char result[4];
            sprintf(result, "%d", i);
            
        
            char converter[] = "./converter ";
            char outputf[]= " testoutput.img";
            char dest[500];
            strcat(dest,converter);
            strcat(dest,randomInputFile);
            strcat(dest,outputf);
            //strcat(dest,"2>&1");
            
            pf = popen(dest,"r");
            if(!pf){
                fprintf(stderr, "Could not open pipe for output.\n");
            }
            // Grab data from process execution

            //setbuf(stdout, data);

            fgets(data, 512 , pf);
            // Print grabbed data to the screen.
            //printf( "-data %d: %s-\n",i,data);

            if (pclose(pf) != 0 ){
                fprintf(stderr,"input %d: Converter program has crashed \n",i);
                readHexFile(inputfs,hexArray);

            }else{
                
                fprintf(stderr,"==========================================================\n");
                fprintf(stderr,"inputf %s: No Crash \n",randomInputFile,i);
                //printf("%d randi: %s",i,randomInputFile);
                readHexFile(inputfs,hexArray);
                deleteHexFile(randomInputFile);     
                
                

            }
            //system(dest);
            
            strcpy(dest,"");

            

            // generates an error
        
            
        }
        
    }
    else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */        
        puts("Press any key to continue ... ");
        getchar();
    }
    return 0;
}