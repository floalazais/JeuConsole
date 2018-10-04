typedef enum InputKey
{
	INPUT_KEY_ESCAPE,
	INPUT_KEY_UP_ARROW,
	INPUT_KEY_DOWN_ARROW,
	INPUT_KEY_LEFT_ARROW,
	INPUT_KEY_RIGHT_ARROW,
	INPUT_KEY_D,
	INPUT_KEY_Q,
	INPUT_KEY_R,
	INPUT_KEY_S,
	INPUT_KEY_Z,
	INPUT_KEY_COUNT
} InputKey;

bool is_input_key_up(InputKey inputKey);
bool is_input_key_down(InputKey inputKey);
bool is_input_key_pressed(InputKey inputKey);
bool is_input_key_released(InputKey inputKey);

extern bool inputKeysBefore[INPUT_KEY_COUNT];
extern bool inputKeysNow[INPUT_KEY_COUNT];