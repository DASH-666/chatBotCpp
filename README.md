# chatBotCpp
This is a simple chatbot I made with C++ for my university project.  
The program is cross-platform, but I tested it only on Linux.  
No need to recompile after making changes to the DB files :)

# ChatBotDB.csv
`ChatBotDB.csv` is the main database file.  
Before the first `,` is the key that the program checks against user input.  
After that comes the answer that the program will print if the key is selected.  
Each `,` after the first one is used to go to the next line.  

Example of ```ChatBotDB.csv```:
```
this is the key of this line,this is the line 1 answer,this is line 2 answer
```


# QuestionDB.csv
The first time you run the program, you have a chance to choose a question by entering an integer number.  
These questions are stored in `QuestionDB.csv`.

Example of ```QuestionDB.csv```:
```
this is the question ?
```


# Note
### also in the main source code are 3 variables that you can change:
databaseFileName (default is ChatBotDB.csv): this is the name and address of the main database file. You can change it, but you will also need to change the DB file name too.  

questionFileName (default is QuestionDB.csv): this is the name and address of the question database file. You can change it, but you will also need to change the DB file name too.  

matchThreshold (default is 1): this shows how many words in the key and user input must match to print that answer from that key.  


# How to build
Simple. Just run this:
```
g++ -O2 ChatBot.cpp -o chatbot-bin
```






this is made by parsa hn
