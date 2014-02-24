#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>

// Compile with: g++ -lglut -lGL -lGLU a1.cxx


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Debugging Setup:
class NullBuffer : public std::streambuf
{
public:
	int overflow(int c) { return c; }
};

class NullStream : public std::ostream
{
public:
	NullStream()
		: std::ostream(&m_sb) {}
private:
	NullBuffer m_sb;
};

NullStream nullstream;

bool debug_enabled = false;

void enable_debug() { debug_enabled = true; }
void disable_debug() { debug_enabled = false; }

std::ostream& DEBUG()
{
	if (debug_enabled)
		return std::cerr;
	else
		return nullstream;
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Configuration Variables:
std::size_t width;
std::size_t height;
float aspect_ratio;

std::size_t depth = 5;

std::size_t line_separation = 15;

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Plot a 2D point.
void plot_point(float x, float y)
{
	glVertex2i(
		static_cast<int>(x + 0.5),
		static_cast<int>(y + 0.5)
	);
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Draw a line using Bresenham's Line Algorithm.
void draw_line(int x1, int y1, int x2, int y2)
{
	DEBUG() << "  >>> draw_line"
	        << "((" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << "))\n";

	// Put the point to the left first.
	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	// Vertical line.
  if (x1 == x2)
  {
	  DEBUG() << "    Vertical Line\n";
    if (y1 > y2)
      std::swap(y1, y2);

    for (float y = y1; y <= y2; ++y)
	    plot_point(x1, y);

    return;
  }

  // Horizontal line.
  if (y1 == y2)
  {
	  DEBUG() << "    Horizontal Line\n";
	  if (x1 > x2)
		  std::swap(x1, x2);

	  for (float x = x1; x <= x2; ++x)
		  plot_point(x, y1);

	  return;
  }

  // |m| == 1
  if ((y2 - y1) == (x2 - x1))
  {
	  DEBUG() << "    |m| == 1\n";
	  int step = (y2 > y1) ? 1 : -1;

	  for (float x = x1, y = y1; x <= x2; ++x, y += step)
		  plot_point(x, y);

	  return;
  }

  float m = (y2 - y1) / static_cast<float>(x2 - x1);

  // If slope is less than 1 (|m| <= 1), index loop by x.
  if (fabs(m) < 1)
  {
	  DEBUG() << "    |m| < 1\n";

	  int dx = x2 - x1;
	  int dy = y2 - y1;

	  if (dy < 0)
		  dy *= -1;

	  int delta_diff = 2*dy - 2*dx;
	  int p = 2*dy - dx;

	  int step = (m > 0) ? 1 : -1;

	  int x = x1;
	  int y = y1;

	  while (x < x2)
	  {
		  plot_point(x, y);
		  ++x;
		  if (p < 0)
		  {
			  p = p + 2*dy;
		  }
		  else
		  {
			  y += step;
			  p = p + delta_diff;
		  }
	  }
  }
  // If slope is greater than 1 (|m| > 1), index loop by y.
  else
  {
	  DEBUG() << "    |m| > 1\n";

	  if (y1 > y2)
	  {
		  std::swap(x1,x2);
		  std::swap(y1,y2);
	  }

	  int dx = x2 - x1;
	  int dy = y2 - y1;

	  if (dx < 0)
		  dx *= -1;

	  int delta_diff = 2*dx - 2*dy;
	  int p = 2*dx - dy;

	  int step = (m > 0) ? 1 : -1;

	  int x = x1;
	  int y = y1;

	  while (y < y2)
	  {
		  plot_point(x, y);
		  ++y;
		  if (p < 0)
		  {
			  p = p + 2*dx;
		  }
		  else
		  {
			  x += step;
			  p = p + delta_diff;
		  }
	  }
  }
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Draw several lines from p0 to p1, where p0 is further away.
// Adds depth to the line.
void draw_receding_line(int x1, int y1, int x2, int y2, int count)
{
	for (int i = 0; i <= count/2; ++i)
	{
		draw_line(x1, y1, x2 + i, y2);
		draw_line(x1, y1, x2, y2 + i);
	}
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Draw a line from the center of the screen to point (x,y).
void from_center(int x, int y)
{
	draw_receding_line(width/2, height/2, x, y, depth);
}

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Function called by glut to draw the scene.
void my_display()
{
	glBegin(GL_POINTS);
	{
		draw_line(0,0, 200, 200);
	}
	glEnd();

#if 0
	DEBUG() << ">>> my_display()\n";
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);
  {
	  // Draw white lines.
	  DEBUG() << "- Drawing white lines.\n";
	  glColor3f(0.5, 0.5, 0.5);

	  for (int h = 0; h <= height; h += line_separation)
	  {
		  from_center(0, h);
		  from_center(width, h);
	  }

	  // Draw gray lines.
	  DEBUG() << "- Drawing gray lines.\n";
	  glColor3f(0.2, 0.2, 0.2);

	  for (int w = 0; w <= width; w += line_separation)
	  {
		  from_center(w, 0);
		  from_center(w, height);
	  }

	  // Draw vertical and horizontal black bars.
	  int line_count = 1;
	  int iteration = 0;

	  glColor3f(0, 0, 0);

	  DEBUG() << "- Drawing horizontal bars.\n";
	  for (int w = 8; aspect_ratio*w <= width; w *= 2)
	  {
		  if (iteration >= 5)
			  line_count += 2;

		  for (int i = 0; i != line_count; ++i)
		  {
			  draw_line(
				  width/2 - aspect_ratio*w - 2*i,
				  height/2 - w - i,
				  width/2 + aspect_ratio*w + 2*i,
				  height/2 - w - i
			  );

			  draw_line(
				  width/2 - aspect_ratio*w - 2*i,
				  height/2 + w + i,
				  width/2 + aspect_ratio*w + 2*i,
				  height/2 + w + i
			  );
		  }

		  ++line_count;
		  ++iteration;
	  }


	  DEBUG() << "- Drawing vertical bars.\n";
	  line_count = 1;
	  iteration = 0;

	  glColor3f(0.01, 0.01, 0.01);

	  for (int h = 4; aspect_ratio*h <= height; h *= 2)
	  {
		  if (iteration >= 5)
			  line_count += 2;

		  for (int i = 0; i != line_count; ++i)
		  {
			  draw_line(
				  width/2 - aspect_ratio*h - i,
				  height/2 - h - i,
				  width/2 - aspect_ratio*h - i,
				  height/2 + h + i
			  );

			  draw_line(
				  width/2 + aspect_ratio*h + i,
				  height/2 - h - i,
				  width/2 + aspect_ratio*h + i,
				  height/2 + h + i
			  );
		  }

		  ++line_count;
		  ++iteration;
	  }

	  DEBUG() << "- Drawing red lines along corners.\n";
	  glColor3f(0.4, 0, 0);

	  draw_receding_line(width/2, height/2, 0, 0, depth);
	  draw_receding_line(width/2, height/2, 0, height, depth);
	  draw_receding_line(width/2, height/2, width, 0, depth);
	  draw_receding_line(width/2, height/2, width, height, depth);
  } // GL_POINTS

  glEnd();
#endif
  glFlush();

}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0]
		          << " <screen_width> <screen_height> [ <line_separation> ]\n";

		return 1;
	}

	width = atoi(argv[1]);
	height = atoi(argv[2]);
	aspect_ratio = static_cast<float>(width)/static_cast<float>(height);

	if (argc == 4)
		line_separation = atoi(argv[3]);

	enable_debug();

	// Glut setup code.
	glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 150);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(my_display);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glPointSize(1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, width, 0.0, height);

  glutMainLoop();

  return 0;
}
