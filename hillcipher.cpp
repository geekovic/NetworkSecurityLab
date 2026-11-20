#include <iostream>
#include <string>
#include <cctype>

int displayMessage()
{
    int choice = 0;

    std::cout << "Action to be performed:" << std::endl;
    std::cout << "Encryption => Enter 1" << std::endl;
    std::cout << "Decryption => Enter 2" << std::endl;
    std::cout << "Action: ";
    std::cin >> choice;

    return choice;
}

int mod26(int x)
{
    return ((x % 26) + 26) % 26;
}

// Multiplicative inverse of a under mod 26 (-1 if it does not exist)
int inverseMod26(int a)
{
    a = mod26(a);
    for (int x = 1; x < 26; x++)
        if ((a * x) % 26 == 1)
            return x;
    return -1;
}

// Keep letters only, make them uppercase and pad with X to an even length
std::string prepareText(std::string text)
{
    std::string clean;
    for (char ch : text)
        if (std::isalpha(ch))
            clean += std::toupper(ch);

    if (clean.length() % 2 != 0)
        clean += 'X';
    return clean;
}

// Multiply every pair of letters by the 2x2 matrix m (mod 26)
std::string transform(int m[2][2], std::string text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i += 2)
    {
        int a = text[i] - 'A';
        int b = text[i + 1] - 'A';

        result += char('A' + mod26(m[0][0] * a + m[0][1] * b));
        result += char('A' + mod26(m[1][0] * a + m[1][1] * b));
    }
    return result;
}

// inverse = det^-1 * adjugate (mod 26); returns false if det is not invertible
bool invertKey(int key[2][2], int inv[2][2])
{
    int det = mod26(key[0][0] * key[1][1] - key[0][1] * key[1][0]);
    int detInv = inverseMod26(det);

    if (detInv == -1)
        return false;

    inv[0][0] = mod26( key[1][1] * detInv);
    inv[0][1] = mod26(-key[0][1] * detInv);
    inv[1][0] = mod26(-key[1][0] * detInv);
    inv[1][1] = mod26( key[0][0] * detInv);
    return true;
}

int main()
{
    int key[2][2], inv[2][2];
    std::string input_text, prepared;

    int action = displayMessage();

    if (action != 1 && action != 2)
    {
        std::cout << "Invalid choice !" << std::endl;
        return 0;
    }

    std::cout << "Enter 2x2 key matrix (4 numbers): ";
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            std::cin >> key[i][j];

    if (!invertKey(key, inv))
    {
        std::cout << "Key matrix is not invertible mod 26, choose another key" << std::endl;
        return 0;
    }

    if (action == 1)
    {
        std::cout << "Enter Plain Text: ";
        std::getline(std::cin >> std::ws, input_text);
        prepared = prepareText(input_text);
        std::cout << "Plain text  => " << prepared << std::endl;
        std::cout << "Ciphertext  => " << transform(key, prepared) << std::endl;
    }
    else
    {
        std::cout << "Enter Cipher Text: ";
        std::getline(std::cin >> std::ws, input_text);
        prepared = prepareText(input_text);
        std::cout << "Inverse key => [" << inv[0][0] << " " << inv[0][1] << "; "
                  << inv[1][0] << " " << inv[1][1] << "]" << std::endl;
        std::cout << "Plaintext   => " << transform(inv, prepared) << std::endl;
    }
    return 0;
}
