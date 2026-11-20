#include <iostream>
#include <string>
#include <cctype>

char grid[5][5];

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

// Fill the 5x5 grid: keyword letters first, then the rest of the alphabet
void buildGrid(std::string key)
{
    bool used[26] = {false};
    used['J' - 'A'] = true;               // J is merged into I
    std::string order;

    for (char ch : key)
    {
        ch = std::toupper(ch);
        if (!std::isalpha(ch)) continue;
        if (ch == 'J') ch = 'I';
        if (!used[ch - 'A'])
        {
            used[ch - 'A'] = true;
            order += ch;
        }
    }

    for (char ch = 'A'; ch <= 'Z'; ch++)
        if (!used[ch - 'A'])
            order += ch;

    for (int i = 0; i < 25; i++)
        grid[i / 5][i % 5] = order[i];
}

void locate(char ch, int &row, int &col)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (grid[i][j] == ch)
            {
                row = i;
                col = j;
            }
}

// Keep letters only, merge J into I, split into digraphs using X as filler
std::string makeDigraphs(std::string text)
{
    std::string clean;
    for (char ch : text)
        if (std::isalpha(ch))
            clean += (std::toupper(ch) == 'J') ? 'I' : std::toupper(ch);

    std::string result;
    for (size_t i = 0; i < clean.length(); )
    {
        char a = clean[i];
        char b = (i + 1 < clean.length()) ? clean[i + 1] : 'X';

        if (a == b)            // identical pair: separate them with X
        {
            b = 'X';
            i += 1;
        }
        else if (i + 1 < clean.length())
            i += 2;
        else                   // last single letter
            i += 1;

        result += a;
        result += b;
    }
    return result;
}

// shift = +1 for encryption, -1 (written as 4) for decryption
std::string convert(std::string text, int shift)
{
    for (size_t i = 0; i + 1 < text.length(); i += 2)
    {
        int r1, c1, r2, c2;
        locate(text[i], r1, c1);
        locate(text[i + 1], r2, c2);

        if (r1 == r2)                          // same row
        {
            text[i]     = grid[r1][(c1 + shift) % 5];
            text[i + 1] = grid[r2][(c2 + shift) % 5];
        }
        else if (c1 == c2)                     // same column
        {
            text[i]     = grid[(r1 + shift) % 5][c1];
            text[i + 1] = grid[(r2 + shift) % 5][c2];
        }
        else                                   // rectangle
        {
            text[i]     = grid[r1][c2];
            text[i + 1] = grid[r2][c1];
        }
    }
    return text;
}

void showGrid()
{
    std::cout << "\nKey Matrix:" << std::endl;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            std::cout << grid[i][j] << ' ';
        std::cout << std::endl;
    }
}

int main()
{
    std::string key, input_text, prepared;
    int action = displayMessage();

    if (action != 1 && action != 2)
    {
        std::cout << "Invalid choice !" << std::endl;
        return 0;
    }

    std::cout << "Enter keyword: ";
    std::getline(std::cin >> std::ws, key);
    buildGrid(key);
    showGrid();

    if (action == 1)
    {
        std::cout << "\nEnter Plain Text: ";
        std::getline(std::cin >> std::ws, input_text);
        prepared = makeDigraphs(input_text);
        std::cout << "Prepared text => " << prepared << std::endl;
        std::cout << "Ciphertext    => " << convert(prepared, 1) << std::endl;
    }
    else
    {
        std::cout << "\nEnter Cipher Text: ";
        std::getline(std::cin >> std::ws, input_text);
        prepared = makeDigraphs(input_text);
        std::cout << "Plaintext     => " << convert(prepared, 4) << std::endl;
    }
    return 0;
}
