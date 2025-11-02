
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// It defines all recognized symbols in the PL/0 lexical analyzer
// Each token assigned a unique numeric value
typedef enum {
    skipsym = 1,    // Skip/ignore token
    identsym,       // Identifier
    numbersym,      // Number
    plussym,        // +
    minussym,       // -
    multsym,        // *
    slashsym,       // /
    eqsym,          // =
    neqsym,         // <>
    lessym,         // <
    leqsym,         // <=
    gtrsym,         // >
    geqsym,         // >=
    lparentsym,     // (
    rparentsym,     // )
    commasym,       // ,
    semicolonsym,   // ;
    periodsym,      // .
    becomessym,     // :=
    beginsym,       // begin
    endsym,         // end
    ifsym,          // if
    fisym,          // fi
    thensym,        // then
    whilesym,       // while
    dosym,          // do
    callsym,        // call
    constsym,       // const
    varsym,         // var
    procsym,        // procedure
    writesym,       // write
    readsym,        // read
    elsesym,        // else
    evensym         // even
} TokenType;

// prototype functions
int Create_lexeme(char *filtered_words, char **lexeme, int filterCount);
int Create_tokens(char **lexeme, int lexcount, int *token);
void Filter_comments(char *word_chars, int count, char *filtered_word_chars, int *filtered_count);

int main(int argc, char **argv)
{
    
    // Ensure that only one argument was passed into the program
    if (argc != 2)
    {
        printf("Error: Exactly one argument is permitted\n");

        return 1;
    }

    int max = 1000; // initial size

    char **lexeme = malloc(sizeof(char*) * max);

    // this will store what is read from the txt file
    char *word_chars = malloc(sizeof(char) * max); // store 1000 characters

    char *filtered_word_chars = malloc(sizeof(char) * max); // stores all characters ignoring the whitespaces and comments 

    int filtered_count = 0;

    int count = 0; // Use to store each character into the array word_char

    char letter; // Just holds each character that gets read

    // read and store the letters into the array
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile != NULL)
    {
        while((letter = fgetc(inputFile)) != EOF && count < max)
        {
            word_chars[count] = letter; // store characters in array

            count++; // move to next index

            if(count >= max)
            {
                max *= 2; // get more room

                word_chars = realloc(word_chars, max * sizeof(char)); //allocate more

                filtered_word_chars = realloc(filtered_word_chars, max * sizeof(char)); //allocate more
                
                // Incase reallocation fails 
                if(!word_chars || !filtered_word_chars)
                {
                    return 1;
                }
            }
        }
    }

    fclose(inputFile);

    // Filter out all the comments in the source code
    Filter_comments(word_chars, count, filtered_word_chars, &filtered_count);

    // create the lexeme and also keep count     
    int lexCount = Create_lexeme(filtered_word_chars, lexeme, filtered_count);

    int *tokens = malloc(lexCount * sizeof(int));   // make the sizeof(int) forgot to change which caused memory corruption
    
    int tokenCount = Create_tokens(lexeme, lexCount, tokens);

    printf("\n");

    // Output (complete later)
    printf("Source Program:\n\n");
    for(int i = 0; i < count; i++)
    {
        printf("%c", word_chars[i]);
    }

    printf("\n\nLexeme Table:\n\n");
    printf("lexeme\ttoken type\n");

    for(int i = 0; i < lexCount; i++)
    {
        if(tokens[i] == skipsym)   // check which was flagged either the numbers or identifiers
        {
            if(isalpha(lexeme[i][0]))
            {
                printf("%s\tIdentifier too long\n", lexeme[i]);
            }
            else if(isdigit(lexeme[i][0]))
            {
                printf("%s\tNumber too long\n", lexeme[i]);
            }
            
        }
        else    // everything gets printed normally
        {
            printf("%s\t%d\n",lexeme[i], tokens[i]);
        }
    }
    printf("\nToken List:\n\n");

    for(int i = 0; i < lexCount; i++)
    {
        if(tokens[i] == skipsym && lexeme[i][0] == '/')
        {
            continue;
        }
        else if(tokens[i] == 2 || tokens[i] == 3)   // check the number or identifier and display the value next to it
        {
            printf("%d %s ",tokens[i], lexeme[i]);
        }
        else
        {
            printf("%d ", tokens[i]);   // any other character
        }
    }
    
    printf("\n");

    // free memory
    free(word_chars);
    
    free(filtered_word_chars);
    
    for(int i = 0; i < lexCount; i++)
    {
        free(lexeme[i]);
    }

    free(lexeme);

    free(tokens);
    
    return 0;
}


// each word will match a token value
int Create_tokens(char **lexeme, int lexcount, int *token)
{
    int count = 0;

    for(int i = 0; i < lexcount; i++)
    {
        int c = 0;  // tracking between the comment

        // reserved words
        if(strcmp(lexeme[i], "begin") == 0)
        {
            token[i] = beginsym;

            count++;
        }
        else if(strcmp(lexeme[i], "end") == 0)
        {
            token[i] = endsym;

            count++;
        }
        else if(strcmp(lexeme[i], "if") == 0)
        {
            token[i] = ifsym;

            count++;
        }
        else if(strcmp(lexeme[i], "fi") == 0)
        {
            token[i] = fisym;

            count++;
        }
        else if(strcmp(lexeme[i], "then") == 0)
        {
            token[i] = thensym;

            count++;
        }
        else if(strcmp(lexeme[i], "while") == 0)
        {
            token[i] = whilesym;

            count++;
        }
        else if(strcmp(lexeme[i], "do") == 0)
        {
            token[i] = dosym;

            count++;
        }
        else if(strcmp(lexeme[i], "call") == 0)
        {
            token[i] = callsym;

            count++;
        }
        else if(strcmp(lexeme[i], "const") == 0)
        {
            token[i] = constsym;

            count++;
        }
        else if(strcmp(lexeme[i], "var") == 0)
        {
            token[i] = varsym;

            count++;
        }
        else if(strcmp(lexeme[i], "procedure") == 0)
        {
            token[i] = procsym;

            count++;
        }
        else if(strcmp(lexeme[i], "write") == 0)
        {
            token[i] = writesym;

            count++;
        }
        else if(strcmp(lexeme[i], "read") == 0)
        {
            token[i] = readsym;

            count++;
        }
        else if(strcmp(lexeme[i], "else") == 0)
        {
            token[i] = elsesym;

            count++;
        }
        else if(strcmp(lexeme[i], "even") == 0)
        {
            token[i] = evensym;
            
            count++;
        }
        // for identifiers
        else if(isalpha(lexeme[i][0]))
        {
            int length = strlen(lexeme[i]); // to check everyth individual character 
            
            int accept = 1; // is a mixture of letters and numbers

            for(int k = 0; k < length; k++)
            {
                if(!isalnum(lexeme[i][k]))
                {
                    accept = 0; // doesnt contain only letters and numbers mixture
                    
                    break;
                }
            }   

            if(length > 11 || !accept)
            {
                token[i] = skipsym;     // invalid identifier meaning lexical error
            }
            // everything is fine and is a valid identifier
            else 
            {
                token[i] = identsym;

                count++;
            }

        }

        // for digits  must make sure theyre all digits and no greater than 5
        else if(isdigit(lexeme[i][0]) )
        {
            int length = strlen(lexeme[i]); // get the length to check each character

            int confirm_digit = 1;  // it is a number 

            for(int d = 0; d < length; d++)
            {
                if(!(isdigit(lexeme[i][d])))
                {
                    confirm_digit = 0;

                    break;  // no need to look further if its not a digit
                }
            }

            // check max or after the checking if digit
            if(strlen(lexeme[i]) > 5 || !confirm_digit)
            {
                token[i] = skipsym;  // this will be use to know if its lexical error
            }
            // everything is good and valid digit
            else
            {
                token[i] = numbersym;
                count++;
            }
        }
        // Symbols
        else if(strcmp(lexeme[i], "+") == 0)
        {
            token[i] = plussym;
            count++;
        }
        else if(strcmp(lexeme[i], "-") == 0)
        {
            token[i] = minussym;
            count++;
        }
        else if(strcmp(lexeme[i], "*") == 0)
        {
            token[i] = multsym;
            count++;
        }
        else if(strcmp(lexeme[i], "/") == 0)
        {
            token[i] = slashsym;
            count++;
        }
        else if(strcmp(lexeme[i], "=") == 0)
        {
            token[i] = eqsym;
            count++;
        }
        else if(strcmp(lexeme[i], "<>") == 0)
        {
            token[i] = neqsym;
            count++;
        }
        else if(strcmp(lexeme[i], "<") == 0)
        {
            token[i] = lessym;
            count++;
        }
        else if(strcmp(lexeme[i], "<=") == 0)
        {
            token[i] = leqsym;
            count++;
        }
        else if(strcmp(lexeme[i], ">") == 0)
        {
            token[i] = gtrsym;
            count++;
        }
        else if(strcmp(lexeme[i], ">=") == 0)
        {
            token[i] = geqsym;
            count++;
        }
        else if(strcmp(lexeme[i], "(") == 0)
        {
            token[i] = lparentsym;
            count++;
        }
        else if(strcmp(lexeme[i], ")") == 0)
        {
            token[i] = rparentsym;
            count++;
        }
        else if(strcmp(lexeme[i], ",") == 0)
        {
            token[i] = commasym;
            count++;
        }
        else if(strcmp(lexeme[i], ";") == 0)
        {
            token[i] = semicolonsym;
            count++;
        }
        else if(strcmp(lexeme[i], ".") == 0)
        {
            token[i] = periodsym;
            count++;
        }
        else if(strcmp(lexeme[i], ":=") == 0)
        {
            token[i] = becomessym;
            count++;
        }


        // testing to see if the symbols letters and numbers are being tokenized correctly
        /*
        printf("\n%d  is for %s\n", token[i], lexeme[i]);
        */
    }
    return count;
}



// group the indivdual characters to display a string word
int Create_lexeme(char *filtered_words, char **lexeme, int filterCount)
{
    int lex_count = 0; // track the created lexeme

    for(int i = 0; i < filterCount; i++)
    {
        if(isspace(filtered_words[i])) // this should help as boundaries
        {
            continue;
        }

        // identifieres begin with letter and could follow with letter or digits
        // helps with reserved words since they are also words so it would follow up with letters
        if(isalpha(filtered_words[i]))
        {
            char lex_word [25]; // this is to store a lexeme ... resets to add another lexeme
            
            int j = 0;

            // this will follow up after the beginning letter
            while(isalpha(filtered_words[i]) || isdigit(filtered_words[i]))
            {
                // store into lex array
                lex_word[j++] = filtered_words[i++];
            }

            lex_word[j] = '\0'; // strcpy doesnt know when to stop unless it sees null
            
            int length = strlen(lex_word);

            lexeme[lex_count] = malloc(length + 1);

            strcpy(lexeme[lex_count], lex_word); // copy into array

            lex_count++;

            i--;    // since the loop already increments itll skip so return it back
        }
        // if it starts with a number it must be a number
        else if(isdigit(filtered_words[i]))
        {
            char lex_word [25];
            int j = 0;

            while(isdigit(filtered_words[i]))
            {
                lex_word[j++] = filtered_words[i++];
            }

            lex_word[j] = '\0'; // strcpy doesnt know when to stop unless it sees null

            int length = strlen(lex_word);

            lexeme[lex_count] = malloc(length + 1);

            strcpy(lexeme[lex_count], lex_word); //copy into array of lexeme

            lex_count++; 

            i--; // return the count back so it doesnt skip
        }
        else
        {
            char lex_word [25];
            int j = 0;

            if((filtered_words[i] == '<' && filtered_words[i+1] == '=') || (filtered_words[i] == '>' && filtered_words[i+1] == '=') || (filtered_words[i] == '<' && filtered_words[i+1] == '>') || (filtered_words[i] == ':' && filtered_words[i+1] == '=') )
            {
                // since its a 2 character it must be added to the string so loop to add both characters
                for(int k = 0; k < 2; k++)
                {
                    lex_word[j++] = filtered_words[i++];
                }
                lex_word[j] = '\0'; // strcpy doesnt know when to stop unless it sees null

                int length = strlen(lex_word);

                lexeme[lex_count] = malloc(length + 1);

                strcpy(lexeme[lex_count], lex_word); //copy into array of lexeme

                lex_count++; 
            }
            // only one string but will be later used to check for invalid characters
            else
            {
                lex_word[j++] = filtered_words[i++];

                lex_word[j] = '\0'; // strcpy doesnt know when to stop unless it sees null

                int length = strlen(lex_word);

                lexeme[lex_count] = malloc(length + 1);

                strcpy(lexeme[lex_count], lex_word); //copy into array of lexeme

                lex_count++;

                i--;    // prevent skipping
            }

        }
    }

    return lex_count; // return the number of lexemes can be used when going through the array
}

void Filter_comments(char *word_chars, int count, char *filtered_word_chars, int *filtered_count)
{
    // Tracks the start of the current comment block. -1 = not in comment block
    int comment_beginning = -1;

    // run through the array and filter out comments
    for(int i = 0; i < count; i++)
    {

        // If we are not in a comment block and we detect a comment beginning
        if(comment_beginning == -1 && word_chars[i] == '/' && word_chars[i+1] == '*')
        {
            comment_beginning = i;

            // Add / and * to filtered_word_chars
            filtered_word_chars[*filtered_count] = word_chars[i++];
            *filtered_count += 1;

            filtered_word_chars[*filtered_count] = word_chars[i];
            *filtered_count += 1;
        }
        // If we are in a comment block and we detect the end of the comment
        else if (comment_beginning != -1 && word_chars[i] == '*' && word_chars[i + 1] == '/')
        {
            // Set filtered_count to the beginning of the comment by subtracting comment length
            *filtered_count -= i - comment_beginning;

            // Replace comment with space
            filtered_word_chars[*filtered_count] = ' ';
            *filtered_count += 1;

            // Reset comment_beginning to show we are not in a comment block
            comment_beginning = -1;

            // Help skip over the * and /
            i += 1;
        }
        else
        {
            filtered_word_chars[*filtered_count] = word_chars[i];
            *filtered_count += 1;
        }
    }
}
