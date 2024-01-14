#include <iostream>
#include <map>
#include <utility>
#include <vector>

class TicTacToe {
 public:
  TicTacToe() {
    board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
  }

  void resetBoard() {
    for (auto &row : board) {
      std::fill(row.begin(), row.end(), ' ');
    }
  }

  bool isMoveValid(size_t row, size_t col) const {
    return board[row][col] == ' ';
  }

  void makeMove(size_t row, size_t col, char player) {
    if (isMoveValid(row, col)) {
      board[row][col] = player;
    }
  }

  std::pair<int, int> getRandomMove() {
    std::vector<std::pair<int, int>> emptyCells;

    // Find all empty cells
    for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
        if (board[row][col] == ' ') {
          emptyCells.emplace_back(row, col);
        }
      }
    }

    // Choose a random empty cell
    if (!emptyCells.empty()) {
      int randomIndex = rand() % emptyCells.size();
      return emptyCells[randomIndex];
    }

    // Return an invalid move if no empty cells are found
    return {-1, -1};
  }

  bool checkWin(char player) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
      if (board[i][0] == player && board[i][1] == player &&
          board[i][2] == player) {
        return true;
      }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
      if (board[0][i] == player && board[1][i] == player &&
          board[2][i] == player) {
        return true;
      }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player &&
        board[2][2] == player) {
      return true;
    }
    if (board[0][2] == player && board[1][1] == player &&
        board[2][0] == player) {
      return true;
    }

    return false;
  }

  bool isBoardFull() {
    for (auto &row : board) {
      for (char cell : row) {
        if (cell == ' ') return false;
      }
    }
    return true;
  }

  std::string getStateAsString() {
    std::string state;
    for (const auto &row : board) {
      for (char cell : row) {
        state += cell;
      }
    }
    return state;
  }

  void printBoard() {
    for (auto &row : board) {
      for (char cell : row) {
        std::cout << cell << " ";
      }
      std::cout << std::endl;
    }
  }

  char getCurrentPlayer() const { return currentPlayer; }

  void switchPlayer() { currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; }

 private:
  std::vector<std::vector<char>> board;
  char currentPlayer;
};