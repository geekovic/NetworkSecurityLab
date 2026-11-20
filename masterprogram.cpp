#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdint>
#include <iomanip>
#include <sstream>
using namespace std;

// -------------------- 1. Caesar Cipher --------------------
string caesarEncrypt(string text)
{
    for (int i = 0; i < (int)text.length(); i++)
    {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        if (isupper(ch))
            text[i] = 'A' + (text[i] - 'A' + 3) % 26;
        else if (islower(ch))
            text[i] = 'a' + (text[i] - 'a' + 3) % 26;
    }
    return text;
}

string caesarDecrypt(string text)
{
    for (int i = 0; i < (int)text.length(); i++)
    {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        if (isupper(ch))
            text[i] = 'A' + (text[i] - 'A' - 3 + 26) % 26;
        else if (islower(ch))
            text[i] = 'a' + (text[i] - 'a' - 3 + 26) % 26;
    }
    return text;
}

void runCaesarCipher()
{
    int action;
    string text;
    cout << "\n1. Encryption\n2. Decryption\nEnter action: ";
    cin >> action;
    if (action == 1 || action == 2)
    {
        cout << (action == 1 ? "Enter Plain Text: " : "Enter Cipher Text: ");
        getline(cin >> ws, text);
        cout << (action == 1 ? "Ciphertext => " : "Plaintext => ")
             << (action == 1 ? caesarEncrypt(text) : caesarDecrypt(text)) << endl;
    }
    else cout << "Invalid choice!\n";
}

// -------------------- 2. Modified Caesar Cipher --------------------
string modifiedCaesarEncrypt(string text, int key)
{
    key = ((key % 26) + 26) % 26;
    for (int i = 0; i < (int)text.length(); i++)
    {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        if (isupper(ch)) text[i] = 'A' + (text[i] - 'A' + key) % 26;
        else if (islower(ch)) text[i] = 'a' + (text[i] - 'a' + key) % 26;
    }
    return text;
}

string modifiedCaesarDecrypt(string text, int key)
{
    key = ((key % 26) + 26) % 26;
    for (int i = 0; i < (int)text.length(); i++)
    {
        unsigned char ch = static_cast<unsigned char>(text[i]);
        if (isupper(ch)) text[i] = 'A' + (text[i] - 'A' - key + 26) % 26;
        else if (islower(ch)) text[i] = 'a' + (text[i] - 'a' - key + 26) % 26;
    }
    return text;
}

void runModifiedCaesarCipher()
{
    int action, key;
    string text;
    cout << "\n1. Encryption\n2. Decryption\nEnter action: ";
    cin >> action;
    if (action != 1 && action != 2) { cout << "Invalid choice!\n"; return; }
    cout << (action == 1 ? "Enter Plain Text: " : "Enter Cipher Text: ");
    getline(cin >> ws, text);
    cout << "Enter Key: ";
    cin >> key;
    cout << (action == 1 ? "Ciphertext => " : "Plaintext => ")
         << (action == 1 ? modifiedCaesarEncrypt(text, key)
                         : modifiedCaesarDecrypt(text, key)) << endl;
}

// -------------------- 3. Playfair Cipher --------------------
char pfGrid[5][5];

void pfBuildGrid(string key)
{
    bool used[26] = {false};
    used['J' - 'A'] = true;                 // J is merged into I
    string order;
    for (char ch : key)
    {
        ch = toupper(ch);
        if (!isalpha(ch)) continue;
        if (ch == 'J') ch = 'I';
        if (!used[ch - 'A']) { used[ch - 'A'] = true; order += ch; }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++)
        if (!used[ch - 'A']) order += ch;
    for (int i = 0; i < 25; i++) pfGrid[i / 5][i % 5] = order[i];
}

void pfLocate(char ch, int &row, int &col)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (pfGrid[i][j] == ch) { row = i; col = j; }
}

string pfDigraphs(string text)
{
    string clean, result;
    for (char ch : text)
        if (isalpha(ch)) clean += (toupper(ch) == 'J') ? 'I' : toupper(ch);
    for (size_t i = 0; i < clean.length(); )
    {
        char a = clean[i];
        char b = (i + 1 < clean.length()) ? clean[i + 1] : 'X';
        if (a == b) { b = 'X'; i += 1; }
        else if (i + 1 < clean.length()) i += 2;
        else i += 1;
        result += a;
        result += b;
    }
    return result;
}

string pfConvert(string text, int shift)        // shift: 1 = encrypt, 4 = decrypt
{
    for (size_t i = 0; i + 1 < text.length(); i += 2)
    {
        int r1, c1, r2, c2;
        pfLocate(text[i], r1, c1);
        pfLocate(text[i + 1], r2, c2);
        if (r1 == r2)
        {
            text[i] = pfGrid[r1][(c1 + shift) % 5];
            text[i + 1] = pfGrid[r2][(c2 + shift) % 5];
        }
        else if (c1 == c2)
        {
            text[i] = pfGrid[(r1 + shift) % 5][c1];
            text[i + 1] = pfGrid[(r2 + shift) % 5][c2];
        }
        else
        {
            text[i] = pfGrid[r1][c2];
            text[i + 1] = pfGrid[r2][c1];
        }
    }
    return text;
}

void runPlayfair()
{
    int action;
    string key, text, prepared;
    cout << "\n1. Encryption\n2. Decryption\nEnter action: ";
    cin >> action;
    if (action != 1 && action != 2) { cout << "Invalid choice!\n"; return; }
    cout << "Enter keyword: ";
    getline(cin >> ws, key);
    pfBuildGrid(key);
    cout << "\nKey Matrix:\n";
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++) cout << pfGrid[i][j] << ' ';
        cout << endl;
    }
    cout << (action == 1 ? "\nEnter Plain Text: " : "\nEnter Cipher Text: ");
    getline(cin >> ws, text);
    prepared = pfDigraphs(text);
    if (action == 1)
    {
        cout << "Prepared text => " << prepared << endl;
        cout << "Ciphertext    => " << pfConvert(prepared, 1) << endl;
    }
    else cout << "Plaintext     => " << pfConvert(prepared, 4) << endl;
}

// -------------------- 4. Hill Cipher (2x2) --------------------
int hillMod26(int x) { return ((x % 26) + 26) % 26; }

int hillInverse(int a)
{
    a = hillMod26(a);
    for (int x = 1; x < 26; x++)
        if ((a * x) % 26 == 1) return x;
    return -1;
}

string hillPrepare(string text)
{
    string clean;
    for (char ch : text)
        if (isalpha(ch)) clean += toupper(ch);
    if (clean.length() % 2 != 0) clean += 'X';
    return clean;
}

string hillTransform(int m[2][2], string text)
{
    string result;
    for (size_t i = 0; i < text.length(); i += 2)
    {
        int a = text[i] - 'A', b = text[i + 1] - 'A';
        result += char('A' + hillMod26(m[0][0] * a + m[0][1] * b));
        result += char('A' + hillMod26(m[1][0] * a + m[1][1] * b));
    }
    return result;
}

bool hillInvertKey(int key[2][2], int inv[2][2])
{
    int det = hillMod26(key[0][0] * key[1][1] - key[0][1] * key[1][0]);
    int detInv = hillInverse(det);
    if (detInv == -1) return false;
    inv[0][0] = hillMod26( key[1][1] * detInv);
    inv[0][1] = hillMod26(-key[0][1] * detInv);
    inv[1][0] = hillMod26(-key[1][0] * detInv);
    inv[1][1] = hillMod26( key[0][0] * detInv);
    return true;
}

void runHillCipher()
{
    int action, key[2][2], inv[2][2];
    string text, prepared;
    cout << "\n1. Encryption\n2. Decryption\nEnter action: ";
    cin >> action;
    if (action != 1 && action != 2) { cout << "Invalid choice!\n"; return; }
    cout << "Enter 2x2 key matrix (4 numbers): ";
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) cin >> key[i][j];
    if (!hillInvertKey(key, inv))
    {
        cout << "Key matrix is not invertible mod 26, choose another key\n";
        return;
    }
    cout << (action == 1 ? "Enter Plain Text: " : "Enter Cipher Text: ");
    getline(cin >> ws, text);
    prepared = hillPrepare(text);
    if (action == 1)
    {
        cout << "Plain text  => " << prepared << endl;
        cout << "Ciphertext  => " << hillTransform(key, prepared) << endl;
    }
    else
    {
        cout << "Inverse key => [" << inv[0][0] << " " << inv[0][1] << "; "
             << inv[1][0] << " " << inv[1][1] << "]" << endl;
        cout << "Plaintext   => " << hillTransform(inv, prepared) << endl;
    }
}

// -------------------- 5. Rail Fence Cipher (zig-zag) --------------------
vector<int> railPattern(int length, int rails)
{
    vector<int> pattern(length);
    int row = 0, step = 1;
    for (int i = 0; i < length; i++)
    {
        pattern[i] = row;
        if (row == 0) step = 1;
        else if (row == rails - 1) step = -1;
        row += step;
    }
    return pattern;
}

string railFenceEncrypt(string input, int rails)
{
    int len = (int)input.length();
    vector<int> pattern = railPattern(len, rails);
    string output;
    for (int r = 0; r < rails; r++)
        for (int i = 0; i < len; i++)
            if (pattern[i] == r) output += input[i];
    return output;
}

string railFenceDecrypt(string input, int rails)
{
    int len = (int)input.length(), index = 0;
    vector<int> pattern = railPattern(len, rails);
    string output(len, ' ');
    for (int r = 0; r < rails; r++)
        for (int i = 0; i < len; i++)
            if (pattern[i] == r) output[i] = input[index++];
    return output;
}

void runRailFence()
{
    int action, rails;
    string text;
    cout << "\n1. Encryption\n2. Decryption\nEnter action: ";
    cin >> action;
    if (action != 1 && action != 2) { cout << "Invalid choice!\n"; return; }
    cout << "Enter number of rails: ";
    cin >> rails;
    if (rails < 2) { cout << "Rails must be at least 2.\n"; return; }
    cout << (action == 1 ? "Enter plaintext: " : "Enter ciphertext: ");
    getline(cin >> ws, text);
    cout << (action == 1 ? "Ciphertext: " : "Plaintext: ")
         << (action == 1 ? railFenceEncrypt(text, rails)
                         : railFenceDecrypt(text, rails)) << endl;
}

// -------------------- 6. Data Encryption Standard (DES) --------------------
// Standard DES tables (positions are numbered from 1)
const int DES_IP[64] = {
    58,50,42,34,26,18,10,2, 60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6, 64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,  59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5, 63,55,47,39,31,23,15,7 };

const int DES_FP[64] = {
    40,8,48,16,56,24,64,32, 39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30, 37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28, 35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26, 33,1,41,9,49,17,57,25 };

const int DES_E[48] = {
    32,1,2,3,4,5,   4,5,6,7,8,9,     8,9,10,11,12,13,  12,13,14,15,16,17,
    16,17,18,19,20,21, 20,21,22,23,24,25, 24,25,26,27,28,29, 28,29,30,31,32,1 };

const int DES_P[32] = {
    16,7,20,21,29,12,28,17, 1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,    19,13,30,6,22,11,4,25 };

const int DES_PC1[56] = {
    57,49,41,33,25,17,9,  1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,  19,11,3,60,52,44,36,
    63,55,47,39,31,23,15, 7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,  21,13,5,28,20,12,4 };

const int DES_PC2[48] = {
    14,17,11,24,1,5,   3,28,15,6,21,10,  23,19,12,4,26,8,  16,7,27,20,13,2,
    41,52,31,37,47,55, 30,40,51,45,33,48, 44,49,39,56,34,53, 46,42,50,36,29,32 };

const int DES_SHIFTS[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

const int DES_SBOX[8][4][16] = {
  { {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
    {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
    {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
    {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} },
  { {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
    {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
    {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
    {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9} },
  { {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
    {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
    {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
    {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12} },
  { {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
    {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
    {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
    {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14} },
  { {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
    {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
    {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
    {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3} },
  { {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
    {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
    {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
    {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13} },
  { {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
    {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
    {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
    {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12} },
  { {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
    {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
    {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
    {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11} }
};

uint64_t desSubKey[16];                 // sixteen 48-bit round keys

uint64_t desPermute(uint64_t input, const int *table, int outBits, int inBits)
{
    uint64_t output = 0;
    for (int i = 0; i < outBits; i++)
    {
        output <<= 1;
        output |= (input >> (inBits - table[i])) & 1;
    }
    return output;
}

uint32_t desRotate28(uint32_t value, int n)
{
    return ((value << n) | (value >> (28 - n))) & 0x0FFFFFFF;
}

void desGenerateKeys(uint64_t key)
{
    uint64_t cd = desPermute(key, DES_PC1, 56, 64);
    uint32_t C = (cd >> 28) & 0x0FFFFFFF;
    uint32_t D = cd & 0x0FFFFFFF;
    for (int r = 0; r < 16; r++)
    {
        C = desRotate28(C, DES_SHIFTS[r]);
        D = desRotate28(D, DES_SHIFTS[r]);
        uint64_t merged = ((uint64_t)C << 28) | D;
        desSubKey[r] = desPermute(merged, DES_PC2, 48, 56);
    }
}

uint32_t desFeistel(uint32_t R, uint64_t K)
{
    uint64_t expanded = desPermute(R, DES_E, 48, 32) ^ K;
    uint32_t sOut = 0;
    for (int box = 0; box < 8; box++)
    {
        int six = (expanded >> (42 - 6 * box)) & 0x3F;
        int row = ((six & 0x20) >> 4) | (six & 0x01);
        int col = (six >> 1) & 0x0F;
        sOut = (sOut << 4) | DES_SBOX[box][row][col];
    }
    return (uint32_t)desPermute(sOut, DES_P, 32, 32);
}

uint64_t desBlock(uint64_t block, bool decrypt)
{
    uint64_t ip = desPermute(block, DES_IP, 64, 64);
    uint32_t L = ip >> 32, R = ip & 0xFFFFFFFF;
    for (int r = 0; r < 16; r++)
    {
        int k = decrypt ? 15 - r : r;
        uint32_t temp = R;
        R = L ^ desFeistel(R, desSubKey[k]);
        L = temp;
    }
    uint64_t preOutput = ((uint64_t)R << 32) | L;
    return desPermute(preOutput, DES_FP, 64, 64);
}

bool isHex16(const string &s)
{
    if (s.length() != 16) return false;
    for (char ch : s)
        if (!isxdigit((unsigned char)ch)) return false;
    return true;
}

void runDES()
{
    int action;
    string blockHex, keyHex;
    cout << "\n1. Encryption\n2. Decryption\nEnter action: ";
    cin >> action;
    if (action != 1 && action != 2) { cout << "Invalid choice!\n"; return; }
    cout << (action == 1 ? "Enter Plain Text (16 hex digits): "
                         : "Enter Cipher Text (16 hex digits): ");
    cin >> blockHex;
    cout << "Enter Key (16 hex digits): ";
    cin >> keyHex;
    if (!isHex16(blockHex) || !isHex16(keyHex))
    {
        cout << "Please enter exactly 16 hexadecimal digits\n";
        return;
    }
    desGenerateKeys(stoull(keyHex, nullptr, 16));
    uint64_t result = desBlock(stoull(blockHex, nullptr, 16), action == 2);
    cout << (action == 1 ? "Ciphertext => " : "Plaintext  => ")
         << hex << uppercase << setw(16) << setfill('0') << result
         << dec << setfill(' ') << endl;
}

// -------------------- 7. RSA Algorithm --------------------
long long rsaPowerMod(long long base, long long exp, long long mod)
{
    long long result = 1;
    while (exp > 0) { result = (result * base) % mod; exp--; }
    return result;
}

long long rsaGcd(long long a, long long b)
{
    while (b != 0) { long long temp = b; b = a % b; a = temp; }
    return a;
}

long long rsaModInverse(long long e, long long phi)
{
    for (long long d = 1; d < phi; d++)
        if ((e * d) % phi == 1) return d;
    return -1;
}

void runRSA()
{
    long long p, q, n, phi, e, d, message, encrypted, decrypted;
    cout << "Enter the value of p: "; cin >> p;
    cout << "Enter the value of q: "; cin >> q;
    if (p <= 1 || q <= 1) { cout << "p and q must be greater than 1.\n"; return; }
    n = p * q;
    phi = (p - 1) * (q - 1);
    e = 2;
    while (e < phi && rsaGcd(e, phi) != 1) e++;
    if (e >= phi) { cout << "Unable to find a valid public key.\n"; return; }
    d = rsaModInverse(e, phi);
    if (d == -1) { cout << "Unable to find the private key.\n"; return; }
    cout << "Public Key: " << e << endl;
    cout << "Private Key: " << d << endl;
    cout << "-----------------------------------\n";
    cout << "Enter message (integer smaller than n=" << n << "): ";
    cin >> message;
    if (message < 0 || message >= n)
    { cout << "Message must be between 0 and n-1.\n"; return; }
    encrypted = rsaPowerMod(message, e, n);
    decrypted = rsaPowerMod(encrypted, d, n);
    cout << "Encrypted message: " << encrypted << endl;
    cout << "Decrypted message: " << decrypted << endl;
}

// -------------------- 8. Diffie-Hellman Key Exchange --------------------
long long dhPowerMod(long long base, long long exp, long long mod)
{
    long long result = 1;
    while (exp > 0) { result = (result * base) % mod; exp--; }
    return result;
}

void runDiffieHellman()
{
    long long p, g, privateA, privateB, x, y, K1, K2;
    cout << "Enter prime number (p): "; cin >> p;
    cout << "Enter primitive root (g): "; cin >> g;
    cout << "Enter big number of Alice (X): "; cin >> privateA;
    cout << "Enter big number of Bob (Y): "; cin >> privateB;
    if (p <= 1 || g < 1 || privateA < 0 || privateB < 0)
    { cout << "Please enter valid positive values.\n"; return; }
    x = dhPowerMod(g, privateA, p);
    y = dhPowerMod(g, privateB, p);
    K1 = dhPowerMod(y, privateA, p);
    K2 = dhPowerMod(x, privateB, p);
    cout << "\nPublic value of Alice: " << x << endl;
    cout << "Public value of Bob: " << y << endl;
    cout << "Secret key calculated by Alice (K1): " << K1 << endl;
    cout << "Secret key calculated by Bob (K2): " << K2 << endl;
    if (K1 == K2) cout << "\nKey exchange successful!\n";
    else cout << "\nKey exchange failed!\n";
}

// -------------------- 9. MD5 Hashing --------------------
uint32_t md5K[64] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
    0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
    0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
    0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
    0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
    0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
    0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
    0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
    0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};

int md5S[64] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
    5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};

uint32_t leftRotate(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

string md5Hash(string input) {
    vector<uint8_t> msg(input.begin(), input.end());
    uint64_t bitLength = (uint64_t)msg.size() * 8;

    // Step 1: Padding
    msg.push_back(0x80);
    while (msg.size() % 64 != 56)
        msg.push_back(0);

    // Append original length in little-endian
    for (int i = 0; i < 8; i++)
        msg.push_back((bitLength >> (8 * i)) & 0xff);

    // Step 2: Initialize MD5 buffers
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    // Step 3: Process each 512-bit block
    for (size_t offset = 0; offset < msg.size(); offset += 64) {
        uint32_t M[16];

        for (int i = 0; i < 16; i++) {
            size_t j = offset + i * 4;
            M[i] = (uint32_t)msg[j]
                 | ((uint32_t)msg[j + 1] << 8)
                 | ((uint32_t)msg[j + 2] << 16)
                 | ((uint32_t)msg[j + 3] << 24);
        }

        uint32_t A = a0, B = b0, C = c0, D = d0;

        // Step 4: Perform 64 MD5 operations
        for (int i = 0; i < 64; i++) {
            uint32_t F;
            int g;

            if (i < 16) {
                F = (B & C) | (~B & D);
                g = i;
            }
            else if (i < 32) {
                F = (D & B) | (~D & C);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            }
            else {
                F = C ^ (B | ~D);
                g = (7 * i) % 16;
            }

            uint32_t temp = D;
            D = C;
            C = B;
            B = B + leftRotate(A + F + md5K[i] + M[g], md5S[i]);
            A = temp;
        }

        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }

    // Step 5: Convert result to hexadecimal
    uint32_t H[4] = {a0, b0, c0, d0};
    stringstream result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result << hex << setfill('0') << setw(2)
                   << ((H[i] >> (8 * j)) & 0xff);
        }
    }

    return result.str();
}

void runMD5()
{
    string input;
    cout << "Enter a string: ";
    getline(cin >> ws, input);
    cout << "MD5 Hash Value (128 bit): " << md5Hash(input) << endl;
}

// -------------------- Master Menu --------------------
int main()
{
    int choice;
    do
    {
        cout << "\n========== NETWORK SECURITY PRACTICALS ==========\n";
        cout << "1. Caesar Cipher\n";
        cout << "2. Modified Caesar Cipher\n";
        cout << "3. Playfair Cipher\n";
        cout << "4. Hill Cipher\n";
        cout << "5. Rail Fence Cipher\n";
        cout << "6. DES\n";
        cout << "7. RSA Algorithm\n";
        cout << "8. Diffie-Hellman Key Exchange\n";
        cout << "9. MD5 Hashing\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) { cout << "Invalid input. Exiting.\n"; break; }

        switch (choice)
        {
            case 1: runCaesarCipher(); break;
            case 2: runModifiedCaesarCipher(); break;
            case 3: runPlayfair(); break;
            case 4: runHillCipher(); break;
            case 5: runRailFence(); break;
            case 6: runDES(); break;
            case 7: runRSA(); break;
            case 8: runDiffieHellman(); break;
            case 9: runMD5(); break;
            case 0: cout << "Exiting Network Security Practicals.\n"; break;
            default: cout << "Invalid choice! Please select 0-9.\n";
        }
    } while (choice != 0);
    return 0;
}
