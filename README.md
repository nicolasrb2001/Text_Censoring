# Text_Censoring
This is my implementation of a program that censors words in [debate.txt](/cmake-build-debug/debate.txt) from a given set of words in [redact.txt](/cmake-build-debug/redact.txt).

## Motivation :flying_saucer:
---
This was part of my coursework during 1st year and I improved my understanding of string and text manipulation in C.

### Features
- This program can distinguish all words from each other in [redact.txt](/cmake-build-debug/redact.txt) as long as they are separated ([see screenshots](#screenshots))
- The program creates a new file (or overwrites it if already existing) [results.txt](/cmake-build-debug/results.txt) that containts the text after being censored.
### Technical Information
This was developed using [CLion](https://www.jetbrains.com/clion/). I had some constraints when developing this program: I was not allowed to use any string manipulation libraries to handle comparisons and more.
### Known Issues/Limitations
This program will ignore any symbols or characters that are not in the English alphabet, e.g having 1000 or (".? in [redact.txt](/cmake-build-debug/redact.txt) will not be assumed to be a word that has to be censored
### Screenshots
As mentioned in [Features](#features), the words to be censored can be stored in **any way**, ie the program ignores all the white space in [redact.txt](/cmake-build-debug/redact.txt).

![](/screenshots/screenshot1.png)

This is an example of part of the [text](/cmake-build-debug/debate.txt) that will be censored.

![](/screenshots/screenshot2.png)

And this is part of the [result](/cmake-build-debug/results.txt).

![](/screenshots/screenshot3.png)
