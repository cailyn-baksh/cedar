#ifndef _LIBCEDAR_DEFS_H_
#define _LIBCEDAR_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <keypadc.h>

struct CedarWindow;
typedef struct CedarWindow CedarWindow;

struct CedarWidget;
typedef struct CedarWidget CedarWidget;

struct CedarMenu;
typedef struct CedarMenu CedarMenu;

struct CedarMenuItem;
typedef struct CedarMenuItem CedarMenuItem;

struct CedarPoint;
typedef struct CedarPoint CedarPoint;

struct CedarRect;
typedef struct CedarRect CedarRect;

struct CedarEventHandler;
typedef struct CedarEventHandler CedarEventHandler;

typedef uint24_t CALLBACKRESULT;
typedef uint24_t EVENT;
typedef uint24_t ID;

typedef CALLBACKRESULT CedarEventHandlerCallback(void *self, EVENT event, uint24_t param);

// events 0x000000-0x000FFF reserved for built-in events
// 0x000000-0x0007FF used for base events
// 0x000800-0x000FFF used for built-in widget events
#define EVENT_CREATE				((EVENT)0x000001)
#define EVENT_DESTROY				((EVENT)0x000002)
#define EVENT_PAINT					((EVENT)0x000003)
#define EVENT_FOCUS					((EVENT)0x000004)
#define EVENT_BLUR					((EVENT)0x000005)
#define EVENT_KEYDOWN				((EVENT)0x000006)
#define EVENT_KEYUP					((EVENT)0x000007)
#define EVENT_MENUSELECT			((EVENT)0x000008)
#define EVENT_HOTKEY				((EVENT)0x000009)		/* DEPRECATED */
#define EVENT_TICK					((EVENT)0x00000A)
#define EVENT_VSCROLL				((EVENT)0x00000B)
#define EVENT_HSCROLL				((EVENT)0x00000C)
#define EVENT_BUTTONPRESS			((EVENT)0x000801)

#define CALLBACK_NORMAL					((CALLBACKRESULT)0x000000)
#define CALLBACK_EXIT					((CALLBACKRESULT)0x000001)
#define CALLBACK_DO_NOT_PROPAGATE		((CALLBACKRESULT)0x000002)

#define ID_ANONYMOUS			((ID)0)

// keys
#define CEDAR_2NDFLAG		(1 << 16)
#define CEDAR_ALPHAFLAG		(1 << 17)

#define CEDAR_KB_GRAPH		((uint24_t)kb_KeyGraph)
#define CEDAR_KB_TRACE		((uint24_t)kb_KeyTrace)
#define CEDAR_KB_ZOOM		((uint24_t)kb_KeyZoom)
#define CEDAR_KB_WINDOW		((uint24_t)kb_KeyWindow)
#define CEDAR_KB_YEQU		((uint24_t)kb_KeyYequ)
#define CEDAR_KB_2ND		((uint24_t)kb_Key2nd)
#define CEDAR_KB_MODE		((uint24_t)kb_KeyMode)
#define CEDAR_KB_DEL		((uint24_t)kb_KeyDel)
#define CEDAR_KB_STO		((uint24_t)kb_KeySto)
#define CEDAR_KB_LN			((uint24_t)kb_KeyLn)
#define CEDAR_KB_LOG		((uint24_t)kb_KeyLog)
#define CEDAR_KB_SQUARE		((uint24_t)kb_KeySquare)
#define CEDAR_KB_RECIP		((uint24_t)kb_KeyRecip)
#define CEDAR_KB_MATH		((uint24_t)kb_KeyMath)
#define CEDAR_KB_ALPHA		((uint24_t)kb_KeyAlpha)
#define CEDAR_KB_0			((uint24_t)kb_Key0)
#define CEDAR_KB_1			((uint24_t)kb_Key1)
#define CEDAR_KB_2			((uint24_t)kb_Key2)
#define CEDAR_KB_3			((uint24_t)kb_Key3)
#define CEDAR_KB_4			((uint24_t)kb_Key4)
#define CEDAR_KB_5			((uint24_t)kb_Key5)
#define CEDAR_KB_6			((uint24_t)kb_Key6)
#define CEDAR_KB_7			((uint24_t)kb_Key7)
#define CEDAR_KB_8			((uint24_t)kb_Key8)
#define CEDAR_KB_9			((uint24_t)kb_Key9)
#define CEDAR_KB_COMMA		((uint24_t)kb_KeyComma)
#define CEDAR_KB_SIN		((uint24_t)kb_KeySin)
#define CEDAR_KB_COS		((uint24_t)kb_KeyCos)
#define CEDAR_KB_TAN		((uint24_t)kb_KeyTan)
#define CEDAR_KB_APPS		((uint24_t)kb_KeyApps)
#define CEDAR_KB_GRAPHVAR	((uint24_t)kb_KeyGraphVar)
#define CEDAR_KB_DECPNT		((uint24_t)kb_KeyDecPnt)
#define CEDAR_KB_DOT		((uint24_t)kb_KeyDecPnt)  // i think this name makes more sense
#define CEDAR_KB_LPAREN		((uint24_t)kb_KeyLParen)
#define CEDAR_KB_RPAREN		((uint24_t)kb_KeyRParen)
#define CEDAR_KB_PRGM		((uint24_t)kb_KeyPrgm)
#define CEDAR_KB_STAT		((uint24_t)kb_KeyStat)
#define CEDAR_KB_CHS		((uint24_t)kb_KeyChs)
#define CEDAR_KB_VARS		((uint24_t)kb_KeyVars)
#define CEDAR_KB_ENTER		((uint24_t)kb_KeyEnter)
#define CEDAR_KB_ADD		((uint24_t)kb_KeyAdd)
#define CEDAR_KB_SUB		((uint24_t)kb_KeySub)
#define CEDAR_KB_MUL		((uint24_t)kb_KeyMul)
#define CEDAR_KB_DIV		((uint24_t)kb_KeyDiv)
#define CEDAR_KB_POWER		((uint24_t)kb_KeyPower)
#define CEDAR_KB_CLEAR		((uint24_t)kb_KeyClear)
#define CEDAR_KB_DOWN		((uint24_t)kb_KeyDown)
#define CEDAR_KB_LEFT		((uint24_t)kb_KeyLeft)
#define CEDAR_KB_RIGHT		((uint24_t)kb_KeyRight)
#define CEDAR_KB_UP			((uint24_t)kb_KeyUp)

#define CEDAR_KB_STATPLOT	((uint24_t)(CEDAR_2NDFLAG | kb_KeyYequ))
#define CEDAR_KB_TBLSET		((uint24_t)(CEDAR_2NDFLAG | kb_KeyWindow))
#define CEDAR_KB_FORMAT		((uint24_t)(CEDAR_2NDFLAG | kb_KeyZoom))
#define CEDAR_KB_CALC		((uint24_t)(CEDAR_2NDFLAG | kb_KeyTrace))
#define CEDAR_KB_TABLE		((uint24_t)(CEDAR_2NDFLAG | kb_KeyGraph))
#define CEDAR_KB_QUIT		((uint24_t)(CEDAR_2NDFLAG | kb_KeyMode))
#define CEDAR_KB_INS		((uint24_t)(CEDAR_2NDFLAG | kb_KeyDel))
#define CEDAR_KB_LINK		((uint24_t)(CEDAR_2NDFLAG | kb_KeyGraphVar))
#define CEDAR_KB_LIST		((uint24_t)(CEDAR_2NDFLAG | kb_KeyStat))
#define CEDAR_KB_TEST		((uint24_t)(CEDAR_2NDFLAG | kb_KeyMath))
#define CEDAR_KB_ANGLE		((uint24_t)(CEDAR_2NDFLAG | kb_KeyApps))
#define CEDAR_KB_DRAW		((uint24_t)(CEDAR_2NDFLAG | kb_KeyPrgm))
#define CEDAR_KB_DISTR		((uint24_t)(CEDAR_2NDFLAG | kb_KeyVars))
#define CEDAR_KB_MATRIX		((uint24_t)(CEDAR_2NDFLAG | kb_KeyRecip))
#define CEDAR_KB_ARCSIN		((uint24_t)(CEDAR_2NDFLAG | kb_KeySin))
#define CEDAR_KB_ARCCOS		((uint24_t)(CEDAR_2NDFLAG | kb_KeyCos))
#define CEDAR_KB_ARCTAN		((uint24_t)(CEDAR_2NDFLAG | kb_KeyTan))
#define CEDAR_KB_PI			((uint24_t)(CEDAR_2NDFLAG | kb_KeyPower))
#define CEDAR_KB_SQRT		((uint24_t)(CEDAR_2NDFLAG | kb_KeySquare))
#define CEDAR_KB_EE			((uint24_t)(CEDAR_2NDFLAG | kb_KeyComma))
#define CEDAR_KB_LCURLY		((uint24_t)(CEDAR_2NDFLAG | kb_KeyLParen))
#define CEDAR_KB_RCURLY		((uint24_t)(CEDAR_2NDFLAG | kb_KeyRParen))
#define CEDAR_KB_EULER		((uint24_t)(CEDAR_2NDFLAG | kb_KeyDiv))
#define CEDAR_KB_POW10		((uint24_t)(CEDAR_2NDFLAG | kb_KeyLog))
#define CEDAR_KB_u			((uint24_t)(CEDAR_2NDFLAG | kb_Key7))  // TODO: figure out what 'u' means
#define CEDAR_KB_v			((uint24_t)(CEDAR_2NDFLAG | kb_Key8))
#define CEDAR_KB_w			((uint24_t)(CEDAR_2NDFLAG | kb_Key9))
#define CEDAR_KB_LSQUARE	((uint24_t)(CEDAR_2NDFLAG | kb_KeyMul))
#define CEDAR_KB_RSQUARE	((uint24_t)(CEDAR_2NDFLAG | kb_KeySub))
#define CEDAR_KB_POWE		((uint24_t)(CEDAR_2NDFLAG | kb_KeyLn))
#define CEDAR_KB_L1			((uint24_t)(CEDAR_2NDFLAG | kb_Key1))
#define CEDAR_KB_L2			((uint24_t)(CEDAR_2NDFLAG | kb_Key2))
#define CEDAR_KB_L3			((uint24_t)(CEDAR_2NDFLAG | kb_Key3))
#define CEDAR_KB_L4			((uint24_t)(CEDAR_2NDFLAG | kb_Key4))
#define CEDAR_KB_L5			((uint24_t)(CEDAR_2NDFLAG | kb_Key5))
#define CEDAR_KB_L6			((uint24_t)(CEDAR_2NDFLAG | kb_Key6))
#define CEDAR_KB_RCL		((uint24_t)(CEDAR_2NDFLAG | kb_KeySto))
#define CEDAR_KB_MEM		((uint24_t)(CEDAR_2NDFLAG | kb_KeyAdd))
#define CEDAR_KB_CATALOG	((uint24_t)(CEDAR_2NDFLAG | kb_Key0))
#define CEDAR_KB_IMAG		((uint24_t)(CEDAR_2NDFLAG | kb_KeyDecPnt))
#define CEDAR_KB_ANS		((uint24_t)(CEDAR_2NDFLAG | kb_KeyChs))
#define CEDAR_KB_ENTRY		((uint24_t)(CEDAR_2NDFLAG | kb_KeyEnter))
#define CEDAR_KB_BRIGHTEN	((uint24_t)(CEDAR_2NDFLAG | kb_KeyUp))
#define CEDAR_KB_DARKEN		((uint24_t)(CEDAR_2NDFLAG | kb_KeyDown))

#define CEDAR_KB_F1			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyYequ))
#define CEDAR_KB_F2			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyWindow))
#define CEDAR_KB_F3			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyZoom))
#define CEDAR_KB_F4			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyTrace))
#define CEDAR_KB_F5			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyGraph))
#define CEDAR_KB_A			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyMath))
#define CEDAR_KB_B			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyApps))
#define CEDAR_KB_C			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyPrgm))
#define CEDAR_KB_D			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyRecip))
#define CEDAR_KB_E			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeySin))
#define CEDAR_KB_F			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyCos))
#define CEDAR_KB_G			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyTan))
#define CEDAR_KB_H			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyPower))
#define CEDAR_KB_I			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeySquare))
#define CEDAR_KB_J			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyComma))
#define CEDAR_KB_K			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyLParen))
#define CEDAR_KB_L			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyRParen))
#define CEDAR_KB_M			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyDiv))
#define CEDAR_KB_N			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyLog))
#define CEDAR_KB_O			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key7))
#define CEDAR_KB_P			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key8))
#define CEDAR_KB_Q			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key9))
#define CEDAR_KB_R			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyMul))
#define CEDAR_KB_S			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyLn))
#define CEDAR_KB_T			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key4))
#define CEDAR_KB_U			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key5))
#define CEDAR_KB_V			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key6))
#define CEDAR_KB_W			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeySub))
#define CEDAR_KB_X			((uint24_t)(CEDAR_ALPHAFLAG | kb_KeySto))
#define CEDAR_KB_Y			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key1))
#define CEDAR_KB_Z			((uint24_t)(CEDAR_ALPHAFLAG | kb_Key2))
#define CEDAR_KB_THETA		((uint24_t)(CEDAR_ALPHAFLAG | kb_Key3))
#define CEDAR_KB_QUOTE		((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyAdd))
#define CEDAR_KB_SPACE		((uint24_t)(CEDAR_ALPHAFLAG | kb_Key0))
#define CEDAR_KB_COLON		((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyDecPnt))
#define CEDAR_KB_QUESTION	((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyChs))
#define CEDAR_KB_SOLVE		((uint24_t)(CEDAR_ALPHAFLAG | kb_KeyEnter))

// 83 Premium CE keys
#define CEDAR_KB_FENETRE	CEDAR_KB_WINDOW
#define CEDAR_KB_FX			CEDAR_KB_YEQU
#define CEDAR_KB_SUPPR		CEDAR_KB_DEL
#define CEDAR_KB_TGLEXACT	CEDAR_KB_RECIP
#define CEDAR_KB_TRIG		CEDAR_KB_SIN
#define CEDAR_KB_MATRICE	CEDAR_KB_APPS
#define CEDAR_KB_RESOL		CEDAR_KB_COS
#define CEDAR_KB_FRAC		CEDAR_KB_TAN
#define CEDAR_KB_ANNUL		CEDAR_KB_CLEAR


#define MENUBAR_HEIGHT 20
#define MENU_DROPDOWN_WIDTH 100
#define MENU_DROPDOWN_HEIGHT (GFX_LCD_HEIGHT - MENUBAR_HEIGHT - 5)


#define ATTR_FOCUSABLE			0x1

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_DEFS_H_
