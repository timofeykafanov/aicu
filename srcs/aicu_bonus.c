#include <stdio.h>

#include "../includes/aicu.h"
#include "../raylib/include/raylib.h"

#define STICK_WIDTH 5
#define STICK_HEIGHT 25
#define SPACE_BETWEEN 10
#define FONT_SIZE 20
#define BUTTON_WIDTH 64

int calculate_horizontal_line_fit(RenderTexture2D texture)
{
	int screen_width = texture.texture.width;
	int num_sticks_fit = screen_width / (STICK_WIDTH + SPACE_BETWEEN) - 3;

	return (num_sticks_fit);
}

void draw_heap(RenderTexture2D texture, int y, int heap, Color color) {

	int screen_width = texture.texture.width;
	int screen_middle = screen_width / 2;
	int num_sticks_fit = calculate_horizontal_line_fit(texture);
	if (num_sticks_fit > heap)
		num_sticks_fit = heap;
	int visible_sticks_width = num_sticks_fit * (STICK_WIDTH + SPACE_BETWEEN);
	int beginning = screen_middle - visible_sticks_width / 2;

	for (int i = 0; i < num_sticks_fit; i++)
	{
        DrawRectangle(beginning + i * (STICK_WIDTH + SPACE_BETWEEN), y, STICK_WIDTH, STICK_HEIGHT, color);
	}

	if (heap > num_sticks_fit)
	{
		const char *text = TextFormat("%d more", heap - num_sticks_fit);
		int text_width = MeasureText(text, FONT_SIZE);

		int text_x = screen_width - text_width - SPACE_BETWEEN;

        DrawRectangle(text_x - 5, y, text_width + 10, FONT_SIZE + 5, LIGHTGRAY);

	    DrawText(text, screen_width - text_width - SPACE_BETWEEN, y + 3, FONT_SIZE, DARKGRAY);
	}
}

int calculate_vertical_line_fit(RenderTexture2D texture)
{
	return (texture.texture.height / (STICK_HEIGHT + SPACE_BETWEEN));
}

void draw_board(RenderTexture2D texture, t_board *board, Color color)
{
	int num_sticks_fit = calculate_vertical_line_fit(texture);
	if (num_sticks_fit > board->num_heaps)
		num_sticks_fit = board->num_heaps;

	int y = 0;
	for (int heap_i = board->num_heaps - num_sticks_fit; heap_i < board->num_heaps; heap_i++)
	{
		draw_heap(texture, (y * (STICK_HEIGHT + 10)), board->heaps[heap_i], color);
		y += 1;
	}
}

typedef enum e_button_status
{
	NONE,
	HOVER,
	PRESSING,
	CLICKED,
	DISABLED,
}	t_button_status;

typedef struct s_button
{
	Rectangle		rect;
	int				border_width;
	char			*text;
	t_button_status	status;
	Color			primary_color;
	Color			secondary_color;
}	t_button;

t_button create_button(int x, int y, int width, int height, int border_width, char *text, Color primary_color, Color secondary_color)
{
	t_button b = {
		.rect = (Rectangle) { x, y, width, height },
		.border_width = border_width,
		.text = text,
		.status = NONE,
		.primary_color = primary_color,
		.secondary_color = secondary_color,
	};

	return (b);
}

void update_button(t_button *button)
{
	Vector2 mouse_point = GetMousePosition();

	if (button->status == DISABLED)
		return ;

	if (CheckCollisionPointRec(mouse_point, button->rect))
	{
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			button->status = PRESSING;
		else
			button->status = HOVER;

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			button->status = CLICKED;

		return ;
	}

	button->status = NONE;
}

void draw_button(t_button *button)
{
	const int BUTTON_FONT_SIZE = 30;

	Color primary_color = button->status == NONE ? button->primary_color : button->secondary_color;
	Color secondary_color = button->status == NONE ? button->secondary_color : button->primary_color;

	if (button->status == DISABLED)
	{
		primary_color = GRAY;
		secondary_color = LIGHTGRAY;
	}

	DrawRectangleRec(button->rect, secondary_color);
	DrawRectangleLinesEx(button->rect, button->border_width, primary_color);

    int text_width = MeasureText(button->text, BUTTON_FONT_SIZE);
    DrawText(button->text, (int) button->rect.x + 4 + (BUTTON_WIDTH - 8) / 2 - text_width / 2, (int) button->rect.y + 16, BUTTON_FONT_SIZE, primary_color);
}

void process_input(t_board *board, bool *player_turn, t_button *button1, t_button *button2, t_button *button3)
{
	if (board->num_heaps == 0)
		return ;

	int move = 0;

	if (IsKeyReleased(KEY_ONE) || button1->status == CLICKED)
		move = 1;
	else if (IsKeyReleased(KEY_TWO) || button2->status == CLICKED)
		move = 2;
	else if (IsKeyReleased(KEY_THREE) || button3->status == CLICKED)
		move = 3;

	if (board->heaps[board->num_heaps - 1] < move)
		return ;

	if (move >= 1 && move <= 3)
	{
		update_board(board, move);
		*player_turn = false;
	}
}

void draw_endscreen(bool player_turn)
{
	const int ES_FONT_SIZE = 40;

	bool player_lost = !player_turn;

	char *text = player_lost ? "You Lost!" : "You Won!";
	int text_width = MeasureText(text, ES_FONT_SIZE);

	int text_x = GetScreenWidth() / 2 - text_width / 2;
	int text_y = GetScreenHeight() / 2 - ES_FONT_SIZE / 2;

    DrawRectangle(text_x - 5, text_y - 5, text_width + 10, ES_FONT_SIZE + 7, player_lost ? MAROON : LIME);

    DrawText(text, text_x, text_y, ES_FONT_SIZE, WHITE);
}

typedef struct s_game
{
	bool	player_turn;
	int		total_heaps;
	bool	finished;
}	t_game;

int main(int argc, char **argv)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 534;

    const int BOARD_WIDTH = 800;
    const int BOARD_HEIGHT = 420;

	int	*strategy = NULL;
	t_board *board;
	int move = 0;

	t_err err = prepare_game(argc, argv, &board, &strategy);
	if (err != SUCCESS)
		return (free(strategy), err);

	t_game game;
	game.player_turn = false;
	game.total_heaps = board->num_heaps;
	game.finished = false;

	SetTraceLogLevel(LOG_NONE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AlCu");
	RenderTexture2D texture_board = LoadRenderTexture(BOARD_WIDTH, BOARD_HEIGHT);

    SetTargetFPS(60);

    t_button button1 = create_button(SCREEN_WIDTH / 2 - (BUTTON_WIDTH * 3 + SPACE_BETWEEN * 2) / 2, BOARD_HEIGHT + FONT_SIZE + SPACE_BETWEEN * 2, BUTTON_WIDTH, BUTTON_WIDTH, 5, "1", BLUE, SKYBLUE);
    t_button button2 = create_button(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, BOARD_HEIGHT + FONT_SIZE + SPACE_BETWEEN * 2, BUTTON_WIDTH, BUTTON_WIDTH, 5, "2", BLUE, SKYBLUE);
    t_button button3 = create_button(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2 + SPACE_BETWEEN , BOARD_HEIGHT + FONT_SIZE + SPACE_BETWEEN * 2, BUTTON_WIDTH, BUTTON_WIDTH, 5, "3", BLUE, SKYBLUE);

    while (!WindowShouldClose())
    {
    	if (game.finished)
    	{
    		BeginDrawing();

    			ClearBackground(RAYWHITE);

    			draw_endscreen(game.player_turn);

    		EndDrawing();

    		continue ;
    	}

    	BeginTextureMode(texture_board);

            ClearBackground(RAYWHITE);
            draw_board(texture_board, board, game.player_turn ? ORANGE : GRAY);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            Rectangle sourceRec = { 0.0f, 0.0f, texture_board.texture.width, -texture_board.texture.height };
	        Vector2 position = { 0.0f, FONT_SIZE + SPACE_BETWEEN };
	        DrawTextureRec(texture_board.texture, sourceRec, position, WHITE);

	        const char *text = TextFormat("%d / %d Lines Cleared\nAI Took: %d", game.total_heaps - board->num_heaps, game.total_heaps, move);
	        DrawText(text, SPACE_BETWEEN, SPACE_BETWEEN, FONT_SIZE / 2, GRAY);


			draw_button(&button1);
			draw_button(&button2);
			draw_button(&button3);

        EndDrawing();

    	if (game.player_turn)
    	{
	    	process_input(board, &game.player_turn, &button1, &button2, &button3);
    	}
	    else
	    {
    		usleep(500000);
	    	int _ = 0;
	    	move = get_ai_move(board, &_, strategy);
	    	update_board(board, move);
	    	game.player_turn = true;
	    }

	    if (board->num_heaps == 0)
	    {
			game.finished = true;
	    }

        int heap = board->heaps[board->num_heaps > 0 ? board->num_heaps - 1 : 0];

		if (heap < 3 || !game.player_turn)
        	button3.status = DISABLED;
		else
        	button3.status = NONE;
		if (heap < 2 || !game.player_turn)
        	button2.status = DISABLED;
		else
        	button2.status = NONE;
		if (heap < 1 || !game.player_turn)
        	button1.status = DISABLED;
		else
        	button1.status = NONE;

	    update_button(&button1);
	    update_button(&button2);
	    update_button(&button3);
    }

    UnloadRenderTexture(texture_board);
    CloseWindow();

    return 0;
}
