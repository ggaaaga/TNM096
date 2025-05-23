#include <iostream>
#include <chrono>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>

std::unordered_set<std::string> visited;
using State = std::string;
std::string goal = "123456780";
std::unordered_map<std::string, std::string> cameFrom;

int calch1(State s);
int calch2(State s);

struct Node {
	State state;
	int g;
	int h;
	State parent; // add this

	bool operator<(const Node& other) const {
		return (g + h) > (other.g + other.h);
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

	std::vector<std::string> path;
	std::string current = goal;

	while (current != start) {
		path.push_back(current);
		current = cameFrom[current];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());

	std::cout << "\nSolution Path:\n";
	for (const auto& s : path) {
		std::cout << s << "\n";
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

int calch2(State s) {
	int dist = 0;
	for (int i = 0; i < 9; i++) {
		if (s[i] != '0') {
			int val = s[i] - '1'; 
			int goalRow = val / 3;
			int goalCol = val % 3;
			int currRow = i / 3;
			int currCol = i % 3;
			dist += std::abs(goalRow - currRow) + std::abs(goalCol - currCol);
		}
	}
	return dist;
}

void findStates(Node curr, int clearTilePos, int steps) {
	Node child = curr;
	std::swap(child.state[clearTilePos], child.state[clearTilePos + steps]);
	if (visited.find(child.state) == visited.end()) {
		child.g = curr.g + 1;
		child.h = calch2(child.state);
		child.parent = curr.state; // record parent
		cameFrom[child.state] = curr.state;
		frontier.push(child);
	}
}