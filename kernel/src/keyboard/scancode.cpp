#include <keyboard/scancode.hpp>

using namespace key;

const KeyMappingEntry key::KeyMappingTable[MappingTableMax] =
{
	/*  0   */{ NONE, NONE },
	/*  1   */{ ESC, ESC },
	/*  2   */{ '1', '!' },
	/*  3   */{ '2', '@' },
	/*  4   */{ '3', '#' },
	/*  5   */{ '4', '$' },
	/*  6   */{ '5', '%' },
	/*  7   */{ '6', '^' },
	/*  8   */{ '7', '&' },
	/*  9   */{ '8', '*' },
	/*  10  */{ '9', '(' },
	/*  11  */{ '0', ')' },
	/*  12  */{ '-', '_' },
	/*  13  */{ '=', '+' },
	/*  14  */{ BACKSPACE, BACKSPACE },
	/*  15  */{ TAB, TAB },
	/*  16  */{ 'q', 'Q' },
	/*  17  */{ 'w', 'W' },
	/*  18  */{ 'e', 'E' },
	/*  19  */{ 'r', 'R' },
	/*  20  */{ 't', 'T' },
	/*  21  */{ 'y', 'Y' },
	/*  22  */{ 'u', 'U' },
	/*  23  */{ 'i', 'I' },
	/*  24  */{ 'o', 'O' },
	/*  25  */{ 'p', 'P' },
	/*  26  */{ '[', '{' },
	/*  27  */{ ']', '}' },
	/*  28  */{ '\n', '\n' },
	/*  29  */{ CTRL, CTRL },
	/*  30  */{ 'a', 'A' },
	/*  31  */{ 's', 'S' },
	/*  32  */{ 'd', 'D' },
	/*  33  */{ 'f', 'F' },
	/*  34  */{ 'g', 'G' },
	/*  35  */{ 'h', 'H' },
	/*  36  */{ 'j', 'J' },
	/*  37  */{ 'k', 'K' },
	/*  38  */{ 'l', 'L' },
	/*  39  */{ ';', ':' },
	/*  40  */{ '\'', '\"' },
	/*  41  */{ '`', '~' },
	/*  42  */{ LSHIFT, LSHIFT },
	/*  43  */{ '\\', '|' },
	/*  44  */{ 'z', 'Z' },
	/*  45  */{ 'x', 'X' },
	/*  46  */{ 'c', 'C' },
	/*  47  */{ 'v', 'V' },
	/*  48  */{ 'b', 'B' },
	/*  49  */{ 'n', 'N' },
	/*  50  */{ 'm', 'M' },
	/*  51  */{ ',', '<' },
	/*  52  */{ '.', '>' },
	/*  53  */{ '/', '?' },
	/*  54  */{ RSHIFT, RSHIFT },
	/*  55  */{ '*', '*' },
	/*  56  */{ LALT, LALT },
	/*  57  */{ ' ', ' ' },
	/*  58  */{ CAPSLOCK, CAPSLOCK },
	/*  59  */{ F1, F1 },
	/*  60  */{ F2, F2 },
	/*  61  */{ F3, F3 },
	/*  62  */{ F4, F4 },
	/*  63  */{ F5, F5 },
	/*  64  */{ F6, F6 },
	/*  65  */{ F7, F7 },
	/*  66  */{ F8, F8 },
	/*  67  */{ F9, F9 },
	/*  68  */{ F10, F10 },
	/*  69  */{ NUMLOCK, NUMLOCK },
	/*  70  */{ SCROLLLOCK, SCROLLLOCK },

	/*  71  */{ HOME, '7' },
	/*  72  */{ UP, '8' },
	/*  73  */{ PAGEUP, '9' },
	/*  74  */{ '-', '-' },
	/*  75  */{ LEFT, '4' },
	/*  76  */{ CENTER, '5' },
	/*  77  */{ RIGHT, '6' },
	/*  78  */{ '+', '+' },
	/*  79  */{ END, '1' },
	/*  80  */{ DOWN, '2' },
	/*  81  */{ PAGEDOWN, '3' },
	/*  82  */{ INS, '0' },
	/*  83  */{ DEL, '.' },
	/*  84  */{ NONE, NONE },
	/*  85  */{ NONE, NONE },
	/*  86  */{ NONE, NONE },
	/*  87  */{ F11, F11 },
	/*  88  */{ F12, F12 }
};