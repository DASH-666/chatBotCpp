#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void printWelcomeMessage();
void printQuestions(const vector<string> &questionLines);
string getInitialInput();
string getUserInput();
bool isIntegerInput(const string &input);
vector<string> loadLines(const string &fileName);
vector<pair<string, string>> loadDatabase(const string &fileName);
vector<string> splitIntoWords(const string &text);
int countSharedWords(const string &keyPhrase, const string &userInput);
string getBestMatchingAnswer(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold);
void printAnswer(const string &rawAnswer);
void processIntegerInput(int questionNumber, const vector<string> &questionLines, const vector<pair<string, string>> &databaseLines, int matchThreshold);
void processTextInput(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold);
void chatLoop(const vector<pair<string, string>> &databaseLines, int matchThreshold);

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

void printWelcomeMessage() {
    cout << "Type \"quit\" or \"q\" to exit the program." << endl;
    cout << "\nChoose a number or enter your question." << endl;
}

void printQuestions(const vector<string> &questionLines) {
    for (int i = 0; i < questionLines.size(); i++) {
        cout << i + 1 << "- " << questionLines[i] << endl;
    }
}

string getInitialInput() {
    cout << "\nYou: ";
    string input;
    getline(cin, input);
    return input;
}

string getUserInput() {
    cout << "\nYou: ";
    string input;
    getline(cin, input);
    return input;
}

bool isIntegerInput(const string &input) {
    if (input.empty())
        return false;
    for (char c : input) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

vector<string> loadLines(const string &fileName) {
    vector<string> lines;
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

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

vector<string> splitIntoWords(const string &text) {
    vector<string> words;
    stringstream inputStream(text);
    string word;
    while (inputStream >> word) {
        words.push_back(word);
    }
    return words;
}

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

void processTextInput(const string &userInput, const vector<pair<string, string>> &databaseLines, int matchThreshold) {
    string rawAnswer = getBestMatchingAnswer(userInput, databaseLines, matchThreshold);
    printAnswer(rawAnswer);
    chatLoop(databaseLines, matchThreshold);
}

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
