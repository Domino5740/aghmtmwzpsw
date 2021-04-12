enum KeyboardState {
	RELEASED,
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3
};
enum KeyboardState eReadButtons(void);
enum KeyboardState eKeyboardRead(void);
void KeyboardInit(void);
