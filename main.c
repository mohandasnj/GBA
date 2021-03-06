#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/bad.h"
#include "images/welcome.h"
#include "images/good.h"
#include "images/blue.h"
#include "images/win.h"
#include "images/lose.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
    START,
    PLAY,
    WIN,
    LOSE,
  };
  
struct Player player = {76, 15, 10, 10, 0};

int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB) {
  return (colA < colB + widthB) && (colA + widthA > colB) && (rowA < rowB + heightB) && (heightA + rowA > rowB);
}

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;
  int score = 5;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    switch (state) {
      case START:
        waitForVBlank();
        drawFullScreenImageDMA(welcome);
        drawString(100, 100, "Press ENTER to play", GREEN);
        if(KEY_DOWN(BUTTON_START, currentButtons)) {
          state = PLAY;
          drawRectDMA(0, 0, 240, 160, BLACK);
        }
        break;
      case PLAY:
        waitForVBlank();
        drawString(100, 100, "", BLACK);
        drawRectDMA(player.row - 1, player.col, player.width, player.height, BLACK);
        drawRectDMA(player.row - 1, player.col + 1, player.width, player.height, BLACK);
        drawRectDMA(player.row + 1, player.col, player.width, player.height, BLACK);
        drawRectDMA(player.row + 1, player.col - 1, player.width, player.height, BLACK);
        drawRectDMA(player.row, player.col - 1, player.width, player.height, BLACK);
        drawRectDMA(player.row - 1, player.col - 1, player.width, player.height, BLACK);
        drawRectDMA(player.row, player.col + 1, player.width, player.height, BLACK);
        drawRectDMA(player.row + 1, player.col + 1, player.width, player.height, BLACK);
        drawImageDMA(player.row, player.col, player.width, player.height, blue);
        drawString(5, 2, "Score: ", WHITE);
        drawChar(5, 40, score + 48, WHITE);
        drawString(150, 2, "Press DELETE to quit game", RED);
        drawImageDMA(12, 170, 70, 69, good);
        drawImageDMA(81, 170, 70, 69, bad);

        if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) {
          if (player.row < 140) {
            player.row++;
          }
        }
        if (KEY_DOWN(BUTTON_UP, BUTTONS)) {
          if (player.row > 12) {
            player.row--;
          }
        }
        if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) {
          if (player.col > 0) {
            player.col--;
          }
        }
        if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) {
          if (player.col < 230) {
            player.col++;
          }
        }

        if (KEY_DOWN(BUTTON_SELECT, BUTTONS)) {
          state = START;
          player.row = 76;
          player.col = 15;
          score = 5;
        }

        if (collision(player.row, player.col, player.width, player.height, 12, 170, 70, 69)) {
          score++;
          drawRectDMA(player.row, player.col, player.width, player.height, BLACK);
          drawRectDMA(player.row - 1, player.col, player.width, player.height, BLACK);
          drawRectDMA(player.row - 1, player.col + 1, player.width, player.height, BLACK);
          drawRectDMA(player.row + 1, player.col, player.width, player.height, BLACK);
          drawRectDMA(player.row + 1, player.col - 1, player.width, player.height, BLACK);
          drawRectDMA(player.row, player.col - 1, player.width, player.height, BLACK);
          drawRectDMA(player.row - 1, player.col - 1, player.width, player.height, BLACK);
          drawRectDMA(player.row, player.col + 1, player.width, player.height, BLACK);
          drawRectDMA(player.row + 1, player.col + 1, player.width, player.height, BLACK);
          drawRectDMA(5, 40, 10, 10, BLACK);
          drawChar(5, 40, score + 48, WHITE);
          player.row = 82;
          player.col = 15;
        }

        if (collision(player.row, player.col, player.width, player.height, 81, 170, 70, 69)) {
          score--;
          drawRectDMA(player.row, player.col, player.width, player.height, BLACK);
          drawRectDMA(player.row - 1, player.col, player.width, player.height, BLACK);
          drawRectDMA(player.row - 1, player.col + 1, player.width, player.height, BLACK);
          drawRectDMA(player.row + 1, player.col, player.width, player.height, BLACK);
          drawRectDMA(player.row + 1, player.col - 1, player.width, player.height, BLACK);
          drawRectDMA(player.row, player.col - 1, player.width, player.height, BLACK);
          drawRectDMA(player.row - 1, player.col - 1, player.width, player.height, BLACK);
          drawRectDMA(player.row, player.col + 1, player.width, player.height, BLACK);
          drawRectDMA(player.row + 1, player.col + 1, player.width, player.height, BLACK);
          drawRectDMA(5, 40, 10, 10, BLACK);  
          drawChar(5, 40, score + 48, WHITE);
          player.row = 70;
          player.col = 15;
          
        }

        if (score == 10) {
          state = WIN;
        }

        if (score == 0) {
          state = LOSE;
        }
        break;
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(win);
        drawString(150, 60, "Press ENTER to play again", GREEN);
        if (KEY_DOWN(BUTTON_START, BUTTONS)) {
          state = PLAY;
          drawRectDMA(0, 0, 240, 160, BLACK);
          player.row = 76;
          player.col = 15;
          score = 5;
        }
        break;
      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(lose);
        drawString(150, 60, "Press ENTER to play again", GREEN);
        if (KEY_DOWN(BUTTON_START, BUTTONS)) {
          state = PLAY;
          drawRectDMA(0, 0, 240, 160, BLACK);
          player.row = 76;
          player.col = 15;
          score = 5;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
