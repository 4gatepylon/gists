#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef NULL
#define NULL 0
#endif

#define SEPARATOR "**********************"

struct ExampleSt {
    char** words;
    int numWords;
    int maxWidth;
};
typedef struct ExampleSt Example;

#define _MAX_WORD_LEN 25
Example example1() {
    // words    = ["This", "is", "an", "example", "of", "text", "justification."]
    // maxWidth = 16
    Example ex;
    ex.maxWidth = 16;
    ex.numWords = 7;
    ex.words = malloc(ex.numWords * sizeof(char*));
    for (int i = 0; i < ex.numWords; i++) {
        ex.words[i] = (char*)malloc(_MAX_WORD_LEN * sizeof(char));
        memset(ex.words[i], 0, _MAX_WORD_LEN * sizeof(char));
    }
    sprintf(ex.words[0], "This");
    sprintf(ex.words[1], "is");
    sprintf(ex.words[2], "an");
    sprintf(ex.words[3], "example");
    sprintf(ex.words[4], "of");
    sprintf(ex.words[5], "text");
    sprintf(ex.words[6], "justification.");
    return ex;
}

Example example2() {
    // words = ["What","must","be","acknowledgment","shall","be"],
    // maxWidth = 16
    Example ex;
    ex.maxWidth = 16;
    ex.numWords = 6;
    ex.words = malloc(ex.numWords * sizeof(char*));
    for (int i = 0; i < ex.numWords; i++) {
        ex.words[i] = (char*)malloc(_MAX_WORD_LEN * sizeof(char));
        memset(ex.words[i], 0, _MAX_WORD_LEN * sizeof(char));
    }
    sprintf(ex.words[0], "What");
    sprintf(ex.words[1], "must");
    sprintf(ex.words[2], "be");
    sprintf(ex.words[3], "acknowledgment");
    sprintf(ex.words[4], "shall");
    sprintf(ex.words[5], "be");
    return ex;
}

Example example3() {
    // words = ["Science","is","what","we","understand","well","enough","to","explain","to","a","computer.","Art","is","everything","else","we","do"]
    // maxWidth = 20
    Example ex;
    ex.maxWidth = 20;
    ex.numWords = 18;
    ex.words = malloc(ex.numWords * sizeof(char*));
    for (int i = 0; i < ex.numWords; i++) {
        ex.words[i] = (char*)malloc(_MAX_WORD_LEN * sizeof(char));
        memset(ex.words[i], 0, _MAX_WORD_LEN * sizeof(char));
    }
    sprintf(ex.words[0], "Science");
    sprintf(ex.words[1], "is");
    sprintf(ex.words[2], "what");
    sprintf(ex.words[3], "we");
    sprintf(ex.words[4], "understand");
    sprintf(ex.words[5], "well");
    sprintf(ex.words[6], "enough");
    sprintf(ex.words[7], "to");
    sprintf(ex.words[8], "explain");
    sprintf(ex.words[9], "to");
    sprintf(ex.words[10], "a");
    sprintf(ex.words[11], "computer.");
    sprintf(ex.words[12], "Art");
    sprintf(ex.words[13], "is");
    sprintf(ex.words[14], "everything");
    sprintf(ex.words[15], "else");
    sprintf(ex.words[16], "we");
    sprintf(ex.words[17], "do");
    return ex;
}

char** fullJustify(char** words, int wordsSize, int maxWidth, int* returnSize) {
    // Over-allocate because why not I guess
    int* leftoverSpace = malloc(wordsSize * sizeof(int));
    int* numWords = malloc(wordsSize * sizeof(int));
    memset(leftoverSpace, 0, wordsSize);
    memset(numWords, 0, wordsSize);

    // Find the number of lines (buffers) we will need
    int line = 0;
    int numLines = 0;
    int currWordsLen = 0;
    int currNumWords = 0;
    for (int i = 0; i < wordsSize; i++){
        int wordLen = strlen(words[i]);
        int candidateTotalLen = (currWordsLen + wordLen) + currNumWords;
        if (candidateTotalLen > maxWidth) {
            leftoverSpace[line] = maxWidth - currWordsLen;
            numWords[line] = currNumWords;
            currWordsLen = wordLen;
            currNumWords = 1;
            line ++;
        } else {
            currWordsLen += wordLen;
            currNumWords ++;
        }
    }
    if (currNumWords > 0) {
        leftoverSpace[line] = maxWidth - currWordsLen;
        numWords[line] = currNumWords;
        line ++;
    }
    numLines = line;
    *returnSize = numLines;

    // Initialize all the buffers to be valid c-strings (empty to begin with)
    char** lines = malloc(numLines * sizeof(char*));
    for (int i = 0; i < numLines; i++) {
        lines[i] = (char*)malloc((maxWidth + 1) * sizeof(char));
        memset(lines[i], 0, (maxWidth + 1) * sizeof(char));
    }

    // Greedily fill all lines but the last, which is not padded the same
    int i = 0;
    for (int k = 0; k < numLines - 1; k++) {
        int leftover = leftoverSpace[k];
        int spacesBetween = numWords[k] - 1;
        int spaceWidth = 0;
        int pad = leftover;
        if (spacesBetween > 0) {
            spaceWidth = leftover / spacesBetween;
            pad = leftover % spacesBetween;
        }
        assert(spaceWidth >= 0);
        assert(leftover >= 0);
        assert(pad >= 0);
        assert(pad + spaceWidth * spacesBetween == leftover);

        int offset = 0;
        int remainingPad = pad;        
        for (int j = 0; j < numWords[k]; j++) {
            char* word = words[i + j];
            int len = strlen(word);

            memcpy(lines[k] + offset, word, len * sizeof(char));
            offset += len;

            // Copy in the spaces as necessary
            if (offset < maxWidth) {
                for (int w = 0; w < spaceWidth; w++) {
                    lines[k][offset + w] = ' ';
                }
                offset += spaceWidth;
                if (remainingPad > 0) {
                    if (numWords[k] - j > 1) {
                        lines[k][offset] = ' ';
                        offset ++;
                        remainingPad -= 1;
                    } else {
                        for (int w = 0; w < remainingPad; w++) {
                            lines[k][offset + w] = ' ';
                        }
                        offset += remainingPad;
                        remainingPad = 0;
                    }
                    
                }
            }
        }
        assert(offset == maxWidth);
        i += numWords[k];
    }

    // Fill in the last with padding only at the end, per the spec
    int offset = 0;
    assert(i + numWords[numLines - 1] == wordsSize);
    for (int j = i; j < wordsSize; j++) {
        char* word = words[j];
        int len = strlen(word);
        memcpy(lines[numLines - 1] + offset, word, len * sizeof(char));
        offset += len;
        if (offset < maxWidth) {
            lines[numLines - 1][offset] = ' ';
            offset ++;
        }
    }
    while (offset < maxWidth) {
        lines[numLines - 1][offset] = ' ';
        offset ++;
    }
    return lines;
}

// By value, but that's OK since it's pointers
void testExample(Example ex) {
    int returnSize = -1;
    char** justified = fullJustify(ex.words, ex.numWords, ex.maxWidth, &returnSize);
    assert(returnSize != -1);
    for (int i = 0; i < returnSize; i++) {
        printf("|%s|\n%s\n", justified[i], SEPARATOR);
    }
    // Remember to free fullJustify's output
    for (int i = 0; i < returnSize; i++) {
        free(justified[i]);
    }
    free(justified);
}

// By value, but that's OK since it's pointers (and it's one less free)
void freeExample(Example ex) {
    for (int i = 0; i < ex.numWords; i++) {
        free(ex.words[i]);
    }
    free(ex.words);
}

int main() {
    #define NUM_EXAMPLES 3
    Example examples[NUM_EXAMPLES] = {example1(), example2(), example3()};
    printf("%s%s%s\n", SEPARATOR, SEPARATOR, SEPARATOR);
    for (int i = 0; i < NUM_EXAMPLES; i++) {
        testExample(examples[i]);
        printf("%s%s%s\n", SEPARATOR, SEPARATOR, SEPARATOR);
    }
    for (int i = 0; i < NUM_EXAMPLES; i++) {
        freeExample(examples[i]);
    }
    return 0;
}