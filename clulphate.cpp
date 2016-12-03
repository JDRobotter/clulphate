#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>
#include <math.h>

void glfw_error_callback(int error, const char *description) {
  std::cerr<<"GLFW ("<<error<<"): "<<description<<"\n";
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(int argc, char **argv) {

  if(!glfwInit()) {
    std::cout<<"Initialization failed\n";
    return 255;
  }

  glfwSetErrorCallback(glfw_error_callback);

  GLFWwindow *window = glfwCreateWindow(640,640, "CLulphate", NULL, NULL);

  if(!window) {
    std::cout<<"Window creation failed\n";
    return 255;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSwapInterval(1);

  int width,height;
  glfwGetFramebufferSize(window, &width, &height);

  // build grid
  const unsigned GRID_N = 600;
  const unsigned GRID_SZ = GRID_N*GRID_N;
  auto grid_vertices = std::make_unique<GLfloat[]>(2*GRID_SZ);
  auto grid_colors = std::make_unique<GLubyte[]>(3*GRID_SZ);

  // build vertices and colors
  for(unsigned j=0;j<GRID_N;j++)
  for(unsigned i=0;i<GRID_N;i++) {
    unsigned k = i+j*GRID_N;
    // x,y
    grid_vertices[2*k]   = (i * width)/(GRID_N-1);
    grid_vertices[2*k+1] = (j * height)/(GRID_N-1);
    // r,g,b
    grid_colors[3*k] = 255*i/GRID_N;
    grid_colors[3*k+1] = 255*j/GRID_N;
    grid_colors[3*k+2] = 255;
  }

  // build indices
  const unsigned GRID_QUADS_N = GRID_N - 1;
  const unsigned GRID_QUADS_SZ = GRID_QUADS_N*GRID_QUADS_N;
  auto grid_indices = std::make_unique<GLuint[]>(GRID_QUADS_SZ*3*2);
  unsigned l = 0;
  for(unsigned j=0;j<GRID_QUADS_N;j++)
  for(unsigned i=0;i<GRID_QUADS_N;i++) {
    unsigned k = i+j*GRID_N;
    // first triangle of quad
    grid_indices[l++] = k; 
    grid_indices[l++] = k+1;
    grid_indices[l++] = k+GRID_N;
    // second triangle of quad
    grid_indices[l++] = k+1;
    grid_indices[l++] = k+GRID_N+1;
    grid_indices[l++] = k+GRID_N;
  }

  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(3, GL_UNSIGNED_BYTE, 0, grid_colors.get());
  glVertexPointer(2, GL_FLOAT, 0, grid_vertices.get());

  unsigned frames = 0;
  double time = glfwGetTime();
  while(!glfwWindowShouldClose(window)) {
  
    // poor's man FPS computation
    frames++;
    if(frames > 60) {
      double _time = glfwGetTime();
      double dt = _time - time;
      std::cout<<"FPS= "<<(1.0*frames/dt)<<"\n";

      time = glfwGetTime();
      frames = 0;
    }

    // modify color grid
    double t = glfwGetTime();
    for(unsigned j=0;j<GRID_N;j++)
    for(unsigned i=0;i<GRID_N;i++) {
      unsigned k = i+j*GRID_N;
      grid_colors[3*k] = ((int)(100*(t))+k)%255;
      grid_colors[3*k+1] = ((int)(100*(2*t))+i)%255;
      grid_colors[3*k+2] = ((int)(100*(3*t+k))+k)%255;
    }

    // draw scene
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width,height,0,0,1);

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3d(1,1,1);
    glDrawElements(GL_TRIANGLES, GRID_QUADS_SZ*3*2, GL_UNSIGNED_INT, grid_indices.get());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
