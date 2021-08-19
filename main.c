#include <stdio.h>
//#include <sys/types.h>
#include <string.h>
//#include <unistd.h>
typedef unsigned char bool;

char str[BUFSIZ];
char out[BUFSIZ];

void print_append(char* src, char** app, int n, bool front);

int main(int argc, char** args){
    bool    front = 0,      // append to front of input
            nline = 0,      // append only to empty lines (newlines)
            Nline = 0,      // append only to non empty lines
            seqnc = 0;      // append string args in a sequence
    
    /* FLAGS */
    if(args[1][0] == '-'){
        for(int j = 1; j < strlen(args[1]); j++){
            switch(args[1][j]){
            case 'f':
                front = 1;
                break;
            case 'n':
                nline = 1;
                break;
            case 'N':
                Nline = 1;
                break;
            case 's':
                seqnc = 1;
                break;
            }
        }
    }
    
    //if first arg did not switch any flags, treat as append string
    int aas = (front | nline | Nline | seqnc) != 0;
    
    /* OPERATION */
    int sqi = 0;
    
    while(fgets(str, BUFSIZ, stdin)){
        bool lt = !(nline || Nline) || (nline && str[0] == '\n') || (Nline && str[0] != '\n');
        bool st = !seqnc || (sqi + 1 + aas < argc);
        
        bool doit = lt && st;
        
        if(doit){
                                             //v~~~~~~~~~~~~~v add sequence index to arg index if seqnc flag is true
                                                                               //v~~~~~~~~~~~~~~v if seqnc, change string arg count to 1
            print_append(str, args + 1 + aas + (seqnc * sqi++), (argc - 1 - aas) * !seqnc + seqnc, front);
        }else{
            printf("%s", str);
        }
    }
    
    return 0;
}

void print_append(char* src, char** app, int n, bool front){
    if(!front) strncpy(out + strlen(out), src, strlen(src));
    for(int i = 0; i < n; i++){
                                //v~~~~~~~~~~~~~~v if is first string arg, and front flag is false, move back one more character
        strncpy(out + strlen(out) - (!i && !front), app[i], strlen(app[i]));
    }
    if(front){
        strncpy(out + strlen(out), src, strlen(src));
        out[strlen(out) - 1] = '\0';
    }
    puts(out);
    memset(out, 0, BUFSIZ);
}
