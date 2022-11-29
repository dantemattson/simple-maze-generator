#include <iostream>
#include <vector>
#include <ctime> // std::time

// Union by rank and path compression
class DisjointSet {
private:
	int size;
	// array of parents
	int *up;
	//array of ranks
	int *weights;
	public:
	// union set with union by rank
	void link(int x, int y) {
		if (x >= size || y >= size || x < 0 || y < 0) {
			std::cout << "Warning: link(" << x << ", " << y << ") not allowed\n";
			return;
		}
		if (weights[x] > weights[y]) {
			up[y] = x;
			weights[x] += weights[y];
		}
		else {
			up[x] = y;
			weights[y] += weights[x];
		}
	}
	
	DisjointSet(int pSize = 0) {
		if (pSize < 0) {
			pSize *= -1;
		}
		
		size = pSize;
		
		up = new int[size];
		weights = new int[size];
		
		for (int i = 0; i < size; i++) {
			up[i] = i;
			weights[i] = 0;
		}
	}
	
	~DisjointSet() {
		delete[] up;
		delete[] weights;
	}

	// If x and y have the same root return true
	bool notInSameSet(int x, int y) {
		if (x >= size || y >= size || x < 0 || y < 0) {
			return false;
		}
		if (find(x) == find(y)) {
			return false;
		}

		return true;
	}
	
	// Returns the representitive of the set containing x
	int find(int x) {
		if (x >= size || x < 0) {
			return -1;
		}
		if (x != up[x]) {
			up[x] = find(up[x]);
		}
		return up[x];
	}
	
	// Prints all members, ranks and parents of the set
	void print() {
		std::cout << "element:\t";
		for (int i = 0; i < size; i++)
			std::cout << i;
		std::cout << "\n";

		std::cout << "up:\t";
		for (int i = 0; i < size; i++)
			std::cout << up[i];
		std::cout << "\n";

		std::cout << "weight:  \t";
		for (int i = 0; i < size; i++)
			std::cout << weights[i];
		std::cout << "\n" << "\n";
	}
	
};

class Maze {
private:
	// Left, right, above and below the current cell
	int right(int i) {
		if (i + 1 > nxm) {
			return -1;
		}
		return i + 1;
	}
	int below(int i) {
		if (i + m > nxm) {
			return -1;
		}
		return i + m;
		
	}
	int left(int i) {
		if (i - 1 < 0) {
			return -1;
		}
		return i - 1;
	}
	int above(int i) {
		if (i - m < 0) {
			return -1;
		}
		return i - m;
	}
	// The maze
	char *maze;
	DisjointSet *A;
	// Length of side
	int n;
	// Length of side
	int m;
	// Total number of cells in the maze
	int nxm;
	
	// Format the maze into ASCII for output
	std::vector<char> format() {
		std::vector<char> outMaze(nxm);
		for (int i = 0; i < nxm; i++) {
			if (maze[i] < 10) {
				outMaze[i] = maze[i] + 48;
			}
			// capital
			else if (maze[i] >= 10) {
				outMaze[i] = maze[i] + 87;
			}
		}
		return outMaze;
	}
	
	// Check if t is within bounds
	bool bounds(int t) {
		return t > 0 && t < nxm;
	}
	
	// Make the maze
	void mazeGen() {
		for (int i = 0; i < nxm; i++) {
			maze[i] = 0xF;
		}
		
		// Set the beginning and end
		maze[0] = 0x8;
		maze[nxm - 1] = 0x2;
	
		// Attempt to ensure it can be solved
		connect(0);
		connect(below(0));
		connect(1);
		connect(above(nxm-1));
		connect(left(nxm-1));
		
		for (int i = 0; i < nxm + 1; i++) {
			connect(i);
			
		}
		for (int i = 0; i < nxm+1; i++) {
			connect(i);
		}
		
	}
	
	// Connects up down left or right walls to i
	void connect(int i) {
		
		// Set k to a std::rand() % 3 to connect a random amount of walls
		for (int k = 1; k > 0; k--) {
			int roll = std::rand() % 3;
			if (roll == 0 && i != nxm && i != 0) {
				//look right   n?
				if (right(i) % m != 0 && A->notInSameSet(i, right(i))) {
					A->link(i, right(i));
					maze[i] = maze[i] & 14;
					maze[right(i)] = maze[right(i)] & 11;
				}
			}
			else if (roll == 1 && i != nxm && i != 0) {
				//look down
				if (below(i) < nxm && A->notInSameSet(i, below(i))) {
					A->link(i, below(i));
					maze[i] = maze[i] & 13;
					maze[below(i)] = maze[below(i)] & 7;
				}
			}
			else if (roll == 2 && i != nxm && i != 0) {
				// look left
				if ((left(i) + 1) % n != 0 && A->notInSameSet(i, left(i))) {
					A->link(i, left(i));
					maze[i] = maze[i] & 11;
					maze[left(i)] = maze[left(i)] & 14;
				}
			}
			else if (roll == 3 && i != nxm && i != 0) {
				// look above
				if (above(i) > 0 && A->notInSameSet(i, above(i))) {
					A->link(i, above(i));
					maze[i] = maze[i] & 7;
					maze[above(i)] = maze[above(i)] & 13;
				}
			}
		}
		
		return;
	}

public:
	Maze(int pN = -1, int pM = -1) {
		std::srand(std::time(0));
		if (pN < 3) {
			pN = 3;
		}
		if (pM < 3) {
			pM = 3;
		}
		n = pN;
		m = pM;
		nxm = n * m;
		A = new DisjointSet(nxm);
		maze = new char[nxm];
		mazeGen();
	}
	
	~Maze() {
		delete A;
		delete[] maze;
	}
	
	// Print the formatted maze
	void print() {
		std::vector<char> outMaze = format();
		for (int i = 0; i < nxm; i++) {
			std::cout << outMaze[i];
			if ((i + 1) % m == 0)
				std::cout << "\n";
		}
	}

};

int main() {
	
	int w = 0, h = 0;
	std::cout << "Width: ";
	std::cin >> w;
	std::cout << "Height: ";
	std::cin >> h;
	
	Maze A(h,w);
	A.print();
}