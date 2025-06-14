#ifndef HANGMAN_H
#define HANGMAN_H

void chooseWord(const char* list[], int listSize, char* word);
int isLetterInWord(char letter, const char* word);
void revealLetter(char letter, const char* word, char* display);
int isWordGuessed(const char* display);

#endif // HANGMAN_H
