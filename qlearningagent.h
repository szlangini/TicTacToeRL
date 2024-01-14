#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "tictactoe.h"

class QLearningAgent {
 public:
  QLearningAgent(double alpha, double gamma) : alpha(alpha), gamma(gamma) {}

  void updateQTable(std::string state, std::string nextState, int action,
                    double reward) {
    double qPredict = qTable[{state, action}];

    double qTarget;
    if (nextState.empty()) {
      qTarget = reward;  // If nextState is empty, it means the game is over
    } else {
      double maxNextQ = -std::numeric_limits<double>::infinity();
      for (int a = 0; a < 9;
           ++a) {  // Assuming 9 possible actions (Tic-Tac-Toe)
        maxNextQ = std::max(maxNextQ, qTable[{nextState, a}]);
      }
      qTarget = reward + gamma * maxNextQ;
    }

    qTable[{state, action}] += alpha * (qTarget - qPredict);
  }

  int chooseAction(const std::string& state, double epsilon,
                   const TicTacToe& game) {
    std::vector<int> validActions;

    // Find all valid actions for the current state
    for (int action = 0; action < 9; ++action) {
      int row = action / 3;
      int col = action % 3;
      if (game.isMoveValid(row, col)) {
        validActions.push_back(action);
      }
    }

    // Epsilon-greedy strategy
    if ((double)rand() / RAND_MAX < epsilon || validActions.empty()) {
      // Choose a random valid action
      int randomIndex = rand() % validActions.size();
      return validActions[randomIndex];
    } else {
      // Choose the best valid action based on Q-values
      double maxQ = -std::numeric_limits<double>::infinity();
      int bestAction = validActions.empty() ? 0 : validActions[0];

      for (int action : validActions) {
        double qValue = qTable[{state, action}];
        if (qValue > maxQ) {
          maxQ = qValue;
          bestAction = action;
        }
      }

      return bestAction;
    }
  }

  void saveQTable(const std::string& filename) const {
    std::ofstream outFile(filename);
    for (const auto& entry : qTable) {
      outFile << entry.first.first << "," << entry.first.second << ","
              << entry.second << "\n";
    }
    outFile.close();
  }

  void loadQTable(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
      std::cerr << "Error opening file for loading Q-table." << std::endl;
      return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
      std::istringstream iss(line);
      std::string state;
      std::string actionStr;
      std::string valueStr;

      // Parse the line by commas
      std::getline(iss, state, ',');
      std::getline(iss, actionStr, ',');
      std::getline(iss, valueStr);

      // Convert action and value from string to appropriate types
      int action = std::stoi(actionStr);
      double value = std::stod(valueStr);

      // Update the Q-table
      qTable[{state, action}] = value;
    }
  }

  void printQTable() const {
    for (const auto& entry : qTable) {
      const auto& stateAction = entry.first;
      const auto& qValue = entry.second;
      std::cout << "State: " << stateAction.first
                << ", Action: " << stateAction.second << ", Q-value: " << qValue
                << std::endl;
    }
  }

 private:
  std::map<std::pair<std::string, int>, double> qTable;  // Q-table
  double alpha;                                          // Learning rate
  double gamma;                                          // Discount factor
};