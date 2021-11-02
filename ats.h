#pragma once

#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <GL/gl.h>
#include "ext/wglext.h"
#include "ext/glext.h"
#include <math.h>

#define PI (3.14159265359f)
#define GLSL_SHADER(shader) "#version 330 core\n" #shader

// ================================================ TYPES =========================================== //

typedef float f32;
typedef double f64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef long long ptrdiff_t;
typedef unsigned long long size_t;

typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

struct Vec2 {
    f32 x;
    f32 y;

    inline f32 operator[](u32 i) const { return (&x)[i]; }
    inline f32& operator[](u32 i) { return (&x)[i]; }

    inline const f32* getPtr() const { return &x; }
};

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;

    inline f32 operator[](u32 i) const { return (&x)[i]; }
    inline f32& operator[](u32 i) { return (&x)[i]; }

    inline const f32* getPtr() const { return &x; }
};

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    inline f32 operator[](u32 i) const { return (&x)[i]; }
    inline f32& operator[](u32 i) { return (&x)[i]; }

    inline const f32* getPtr() const { return &x; }
};

struct Mat2 {
    Vec2 x;
    Vec2 y;

    inline f32 operator[](u32 i) const { return (&x.x)[i]; }
    inline f32& operator[](u32 i) { return (&x.x)[i]; }

    inline const f32* getPtr() const { return &x.x; }
};

struct Mat3 {
    Vec3 x;
    Vec3 y;
    Vec3 z;

    inline f32 operator[](u32 i) const { return (&x.x)[i]; }
    inline f32& operator[](u32 i) { return (&x.x)[i]; }

    inline const f32* getPtr() const { return &x.x; }
};

struct Mat4 {
    Vec4 x;
    Vec4 y;
    Vec4 z;
    Vec4 w;

    inline f32 operator[](u32 i) const { return (&x.x)[i]; }
    inline f32& operator[](u32 i) { return (&x.x)[i]; }

    inline const f32* getPtr() const { return &x.x; }
};

// ================================================ MATH =========================================== //

static f32 sqrt32(f32 n) {
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

static f32 rsqrt32(f32 n) {
	f32 x2 = n * 0.5f;
	f32 y  = n;
	int i  = *(long*)&y;           // evil floating point bit level hacking

	i = 0x5f3759df - (i >> 1);     // what the fuck? 
	y = *(f32*) &i;
	y = y * (1.5f - (x2 * y * y)); // 1st iteration

	return y;
}

static Vec2 v2(f32 x, f32 y)                { return { x, y }; }
static Vec3 v3(f32 x, f32 y, f32 z)         { return { x, y, z }; }
static Vec4 v4(f32 x, f32 y, f32 z, f32 w)  { return { x, y, z, w }; }

static Vec2 operator-(Vec2 a) { return v2(-a.x, -a.y); }
static Vec2 operator+(Vec2 a, Vec2 b) { return v2(a.x + b.x, a.y + b.y); }
static Vec2 operator-(Vec2 a, Vec2 b) { return v2(a.x - b.x, a.y - b.y); }
static Vec2 operator*(Vec2 a, Vec2 b) { return v2(a.x * b.x, a.y * b.y); }
static Vec2 operator/(Vec2 a, Vec2 b) { return v2(a.x / b.x, a.y / b.y); }
static Vec2 operator*(Vec2 a, f32 s) { return v2(a.x * s, a.y * s); }
static Vec2 operator*(f32 s, Vec2 a) { return v2(a.x * s, a.y * s); }
static Vec2 operator/(Vec2 a, f32 s) { return v2(a.x / s, a.y / s); }
static Vec2& operator+=(Vec2& a, Vec2 b) { a = a + b; return a; }
static Vec2& operator-=(Vec2& a, Vec2 b) { a = a - b; return a; }
static Vec2& operator*=(Vec2& a, Vec2 b) { a = a * b; return a; }
static Vec2& operator/=(Vec2& a, Vec2 b) { a = a / b; return a; }
static Vec2& operator*=(Vec2& a, f32 s) { a = a * s; return a; }
static Vec2& operator/=(Vec2& a, f32 s) { a = a / s; return a; }

static Vec3 operator-(Vec3 a) { return v3(-a.x, -a.y, -a.z); }
static Vec3 operator+(Vec3 a, Vec3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static Vec3 operator-(Vec3 a, Vec3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static Vec3 operator*(Vec3 a, Vec3 b) { return v3(a.x * b.x, a.y * b.y, a.z * b.z); }
static Vec3 operator/(Vec3 a, Vec3 b) { return v3(a.x / b.x, a.y / b.y, a.z / b.z); }
static Vec3 operator*(Vec3 a, f32 s) { return v3(a.x * s, a.y * s, a.z * s); }
static Vec3 operator*(f32 s, Vec3 a) { return v3(a.x * s, a.y * s, a.z * s); }
static Vec3 operator/(Vec3 a, f32 s) { return v3(a.x / s, a.y / s, a.z / s); }
static Vec3& operator+=(Vec3& a, Vec3 b) { a = a + b; return a; }
static Vec3& operator-=(Vec3& a, Vec3 b) { a = a - b; return a; }
static Vec3& operator*=(Vec3& a, Vec3 b) { a = a * b; return a; }
static Vec3& operator/=(Vec3& a, Vec3 b) { a = a / b; return a; }
static Vec3& operator*=(Vec3& a, f32 s) { a = a * s; return a; }
static Vec3& operator/=(Vec3& a, f32 s) { a = a / s; return a; }

static Vec4 operator-(Vec4 a) { return v4(-a.x, -a.y, -a.z, -a.w); }
static Vec4 operator+(Vec4 a, Vec4 b) { return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static Vec4 operator-(Vec4 a, Vec4 b) { return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static Vec4 operator*(Vec4 a, Vec4 b) { return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
static Vec4 operator/(Vec4 a, Vec4 b) { return v4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }
static Vec4 operator*(Vec4 a, f32 s) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }
static Vec4 operator*(f32 s, Vec4 a) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }
static Vec4 operator/(Vec4 a, f32 s) { return v4(a.x / s, a.y / s, a.z / s, a.w / s); }
static Vec4& operator+=(Vec4& a, Vec4 b) { a = a + b; return a; }
static Vec4& operator-=(Vec4& a, Vec4 b) { a = a - b; return a; }
static Vec4& operator*=(Vec4& a, Vec4 b) { a = a * b; return a; }
static Vec4& operator/=(Vec4& a, Vec4 b) { a = a / b; return a; }
static Vec4& operator*=(Vec4& a, f32 s) { a = a * s; return a; }
static Vec4& operator/=(Vec4& a, f32 s) { a = a / s; return a; }

static f32 v2Dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
static f32 v3Dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static f32 v4Dot(Vec4 a, Vec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static f32 v2LenSq(Vec2 a) { return v2Dot(a, a); }
static f32 v3LenSq(Vec3 a) { return v3Dot(a, a); }
static f32 v4LenSq(Vec4 a) { return v4Dot(a, a); }

static f32 v2Len(Vec2 a) { return sqrt32(v2LenSq(a)); }
static f32 v3Len(Vec3 a) { return sqrt32(v3LenSq(a)); }
static f32 v4Len(Vec4 a) { return sqrt32(v4LenSq(a)); }

static Vec2 v2Norm(Vec2 a) { return a * rsqrt32(v2LenSq(a)); }
static Vec3 v3Norm(Vec3 a) { return a * rsqrt32(v3LenSq(a)); }
static Vec4 v4Norm(Vec4 a) { return a * rsqrt32(v4LenSq(a)); }

static Vec3 v3Cross(Vec3 a, Vec3 b) {
    return v3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

static Vec3 operator*(const Mat3& m, Vec3 u) {
    return v3(
        m[0] * u[0] + m[3] * u[1] + m[6] * u[2],
        m[1] * u[0] + m[4] * u[1] + m[7] * u[2],
        m[2] * u[0] + m[5] * u[1] + m[8] * u[2]);
}

static Vec4 operator*(const Mat4& m, Vec4 u) {
    return v4(
        m[0] * u[0] + m[4] * u[1] + m[8]  * u[2] + m[12] * u[3],
        m[1] * u[0] + m[5] * u[1] + m[9]  * u[2] + m[13] * u[3],
        m[2] * u[0] + m[6] * u[1] + m[10] * u[2] + m[14] * u[3],
        m[3] * u[0] + m[7] * u[1] + m[11] * u[2] + m[15] * u[3]);
}

static Mat2 operator*(const Mat2& a, const Mat2& b) {
    return {
        a[0] * b[0] + a[2] * b[1],
        a[1] * b[0] + a[3] * b[1],
        a[0] * b[2] + a[2] * b[3],
        a[1] * b[2] + a[3] * b[3],
    };
}

static Mat3 operator*(const Mat3& a, const Mat3& b) {
    return {
        a[0] * b[0] + a[3] * b[1]  + a[6] * b[2],
        a[1] * b[0] + a[4] * b[1]  + a[7] * b[2],
        a[2] * b[0] + a[5] * b[1]  + a[8] * b[2],

        a[0] * b[3] + a[3] * b[4]  + a[6] * b[5],
        a[1] * b[3] + a[4] * b[4]  + a[7] * b[5],
        a[2] * b[3] + a[5] * b[4]  + a[8] * b[5],

        a[0] * b[6] + a[3] * b[7]  + a[6] * b[8],
        a[1] * b[6] + a[4] * b[7]  + a[7] * b[8],
        a[2] * b[6] + a[5] * b[7]  + a[8] * b[8],
    };
}

static Mat4 operator*(const Mat4& a, const Mat4& b) {
    return {
        a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3],
        a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3],
        a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3],
        a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3],

        a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7],
        a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7],
        a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7],
        a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7],

        a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11],
        a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11],
        a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11],
        a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11],

        a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15],
        a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15],
        a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15],
        a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15],
    };
}

// --------------- view matricies --------------- //

static Mat4 m4Ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return {
        2 / (r - l), 0, 0, 0,
        0, 2 / (t - b), 0, 0,
        0, 0, -2 / (f - n), 0,

        -(r + l) / (r - l),
        -(t + b) / (t - b),
        -(f + n) / (f - n),
        1,
    };
}

static Mat4 m4Perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return {
        a / aspect, 0, 0, 0,
        0, a, 0, 0,
        0, 0, -((f + n) / (f - n)), -1,
        0, 0, -((2 * f * n) / (f - n)), 0,
    };
}

static Mat4 m4LookAt(Vec3 eye, Vec3 center, Vec3 up) {
    auto f = v3Norm(center - eye);
    auto s = v3Norm(v3Cross(f, up));
    auto t = v3Cross(s, f);

    Mat4 out = {};

	out[0]  =  s[0];
	out[1]  =  t[0];
	out[2]  = -f[0];
	out[3]  =  0;

	out[4]  =  s[1];
	out[5]  =  t[1];
	out[6]  = -f[1];
	out[7]  =  0;

	out[8]  =  s[2];
	out[9]  =  t[2];
	out[10] = -f[2];
	out[11] =  0;

    out[12] = -(out[0] * eye[0] + out[4] * eye[1] + out[8]  * eye[2]);
    out[13] = -(out[1] * eye[0] + out[5] * eye[1] + out[9]  * eye[2]);
    out[14] = -(out[2] * eye[0] + out[6] * eye[1] + out[10] * eye[2]);
    out[15] = -(out[3] * eye[0] + out[7] * eye[1] + out[11] * eye[2] - 1);

    return out;
}

// ============================================== PLATFORM LAYER ========================================== //

#define KEY_NONE                 0x00
#define KEY_LBUTTON              0x01     // Left  mouse button
#define KEY_RBUTTON              0x02     // Right mouse button
#define KEY_CANCEL               0x03     // Control-break processing
#define KEY_MBUTTON              0x04     // Middle mouse button (three-button mouse)
#define KEY_XBUTTON1             0x05     // X1 mouse button
#define KEY_XBUTTON2             0x06     // X2 mouse button
#define KEY_BACK                 0x08     // BACKSPACE key
#define KEY_TAB                  0x09     // TAB key
#define KEY_CLEAR                0x0C     // CLEAR key
#define KEY_RETURN               0x0D     // ENTER key
#define KEY_SHIFT                0x10     // SHIFT key
#define KEY_CONTROL              0x11     // CTRL key
#define KEY_MENU                 0x12     // ALT key
#define KEY_PAUSE                0x13     // PAUSE key
#define KEY_CAPITAL              0x14     // CAPS LOCK key
#define KEY_KANA                 0x15     // IME Kana mode
#define KEY_HANGUEL              0x15     // IME Hanguel mode (maintained for compatibility; use KEY_HANGUL)
#define KEY_HANGUL               0x15     // IME Hangul mode
#define KEY_JUNJA                0x17     // IME Junja mode
#define KEY_FINAL                0x18     // IME final mode
#define KEY_HANJA                0x19     // IME Hanja mode
#define KEY_KANJI                0x19     // IME Kanji mode
#define KEY_ESCAPE               0x1B     // ESC key
#define KEY_CONVERT              0x1C     // IME convert
#define KEY_NONCONVERT           0x1D     // IME nonconvert
#define KEY_ACCEPT               0x1E     // IME accept
#define KEY_MODECHANGE           0x1F     // IME mode change request
#define KEY_SPACE                0x20     // SPACEBAR
#define KEY_PRIOR                0x21     // PAGE UP key
#define KEY_NEXT                 0x22     // PAGE DOWN key
#define KEY_END                  0x23     // END key
#define KEY_HOME                 0x24     // HOME key
#define KEY_LEFT                 0x25     // LEFT ARROW key
#define KEY_UP                   0x26     // UP ARROW key
#define KEY_RIGHT                0x27     // RIGHT ARROW key
#define KEY_DOWN                 0x28     // DOWN ARROW key
#define KEY_SELECT               0x29     // SELECT key
#define KEY_PRINT                0x2A     // PRINT key
#define KEY_EXEC                 0x2B     // EXECUTE key
#define KEY_SNAPSHOT             0x2C     // PRINT SCREEN key
#define KEY_INSERT               0x2D     // INS key
#define KEY_DELETE               0x2E     // DEL key
#define KEY_HELP                 0x2F     // HELP key
#define KEY_0                    0x30     // 0 key
#define KEY_1                    0x31     // 1 key
#define KEY_2                    0x32     // 2 key
#define KEY_3                    0x33     // 3 key
#define KEY_4                    0x34     // 4 key
#define KEY_5                    0x35     // 5 key
#define KEY_6                    0x36     // 6 key
#define KEY_7                    0x37     // 7 key
#define KEY_8                    0x38     // 8 key
#define KEY_9                    0x39     // 9 key
#define KEY_A                    0x41     // A key
#define KEY_B                    0x42     // B key
#define KEY_C                    0x43     // C key
#define KEY_D                    0x44     // D key
#define KEY_E                    0x45     // E key
#define KEY_F                    0x46     // F key
#define KEY_G                    0x47     // G key
#define KEY_H                    0x48     // H key
#define KEY_I                    0x49     // I key
#define KEY_J                    0x4A     // J key
#define KEY_K                    0x4B     // K key
#define KEY_L                    0x4C     // L key
#define KEY_M                    0x4D     // M key
#define KEY_N                    0x4E     // N key
#define KEY_O                    0x4F     // O key
#define KEY_P                    0x50     // P key
#define KEY_Q                    0x51     // Q key
#define KEY_R                    0x52     // R key
#define KEY_S                    0x53     // S key
#define KEY_T                    0x54     // T key
#define KEY_U                    0x55     // U key
#define KEY_V                    0x56     // V key
#define KEY_W                    0x57     // W key
#define KEY_X                    0x58     // X key
#define KEY_Y                    0x59     // Y key
#define KEY_Z                    0x5A     // Z key
#define KEY_LWIN                 0x5B     // Left Windows key (Natural keyboard) 
#define KEY_RWIN                 0x5C     // Right Windows key (Natural keyboard)
#define KEY_APPS                 0x5D     // Applications key (Natural keyboard)
#define KEY_SLEEP                0x5F     // Computer Sleep key
#define KEY_NUMPAD0              0x60     // Numeric keypad 0 key
#define KEY_NUMPAD1              0x61     // Numeric keypad 1 key
#define KEY_NUMPAD2              0x62     // Numeric keypad 2 key
#define KEY_NUMPAD3              0x63     // Numeric keypad 3 key
#define KEY_NUMPAD4              0x64     // Numeric keypad 4 key
#define KEY_NUMPAD5              0x65     // Numeric keypad 5 key
#define KEY_NUMPAD6              0x66     // Numeric keypad 6 key
#define KEY_NUMPAD7              0x67     // Numeric keypad 7 key
#define KEY_NUMPAD8              0x68     // Numeric keypad 8 key
#define KEY_NUMPAD9              0x69     // Numeric keypad 9 key
#define KEY_MULTIPLY             0x6A     // Multiply key
#define KEY_ADD                  0x6B     // Add key
#define KEY_SEPARATOR            0x6C     // Separator key
#define KEY_SUBTRACT             0x6D     // Subtract key
#define KEY_DECIMAL              0x6E     // Decimal key
#define KEY_DIVIDE               0x6F     // Divide key
#define KEY_F1                   0x70     // F1 key
#define KEY_F2                   0x71     // F2 key
#define KEY_F3                   0x72     // F3 key
#define KEY_F4                   0x73     // F4 key
#define KEY_F5                   0x74     // F5 key
#define KEY_F6                   0x75     // F6 key
#define KEY_F7                   0x76     // F7 key
#define KEY_F8                   0x77     // F8 key
#define KEY_F9                   0x78     // F9 key
#define KEY_F10                  0x79     // F10 key
#define KEY_F11                  0x7A     // F11 key
#define KEY_F12                  0x7B     // F12 key
#define KEY_F13                  0x7C     // F13 key
#define KEY_F14                  0x7D     // F14 key
#define KEY_F15                  0x7E     // F15 key
#define KEY_F16                  0x7F     // F16 key
#define KEY_F17                  0x80     // F17 key
#define KEY_F18                  0x81     // F18 key
#define KEY_F19                  0x82     // F19 key
#define KEY_F20                  0x83     // F20 key
#define KEY_F21                  0x84     // F21 key
#define KEY_F22                  0x85     // F22 key
#define KEY_F23                  0x86     // F23 key
#define KEY_F24                  0x87     // F24 key
#define KEY_NUMLOCK              0x90     // NUM LOCK key
#define KEY_SCROLL               0x91     // SCROLL LOCK key
#define KEY_LSHIFT               0xA0     // Left SHIFT key
#define KEY_RSHIFT               0xA1     // Right SHIFT key
#define KEY_LCONTROL             0xA2     // Left CONTROL key
#define KEY_RCONTROL             0xA3     // Right CONTROL key
#define KEY_LMENU                0xA4     // Left MENU key
#define KEY_RMENU                0xA5     // Right MENU key
#define KEY_BROWSER_BACK         0xA6     // Browser Back key
#define KEY_BROWSER_FORWARD      0xA7     // Browser Forward key
#define KEY_BROWSER_REFRESH      0xA8     // Browser Refresh key
#define KEY_BROWSER_STOP         0xA9     // Browser Stop key
#define KEY_BROWSER_SEARCH       0xAA     // Browser Search key 
#define KEY_BROWSER_FAVORITES    0xAB     // Browser Favorites key
#define KEY_BROWSER_HOME         0xAC     // Browser Start and Home key
#define KEY_VOLUME_MUTE          0xAD     // Volume Mute key
#define KEY_VOLUME_DOWN          0xAE     // Volume Down key
#define KEY_VOLUME_UP            0xAF     // Volume Up key
#define KEY_MEDIA_NEXT_TRACK     0xB0     // Next Track key
#define KEY_MEDIA_PREV_TRACK     0xB1     // Previous Track key
#define KEY_MEDIA_STOP           0xB2     // Stop Media key
#define KEY_MEDIA_PLAY_PAUSE     0xB3     // Play/Pause Media key
#define KEY_LAUNCH_MAIL          0xB4     // Start Mail key
#define KEY_LAUNCH_MEDIA_SELECT  0xB5     // Select Media key
#define KEY_LAUNCH_APP1          0xB6     // Start Application 1 key
#define KEY_LAUNCH_APP2          0xB7     // Start Application 2 key
#define KEY_OEM_1                0xBA     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key 
#define KEY_OEM_PLUS             0xBB     // For any country/region, the '+' key
#define KEY_OEM_COMMA            0xBC     // For any country/region, the ',' key
#define KEY_OEM_MINUS            0xBD     // For any country/region, the '-' key
#define KEY_OEM_PERIOD           0xBE     // For any country/region, the '.' key
#define KEY_OEM_2                0xBF     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key 
#define KEY_OEM_3                0xC0     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key 
#define KEY_OEM_4                0xDB     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
#define KEY_OEM_5                0xDC     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
#define KEY_OEM_6                0xDD     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
#define KEY_OEM_7                0xDE     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
#define KEY_OEM_8                0xDF     // Used for miscellaneous characters; it can vary by keyboard.
#define KEY_OEM_102              0xE2     // Either the angle bracket key or the backslash key on the RT 102-key keyboard
#define KEY_PROCESSKEY           0xE5     // IME PROCESS key
#define KEY_PACKET               0xE7
#define KEY_ATTN                 0xF6     // Attn key
#define KEY_CRSEL                0xF7     // CrSel key
#define KEY_EXSEL                0xF8     // ExSel key
#define KEY_EREOF                0xF9     // Erase EOF key
#define KEY_PLAY                 0xFA     // Play key
#define KEY_ZOOM                 0xFB     // Zoom key
#define KEY_NONAME               0xFC     // Reserved 
#define KEY_PA1                  0xFD     // PA1 key
#define KEY_OEM_CLEAR            0xFE

static struct {
    b32 close;

    struct {
        i32     width;
        i32     height;
        f32     aspect_ratio;

        HWND    handle;
        HDC     hdc;
        HGLRC   hrc;
    } window;

    struct {
        b8 down[256];
        b8 pressed[256];
        b8 released[256];
    } keyboard;

    struct {
        HANDLE output;
        HANDLE input;
    } console;

    struct {
        f32 total;
        f32 delta;
    } time;
} platform;

static f64 getCurrentTime(void) {
    static LARGE_INTEGER start_counter;
    static LARGE_INTEGER frequency;

    if (start_counter.QuadPart == 0) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start_counter);
    }

    LARGE_INTEGER counter = {};
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart - start_counter.QuadPart) / (f64)frequency.QuadPart;
}

static LRESULT win32_windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_DESTROY: {
            platform.close = true;
            PostQuitMessage(0);
        } break;
        case WM_KEYDOWN: {
            platform.keyboard.pressed[wparam] = true;
            platform.keyboard.down[wparam] = true;
        } break;
        case WM_KEYUP: {
            platform.keyboard.released[wparam] = true;
            platform.keyboard.down[wparam] = false;
        } break;
    }

    return DefWindowProcA(window, message, wparam, lparam);
}

#define GL_PROC_LIST \
    GLProc(CreateShader, CREATESHADER) \
    GLProc(ShaderSource, SHADERSOURCE) \
    GLProc(CompileShader, COMPILESHADER) \
    GLProc(CreateProgram, CREATEPROGRAM) \
    GLProc(AttachShader, ATTACHSHADER) \
    GLProc(LinkProgram, LINKPROGRAM) \
    GLProc(DeleteShader, DELETESHADER) \
    GLProc(UseProgram, USEPROGRAM) \
    GLProc(GetUniformLocation, GETUNIFORMLOCATION) \
    GLProc(Uniform1i, UNIFORM1I) \
    GLProc(Uniform1F, UNIFORM1F) \
    GLProc(Uniform2f, UNIFORM2F) \
    GLProc(Uniform3f, UNIFORM3F) \
    GLProc(Uniform4f, UNIFORM4F) \
    GLProc(GetProgramiv, GETPROGRAMIV) \
    GLProc(GetShaderiv, GETSHADERIV) \
    GLProc(GetShaderInfoLog, GETSHADERINFOLOG) \
    GLProc(GetProgramInfoLog, GETPROGRAMINFOLOG) \

#define GLProc(name, type) static PFNGL##type##PROC gl##name = 0;
GL_PROC_LIST
#undef GLProc

static PFNWGLCHOOSEPIXELFORMATARBPROC       wglChoosePixelFormatARB;
static PFNWGLCREATECONTEXTATTRIBSARBPROC    wglCreateContextAttribsARB;
static PFNWGLMAKECONTEXTCURRENTARBPROC      wglMakeContextCurrentARB;
static PFNWGLSWAPINTERVALEXTPROC            wglSwapIntervalEXT;

static void initOpenGL(void) {
    PIXELFORMATDESCRIPTOR pfd = {};

    pfd.nSize          = sizeof (PIXELFORMATDESCRIPTOR);
    pfd.nVersion       = 1;
    pfd.dwFlags        = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType     = PFD_TYPE_RGBA;
    pfd.cColorBits     = 32;
    pfd.cDepthBits     = 24;
    pfd.cStencilBits   = 8;
    pfd.iLayerType     = PFD_MAIN_PLANE;

    platform.window.hdc = GetDC(platform.window.handle);
    auto pixel_format = ChoosePixelFormat(platform.window.hdc, &pfd);


    if (pixel_format) {
        SetPixelFormat(platform.window.hdc, pixel_format, &pfd);
        auto dummy_hrc = wglCreateContext(platform.window.hdc);
        wglMakeCurrent(platform.window.hdc, dummy_hrc);

        wglChoosePixelFormatARB    = (PFNWGLCHOOSEPIXELFORMATARBPROC)    wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
        wglMakeContextCurrentARB   = (PFNWGLMAKECONTEXTCURRENTARBPROC)   wglGetProcAddress("wglMakeContextCurrentARB");
        wglSwapIntervalEXT         = (PFNWGLSWAPINTERVALEXTPROC)         wglGetProcAddress("wglSwapIntervalEXT");

        {
            int attribs[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
                WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB,     32,
                WGL_DEPTH_BITS_ARB,     24,
                WGL_STENCIL_BITS_ARB,   8,
                0
            };

            UINT num_formats = 0;
            wglChoosePixelFormatARB(platform.window.hdc, attribs, 0, 1, &pixel_format, &num_formats);
        }

        if (pixel_format) {
            const int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                0
            };

            platform.window.hrc = wglCreateContextAttribsARB(platform.window.hdc, dummy_hrc, attribs);

            if (platform.window.hrc) {
                wglMakeCurrent(platform.window.hdc, 0);
                wglDeleteContext(dummy_hrc);
                wglMakeCurrent(platform.window.hdc, platform.window.hrc);
                wglSwapIntervalEXT(0);
            }
        }

        // laod all opengl funcitns
        {
#define GLProc(name, type) gl##name = (PFNGL##type##PROC)wglGetProcAddress("gl" #name);
            GL_PROC_LIST
#undef GLProc
        }
    }
}

static void initPlatform(const char* title, i32 width, i32 height) {
    platform.window.width  = width;
    platform.window.height = height;

    platform.console.input = GetStdHandle(STD_INPUT_HANDLE);
    platform.console.output = GetStdHandle(STD_OUTPUT_HANDLE);

    auto instance = GetModuleHandleA(NULL);

    WNDCLASS wc = {};

    wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc    = win32_windowProc;
    wc.hInstance      = instance;
    wc.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    wc.lpszClassName  = title;

    RegisterClassA(&wc);

    RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 0);

    platform.window.handle = CreateWindowExA(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        title, title,
        WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL,
        instance, NULL);

    initOpenGL();

    ShowWindow(platform.window.handle, SW_SHOW);
    SetForegroundWindow(platform.window.handle);
    SetFocus(platform.window.handle);

    int screen_width    = GetSystemMetrics(SM_CXSCREEN);
    int screen_height   = GetSystemMetrics(SM_CYSCREEN);

    MoveWindow(platform.window.handle, (screen_width / 2)  - (width / 2), (screen_height / 2) - (height / 2), width, height, FALSE);

    platform.time.total = getCurrentTime();
}

static void updatePlatform(void) {
    // event loop stuff:
    {
        memset(platform.keyboard.pressed, 0, sizeof (platform.keyboard.pressed));
        memset(platform.keyboard.released, 0, sizeof (platform.keyboard.released));

        MSG message = {0};
        while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&message);

            if (message.message == WM_QUIT) {
                platform.close = true;
            }
        }
    }

    RECT rect;
    GetClientRect(platform.window.handle, &rect);
    
    platform.window.width  = rect.right - rect.left;
    platform.window.height = rect.bottom - rect.top;
    platform.window.aspect_ratio = (f32)platform.window.width / (f32)platform.window.height;

    glViewport(0, 0, platform.window.width, platform.window.height);

    //SwapBuffers(platform.window.hdc);
    wglSwapLayerBuffers(platform.window.hdc, WGL_SWAP_MAIN_PLANE);

    {
        platform.time.delta = getCurrentTime() - platform.time.total;
        platform.time.total += platform.time.delta;
    }
}

static void exitPlatform(void) {
    ExitProcess(0);
}

// ==================================================== OPENGL STUFF ============================================== //

static u32 gl_compileShader(const char* source, unsigned int type) {
    int success = 0;
    char info_log[512] = {0};
    u32 shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLsizei len = 0;
        glGetShaderInfoLog(shader, 512, &len, info_log);
        WriteConsole(platform.console.output, info_log, len, NULL, NULL);
        exitPlatform();
    }

    return shader;
}

static u32 gl_linkShader(u32 vertex_shader, u32 fragment_shader) {
    int success = 0;
    char info_log[512] = {0};

    u32 shader = glCreateProgram();

    glAttachShader(shader, vertex_shader);
    glAttachShader(shader, fragment_shader);

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success) {
        GLsizei len = 0;
        glGetProgramInfoLog(shader, 512, &len, info_log);
        WriteConsole(platform.console.output, info_log, len, NULL, NULL);
        exitPlatform();
    }

    return shader;
}

extern u32 gl_createShader(const char* vs, const char* fs) {
    u32 vertex   = gl_compileShader(vs, GL_VERTEX_SHADER);
    u32 fragment = gl_compileShader(fs, GL_FRAGMENT_SHADER);
    u32 program  = gl_linkShader(vertex, fragment);

    glUseProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

