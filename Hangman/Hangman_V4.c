#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_TRIES 6

typedef struct Node {
    char content[600];
    struct Node *next;
    struct Node *linkedNode;  // Points to corresponding node in other list
} Node;

Node *CreateNode(char *content) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    strcpy(new_node->content, content);
    new_node->next = NULL;
    new_node->linkedNode = NULL;
    return new_node;
}

Node *InitializeList(char *items[]) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; items[i] != NULL; i++) {
        Node *new_node = CreateNode(items[i]);
        if (!head) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
    }
    return head;
}

void LinkNodes(Node *wordList, Node *questionList) {
    Node *tempWord = wordList;
    Node *tempQuestion = questionList;
    while (tempWord && tempQuestion) {
        tempWord->linkedNode = tempQuestion;
        tempWord = tempWord->next;
        tempQuestion = tempQuestion->next;
    }
}

void DrawHangman(int tries) {
    int startX = 550;
    int startY = 280;
    // Draw gallows in white for contrast
    DrawLine(startX, startY, startX, startY + 150, WHITE); // Vertical line
    DrawLine(startX, startY, startX + 100, startY, WHITE);  // Horizontal line
    DrawLine(startX + 100, startY, startX + 100, startY + 30, WHITE); // Rope

    // Hangman parts also in white
    if (tries < 6) {
        DrawCircle(startX + 100, startY + 40, 10, WHITE); // Head
    }
    if (tries < 5) {
        DrawLine(startX + 100, startY + 50, startX + 100, startY + 90, WHITE); // Body
    }
    if (tries < 4) {
        DrawLine(startX + 100, startY + 60, startX + 120, startY + 70, WHITE); // Right arm
    }
    if (tries < 3) {
        DrawLine(startX + 100, startY + 60, startX + 80, startY + 70, WHITE);  // Left arm
    }
    if (tries < 2) {
        DrawLine(startX + 100, startY + 90, startX + 120, startY + 120, WHITE); // Right leg
    }
    if (tries < 1) {
        DrawLine(startX + 100, startY + 90, startX + 80, startY + 120, WHITE);  // Left leg
    }
}

// Function to wrap text without breaking words
char** WrapText(const char* text, int lineLength, int* lineCount) {
    int textLength = strlen(text);
    int maxLines = textLength / lineLength + 1; // Maximum possible lines
    char** lines = (char**)malloc(maxLines * sizeof(char*));
    *lineCount = 0;

    const char* start = text;
    while (*start) {
        const char* end = start + lineLength;
        if (end >= text + textLength) {
            end = text + textLength;
        } else {
            const char* space = end;
            while (space > start && *space != ' ') space--;
            if (space > start) {
                end = space;
            }
        }

        int actualLineLength = end - start;
        lines[*lineCount] = (char*)malloc((actualLineLength + 1) * sizeof(char));
        strncpy(lines[*lineCount], start, actualLineLength);
        lines[*lineCount][actualLineLength] = '\0';

        (*lineCount)++;
        start = end;
        while (*start == ' ') start++; // Skip spaces
    }

    return lines;
}

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Hangman Game");
    SetTargetFPS(120);

    Font font = LoadFont("Lucida Bright Regular/CONSOLA.TTF");  // Load a custom font
    srand(time(NULL));
    int lives = MAX_TRIES;
    char *words[] = {
            "EFFICIENT USE OF RESOURCES",
            "REGENERATIVE",
            "ZERO WASTE",
            "AFFORDABLE AND CLEAN ENERGY",
            "DECENT WORK AND ECONOMIC GROWTH",
            "INDUSTRY INNOVATION INFRASTRUCTURE",
            "SUSTAINABLE CITIES AND COMMUNITIES",
            "RESPONSIBLE CONSUMPTION AND PRODUCTION",
            "CLIMATE ACTION",
            "PROTECTS",
            "REDUCES",
            "CREATES",
            "SAVES",
            "BIOECONOMY",
            "BIOTECHONOMY",
            "BIOLOGICAL",
            "CIRCULAR ECONOMY",
            "INTERSECTION",
            "INCREASE",
            "SUPPLYING",
            "CREATING",
            "EFFICIENT",
            "ACCELERATE",
            "DESIGNING",
            "EMPOWERING",
            "PRODUCTION PROCESSES",
            "FORK",
            "PRESERVATION",
            "POLLUTION",
            NULL
    };

    char *questions[] = {
        "What kind of model represents circular economy?",
        "What type of industrial economy represents the circular economy?",
        "What target does the circular economy have? (regarding the environment)",
        "The circular economy has the potential to contribute significantly to the following sustainable development goals (SDGs) set by the United Nations (UN). Specify SDG 7: _ _ _ _. ",
        "The circular economy has the potential to contribute significantly to the following sustainable development goals (SDGs) set by the United Nations (UN). SpecifySDG 8: _ _ _ _ _.",
        "The circular economy has the potential to contribute significantly to the following sustainable development goals (SDGs) set by the United Nations (UN). Specify SDG 9:_ _ _.",
        "The circular economy has the potential to contribute significantly to the following sustainable development goals (SDGs) set by the United Nations (UN). Specify SDG 11: _ _ _ _.",
        "The circular economy has the potential to contribute significantly to the following sustainable development goals (SDGs) set by the United Nations (UN). Specify SDG 12: _ _ _ _.",
        "The circular economy has the potential to contribute significantly to the following sustainable development goals (SDGs) set by the United Nations (UN). Specify SDG 13: _ _.",
        "Circular economy _ the environment (benefit)",
        "Switching to a circular economy _ raw material dependence",
        "Circular economy _ jobs",
        "Circular economy _  consumers money",
        "Bio-based economy is synonimous with:",
        "Bio-based economy is synonimous with:",
        "The definition of Circular bioeconomy definition is: the application of the Circular Economy concept to _ resources, products and materials.",
        "The definition of Circular bioeconomy definition is: A part of _ _.",
        "The definition of Circular bioeconomy definition is: _ of  Bioeconomy and Circular economy.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the main policies set in 2021 by the EU Commission  included in the European Green Deal are in connection to _ the EU's climate ambition for 2030 and 2050.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the  main policies set in 2021 by the EU Commission  included in the European Green Deal are in connection to _ clean, affordable, and secure energy.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the  main policies set in 2021 by the EU Commission  included in the European Green Deal are in connection to _ a clean and circular economy.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the  main policies set in 2021 by the EU Commission  included in the European Green Deal are in connection to _ building and renovating.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the  main policies set in 2021 by the EU Commission  included in the European Green Deal are in connection to _ the shift to sustainable and smart mobility.",
        "Circular Economy Action Plan is a product policy framework that will make sustainable products, services and business models the norm and transform consumption patterns so that no waste is produced. One of the main proposed actions for this aim is the following: _ sustainable products.",
        "Circular Economy Action Plan is a product policy framework that will make sustainable products, services, and business models the norm and transform consumption patterns so that no waste is produced One of the main proposed actions for this aim is the following: _ consumers and public buyers.",
        "Circular Economy Action Plan is a product policy framework that will make sustainable products, services, and business models the norm and transform consumption patterns so that no waste is produced One of the main proposed actions for this aim is the following: Circularity in _ _.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the main policies set in 2021 by the EU Commission included in the European Green Deal are in connection to: <<From farm to _>>",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the main policies set in 2021 by the EU Commission included in the European Green Deal are in connection to: Ecosystem and biodiversity _ and restoration.",
        "The European Green Deal (EDG) is a package of policy initiatives, which aims to set the EU on the path to a green transition, with the ultimate goal of reaching climate neutrality by 2050. The primary goals under the Deal are net zero emissions of greenhouse gasses by 2050; economic growth is decoupled from the exploitation of resources; no one being left behind. In this aim, the main policies set in 2021 by the EU Commission included in the European Green Deal are in connection to: A zero _ ambition for a toxic-free environment.",
        NULL
    };

    Node *word_list = InitializeList(words);
    Node *question_list = InitializeList(questions);

    LinkNodes(word_list, question_list);

    int wc = 0;
    Node *temp = word_list;
    while (temp) {
        wc++;
        temp = temp->next;
    }

    int questions_used = 0;
    int total_questions = wc;
    int max_questions = total_questions * 0.8; // 80% of total questions

    bool *questionsAsked = (bool *)malloc(sizeof(bool) * total_questions);
    memset(questionsAsked, 0, sizeof(bool) * total_questions);

    int score = 0;
    bool gameFinished = false;
    bool gameCompleted = false;
    bool finalMessageShown = false;
    char letters[256] = {0}; // Track used letters
    bool scoreAdded = false;
    bool scoreSubtracted = false;
    bool finish = false;

    Node *selectedWordNode = word_list;
    char *word = NULL;
    char *question = NULL;
    char *dword = NULL;
    int wordlen = 0;

    void selectNewQuestion() {
        int rand_idx = rand() % total_questions;
        while (questionsAsked[rand_idx]) {
            rand_idx = rand() % total_questions;
        }
        questionsAsked[rand_idx] = true;
        questions_used++;

        selectedWordNode = word_list;
        for (int i = 0; i < rand_idx; i++) {
            selectedWordNode = selectedWordNode->next;
        }
        word = selectedWordNode->content;
        question = selectedWordNode->linkedNode->content;

        wordlen = strlen(word);
        if (dword) free(dword);
        dword = (char *)malloc(wordlen + 1);
        for (int i = 0; i < wordlen; i++) {
            dword[i] = (word[i] == '-') ? '-' : (word[i] == ' ') ? ' ' : '_';
        }
        dword[wordlen] = '\0';
        memset(letters, 0, sizeof(letters)); // Reset used letters
        lives = MAX_TRIES;
        gameFinished = false; // Allow new game input
    }

    selectNewQuestion();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK); // Changed from RAYWHITE to BLACK for dark theme
        int lineLength = 95; // Maximum characters per line
        int lineCount = 0;
        int lineCount_2 = 0;
        if (!finalMessageShown) {
            int lineLength = 95; // Maximum characters per line
            int lineCount = 0;
            char** wrappedText = WrapText(question, lineLength, &lineCount);
            DrawTextEx(font, "Complete the missing words from the spaces (_):", (Vector2){20, 20}, 20, 2, PURPLE);
            for (int i = 0; i < lineCount; i++) {
                DrawTextEx(font, wrappedText[i], (Vector2){ 20, 50 + i * 30 }, 20, 2, SKYBLUE); // Adjust the y-position for each line
            }

            DrawTextEx(font, "Guess the word:", (Vector2){ 20, 80 + (lineCount - 1) * 30 }, 20, 2, LIGHTGRAY); // Changed text color to LIGHTGRAY for visibility

            for (int i = 0; i < wordlen; i++) {
                DrawTextEx(font, TextFormat("%c ", dword[i]), (Vector2){ 20 + 18 * i, 110 + (lineCount - 1) * 30 }, 20, 2, LIGHTGRAY); // Changed color to LIGHTGRAY
            }

            DrawTextEx(font, TextFormat("Score: %d", score), (Vector2){ 20, 170 + (lineCount - 1) * 30 }, 20, 2, MAGENTA);
            DrawTextEx(font, "Used letters: ", (Vector2){ 20, 140 + (lineCount - 1) * 30 }, 20, 2, LIGHTGRAY); // Changed text color to LIGHTGRAY
            int offsetX = 0;
            for (int i = 0; i < 256; i++) {
                if (letters[i]) {
                    DrawTextEx(font, TextFormat("%c ", i), (Vector2){ 200 + offsetX, 140 + (lineCount - 1) * 30 }, 20, 2, GRAY); // Changed color to GRAY for a softer contrast
                    offsetX += 20;
                }
            }

            DrawHangman(lives);

            int key = GetKeyPressed();
            while (key > 0) {
                if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) { // A-Z or a-z
                    char ch = toupper(key);
                    if (!letters[ch]) {
                        letters[ch] = 1;
                        int found = 0;
                        for (int i = 0; i < wordlen; i++) {
                            if (word[i] == ch) {
                                dword[i] = ch;
                                found = 1;
                            }
                        }
                        if (!found) {
                            lives--;
                        }
                    }
                }
                key = GetKeyPressed(); // Check next pressed key
            }
            if (lives <= 0 || strcmp(dword, word) == 0) {
                finish = true;
                DrawTextEx(font, (lives <= 0) ? "You lost!" : "You won!", (Vector2){ 300, 480 }, 20, 2, (lives <= 0) ? DARKBLUE : DARKGREEN);
                DrawTextEx(font, TextFormat("The word was: %s", word), (Vector2){ 300, 510 }, 20, 2, LIGHTGRAY);
                if (strcmp(dword, word) == 0 && !scoreAdded) {
                    score += 10;
                    scoreAdded = true;
                }
                else if (!scoreSubtracted && lives <= 0) {
                    score -= 5;
                    scoreSubtracted = true;
                }
                gameFinished = true; // Game ends, waiting for reset
                DrawTextEx(font, "Press [BACKSPACE] to exit or [SPACE] to go to the next round", (Vector2){ 300, 540 }, 20, 2, LIME);
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    break;
                }
            }

            if (IsKeyPressed(KEY_SPACE) && !gameCompleted) {
                if (!finish)
                    score -= 5; // Deduct 5 points for skipping
                if (questions_used >= max_questions) {
                    gameCompleted = true;
                } else {
                    scoreAdded = false; // Reset the game when SPACE is pressed
                    scoreSubtracted = false;
                    finish = false;
                    selectNewQuestion();
                }
            }

            if (gameCompleted) {
                DrawTextEx(font, "Game Completed!", (Vector2){ 300, 570 }, 20, 2, LIME);
                DrawTextEx(font, TextFormat("Final Score: %d", score), (Vector2){ 300, 600 }, 20, 2, LIME);
                DrawTextEx(font, "Press ENTER for displaying the final score and credentials", (Vector2){ 300, 630 }, 20, 2, LIME);
            }
        }

        if (gameCompleted && IsKeyPressed(KEY_ENTER) && !finalMessageShown) {
            finalMessageShown = true;
        }
        
        if (finalMessageShown) {
            ClearBackground(BLACK);
            char** wrappedText = WrapText("Students: Alexandru Gabriel, Dumitru Claudia Stefania, Dobrescu Diana, Pralea Bogdan-Stefan, Odoroaga Vlad-Ionut, Manole Daniel", lineLength, &lineCount);
            char **wrappedText_2 = WrapText("Student's e-mail: gabriel.alexandru04@stud.fiir.upb.ro, claudia.dumitru0208@stud.fiir.upb.ro, diana.dobrescu@stud.fiir.upb.ro, bogdan.pralea@stud.fiir.upb.ro, vlad_ionut.odoroaga@stud.fiir.upb.ro, daniel.manole@stud.fiir.upb.ro", lineLength, &lineCount_2);
            DrawTextEx(font, "Final Score:", (Vector2){ 20, 20 }, 20, 2, SKYBLUE);
            DrawTextEx(font, TextFormat("%d", score), (Vector2){ 180, 20 }, 20, 2, LIME);
            DrawTextEx(font, "Credentials:", (Vector2){ 20, 50 }, 20, 2, RED);
            DrawTextEx(font, "EU Programme Erasmus+", (Vector2){ 50, 80 }, 20, 2, VIOLET);
            DrawTextEx(font, "Project title: Gender, Digitalization, Green: Ensuring a Sustainable Future for all in Europe", (Vector2){ 50, 110 }, 20, 2, BEIGE);
            DrawTextEx(font, "Project No: 2023-1-RO01- KA220-HED-000154433", (Vector2){ 50, 140 }, 20, 2, BEIGE);
            for (int i = 0; i < lineCount; i ++) {
                DrawTextEx(font, wrappedText[i], (Vector2){ 50, 170 + i * 30}, 20, 2, ORANGE);
            }
            for (int i = 0; i < lineCount_2; i ++) {
                DrawTextEx(font, wrappedText_2[i], (Vector2){ 50, 200 + (lineCount - 1) * 30 + i * 30}, 20, 2, MAGENTA);
            }
            DrawTextEx(font, "Teachers: Prof M. Caramihai & D Chis", (Vector2){50, 200 + (lineCount + lineCount_2 - 1) * 30}, 20, 2, BLUE);
        }

        EndDrawing();
    }

    UnloadFont(font); // Unload the font
    CloseWindow();
    free(dword);
    free(questionsAsked);

    return 0;
}
