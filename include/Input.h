#ifndef INPUT_H
#define INPUT_H

typedef struct KeyboardState
{
	const uint8_t *keys;
} KeyboardState;

bool key_held(int code, KeyboardState *state)
{
	return state->keys[code] == true;
}

bool key_not_held(int code, KeyboardState *state)
{
	return state->keys[code] == false;
}

bool key_down(int code, KeyboardState *current, KeyboardState *last)
{
	return key_held(code, current) && key_not_held(code, last);
}

bool key_up(int code, KeyboardState *current, KeyboardState *last)
{
	return key_not_held(code, current) && key_held(code, last);
}

void get_key_state(KeyboardState *state)
{
	state->keys = SDL_GetKeyboardState(NULL);
}

#endif // INPUT_H