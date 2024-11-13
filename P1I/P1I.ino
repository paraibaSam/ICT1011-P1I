#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>

TinyScreen display = TinyScreen(TinyScreenDefault);

// GLOBAL VARIABLES //
String studentID = "_______";
int studentCurrentPosition = 0;
String roomID = "___";
int roomCurrentPosition = 0;
int score = 0;

bool studentfilled = false;
bool roomfilled = false;
bool hoststarted = false;
bool waiting = false;
bool quizcomplete = false;

char questionsArray[5] = "ABCD"; // PLACEHOLDER QUESTION ANSWERS

char answersArray[10];   // Array to hold characters A, B, C, or D max size of 10 + null
int answersIndex = 0;    // Tracks the next empty position in the array


void setup(void) {
  Wire.begin();
  display.begin();
  display.setBrightness(10); //0-15
  display.setFont(thinPixel7_10ptFontInfo);
}

// ENTER STUDENT ID > ENTER ROOM ID > LOBBY > QUIZ/WAIT > END
void loop() {
  if(hoststarted){
    if(quizcomplete){
        finish();
      }
    else if(waiting){
      wait();
      delay(3000);
      waiting = false;
    }
    else{
      quiz();
      quizButton();
    }
  }
  else if(studentfilled && roomfilled){
    lobby();
  }
  else{
    idMenu();
    idButton();
  }
}

void idMenu() {
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();

  if(studentfilled == false){
    display.setCursor(48 - (display.getPrintWidth("Enter Student ID")/2), 0);
    display.print("Enter Student ID");

    char studentIDArray[studentID.length() + 1];
    studentID.toCharArray(studentIDArray, sizeof(studentIDArray));
    display.setCursor(48 - (display.getPrintWidth(studentIDArray) / 2), 30);
    display.print(studentIDArray);

    display.setCursor((48 - (display.getPrintWidth(studentIDArray) / 2) + (display.getPrintWidth("0")*studentCurrentPosition)), 40);
    display.print("^");
  }
  else{
    display.setCursor(48 - (display.getPrintWidth("Enter Room ID")/2), 0);
    display.print("Enter Room ID");

    char roomIDArray[roomID.length() + 1];
    roomID.toCharArray(roomIDArray, sizeof(roomIDArray));
    display.setCursor(48 - (display.getPrintWidth(roomIDArray) / 2), 30);
    display.print(roomIDArray);

    display.setCursor((48 - (display.getPrintWidth(roomIDArray) / 2) + (display.getPrintWidth("0")*roomCurrentPosition)), 40);
    display.print("^");
  }
  delay(200);
}

void idButton() {
  if(studentfilled == false){
    // Up button
    if (display.getButtons(TSButtonUpperLeft)) {
    char currentChar = studentID.charAt(studentCurrentPosition);
      if (currentChar == '_') {
        studentID.setCharAt(studentCurrentPosition, '0');
      } 
      else {
        studentID.setCharAt(studentCurrentPosition, (currentChar == '9') ? '0' : (currentChar + 1));
      }
    delay(100);
    }

    // Down button
    if (display.getButtons(TSButtonLowerLeft)) {
    char currentChar = studentID.charAt(studentCurrentPosition);
      if (currentChar == '_') {
        studentID.setCharAt(studentCurrentPosition, '0');
      } 
      else {
        studentID.setCharAt(studentCurrentPosition, (currentChar == '0') ? '9' : (currentChar - 1));
      }
    delay(100);
    }

    // Enter button
    if (display.getButtons(TSButtonUpperRight)) {
      if (studentCurrentPosition < studentID.length() - 1 && studentID.charAt(studentCurrentPosition) != '_') {
        studentCurrentPosition++;
      }
      else if(studentCurrentPosition >= 5 && studentfilled == false){
        studentfilled = true;
      }
      delay(100);
    }
  }
  else{
    // Up button
    if (display.getButtons(TSButtonUpperLeft)) {
    char currentChar = roomID.charAt(roomCurrentPosition);
      if (currentChar == '_') {
        roomID.setCharAt(roomCurrentPosition, '0');
      } 
      else {
        roomID.setCharAt(roomCurrentPosition, (currentChar == '9') ? '0' : (currentChar + 1));
      }
    delay(100);
    }

    // Down button
    if (display.getButtons(TSButtonLowerLeft)) {
    char currentChar = roomID.charAt(roomCurrentPosition);
      if (currentChar == '_') {
        roomID.setCharAt(roomCurrentPosition, '0');
      } 
      else {
        roomID.setCharAt(roomCurrentPosition, (currentChar == '0') ? '9' : (currentChar - 1));
      }
    delay(100);
    }

    // Enter button
    if (display.getButtons(TSButtonUpperRight)) {
      if (roomCurrentPosition < roomID.length() - 1 && roomID.charAt(roomCurrentPosition) != '_') {
        roomCurrentPosition++;
      }
      else if(roomCurrentPosition >= 2 && roomfilled == false){
        roomfilled = true;
      }
      delay(100);
    }
  }
}

void lobby() {
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();
  display.setCursor(48 - (display.getPrintWidth("Waiting")/2), 30);
  display.print("Waiting");
  delay(3000);
  hoststarted = true;
}

//can be optimized
void quiz() {
  display.clearScreen();
  display.drawRect(0,0,44,28,TSRectangleFilled,TS_8b_Red);
  display.fontColor(TS_8b_White, TS_8b_Red);
  display.setCursor(22 - (display.getPrintWidth("A")/2), 10);
  display.print("A");

  display.drawRect(48,0,44,28,TSRectangleFilled,TS_8b_Blue);
  display.fontColor(TS_8b_White, TS_8b_Blue);
  display.setCursor(70 - (display.getPrintWidth("B")/2), 10);
  display.print("B");

  display.drawRect(0,32,44,28,TSRectangleFilled,TS_8b_Green);
  display.fontColor(TS_8b_White, TS_8b_Green);
  display.setCursor(22 - (display.getPrintWidth("C")/2), 42);
  display.print("C");

  display.drawRect(48,32,44,28,TSRectangleFilled,TS_8b_Yellow);
  display.fontColor(TS_8b_White, TS_8b_Yellow);
  display.setCursor(70 - (display.getPrintWidth("D")/2), 42);
  display.print("D");

  delay(200);
}

//can be optimized
void quizButton(){
  if(display.getButtons(TSButtonUpperLeft)){
    answersArray[answersIndex] = 'A';
    quizButtonPress();
  }
  if(display.getButtons(TSButtonUpperRight)){
    answersArray[answersIndex] = 'B';
    quizButtonPress();
  }
  if(display.getButtons(TSButtonLowerLeft)){
    answersArray[answersIndex] = 'C';
    quizButtonPress();
  }
  if(display.getButtons(TSButtonLowerRight)){
    answersArray[answersIndex] = 'D';
    quizButtonPress();
  }
}

void quizButtonPress(){
  answersIndex++;
  waiting = true;
  delay(200);
  if(answersArray[answersIndex - 1] == questionsArray[answersIndex - 1]){
    score++;
  }
  if(answersIndex >= (sizeof(questionsArray) - 1)){
    quizcomplete = true;
  }
}

void wait(){
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();

  display.setCursor(48 - (display.getPrintWidth("You answered: X")/2), 20);
  String message = "You answered: ";
  message += answersArray[answersIndex - 1];
  display.print(message);

  display.setCursor(48 - (display.getPrintWidth("Correct answer: X")/2), 40);
  String message2 = "Correct answer: ";
  message2 += questionsArray[answersIndex - 1];
  display.print(message2);
}

void finish(){
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();
  display.setCursor(48 - (display.getPrintWidth("QUIZ COMPLETE")/2), 20);
  display.print("QUIZ COMPLETE");
  display.setCursor(48 - (display.getPrintWidth("1/1")/2), 40);
  display.print(String(score) + "/" + String(sizeof(questionsArray) - 1));
  delay(200);
}

