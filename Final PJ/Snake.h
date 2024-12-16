#ifndef SNAKE_H
#define SNAKE_H

#include <matrix.h>
#include <INTRINS.h>
#include <REG51.h>


/*
Note: + Since the game doesnt actually use 2d array to display Snake game but 1d instead, There is no need to resituate the head and bottom edge
			+ The snake cannot go down when go up, right when go left and so on
*/

// Define button pins
sbit UE = P3^2;    // Button for UP
sbit SHITA = P3^3;  // Button for DOWN
sbit HIDARI = P3^0;  // Button for LEFT
sbit MIGI = P3^1; // Button for RIGHT

unsigned char snake[64]; // 8x8 Array in 1
unsigned char food;
unsigned char direction;
unsigned char snake_length;
unsigned char display_buffer[8];
unsigned char last_direction;// Keep track of last direction
unsigned int Speed = 20; // The higher the slower

void init_game()
{
	unsigned char i;
  // Clear snake array
  for (i = 0; i < 64; i++)
	{
		snake[i] = 0;
  }
  // Initialize starting position
  snake[0] = 28;  // Head
  snake[1] = 27;  // Tail
  food = 10;      // Initial food position
  direction = 3;  // Start moving right
  last_direction = 3;
  snake_length = 2;
}

void check_direction()
{
	// Store previous direction
	unsigned char new_direction = last_direction;
	
	// Check old direction to set new direction
	if (UE == 0 && last_direction != 1){new_direction = 0;}
	if (SHITA == 0 && last_direction != 0){new_direction = 1;}
	if (HIDARI == 0 && last_direction != 3){new_direction = 2;}
	if (MIGI == 0 && last_direction != 2){new_direction = 3; }
	
	// Update direction iff it changed
	if (new_direction != last_direction)
	{
			direction = new_direction;
			last_direction = new_direction;
	}
}

void update_snake()
{
  unsigned char i;
  // Move body
  for (i = snake_length; i > 0; i--)
	{
		snake[i] = snake[i-1];
  }
    
	// Move head based on direction
  switch(direction)
	{
		case 0: // Up
			snake[0] = (snake[0] - 8 + 64) % 64;
      break;
    case 1: // Down
      snake[0] = (snake[0] + 8) % 64;
      break;
    case 2: // Left
      if (snake[0] % 8 == 0) // If at left edge
				snake[0] += 7;     // Wrap to right edge
      else
        snake[0] -= 1;     // Move left
        break;
    case 3: // Right
      if (snake[0] % 8 == 7) // If at right edge
				snake[0] -= 7;     // Wrap to left edge
      else
				snake[0] += 1;     // Move right
        break;
	}
}

void generate_new_food()
{
	unsigned char i;
	unsigned char is_valid;
    
  do
	{
		is_valid = 1;
		food = (food + 17) % 64;
		
		for (i = 0; i < snake_length; i++)
		{
				if (food == snake[i]) {is_valid = 0;break;}
		}
	}
	while (is_valid == 0);
}

void check_collision() {
	unsigned char i;
	// Check for self collision
	for (i = 1; i < snake_length; i++)
	{
		if (snake[0] == snake[i])
		{
			init_game();  // Reset game on collision
			return;
		}
	}
	
	// Check for food collision
	if (snake[0] == food)
	{
		if(snake_length < 63)
		{  // Prevent overflow
			snake_length++;
		}
		generate_new_food();
	}
}

void update_display_buffer()
{
	unsigned char i, row, col;
	// Clear display buffer
	for (i = 0; i < 8; i++)
	{
		display_buffer[i] = 0x00;
	}
	
	// Draw snake
	for (i = 0; i < snake_length; i++)
	{
		row = snake[i] / 8;
		col = snake[i] % 8;
		display_buffer[row] |= (1 << col);
	}
	
	// Draw food
	row = food / 8;
	col = food % 8;
	display_buffer[row] |= (1 << col);
}


void Running_Game()
{
	check_direction();
	update_snake();
	check_collision();
	update_display_buffer();
}
#endif