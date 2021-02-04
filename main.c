#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#define RETURN_SUCCESS 0
#define RETURN_BAD_ARG 1

typedef unsigned char bool;

void usage(){
    printf( "app [FILE] -[biIns] [-m|--match STRING_PATTERN] [STRING...]\n"
            "   -b : Append to beginning of lines\n"
            "   -i : Do not append to empty lines\n"
            "   -I : Only append to empty lines\n"
            "   -n : Append on new lines\n"
            "   -m : Append only to lines matching the given string pattern\n"
            "   -s : Append one string per line from sequence of strings\n");
}

int main(int argc, char** args) {
    if(argc < 2){
		printf("Not enough arguments\n");
        usage();
        return RETURN_BAD_ARG;
    }
    
    args++;
    argc--;

    if(!strcmp(*args, "--help")){
        usage();
        return RETURN_SUCCESS;
    }

    // If first arg not a file, read stdin
    FILE* ist = fopen(*args, "r");
    if(ist){
        args++;
        argc--;
		if(!argc){
			printf("Not enough arguments\n");
			usage();
			return RETURN_BAD_ARG;
		}
    }else{
        ist = stdin;
    }

    // Switches
    bool    nline       = 0,
            ignempty    = 0,
            onlyempty   = 0,
            appbeg      = 0,
            sequence    = 0;
    
    char*   tmatch      = 0;
    
    //TODO improve switch parsing
    //TODO add sequence switch
    while((*args)[0] == '-' && (*args)[1] != 'm' && (*args)[1] != '-'){
        for(int i = 1; (*args)[i]; i++){
            switch((*args)[i]){
            case 'b':
                appbeg = 1;
                break;
            case 'h':
                usage();
                return RETURN_SUCCESS;
            case 'i':
                ignempty = 1;
                break;
            case 'I':
                onlyempty = 1;
                break;
            case 'n':
                nline = 1;
                break;
            case 's':
                sequence = 1;
                break;
            default:
                printf("Invalid argument: -%c\n", (*args)[i]);
                usage();
                return RETURN_BAD_ARG;
            }
        }
        
        args++;
        argc--;
        
        if(!argc){
            printf("Not enough arguments\n");
            usage();
            return RETURN_BAD_ARG;
        }
    }
    
    // Take text match string
    if(!strcmp(*args, "-m") || !strcmp(*args, "--match")){
        tmatch = args[1];
        args += 2;
        argc -= 2;
    }
    
    //check for append text args given
    if(argc < 1){
        printf("No appendable text given\n");
        usage();
        return RETURN_BAD_ARG;
    }
    
    // Output
    char inp[BUFSIZ];
    char spacers[3] = {'\0', ' ', '\n'};
    
    while(fgets(inp, sizeof(inp), ist)){
        // Strip newline.
        int nli = 0; // newline index
        while(inp[nli] != '\n'){ nli++; }
        inp[nli] = '\0';
        
        //TODO add regex maybe
        // Check for text match
        bool match;
        if(tmatch){
            match = 1;
            int b = 0;
            for(int i = 0; i < strlen(tmatch); i++){
                if(tmatch[i] == '*'){
                    i++;
                    if(tmatch[i] == '\0') break;
                    while(b < nli && inp[b] != tmatch[i]){ b++; }
                    if(b == nli){
                        match = 0;
                        break;
                    }
                }else if(tmatch[i] != inp[b]){
                    match = 0;
                    break;
                }
                b++;
            }
        }
        
        //if ignempty and is not empty
        bool unempt = !ignempty || nli;
        
        //if only empty and is empty
        bool emptt = !onlyempty || !nli;
        
        match = (!tmatch || match) && unempt && emptt && (sequence ? argc : 1);
        
        // Print
        if(!appbeg) fputs(inp, stdout);

        for(int i = 0; i < (sequence ? 1 : argc); i++){
            if(match){
                if(!appbeg) fputc(spacers[nline << 1], stdout);
                if(!sequence || argc) fputs(sequence ? *args : args[i], stdout);
                if(appbeg) fputc(spacers[nline << 1], stdout);

                if(sequence && argc){
                    args++;
                    argc--;
                }
            }
        }

        if(appbeg) fputs(inp, stdout);
        fputc('\n', stdout);
    }

    fclose(ist);
    
    return RETURN_SUCCESS;
}
