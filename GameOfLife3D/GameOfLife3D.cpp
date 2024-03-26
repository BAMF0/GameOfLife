// GameOfLife3D.cpp : Defines the entry point for the application.
//

#include "GameOfLife3D.h"
#include <vector>
#include <chrono>

#define N 100

class GameOfLifeApp {
public:
	GameOfLifeApp() {
		initializeState();
	}

	void tick() {
		updateState();
	}

	void printState() {
		for (auto i = 0; i < N; i++) {
			std::cout << "\t";
			for (auto j = 0; j < N; j++) {
				char cell = state[i * j] ? '*' : '.';
				std::cout << cell << " ";
			}
			std::cout << "\n";
		}
		std::cout << std::endl;
	}

private:
	// TODO change datastructure
	bool state[N * N];

	void initializeState() {
		for (auto i = 0; i < N; i++) {
			for (auto j = 0; j < N; j++) {
				state[i * j] = rand() % (100) + 1 < 30;
			}
		}
	}

	uint32_t countNeighbours(bool state[], uint32_t i, uint32_t j) {
		uint32_t nNeighbours = 0;

		// naive edge cases, ineffective
		if (0 < i) {
			nNeighbours += state[(i - 1) * j];
		}
		if (i < N - 1) {
			nNeighbours += state[(i + 1) * j];
		}
		if (0 < j) {
			nNeighbours += state[i * (j - 1)];
		}
		if (j < N - 1) {
			nNeighbours += state[i * (j + 1)];
		}

		if (0 < i && 0 < j) {
			nNeighbours += state[(i - 1) * (j - 1)];
		}
		if (i < N - 1 && j < N - 1) {
			nNeighbours += state[(i + 1) * (j + 1)];
		}
		if (i < N - 1 && 0 < j) {
			nNeighbours += state[(i + 1) * (j - 1)];
		}
		if (0 < i && j < N - 1) {
			nNeighbours += state[(i - 1) * (j + 1)];
		}

		return nNeighbours;
	}

	void updateState() {
		bool newState[N * N]{};

		for (uint32_t i = 0; i < N; i++) {
			for (uint32_t j = 0; j < N; j++) {
				int nNeighbours = countNeighbours(state, i, j);
				bool newCell = false;

				// live cell
				if (state[i * j]) {
					if (nNeighbours < 2) {
						newCell = false;
					}
					else if (nNeighbours == 2 || nNeighbours == 3) {
						newCell = true;
					}
					else if (nNeighbours > 3) {
						newCell = false;
					}
				}
				// dead cell
				else {
					if (nNeighbours == 3) {
						newCell = true;
					}
				}

				newState[i * j] = newCell;
			}
		}
		
		std::copy(std::begin(newState), std::end(newState), std::begin(state));
	}
};

int main() {
	GameOfLifeApp app;

	uint32_t iterations = 100000;
	
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < iterations; i++) {
		app.tick();
	}
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	float throughput = (float)iterations / duration.count();

	std::cout << duration << " elapsed" << std::endl;
	std::cout << throughput * 10e6 << " ticks/s" << std::endl;

	return 0;
}
