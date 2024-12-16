#include <matrix.h>
#include <INTRINS.h>
#include <REG51.h>
#include <Snake.h>

#define COMMONPORTS P0
void main()
{
	unsigned char tab;
	unsigned char i;

	init_game();

	while(1)
	{
		Running_Game();
		// Refresh display
		for(i = 0; i < Speed; i++)
		{
			for(tab = 0; tab < 8; tab++)
			{
				Hc595SendByte(0x00);
				COMMONPORTS = Cols[tab];
				Hc595SendByte(display_buffer[tab]);
				delay(2);
			}
		}
	}
}

void showGameOverScreen() {
    matrix_clear();
    matrix_display_text("GAME", 0);    // Show "GAME" on the top row
    delay(1000);
    matrix_clear();
    matrix_display_text("OVER", 0);    // Show "OVER" on the second row
    delay(1000);
    matrix_clear();
    // Player score
    char scoreText[16];
    snprintf(scoreText, sizeof(scoreText), "Score:%d", getScore()); 
    matrix_display_text(scoreText, 0);  // getscore to retrive playerscore
    delay(2000);
    matrix_clear();
    // Option screen
    matrix_display_text("R:Restart", 0);
    delay(1000);
    matrix_display_text("Q:Quit", 0);
    
    char choice;
    do {
        choice = getchar();
        if (choice == 'R' || choice == 'r') {
            resetGame();  // game reboot
            break;
        } else if (choice == 'Q' || choice == 'q') {
            exit(0);
        }
    } while (choice != 'R' && choice != 'r' && choice != 'Q' && choice != 'q');

    matrix_clear();
}