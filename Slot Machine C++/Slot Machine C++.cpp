#include <iostream>
#include <random>
#include <vector>
#include <map>
using namespace std;

const int MAX_LINES = 3;
const int MIN_LINES = 1;

const int ROWS = 3;
const int COLS = 3;

map<char, int> symbols = {
    {'A', 3},
    {'B', 4},
    {'C', 5},
    {'D', 7}
};

map<char, int> values = {
    {'A', 13},
    {'B', 10},
    {'C', 8},
    {'D', 6}
};

int get_balance() {
    int balance;
    while (true) {
        cout << "What amount would you like to deposit into your account?: $";
        cin >> balance;
        if (cin.fail() || balance <= 0) {
            cout << "Please enter a positive integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    return balance;
}

int get_lines() {
    int lines;
    while (true) {
        cout << "Enter the number of lines you want to bet on (1-3): ";
        cin >> lines;
        if (cin.fail() || lines < MIN_LINES || lines > MAX_LINES) {
            cout << "Please enter a number between 1 and 3." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    return lines;
}

int get_bet() {
    int bet;
    while (true) {
        cout << "How much would you like to bet on each line? : $";
        cin >> bet;
        if (cin.fail() || bet <= 0) {
            cout << "Please enter a positive integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }
    return bet;
}

vector<vector<char>> load_slots(map<char, int> symbols) {
    vector<char> all_symbols;
    for (const auto& symbol : symbols) {
        char symbol_char = symbol.first;
        int symbol_count = symbol.second;
        for (int i = 0; i < symbol_count; i++) {
            all_symbols.push_back(symbol_char);
        }
    }

    vector<vector<char>> columns;
    vector<char> current_symbols = all_symbols;
    for (int col = 0; col < COLS; col++) {
        vector<char> column;
        for (int row = 0; row < ROWS; row++) {
            int random_index = rand() % current_symbols.size();
            char value = current_symbols[random_index];
            column.push_back(value);
            current_symbols.erase(current_symbols.begin() + random_index);
        }
        columns.push_back(column);
    }

    cout << "[";
    for (const auto& column : columns) {
        cout << "[";
        for (int i = 0; i < ROWS; i++) {
            cout << column[i];
            if (i != ROWS - 1) {
                cout << ", ";
            }
        }
        cout << "]";
        if (&column != &columns.back()) {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    return columns;
}

void print_slots(const vector<vector<char>>& columns) {
    for (int row = 0; row < ROWS; row++) {
        for (size_t i = 0; i < columns.size(); i++) {
            const vector<char>& column = columns[i];
            cout << column[row];
            if (i != columns.size() - 1) {
                cout << " | ";
            }
        }
        cout << endl;
    }
}

int check_winnings(int lines, const vector<vector<char>>& columns, int bet, map<char, int> values, vector<int>& winning_lines) {
    int winnings = 0;
    for (int line = 0; line < lines; line++) {
        char symbol = columns[0][line];
        bool winning_line = true;
        for (const auto& column : columns) {
            char symbol_to_check = column[line];
            if (symbol != symbol_to_check) {
                winning_line = false;
                break;
            }
        }
        if (winning_line) {
            winning_lines.push_back(line + 1);
            winnings += values[symbol] * bet;
        }
    }
    return winnings;
}

int main() {
    srand(time(nullptr));

    int balance = get_balance();

    while (true) {
        vector<int> winning_lines;
        int lines = get_lines();
        int bet;
        while (true) {
            bet = get_bet();
            int total_bet = bet * lines;
            if (total_bet <= balance) {
                break;
            }
            else {
                cout << "Insufficient balance to bet this amount." << endl;
            }
        }

        vector<vector<char>> columns = load_slots(symbols);
        print_slots(columns);
        int winnings = check_winnings(lines, columns, bet, values, winning_lines);
        cout << "Winnings = " << winnings << endl;
        cout << "You won on lines: ";
        for (const auto& line : winning_lines) {
            cout << line << " ";
        }
        cout << endl;

        balance = balance - (bet * lines) + winnings;
        cout << "Balance = $" << balance << endl;

        char cont;
        cout << "Press q to quit or any letter to run again: ";
        cin >> cont;
        if (cont == 'q' || cont == 'Q') {
            break;
        }
    }

    return 0;
}
