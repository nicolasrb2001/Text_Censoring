/*
 ============================================================================

 Implement an algorithm in C which given a file containing a block of text as
 input, redacts all words from a given set of “redactable” words (also from a
 file), and outputs the result to a file called “result.txt”. For example,
 given the block of text:
    The quick brown fox jumps over the lazy dog

 and the redactable set of words:
    the, jumps, lazy

 the output text in “result.txt” should be
    *** quick brown fox ***** over *** **** dog

 Note that the number of stars in the redacted text is the same as the number
 of letters in the word that has been redacted, and that capitalization is
 ignored. You should not use any of the string libraries to answer this
 question. You should also test your program using the example files
 provided.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int size= 0;//stores the length of redact_strings ie the length of all the words + the null characters that separate them
char* set_redact_words( const char *redact_words_filename){
    FILE *inp;
    inp = fopen(redact_words_filename,"r");
    int nread;
    char buf[100];
    char *redact_strings;//stores each redact string
    if(inp == NULL){
        printf("redact file cannot be found");
        fclose(inp);
        return NULL;
    }else{
        redact_strings = (char*) malloc(sizeof(char));
        int ignore = 1;//initially set to 1 so that if a new string isn't being added to redact strings, any space or \0 \n characters are ignored
        do{
            nread = fread(buf, 1, 100, inp);
            for(int i = 0; i<nread; i++){
                if(buf[i]!= ' ' && buf[i]!= '\0' && buf[i]!= '\n'){
                    *(redact_strings+size) = buf[i];//appends character to the last index
                    redact_strings = (char*) realloc(redact_strings,size+1);//memory allocated is incremented by 1
                    size++;
                    ignore = 0;//the next \0 or \n or space encountered is not ignored; ie it will determine the end of the current string
                }else{
                    if(ignore==0){
                        *(redact_strings+size) = '\0';
                        redact_strings = (char*) realloc(redact_strings,size+1);
                        size++;
                        ignore = 1;
                    }
                }
            }
        }while(nread>0);
    }
    fclose(inp);
    return redact_strings;
}
//this method returns whether the input characters are the same letter regardless of capitalisation
int ascii(char c, char b){
    int cc = (int) c;
    int bb = (int) b;
    if(cc== bb+32 || cc == bb-32 || cc == bb){
        return 1;
    }
    return 0;
}

//this function compares the current word read from debate txt against each of the strings stored in redact_strings
void compare(char* words, int number,const int* indexes, int total_indexes, char write[], FILE *out,const char *redact_strings,const char buf[], int i){
    words = (char *) realloc(words, (number) * sizeof(char));//the memory allocated is decremented by 1 since previously it had a size of number + 1
    //iterates over the total number of indexes, ie the starting positions of each string in redact_strings
    for(int x = 0; x<total_indexes; x++){
        int end;
        int done =0;//if = 1 it means that a comparison has been successful and it exits the loop
        if(x == total_indexes-1){
            end = size-1;//since the last index is not stored it is made equal to the length of redact_strings -1, which represents the last \0 in redact_strings
        }else{
            end =*(indexes+x+1) -1;// end = starting index of the next string -1
        }
        int count = 0;//counter to iterate over the memory contents assigned to char* words

        //starts at index = indexes + x
        for(int y = *(indexes+x); y<end; y++){
            if(ascii(*(words+count), *(redact_strings+y)) !=1){
                //if the words are not equivalent and its the last indexed to be checked then the uncensored word is written
                if(x == total_indexes-1){
                    for(int u = 0; u<number;u++){
                        write[0] = *(words+u);
                        fwrite(write, 1, 1, out);
                    }
                    done = 1;
                    write[0] = buf[i];
                    fwrite(write, 1, 1, out);//writes the current buf[i] character ie ' ' or \0 etc

                }
                break;
            }else{
                count++;
                //if the sizes of the word and string in redact_string are equal and the index is the last to be checked then the word is censored
                if(y-*(indexes+x) == number-1 && y == end-1){
                    for(int u =0; u<number;u++){
                        write[0] = '*';
                        fwrite(write, 1, 1, out);
                    }
                    done = 1;
                    write[0] = buf[i];
                    fwrite(write, 1, 1, out);
                    break;
                }
            }
        }
        if(done == 1) {
            break;
        }
    }
}

//this function reads characters from debate.txt and writes the result in result.txt
void write_to_file(const char* redact_strings,const char *text_filename, int* indexes, int total_indexes){
    FILE *out, *inp;
    out = fopen("./result.txt","w");
    inp = fopen(text_filename,"r");
    int nread;
    char buf[100];
    char write[1];
    char* words;
    int number = 0;
    words = (char*) malloc(sizeof(char));// pointer which will store a string
    do{
        nread = fread(buf, 1, 100, inp);
        int added= 0;//determines whether there is a word that is currently being added to words or not, if = 0 then space \0 etc are simply written to result.txt
        for(int i = 0; i< nread; i++) {
            //if the current character isn't a character
            if ((int)buf[i]<65 || ((int)buf[i] >90 && (int)buf[i]<97 )|| (int)buf[i]>122) {
                if (added == 0) {
                    //since added = 0 before the loop the program doesn't know if buf[99] ended with a terminating character or not
                    //if number != 0 then it means that there was a word being added to words and therefore gets compared against redact_strings
                    if(number == 0){
                        write[0] = buf[i];//there was no word being added to words, so it simply writes the current character
                        fwrite(write, 1, 1, out);
                    }else{

                        compare(words, number, indexes, total_indexes, write, out, redact_strings, buf, i);
                        number = 0;
                        added =0;
                        words = (char *) realloc(words, (1) * sizeof(char));//this frees some memory space
                    }
                } else {

                    compare(words, number, indexes, total_indexes, write, out, redact_strings, buf, i);
                    number = 0;
                    added = 0;
                    words = (char *) realloc(words, (1) * sizeof(char));
                }
            } else {
                //this appends characters to char* words until a space \n etc is found
                *(words + number) = buf[i];
                number++;
                words = (char *) realloc(words, (number + 1) * sizeof(char));
                added = 1;
            }
        }
    }while(nread>0);
    free(words);
    free(indexes);
    fclose(inp);
    fclose(out);

}






void redact_words(const char *text_filename, const char *redact_words_filename){
    char *redact_strings = set_redact_words(redact_words_filename);
    int* indexes;
    int total_indexes = 0;
    indexes = (int*) malloc(sizeof(int));
    *indexes = 0;
    for(int i = 0; i<size;i++){
        if(*(redact_strings+i)== '\0'){
            total_indexes++;
            indexes = (int*) realloc(indexes, (total_indexes+1)*sizeof(int));
            *(indexes+total_indexes) = i+1;
        }
    }
    indexes = (int*) realloc(indexes, (total_indexes-1)*sizeof(int));

    write_to_file(redact_strings, text_filename, indexes, total_indexes);
    free(redact_strings);
}

int main(int argc, char *argv[]) {
    const char *input_file = "./debate.txt";
    const char *redact_file = "./redact.txt";
    redact_words(input_file, redact_file);
    return EXIT_SUCCESS;
}
