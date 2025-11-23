// let me tell you how this program works

// Program starts in main()
// Loads chatbot database from ChatBotDB.csv using loadDatabase()
// Loads question list from QuestionDB.csv using loadLines()

// Prints welcome message using printWelcomeMessage()
// Prints all questions with line numbers using printQuestions()

// Gets user's first input using getInitialInput()
// Checks if input is a number using isIntegerInput()

// If input is a number:
//     Converts it to int
//     Passes it to processIntegerInput()
//     That function uses questionLines[questionNumber - 1] as input
//     Then calls getBestMatchingAnswer() to find best response
//     Then prints the answer using printAnswer()
//     Then enters chatbot loop using chatLoop()

// If input is text:
//     Passes it to processTextInput()
//     That function calls getBestMatchingAnswer() to find best response
//     Then prints the answer using printAnswer()
//     Then enters chatbot loop using chatLoop()

// Inside chatLoop():
//     Repeatedly gets user input using getUserInput()
//     If input is "quit" or "q", exits program
//     Otherwise, finds best answer using getBestMatchingAnswer()
//     Then prints it using printAnswer()

// getBestMatchingAnswer() compares user input with all database keys
// Uses countSharedWords() to count matching words
// Keeps the best-scoring answer above matchThreshold

// countSharedWords() splits both strings using splitIntoWords()
// Then compares word by word to count matches

// All file reading is done using loadLines() and loadDatabase()
// All printing is done using printAnswer() or cout inside helper functions



#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ===== Function Prototypes =====

// Prints welcome message and instructions
void printWelcomeMessage();

// Prints all questions from the question file
void printQuestions(const vector<string> &questionLines);

// Gets the first input from the user
string getInitialInput();

// Gets input during chatbot loop
string getUserInput();

// Checks if input is a valid integer
bool isIntegerInput(const string &input);

// Loads lines from a file into a string vector
vector<string> loadLines(const string &fileName);

// Loads key-answer pairs from CSV database
vector<pair<string, string>> loadDatabase(const string &fileName);

// Splits a sentence into individual words
vector<string> splitIntoWords(const string &text);

// Counts how many words are shared between key and user input
int countSharedWords(const string &keyPhrase, const string &userInput);

// Finds the best matching answer from the database
string getBestMatchingAnswer(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold);

// Prints chatbot answer line by line
void printAnswer(const string &rawAnswer);

// Handles numeric input and prints corresponding answer
void processIntegerInput(int questionNumber, const vector<string> &questionLines, const vector<pair<string, string>> &databaseLines, int matchThreshold);

// Handles text input and prints chatbot response
void processTextInput(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold);

// Main chatbot loop for continuous interaction
void chatLoop(const vector<pair<string, string>> &databaseLines, int matchThreshold);

// ===== Main Function =====
int main() {
    string databaseFileName = "ChatBotDB.csv";
    string questionFileName = "QuestionDB.csv";
    int matchThreshold = 1;

    vector<pair<string, string>> databaseLines = loadDatabase(databaseFileName);
    vector<string> questionLines = loadLines(questionFileName);

    printWelcomeMessage();
    printQuestions(questionLines);

    string firstInput = getInitialInput();

    if (isIntegerInput(firstInput)) {
        int questionNumber = stoi(firstInput);
        processIntegerInput(questionNumber, questionLines, databaseLines, matchThreshold);
    }
    else {
        processTextInput(firstInput, databaseLines, matchThreshold);
    }

    return 0;
}

// ===== Function Definitions =====

// Prints welcome message and instructions
void printWelcomeMessage() {
    cout << "Type \"quit\" or \"q\" to exit the program." << endl;
    cout << "\nChoose a number or enter your question." << endl;
}

// Prints all questions from the question file
void printQuestions(const vector<string> &questionLines) {
    for (int i = 0; i < questionLines.size(); i++) {
        cout << i + 1 << "- " << questionLines[i] << endl;
    }
}

// Gets the first input from the user
string getInitialInput() {
    cout << "\nYou: ";
    string input;
    getline(cin, input);
    return input;
}

// Gets input during chatbot loop
string getUserInput() {
    cout << "\nYou: ";
    string input;
    getline(cin, input);
    return input;
}

// Checks if input is a valid integer
bool isIntegerInput(const string &input) {
    if (input.empty())
        return false;
    for (char c : input) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

// Loads lines from a file into a string vector
vector<string> loadLines(const string &fileName) {
    vector<string> lines;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Loads key-answer pairs from CSV database
vector<pair<string, string>> loadDatabase(const string &fileName) {
    vector<pair<string, string>> database;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string key = line.substr(0, commaPos);
            string value = line.substr(commaPos + 1);
            database.push_back(pair<string, string>(key, value));
        }
    }
    return database;
}

// Splits a sentence into individual words
vector<string> splitIntoWords(const string &text) {
    vector<string> words;
    stringstream inputStream(text);
    string word;
    while (inputStream >> word) {
        words.push_back(word);
    }
    return words;
}

// Counts how many words are shared between key and user input
int countSharedWords(const string &keyPhrase, const string &userInput) {
    vector<string> keyWords = splitIntoWords(keyPhrase);
    vector<string> inputWords = splitIntoWords(userInput);

    int matchCount = 0;
    for (int i = 0; i < keyWords.size(); i++) {
        for (int j = 0; j < inputWords.size(); j++) {
            if (keyWords[i] == inputWords[j]) {
                matchCount++;
                break;
            }
        }
    }
    return matchCount;
}

// Finds the best matching answer from the database
string getBestMatchingAnswer(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold) {
    int bestScore = 0;
    string bestAnswer = "Sorry, I couldn't find a suitable answer.";

    for (int i = 0; i < databaseLines.size(); i++) {
        int score = countSharedWords(databaseLines[i].first, userInput);
        if (score >= matchThreshold && score > bestScore) {
            bestScore = score;
            bestAnswer = databaseLines[i].second;
        }
    }
    return bestAnswer;
}

// Prints chatbot answer line by line
void printAnswer(const string &rawAnswer) {
    stringstream inputStream(rawAnswer);
    string part;
    bool isFirstLine = true;

    while (getline(inputStream, part, ',')) {
        if (isFirstLine) {
            cout << "ChatBot:" << part << endl;
            isFirstLine = false;
        }
        else {
            cout << part << endl;
        }
    }
}

// Handles numeric input and prints corresponding answer
void processIntegerInput(int questionNumber, const vector<string> &questionLines, const vector<pair<string, string>> &databaseLines, int matchThreshold) {
    while (questionNumber < 1 || questionNumber > questionLines.size()) {
        cout << "Invalid line number. You can enter a valid number or type a question: ";
        string retryInput;
        getline(cin, retryInput);

        if (retryInput == "q" || retryInput == "quit") {
            cout << "ChatBot: Goodbye!" << endl;
            exit(0);
        }

        if (isIntegerInput(retryInput)) {
            questionNumber = stoi(retryInput);
        } else {
            // If input is not a number, treat it as a question
            string rawAnswer = getBestMatchingAnswer(retryInput, databaseLines, matchThreshold);
            printAnswer(rawAnswer);
            chatLoop(databaseLines, matchThreshold);
            return;
        }
    }

    string selectedLine = questionLines[questionNumber - 1];
    string rawAnswer = getBestMatchingAnswer(selectedLine, databaseLines, matchThreshold);
    printAnswer(rawAnswer);
    chatLoop(databaseLines, matchThreshold);
}

// Handles text input and prints chatbot response
void processTextInput(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold) {
    string rawAnswer = getBestMatchingAnswer(userInput, databaseLines, matchThreshold);
    printAnswer(rawAnswer);
    chatLoop(databaseLines, matchThreshold);
}

// Main chatbot loop for continuous interaction
void chatLoop(const vector<pair<string, string>> &databaseLines, int matchThreshold) {
    while (true) {
        string userInput = getUserInput();
        if (userInput == "quit" || userInput == "q") {
            cout << "ChatBot: Goodbye!" << endl;
            exit(0);
        }
        string rawAnswer = getBestMatchingAnswer(userInput, databaseLines, matchThreshold);
        printAnswer(rawAnswer);
    }
}
