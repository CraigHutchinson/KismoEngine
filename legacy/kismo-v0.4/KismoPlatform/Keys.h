/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoPlatform\Keys.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef KISMOPLATFORM_KEYS_H
#define KISMOPLATFORM_KEYS_H

namespace kismo { namespace platform
{
	enum eKey 
	{
		/* From SDL: The keyboard syms have been cleverly chosen to map to ASCII */
		Key_UNKNOWN		= 0,
		Key_FIRST		= 0,
		Key_BACKSPACE		= 8,
		Key_TAB		= 9,
		Key_CLEAR		= 12,
		Key_RETURN		= 13,
		Key_Pause		= 19,
		Key_Escape		= 27,
		Key_SPACE		= 32,
		Key_EXCLAIM		= 33,
		Key_QUOTEDBL		= 34,
		Key_HASH		= 35,
		Key_DOLLAR		= 36,
		Key_AMPERSAND		= 38,
		Key_QUOTE		= 39,
		Key_LEFTPAREN		= 40,
		Key_RIGHTPAREN		= 41,
		Key_ASTERISK		= 42,
		Key_PLUS		= 43,
		Key_COMMA		= 44,
		Key_MINUS		= 45,
		Key_PERIOD		= 46,
		Key_SLASH		= 47,
		Key_0			= 48,
		Key_1			= 49,
		Key_2			= 50,
		Key_3			= 51,
		Key_4			= 52,
		Key_5			= 53,
		Key_6			= 54,
		Key_7			= 55,
		Key_8			= 56,
		Key_9			= 57,
		Key_COLON		= 58,
		Key_SEMICOLON	= 59,
		Key_LESS		= 60,
		Key_EQUALS		= 61,
		Key_GREATER		= 62,
		Key_QUESTION		= 63,
		Key_AT			= 64,
		/* 
		   Skip uppercase letters
		 */
		Key_LEFTBRACKET	= 91,
		Key_BACKSLASH		= 92,
		Key_RIGHTBRACKET	= 93,
		Key_CARET		= 94,
		Key_UNDERSCORE		= 95,
		Key_BACKQUOTE		= 96,
		Key_a			= 97,
		Key_b			= 98,
		Key_c			= 99,
		Key_d			= 100,
		Key_e			= 101,
		Key_f			= 102,
		Key_g			= 103,
		Key_h			= 104,
		Key_i			= 105,
		Key_j			= 106,
		Key_k			= 107,
		Key_l			= 108,
		Key_m			= 109,
		Key_n			= 110,
		Key_o			= 111,
		Key_p			= 112,
		Key_q			= 113,
		Key_r			= 114,
		Key_s			= 115,
		Key_t			= 116,
		Key_u			= 117,
		Key_v			= 118,
		Key_w			= 119,
		Key_x			= 120,
		Key_y			= 121,
		Key_z			= 122,
		Key_DELETE		= 127,
		/* End of ASCII mapped keysyms */

		/* International keyboard syms */
		Key_World_0		= 160,		/* 0xA0 */
		Key_World_1		= 161,
		Key_World_2		= 162,
		Key_World_3		= 163,
		Key_World_4		= 164,
		Key_World_5		= 165,
		Key_World_6		= 166,
		Key_World_7		= 167,
		Key_World_8		= 168,
		Key_World_9		= 169,
		Key_World_10		= 170,
		Key_World_11		= 171,
		Key_World_12		= 172,
		Key_World_13		= 173,
		Key_World_14		= 174,
		Key_World_15		= 175,
		Key_World_16		= 176,
		Key_World_17		= 177,
		Key_World_18		= 178,
		Key_World_19		= 179,
		Key_World_20		= 180,
		Key_World_21		= 181,
		Key_World_22		= 182,
		Key_World_23		= 183,
		Key_World_24		= 184,
		Key_World_25		= 185,
		Key_World_26		= 186,
		Key_World_27		= 187,
		Key_World_28		= 188,
		Key_World_29		= 189,
		Key_World_30		= 190,
		Key_World_31		= 191,
		Key_World_32		= 192,
		Key_World_33		= 193,
		Key_World_34		= 194,
		Key_World_35		= 195,
		Key_World_36		= 196,
		Key_World_37		= 197,
		Key_World_38		= 198,
		Key_World_39		= 199,
		Key_World_40		= 200,
		Key_World_41		= 201,
		Key_World_42		= 202,
		Key_World_43		= 203,
		Key_World_44		= 204,
		Key_World_45		= 205,
		Key_World_46		= 206,
		Key_World_47		= 207,
		Key_World_48		= 208,
		Key_World_49		= 209,
		Key_World_50		= 210,
		Key_World_51		= 211,
		Key_World_52		= 212,
		Key_World_53		= 213,
		Key_World_54		= 214,
		Key_World_55		= 215,
		Key_World_56		= 216,
		Key_World_57		= 217,
		Key_World_58		= 218,
		Key_World_59		= 219,
		Key_World_60		= 220,
		Key_World_61		= 221,
		Key_World_62		= 222,
		Key_World_63		= 223,
		Key_World_64		= 224,
		Key_World_65		= 225,
		Key_World_66		= 226,
		Key_World_67		= 227,
		Key_World_68		= 228,
		Key_World_69		= 229,
		Key_World_70		= 230,
		Key_World_71		= 231,
		Key_World_72		= 232,
		Key_World_73		= 233,
		Key_World_74		= 234,
		Key_World_75		= 235,
		Key_World_76		= 236,
		Key_World_77		= 237,
		Key_World_78		= 238,
		Key_World_79		= 239,
		Key_World_80		= 240,
		Key_World_81		= 241,
		Key_World_82		= 242,
		Key_World_83		= 243,
		Key_World_84		= 244,
		Key_World_85		= 245,
		Key_World_86		= 246,
		Key_World_87		= 247,
		Key_World_88		= 248,
		Key_World_89		= 249,
		Key_World_90		= 250,
		Key_World_91		= 251,
		Key_World_92		= 252,
		Key_World_93		= 253,
		Key_World_94		= 254,
		Key_World_95		= 255,		/* 0xFF */

		/* Numeric keypad */
		Key_Pad0		= 256,
		Key_Pad1		= 257,
		Key_Pad2		= 258,
		Key_Pad3		= 259,
		Key_Pad4		= 260,
		Key_Pad5		= 261,
		Key_Pad6		= 262,
		Key_Pad7		= 263,
		Key_Pad8		= 264,
		Key_Pad9		= 265,
		Key_Pad_Period		= 266,
		Key_Pad_Divide		= 267,
		Key_Pad_Multiply	= 268,
		Key_Pad_Subtract	= 269,
		Key_Pad_Add			= 270,
		Key_Pad_Enter		= 271,
		Key_Pad_Equals		= 272,

		/* Arrows + Home/End pad */
		Key_Up			= 273,
		Key_Down		= 274,
		Key_Right		= 275,
		Key_Left		= 276,
		Key_Insert		= 277,
		Key_Home		= 278,
		Key_End		= 279,
		Key_PageUp		= 280,
		Key_PageDown		= 281,

		/* Function keys */
		Key_F1			= 282,
		Key_F2			= 283,
		Key_F3			= 284,
		Key_F4			= 285,
		Key_F5			= 286,
		Key_F6			= 287,
		Key_F7			= 288,
		Key_F8			= 289,
		Key_F9			= 290,
		Key_F10		= 291,
		Key_F11		= 292,
		Key_F12		= 293,
		Key_F13		= 294,
		Key_F14		= 295,
		Key_F15		= 296,

		/* Key state modifier keys */
		Key_NumLock		= 300,
		Key_CapsLock		= 301,
		Key_ScrollLock		= 302,
		Key_RightShift		= 303,
		Key_LeftShift		= 304,
		Key_RightCtrl		= 305,
		Key_LeftCtrl		= 306,
		Key_RightAlt		= 307,
		Key_LeftAlt		= 308,
		Key_RMETA		= 309,
		Key_LMETA		= 310,
		Key_LeftSystem		= 311,		/* Left "Windows" key */
		Key_RightSystem		= 312,		/* Right "Windows" key */
		Key_MODE		= 313,		/* "Alt Gr" key */
		Key_COMPOSE		= 314,		/* Multi-key compose key */

		/* Miscellaneous function keys */
		Key_Help		= 315,
		Key_Print		= 316,
		Key_SYSREQ		= 317,
		Key_Break		= 318,
		Key_Menu		= 319,
		Key_POWER		= 320,		/* Power Macintosh power key */
		Key_EURO		= 321,		/* Some european keyboards */
		Key_UNDO		= 322,		/* Atari keyboard has Undo */

		/* Add any other keys here */

		_Key_Count
	};

	enum eKeyModifier 
	{
		KeyModifier_None  = 0,
		KeyModifier_LeftShift= 1<<0,
		KeyModifier_RightShift= 1<<1,
		KeyModifier_LeftCtrl = 1<<2,
		KeyModifier_RightCtrl = 1<<3,
		KeyModifier_LeftAlt  = 1<<4,
		KeyModifier_RightAlt  = 1<<5,
		KeyModifier_LeftMeta = 1<<6, //, Window key?
		KeyModifier_RightMeta = 1<<7,//, Window key?
		KeyModifier_NumLock   = 1<<8,
		KeyModifier_Caps  = 1<<9,
		KeyModifier_Mode  = 1<<10, //,?

		_KeyModifier_Count = 11,

		KeyModifier_Shift = KeyModifier_LeftShift|KeyModifier_RightShift,
		KeyModifier_Ctrl = KeyModifier_LeftCtrl|KeyModifier_RightCtrl,
		KeyModifier_Alt = KeyModifier_LeftAlt|KeyModifier_RightAlt,
		KeyModifier_Meta = KeyModifier_LeftMeta|KeyModifier_RightMeta,
	};

	enum eButton 
	{
		Button_Left		= 1<<0,
		Button_Middle	= 1<<1,
		Button_Right	= 1<<2,
		Button_X1		= 1<<3,
		Button_X2		= 1<<4,
	};

} /* END: Platform*/ } /*END: Kismo*/

#endif