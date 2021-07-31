#define MASK_ALL      0xffff //! 字符代码区域

//#- 分类码 -
#define MASK_CTRL     0x8000 //! 控制字符
#define MASK_FN       0x4000 //! 其他功能字符
#define MASK_DIGIT    0x2000 //! 数字
#define MASK_ALPHABET 0x1000 //! 字母

//#- 属性码 -
#define MASK_SYMBOL   0x0100 //! 可用于符号命名
#define MASK_ESCAPE   0x0200 //! 保留字符（表示需转义）
#define MASK_QUOTE    0x0400 //! 引号
#define MASK_BRACKET  0x0800 //! 括号

//#- 编号码 -
#define MASK_ID       0x00ff //! 字符组内代码

int _chr_code_map[256] =
{
	MASK_CTRL | 0x01,                          // 0   0x00 NUL (NULL)
	MASK_CTRL,                                 // 1   0x01 SOH (Start Of Headling)
	MASK_CTRL,                                 // 2   0x02 STX (Start Of Text)
	MASK_CTRL,                                 // 3   0x03 ETX (End Of Text)
	MASK_CTRL,                                 // 4   0x04 EOT (End Of Transmission)
	MASK_CTRL,                                 // 5   0x05 ENQ (Enquiry)
	MASK_CTRL,                                 // 6   0x06 ACK (Acknowledge)
	MASK_CTRL,                                 // 7   0x07 BEL (Bell)
	MASK_CTRL,                                 // 8   0x08 BS (Backspace)
	MASK_CTRL | 0x02,                          // 9   0x09 HT (Horizontal Tab)
	MASK_CTRL | 0x03,                          // 10  0x0A LF/NL(Line Feed/New Line)
	MASK_CTRL,                                 // 11  0x0B VT (Vertical Tab)
	MASK_CTRL,                                 // 12  0x0C FF/NP (Form Feed/New Page)
	MASK_CTRL,                                 // 13  0x0D CR (Carriage Return)
	MASK_CTRL,                                 // 14  0x0E SO (Shift Out)
	MASK_CTRL,                                 // 15  0x0F SI (Shift In)
	MASK_CTRL,                                 // 16  0x10 DLE (Data Link Escape)
	MASK_CTRL,                                 // 17  0x11 DC1/XON (Device Control 1/Transmission On)
	MASK_CTRL,                                 // 18  0x12 DC2 (Device Control 2)
	MASK_CTRL,                                 // 19  0x13 DC3/XOFF (Device Control 3/Transmission Off)
	MASK_CTRL,                                 // 20  0x14 DC4 (Device Control 4)
	MASK_CTRL,                                 // 21  0x15 NAK (Negative Acknowledge)
	MASK_CTRL,                                 // 22  0x16 SYN (Synchronous Idle)
	MASK_CTRL,                                 // 23  0x17 ETB (End of Transmission Block)
	MASK_CTRL,                                 // 24  0x18 CAN (Cancel)
	MASK_CTRL,                                 // 25  0x19 EM (End of Medium)
	MASK_CTRL,                                 // 26  0x1A SUB (Substitute)
	MASK_CTRL,                                 // 27  0x1B ESC (Escape)
	MASK_CTRL,                                 // 28  0x1C FS (File Separator)
	MASK_CTRL,                                 // 29  0x1D GS (Group Separator)
	MASK_CTRL,                                 // 30  0x1E RS (Record Separator)
	MASK_CTRL,                                 // 31  0x1F US (Unit Separator)
	MASK_FN | 0x00,                            // 32  0x20 (Space)
	MASK_FN | MASK_ESCAPE | 0x01,              // 33  0x21 !
	MASK_FN | MASK_ESCAPE | MASK_QUOTE | 0x02, // 34  0x22 "
	MASK_FN | MASK_ESCAPE | 0x03,              // 35  0x23 #
	MASK_FN | MASK_ESCAPE | 0x04,              // 36  0x24 $
	MASK_FN | 0x05,                            // 37  0x25 %
	MASK_FN | MASK_ESCAPE | 0x06,              // 38  0x26 &
	MASK_FN | MASK_ESCAPE | MASK_QUOTE | 0x07, // 39  0x27 '
	MASK_FN | MASK_BRACKET | 0x08,             // 40  0x28 (
	MASK_FN | MASK_BRACKET | 0x09,             // 41  0x29 )
	MASK_FN | MASK_ESCAPE | 0x0a,              // 42  0x2A *
	MASK_FN | 0x0b,                            // 43  0x2B +
	MASK_FN | 0x0c,                            // 44  0x2C ,
	MASK_FN | 0x0d,                            // 45  0x2D -
	MASK_FN | 0x0e,                            // 46  0x2E .
	MASK_FN | 0x0f,                            // 47  0x2F /
	MASK_DIGIT | MASK_SYMBOL | 0x00,           // 48  0x30 0
	MASK_DIGIT | MASK_SYMBOL | 0x01,           // 49  0x31 1
	MASK_DIGIT | MASK_SYMBOL | 0x02,           // 50  0x32 2
	MASK_DIGIT | MASK_SYMBOL | 0x03,           // 51  0x33 3
	MASK_DIGIT | MASK_SYMBOL | 0x04,           // 52  0x34 4
	MASK_DIGIT | MASK_SYMBOL | 0x05,           // 53  0x35 5
	MASK_DIGIT | MASK_SYMBOL | 0x06,           // 54  0x36 6
	MASK_DIGIT | MASK_SYMBOL | 0x07,           // 55  0x37 7
	MASK_DIGIT | MASK_SYMBOL | 0x08,           // 56  0x38 8
	MASK_DIGIT | MASK_SYMBOL | 0x09,           // 57  0x39 9
	MASK_FN | 0x10,                            // 58  0x3A :
	MASK_FN | 0x11,                            // 59  0x3B ;
	MASK_FN | MASK_ESCAPE | 0x12,              // 60  0x3C <
	MASK_FN | 0x13,                            // 61  0x3D =
	MASK_FN | MASK_ESCAPE | 0x14,              // 62  0x3E >
	MASK_FN | MASK_ESCAPE | 0x15,              // 63  0x3F ?
	MASK_FN | 0x16,                            // 64  0x40 @
	MASK_ALPHABET | MASK_SYMBOL | 0x00,        // 65  0x41 A
	MASK_ALPHABET | MASK_SYMBOL | 0x01,        // 66  0x42 B
	MASK_ALPHABET | MASK_SYMBOL | 0x02,        // 67  0x43 C
	MASK_ALPHABET | MASK_SYMBOL | 0x03,        // 68  0x44 D
	MASK_ALPHABET | MASK_SYMBOL | 0x04,        // 69  0x45 E
	MASK_ALPHABET | MASK_SYMBOL | 0x05,        // 70  0x46 F
	MASK_ALPHABET | MASK_SYMBOL | 0x06,        // 71  0x47 G
	MASK_ALPHABET | MASK_SYMBOL | 0x07,        // 72  0x48 H
	MASK_ALPHABET | MASK_SYMBOL | 0x08,        // 73  0x49 I
	MASK_ALPHABET | MASK_SYMBOL | 0x09,        // 74  0x4A J
	MASK_ALPHABET | MASK_SYMBOL | 0x0a,        // 75  0x4B K
	MASK_ALPHABET | MASK_SYMBOL | 0x0b,        // 76  0x4C L
	MASK_ALPHABET | MASK_SYMBOL | 0x0c,        // 77  0x4D M
	MASK_ALPHABET | MASK_SYMBOL | 0x0d,        // 78  0x4E N
	MASK_ALPHABET | MASK_SYMBOL | 0x0e,        // 79  0x4F O
	MASK_ALPHABET | MASK_SYMBOL | 0x0f,        // 80  0x50 P
	MASK_ALPHABET | MASK_SYMBOL | 0x10,        // 81  0x51 Q
	MASK_ALPHABET | MASK_SYMBOL | 0x11,        // 82  0x52 R
	MASK_ALPHABET | MASK_SYMBOL | 0x12,        // 83  0x53 S
	MASK_ALPHABET | MASK_SYMBOL | 0x13,        // 84  0x54 T
	MASK_ALPHABET | MASK_SYMBOL | 0x14,        // 85  0x55 U
	MASK_ALPHABET | MASK_SYMBOL | 0x15,        // 86  0x56 V
	MASK_ALPHABET | MASK_SYMBOL | 0x16,        // 87  0x57 W
	MASK_ALPHABET | MASK_SYMBOL | 0x17,        // 88  0x58 X
	MASK_ALPHABET | MASK_SYMBOL | 0x18,        // 89  0x59 Y
	MASK_ALPHABET | MASK_SYMBOL | 0x19,        // 90  0x5A Z
	MASK_FN | MASK_BRACKET | 0x17,             // 91  0x5B [
	MASK_FN | MASK_ESCAPE | 0x00,              // 92  0x5C \ 
	MASK_FN | MASK_BRACKET | 0x18,             // 93  0x5D ]
	MASK_FN | 0x19,                            // 94  0x5E ^
	MASK_FN | MASK_SYMBOL | 0x1a,              // 95  0x5F _
	MASK_FN | MASK_ESCAPE | 0x1b,              // 96  0x60 `
	MASK_ALPHABET | MASK_SYMBOL | 0x1a,        // 97  0x61 a
	MASK_ALPHABET | MASK_SYMBOL | 0x1b,        // 98  0x62 b
	MASK_ALPHABET | MASK_SYMBOL | 0x1c,        // 99  0x63 c
	MASK_ALPHABET | MASK_SYMBOL | 0x1d,        // 100  0x64 d
	MASK_ALPHABET | MASK_SYMBOL | 0x1e,        // 101  0x65 e
	MASK_ALPHABET | MASK_SYMBOL | 0x1f,        // 102  0x66 f
	MASK_ALPHABET | MASK_SYMBOL | 0x10,        // 103  0x67 g
	MASK_ALPHABET | MASK_SYMBOL | 0x11,        // 104  0x68 h
	MASK_ALPHABET | MASK_SYMBOL | 0x12,        // 105  0x69 i
	MASK_ALPHABET | MASK_SYMBOL | 0x13,        // 106  0x6A j
	MASK_ALPHABET | MASK_SYMBOL | 0x14,        // 107  0x6B k
	MASK_ALPHABET | MASK_SYMBOL | 0x15,        // 108  0x6C l
	MASK_ALPHABET | MASK_SYMBOL | 0x16,        // 109  0x6D m
	MASK_ALPHABET | MASK_SYMBOL | 0x17,        // 110  0x6E n
	MASK_ALPHABET | MASK_SYMBOL | 0x18,        // 111  0x6F o
	MASK_ALPHABET | MASK_SYMBOL | 0x19,        // 112  0x70 p
	MASK_ALPHABET | MASK_SYMBOL | 0x20,        // 113  0x71 q
	MASK_ALPHABET | MASK_SYMBOL | 0x21,        // 114  0x72 r
	MASK_ALPHABET | MASK_SYMBOL | 0x22,        // 115  0x73 s
	MASK_ALPHABET | MASK_SYMBOL | 0x23,        // 116  0x74 t
	MASK_ALPHABET | MASK_SYMBOL | 0x24,        // 117  0x75 u
	MASK_ALPHABET | MASK_SYMBOL | 0x25,        // 118  0x76 v
	MASK_ALPHABET | MASK_SYMBOL | 0x26,        // 119  0x77 w
	MASK_ALPHABET | MASK_SYMBOL | 0x27,        // 120  0x78 x
	MASK_ALPHABET | MASK_SYMBOL | 0x28,        // 121  0x79 y
	MASK_ALPHABET | MASK_SYMBOL | 0x29,        // 122  0x7A z
	MASK_FN | MASK_BRACKET | 0x1c,             // 123  0x7B {
	MASK_FN | MASK_ESCAPE | 0x1d,              // 124  0x7C |
	MASK_FN | MASK_BRACKET | 0x1e,             // 125  0x7D }
	MASK_FN | MASK_ESCAPE | 0x1f,              // 126  0x7E ~
	MASK_CTRL,                                 // 127  0x7F DEL (Delete)
};