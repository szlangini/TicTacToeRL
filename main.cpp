#include <fstream>

#include "fstream"
#include "qlearningagent.h"

void trainAgent(QLearningAgent& agent, TicTacToe& game, size_t episodes,
                double epsilon) {
  for (int e = 0; e < episodes; ++e) {
    game.resetBoard();
    bool done = false;
    std::string state = game.getStateAsString();

    while (!done) {
      int action = agent.chooseAction(state, epsilon, game);
      int row = action / 3;
      int col = action % 3;

      if (game.isMoveValid(row, col)) {
        game.makeMove(row, col, 'X');  // Agent's move

        if (game.checkWin('X')) {
          agent.updateQTable(state, "", action, 1);  // Win
          done = true;
        } else if (game.isBoardFull()) {
          agent.updateQTable(state, "", action, 0.5);  // Draw
          done = true;
        } else {
          // Opponent's move (random for simplicity)
          std::pair<int, int> opponentMove = game.getRandomMove();
          game.makeMove(opponentMove.first, opponentMove.second, 'O');

          if (game.checkWin('O')) {
            agent.updateQTable(state, "", action, -1);  // Lose
            done = true;
          } else if (game.isBoardFull()) {
            agent.updateQTable(state, "", action, 0.5);  // Draw
            done = true;
          } else {
            std::string nextState = game.getStateAsString();
            agent.updateQTable(state, nextState, action, 0);  // Continue
            state = nextState;
          }
        }
      }
    }
  }
  std::cout << "Training complete!" << std::endl;
}

void playAgainstAgent(TicTacToe& game, QLearningAgent& agent) {
  char humanPlayer = 'X';
  char agentPlayer = 'O';
  bool isGameOver = false;
  game.resetBoard();

  while (!isGameOver) {
    game.printBoard();

    // Human player's turn
    if (game.getCurrentPlayer() == humanPlayer) {
      int row, col;
      std::cout << "Your move (enter row, then column): ";
      std::cin >> row >> col;

      // Validate and make the human player's move
      if (game.isMoveValid(row, col)) {
        game.makeMove(row, col, humanPlayer);
        if (game.checkWin(humanPlayer)) {
          std::cout << "Congratulations! You win!" << std::endl;
          isGameOver = true;
        }
      } else {
        std::cout << "Invalid move. Try again." << std::endl;
        continue;
      }
    }
    // Agent's turn
    else {
      std::string state = game.getStateAsString();
      int action =
          agent.chooseAction(state, 0, game);  // Epsilon 0 for greedy action
      int row = action / 3;
      int col = action % 3;

      game.makeMove(row, col, agentPlayer);
      std::cout << "Agent's move: " << row << ' ' << col << std::endl;
      if (game.checkWin(agentPlayer)) {
        std::cout << "Agent wins!" << std::endl;
        isGameOver = true;
      }
    }

    // Check for draw
    if (game.isBoardFull() && !isGameOver) {
      std::cout << "It's a draw!" << std::endl;
      isGameOver = true;
    }

    game.switchPlayer();
  }

  game.printBoard();
}

int main() {
  TicTacToe game;
  QLearningAgent agent(0.1, 0.9);  // Learning rate, discount factor

  std::string qTableFileName = "qtable.txt";
  std::ifstream ifile(qTableFileName);

  if (ifile) {
    agent.loadQTable(qTableFileName);
    std::cout << "Loaded trained Q-table." << std::endl;
    // agent.printQTable();
  } else {
    const size_t episodes =
        10000;  // Total number of games to be played for training
    const double epsilon = 0.2;                  // Exploration rate
    trainAgent(agent, game, episodes, epsilon);  // Train agent
    agent.saveQTable(qTableFileName);            // save the trained qTable
  }

  // Logic for playing -- starts game of human vs. agent
  playAgainstAgent(game, agent);

  return 0;
}