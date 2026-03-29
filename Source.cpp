#include "SFML/Graphics.hpp"
#include <vector>

using namespace std;

const int winWid = 800, winHig = 800;
const int gridW = 80, gridH = 80;
const float c1 = 0.15f;
const float c2 = 0.55f;
const float a = 6.0f;
const float dt = 0.05f;
const sf::Color skyColor = sf::Color(0, 0, 20);
const float iterDiff = 4.0f;
const float waveYDiff = 40;
const float glowLength = 9;
const float glowDissipation = 24;
const int startHue = 255;
const float beamFreq = 2;
const float ba = 1.25f;


void updateWaves(vector<vector<sf::Color>>& grid, float t) {
	for (vector<sf::Color>& v : grid) {
		for (sf::Color& c : v) {
			c = skyColor;
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int x = 0; x < gridW; x++) {
			int yS = a * sinf((c1 - (float)x / 1000) * x + t + i * iterDiff) - x * c2 + gridH / 2 + i * waveYDiff;
			if (yS >= 0 && yS < gridH)
				grid[yS][x] = sf::Color(0, startHue, 0);

			int chosenInt = ba * sinf(beamFreq * x - t);
			for (int j = 0; j < glowLength + chosenInt; j++) {
				int y = yS - 1 - j;
				if (y < 0 || y >= gridH) continue;

				int col = startHue - glowDissipation * j;
				if (col < 0) break;

				grid[y][x] = sf::Color(0, col, glowDissipation * j);
			}
		}
	}
}

int main() {
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(winWid, winHig), "Aurora Borealis");
	window.setFramerateLimit(60);

	vector<vector<sf::Color>> grid(gridH, vector<sf::Color>(gridW, skyColor));
	sf::RectangleShape cell(sf::Vector2f(winWid / gridW, winHig / gridH));

	float t = 0.0f;

	while (window.isOpen()) {
		sf::Event ev;

		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}

		updateWaves(grid, t);
		t += dt;

		window.clear();

		for (int y = 0; y < gridH; y++) {
			for (int x = 0; x < gridW; x++) {
				cell.setPosition(sf::Vector2f(x * cell.getSize().x, y * cell.getSize().y));
				cell.setFillColor(grid[y][x]);
				window.draw(cell);
			}
		}

		window.display();
	}

	return 0;
}