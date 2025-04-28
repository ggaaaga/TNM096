#include <iostream>
#include <chrono>
#include <string>
#include <queue>
#include <unordered_set>

std::unordered_set<std::string> visited;
using State = std::string;
std::string goal = "123456780";

int calch1(State s);

struct Node {
	State state;
	int g;
	int h1;

	bool operator<(const Node& other) const {
		return (g + h1) > (other.g + other.h1); 
	}
};

void findStates(Node curr, int clearTilePos, int steps);
std::priority_queue<Node> frontier;

int main() {

	std::string start;
	std::cout << "Input puzzle: \n";
	std::cin >> start;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Solve Puzzle

	frontier.push({ start, 0, calch1(start)});

	while (frontier.top().state != goal)
	{
		Node curr = frontier.top();
		frontier.pop();
		visited.insert(curr.state);

		int clearTilePos = curr.state.find("0");
		int row = clearTilePos / 3 + 1;
		int col = clearTilePos % 3 + 1;

		if (row - 1 >= 1) findStates(curr, clearTilePos, -3);
		if (row + 1 <= 3) findStates(curr, clearTilePos, 3);
		if (col - 1 >= 1) findStates(curr, clearTilePos, -1);
		if (col + 1 <= 3) findStates(curr, clearTilePos, 1);

	}

	std::cout << "\n Solved in " << frontier.top().g << " moves.\n";

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "\n\nTime = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s" << std::endl;
}

int calch1(State s) {
	int misplaced = 0;
	for (int i = 0; i <= 7; i++) {
		if (s[i] != goal[i]) misplaced++;
	}

	return misplaced;
}

void findStates(Node curr, int clearTilePos, int steps) {
	Node child = curr;
	std::swap(child.state[clearTilePos], child.state[clearTilePos + steps]);
	if (visited.find(child.state) == visited.end()) {
		frontier.push({ child.state, curr.g + 1, calch1(child.state)});
	} 
}
