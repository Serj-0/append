#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
typedef unsigned char bool;

char str[BUFSIZ];
char out[BUFSIZ];

static inline void print_append(char* src, char** app, int n, bool front);

void usage(){
    printf("%s", "app [-fnNs] [FILE] STRING...\n");
}


//FIXME do not treat arg as file if file cannot be opened
int main(int argc, char** args){
    if(argc < 2 || !strcmp(args[1], "--help")){
        usage();
        return 0;
    }
    
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
            case 'h':
                usage();
                return 0;
            }
        }
    }
    
    //if first was a flag switch, move arg index forward one
    int aas = (front || nline || Nline || seqnc);
    
    /* FILE */
    
    FILE* ist = fopen(args[1 + aas], "r");
    
    {
        struct stat st;
        stat(args[1 + aas], &st);
        
        if(!ist || (st.st_mode & __S_IFDIR)){
            ist = stdin;
        }else{
            aas++;
        }
    }
    
//    if(!ist){
//        ist = stdin;
//    }else{
//        aas++;
//    }
    
    //if first or second are a file, move arg index forward one
//    aas += (ist != stdin);
    
    /* OPERATION */
    int sqi = 0;
    
    while(fgets(str, BUFSIZ, ist)){
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

/*************************************************************************************************************/

//FIXME issues with \0
static inline void print_append(char* src, char** app, int n, bool front){
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
