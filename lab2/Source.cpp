#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>


using namespace std;


vector<int> ReadValuesFromFile() {
    string file_name;

    cout << "Enter .txt file name from following or your own file" << endl;
    cout << "1. numbers" << endl;
    cout << "Input: ";
    cin >> file_name;
    file_name += ".txt";
    ifstream inputFile(file_name);

    // existing file with txt extension
    if (!inputFile) {
        cerr << "Error: open file: " + file_name + " is unsuccesed!" << endl;
        exit(1);
    }

    vector<int> numbers;
    int number;

    // read values from file
    while (inputFile >> number) {
        numbers.push_back(number);
    }
    inputFile.close();

    return numbers;
}


vector<int> ReadValuesFromKeyabord() {
    string input;
    cout << "Enter int numbers throught the space: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean bufer
    getline(cin, input); // read full line

    stringstream ss(input); 

    vector<int> numbers;
    int number;

    // skips not int numbers
    while (ss >> number) { 
        numbers.push_back(number);
    }
    return numbers;
}


int ChooseMenuAction() {
    int choice;

    cout << "Choose read mode:\n"; 
    cout << "1. Read from txt file\n";
    cout << "2. Read from keyboard\n";
    cin >> choice;
    return choice;
}


vector<int> ReadValues() {
    vector<int> numbers;
    int action_number = 0;

    while (
        action_number != 1 && action_number != 2
     ) {
        cout << "Choose 1 from 2.";
        action_number = ChooseMenuAction();

        if (action_number == 1) {
            numbers = ReadValuesFromFile();
        }
        if (action_number == 2) {
            numbers = ReadValuesFromKeyabord();
        }
    }
    
    // not empty vector
    if (numbers.size() == 0) {
        cout << "Not numbers!";
        exit(1);
    }
    cout << endl;
    return numbers;
}


pair<int, int> FindMaxValue(int left, int right, vector<int>& numbers) {
    // pair: index, value
    if (left == right) {
        cout << "Return one element: index = " << left << ", value = " << numbers[left] << endl;
        return { left, numbers[left] };
    }
        
    if (right - left == 1) {
        cout << "Compare two elements: index1 = " << left << ", value1 = " << numbers[left]
             << ", index2 = " << right << ", value2 = " << numbers[right] << endl;
        return (
            (numbers[left] > numbers[right]) ? 
             make_pair(left, numbers[left]) : make_pair(right, numbers[right])
        );
    }

    int mid = left + (right - left) / 2;
    cout << "Divide range: left borders (" << left << ", " << mid << "), right borders (" << mid + 1 << ", " << right << ")" << endl;

    pair<int, int> left_max = FindMaxValue(left, mid - 1, numbers); 
    pair<int, int> right_max = FindMaxValue(mid, right, numbers);

    cout << "Return for range: (" << left << ", " << right << "): "
         << "left max value: index = " << left_max.first << ", value = " << left_max.second << ", "
         << "right max value: index = " << right_max.first << ", value = " << right_max.second << endl;

    return (left_max.second > right_max.second) ? left_max : right_max;
}


void printMaxElement(pair<int, int> ind_max, vector<int>& numbers) {
    cout << endl;
    cout << "all numbers:";
    for (int num : numbers) {
        cout << " " << num;
    }
    cout << endl;

    // inscription
    string max_index = "max value index - " + to_string(ind_max.first);
    string max_value = "max value - " + to_string(ind_max.second);
    cout << max_index << ", " << max_value << endl;
}


void hanoi_tower(int n, char tw1, char tw2, char tw3) {
    if (n == 1) {
        cout << "Disc from tower " << tw1 << " transfer to tower " << tw3 << endl;
    }
    else {
        hanoi_tower(n - 1, tw1, tw3, tw2);
        cout << "Disc from tower " << tw1 << " transfer to tower " << tw3 << endl;
        hanoi_tower(n - 1, tw2, tw1, tw3);
    }
}


void print_table(int** table, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << table[i][j] << " ";
        }
        cout << endl;
    }
}

void print_axes_location()
{
    cout << endl;
    std::cout << "         ^\n";  
    std::cout << "         |\n";  
    std::cout << "         | x\n";   
    std::cout << "         |\n";   
    std::cout << "-------------------> y\n";  
    cout << endl;
}


bool is_valid_move(int x, int y, int board_size) {
    return (x >= 0 && x < board_size) && (y >= 0 && y < board_size);
}

bool is_feel_cells(int cur_turn, int board_size) {
    return cur_turn == board_size * board_size + 1;
}

// global visibility
int possible_moves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
};


bool knights_turns_wrapper(int cur_turn, int x, int y, int board_size, int** table)
{
    cout << "Call knights_turns_wrapper: cur_turn = " << cur_turn << ", x = " << x << ", y = " << y << endl;

    if (is_feel_cells(cur_turn, board_size)) {
        cout << "all cells are visited";
        return true;
    }

    bool flag = false;
    int new_x, new_y;

    for (const int* move : possible_moves) {
        new_x = x + move[0];
        new_y = y + move[1];
        if (is_valid_move(new_x, new_y, board_size) && table[new_x][new_y] == 0) {
            table[new_x][new_y] = cur_turn;
            cout << "Cell (" << new_x << ", " << new_y << ") visited (turn: " << cur_turn << ")." << endl;
            flag = knights_turns_wrapper(cur_turn + 1, new_x, new_y, board_size, table);

            if (!flag)
            {
                cout << "Return from cell (" << new_x << ", " << new_y << ") to preveious turn (turn: " << cur_turn << ")." << endl;
                table[new_x][new_y] = 0; // cancel turn
            }
            else {
                cout << "Succesfully continue with cell (" << new_x << ", " << new_y << ")." << endl;
                break;
            }
                
        }
    }
    return flag;
}

void knights_turns(const int n, int x, int y)
{
    if (
        x <= n && y <= n 
        &&
        y >= 1 && x >= 1
    )
        cout << "x: " << x << ", " << "y: " << y << endl;
    else {
        cout << "Invalid coordinates!" << endl;
        return;
    }

    // dynamic allocate memory
    int** table = new int* [n]; // create array of pointers
    for (int i = 0; i < n; i++) {
        table[i] = new int[n]; 
    }

    // initialize table by zeros
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            table[i][j] = 0;  
        }
    }

    // index coordinates
    int turn = 1;
    int ix = x - 1, iy = y - 1;
    // knight is here
    table[ix][iy] = turn;  

    cout << "table before knight's turns:" << endl;
    print_table(table, n);

    int next_turn = ++turn;
    if (knights_turns_wrapper(next_turn, ix, iy, n, table)) {
        cout << "table after knight's turns: " << endl;
        print_table(table, n); // solution has found
    }
    else 
        cout << "Solution hasn't found for this knight's coordinates!" << endl;
}


bool is_spreaded(int cur_line, int board_size) {
    return cur_line == board_size;
}


void print_queen_board(int* queens_positions, int board_size) {
    for (int i = 0; i < board_size; ++i) {
        for (int j = 0; j < board_size; ++j) {
            if (queens_positions[j] == i) {
                cout << " Q "; 
            }
            else {
                cout << " . "; 
            }
        }
        cout << endl;
    }
    cout << endl;
}


bool is_available_cell(int j, int cur_line, int board_size, int* occupied_columns, int* occupied_diagonals, int* occupied_diagonals2) {
    return (
        occupied_columns[j] && 
        occupied_diagonals[cur_line + j] &&
        occupied_diagonals2[cur_line - j + board_size]
    );
}


void spread_queens_wrapper(
    int cur_line, int board_size, int* occupied_columns, int* occupied_diagonals, int* occupied_diagonals2, int* queens_positions
) {
    if (is_spreaded(cur_line, board_size)) {
        print_queen_board(queens_positions, board_size);
        return;
    }

    for (int j = 0; j < board_size; j++) {
        if (
            is_available_cell(
                j,
                cur_line, 
                board_size, 
                occupied_columns,
                occupied_diagonals, 
                occupied_diagonals2)
            ) {

            queens_positions[cur_line] = j; // set up queen
            cout << "Queen spreads to (" << j << ", " << cur_line << ")." << endl;

            occupied_columns[j] = 0; //spread column
            occupied_diagonals[cur_line + j] = 0; // spread diagonal
            occupied_diagonals2[cur_line - j + board_size] = 0; // spread diagonal2

            spread_queens_wrapper(
                cur_line + 1,
                board_size,
                occupied_columns,
                occupied_diagonals,
                occupied_diagonals2,
                queens_positions
            );
            cout << "Return from position (" << j << ", " << cur_line << ")." << endl;
        }
    }
}


void spread_queuns(int board_size)
{
    int* occupied_columns = new int[board_size + 1];
    int* occupied_diagonals = new int[2 * board_size + 1]; // north and east
    int* occupied_diagonals2 = new int[2 * board_size - 1]; // south and east
    int* queens_positions = new int[board_size + 1];

    // init columns
    for (int i = 1; i < board_size + 1; i++) occupied_columns[i] = 1; 
    // init diagonals
    for (int i = 2; i < 2 * board_size + 1; i++) occupied_diagonals[i] = 1; 
    for (int i = 0; i < 2 * board_size - 1; i++) occupied_diagonals2[i] = 1;

    spread_queens_wrapper(
        0, 
        board_size,
        occupied_columns, 
        occupied_diagonals,
        occupied_diagonals2,
        queens_positions
    ); // try to spread queen in first line
}


int main() {
	vector<int> numbers;
	numbers = ReadValues();

    // find index of max_value, max_value
    pair<int, int> ind_max = FindMaxValue(0, numbers.size() - 1, numbers);
    printMaxElement(ind_max, numbers);

    // info about hanoi tower transfers
    int disc_numbers;

    cout << "enter quantity of discs for hanoi's tower: ";
    cin >> disc_numbers;
    cout << endl;

    if (disc_numbers > 0)
        hanoi_tower(disc_numbers, '1', '2', '3');
    else
        cout << "invalid quantity of discs";

    // knight's turn
    int rows_columns, start_x, start_y;

    cout << "Enter quantity board rows (columns): ";
    cin >> rows_columns;

    if (rows_columns > 0) {
        print_axes_location();

        cout << "enter x coordinate of knight's start (start to 1): ";
        cin >> start_x;
        cout << "enter y coordinate of knight's start (start to 1): ";
        cin >> start_y;

        knights_turns(rows_columns, start_x, start_y);
    }
    else
        cout << "invalid number for rows (columns)";

    // queun formation
    cout << "Enter board size (quantity board rows (columns): ";
    cin >> rows_columns;
    if (rows_columns > 0) {
        spread_queuns(rows_columns); // start formation
    }
    else
        cout << "Invalid number for rows (columns)";
	return 0;
}
