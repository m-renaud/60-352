#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "common.h"

// 1440x900:  echo "150000 10 350 720 800 1430 350 720 5" | ./a.out
// 1920x1080: echo "500000 10 600 970 1070 1910 600 970 5" | ./a.out

struct point
{
	GLint x, y;
};

int iterations;
std::vector<point> pts;

void gasket()
{
	GLint x, y;

	glColor3f(1, 1, 1);

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	x = pts[0].x;
	y = pts[0].y;

	for (int i = 0; i < iterations; ++i)
	{
		plot_point(x, y);
		int j = rand() % pts.size();
		x = (x + pts[j].x) / 2;
		y = (y + pts[j].y) / 2;
	}

	glEnd();
	glFlush();
}

int main(int argc, char* argv[])
{
	std::cin >> iterations;

	int x, y;
	while (std::cin >> x >> y)
		pts.push_back({x, y});

	myGlutInit(
		&argc, argv,
		gasket,
		1920, 1080
	);

	glutMainLoop();
}
