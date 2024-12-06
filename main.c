#include <float.h>
#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__ // include Mac OS X versions of headers
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif // __APPLE__

#include "initShader.h"
#include "myLib.h"

// VERTICES
int n;
vec4 *positions;
vec4 *colors;
int cylinder_pos;
int cylinder_cols;
int curr_index = 0;

// INDICES
int floor_idx = 0;
int base_idx = 438;
int lower_joint_idx = 876;
int lower_arm_idx = 1314;
int middle_joint_idx = 1752;
int middle_arm_idx = 2190;
int upper_joint_idx = 2628;
int upper_arm_idx = 3066;
int wrist_idx = 3504;
int finger_1_idx = 3942;
int finger_2_idx = 4380;

// FLOOR INFO
mat4 ctm_floor = IDENTITY;
mat4 floor_translation = IDENTITY;
mat4 ctm_floor_f = IDENTITY;
vec4 floor_dimensions;

// BASE INFO
mat4 base_rotation = IDENTITY;
mat4 ctm_base = IDENTITY;
mat4 ctm_base_f = IDENTITY;
mat4 base_translation = IDENTITY;
mat4 base_tranformations = IDENTITY;
vec4 base_dimensions;
float base_delta = 0.0;

// LOWER JOINT INFO
mat4 lower_joint_rotation = IDENTITY;
mat4 lower_joint_translation = IDENTITY;
mat4 ctm_lower_joint = IDENTITY;
mat4 ctm_c_lower_joint = IDENTITY;
mat4 ctm_lower_joint_f = IDENTITY;
mat4 lower_joint_transformations = IDENTITY;
vec4 lower_joint_dimensions;
float lower_joint_delta = 0.0;

// LOWER ARM INFO
mat4 ctm_lower_arm = IDENTITY;
mat4 ctm_lower_arm_f = IDENTITY;
vec4 lower_arm_dimensions;

// MIDDLE JOINT INFO
mat4 middle_joint_rotation = IDENTITY;
mat4 middle_joint_translation = IDENTITY;
mat4 ctm_middle_joint = IDENTITY;
mat4 ctm_c_middle_joint = IDENTITY;
mat4 ctm_middle_joint_f = IDENTITY;
mat4 middle_joint_transformations = IDENTITY;
vec4 middle_joint_dimensions;
float middle_joint_delta = 0.0;

// MIDDLE ARM INFO
mat4 ctm_middle_arm = IDENTITY;
mat4 ctm_middle_arm_f = IDENTITY;
vec4 middle_arm_dimensions;

// UPPER JOINT INFO
mat4 upper_joint_rotation = IDENTITY;
mat4 upper_joint_translation = IDENTITY;
mat4 ctm_upper_joint = IDENTITY;
mat4 ctm_c_upper_joint = IDENTITY;
mat4 ctm_upper_joint_f = IDENTITY;
mat4 upper_joint_transformations = IDENTITY;
vec4 upper_joint_dimensions;
float upper_joint_delta = 0.0;

// UPPER ARM INFO
mat4 ctm_upper_arm = IDENTITY;
mat4 ctm_upper_arm_f = IDENTITY;
vec4 upper_arm_dimensions;

// WRIST INFO
mat4 wrist_rotation = IDENTITY;
mat4 wrist_translation = IDENTITY;
mat4 ctm_wrist = IDENTITY;
mat4 ctm_c_wrist = IDENTITY;
mat4 ctm_wrist_f = IDENTITY;
mat4 wrist_transformations = IDENTITY;
vec4 wrist_dimensions;
float wrist_delta = 0.0;

// FINGER INFO
mat4 ctm_finger_1 = IDENTITY;
mat4 ctm_finger_2 = IDENTITY;
mat4 ctm_finger_1_f = IDENTITY;
mat4 ctm_finger_2_f = IDENTITY;
mat4 finger_transformations = IDENTITY;
vec4 finger_dimensions;

// Mouse Control
vec4 drag_from = ORIGIN;
vec4 drag_to = ORIGIN;
float drag_speed = 0.5f;

// Perspective Projection
mat4 projection = IDENTITY;
GLuint projection_location;

// Viewing
mat4 model_view = IDENTITY;
GLuint model_view_location;
vec4 eye = (vec4){0.0f, 10.0f, 20.0f, 1.0f};
vec4 at = ZERO;
vec4 up = (vec4){0.0f, 1.0f, 0.0f, 1.0f};

// CTM
mat4 current_transformation_matrix = IDENTITY;
GLuint ctm_location;

float zoom_level = 0.0f;
float zoom_speed = 0.1f;

void init_buffers(int);
void print_controls();
void updateLightPosition();
void init_cylinder(vec4 *positions, vec4 *colors, int squares);

void update_ctms()
{

  // FLOOR CTM
  ctm_floor_f = ctm_floor;

  // BASE CTM
  ctm_base_f = mm_mult(ctm_base, base_rotation);                  // apply any base rotations then scaling/translating COM
  ctm_base_f = mm_mult(floor_translation, ctm_base_f);            // apply floor translations
  base_tranformations = mm_mult(base_translation, base_rotation); // get base transformations --> translation and rotation
  base_tranformations = mm_mult(floor_translation, base_tranformations);

  // LOWER JOINT CTM
  ctm_lower_joint_f = mm_mult(lower_joint_rotation, ctm_lower_joint);  // apply lower joint rotations then lower joint scaling/translating COM
  ctm_lower_joint_f = mm_mult(ctm_c_lower_joint, ctm_lower_joint_f);   // apply translation to top of base
  ctm_lower_joint_f = mm_mult(base_tranformations, ctm_lower_joint_f); // finally apply any base translations or rotations
  lower_joint_transformations = mm_mult(lower_joint_translation, lower_joint_rotation);
  lower_joint_transformations = mm_mult(base_tranformations, lower_joint_transformations);

  // LOWER ARM CTM
  ctm_lower_arm_f = ctm_lower_arm; // apply lower arm scaling/translating COM then translation to top of base
  ctm_lower_arm_f = mm_mult(lower_joint_transformations, ctm_lower_arm_f);

  // MIDDLE JOINT CTM
  ctm_middle_joint_f = mm_mult(middle_joint_rotation, ctm_middle_joint);         // apply middle joint rotations then middle joint scaling/translating COM
  ctm_middle_joint_f = mm_mult(ctm_c_middle_joint, ctm_middle_joint_f);          // apply translation to top of lower arm
  ctm_middle_joint_f = mm_mult(lower_joint_transformations, ctm_middle_joint_f); // finally apply any lower joint translations or rotations
  middle_joint_transformations = mm_mult(middle_joint_translation, middle_joint_rotation);
  middle_joint_transformations = mm_mult(lower_joint_transformations, middle_joint_transformations);

  // MIDDLE ARM CTM
  ctm_middle_arm_f = ctm_middle_arm; // apply middle arm scaling/translating COM then translation to top of lower arm
  ctm_middle_arm_f = mm_mult(middle_joint_transformations, ctm_middle_arm_f);

  // UPPER JOINT CTM
  ctm_upper_joint_f = mm_mult(upper_joint_rotation, ctm_upper_joint);           // apply middle joint rotations then middle joint scaling/translating COM
  ctm_upper_joint_f = mm_mult(ctm_c_upper_joint, ctm_upper_joint_f);            // apply translation to top of lower arm
  ctm_upper_joint_f = mm_mult(middle_joint_transformations, ctm_upper_joint_f); // finally apply any lower joint translations or rotations
  upper_joint_transformations = mm_mult(upper_joint_translation, upper_joint_rotation);
  upper_joint_transformations = mm_mult(middle_joint_transformations, upper_joint_transformations);

  // UPPER ARM CTM
  ctm_upper_arm_f = ctm_upper_arm; // apply middle arm scaling/translating COM then translation to top of lower arm
  ctm_upper_arm_f = mm_mult(upper_joint_transformations, ctm_upper_arm_f);

  // WRIST CTM
  ctm_wrist_f = mm_mult(wrist_rotation, ctm_wrist);                // apply middle joint rotations then middle joint scaling/translating COM
  ctm_wrist_f = mm_mult(ctm_c_wrist, ctm_wrist_f);                 // apply translation to top of lower arm
  ctm_wrist_f = mm_mult(upper_joint_transformations, ctm_wrist_f); // finally apply any lower joint translations or rotations
  wrist_transformations = mm_mult(wrist_translation, wrist_rotation);
  wrist_transformations = mm_mult(upper_joint_transformations, wrist_transformations);

  // FINGER 1 CTM
  ctm_finger_1_f = mm_mult(finger_transformations, ctm_finger_1); // apply finger scaling/translating COM then translation to top of upper arm and then finger translations
  ctm_finger_1_f = mm_mult(wrist_transformations, ctm_finger_1_f);

  // FINGER 2 CTM
  ctm_finger_2_f = mm_mult(finger_transformations, ctm_finger_2); // apply finger scaling/translating COM then translation to top of upper arm and then finger translations
  ctm_finger_2_f = mm_mult(wrist_transformations, ctm_finger_2_f);
}

void init_arm()
{

  // CREATE FLOOR //
  floor_dimensions = (vec4){9.0, 0.1, 9.0, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_floor = mm_mult(translate_mat4((vec4){0.0, -floor_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(floor_dimensions));
  floor_translation = translate_mat4((vec4){0.0, -floor_dimensions.y / 2.0, 0.0, 1.0});

  // CREATE BASE //
  base_dimensions = (vec4){2.0, 7.0, 2.0, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_base = mm_mult(translate_mat4((vec4){0.0, base_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(base_dimensions));
  base_translation = translate_mat4((vec4){0.0, base_dimensions.y / 2.0, 0.0, 1.0});

  // CREATE LOWER ARM JOINT //
  lower_joint_dimensions = (vec4){3.5, 2.5, 2.5, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_lower_joint = mm_mult(scale_mat4(lower_joint_dimensions), rotate_z_mat4(90.0)); // rotate, scale
  ctm_c_lower_joint = translate_mat4((vec4){0.0, base_dimensions.y / 2.0, 0.0, 1.0}); // translate on top of base
  lower_joint_translation = translate_mat4((vec4){0.0, base_dimensions.y / 2.0, 0.0, 1.0});

  // CREATE LOWER ARM //
  lower_arm_dimensions = (vec4){1.7, 5.0, 1.7, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_lower_arm = mm_mult(translate_mat4((vec4){0.0, lower_arm_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(lower_arm_dimensions)); // scale, translate center of mass

  // CREATE MIDDLE JOINT //
  middle_joint_dimensions = (vec4){3.5, 2.0, 2.0, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_middle_joint = mm_mult(scale_mat4(middle_joint_dimensions), rotate_z_mat4(90.0)); // rotate, scale
  ctm_c_middle_joint = translate_mat4((vec4){0.0, lower_arm_dimensions.y, 0.0, 1.0});   // translate on top of lower arm
  middle_joint_translation = translate_mat4((vec4){0.0, lower_arm_dimensions.y, 0.0, 1.0});

  // CREATE MIDDLE ARM //
  middle_arm_dimensions = (vec4){1.5, 5.0, 1.5, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_middle_arm = mm_mult(translate_mat4((vec4){0.0, middle_arm_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(middle_arm_dimensions)); // scale, translate center of mass

  // CREATE UPPER JOINT //
  upper_joint_dimensions = (vec4){3.2, 1.75, 1.75, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_upper_joint = mm_mult(scale_mat4(upper_joint_dimensions), rotate_z_mat4(90.0)); // rotate, scale
  ctm_c_upper_joint = translate_mat4((vec4){0.0, middle_arm_dimensions.y, 0.0, 1.0}); // translate on top of the middle arm
  upper_joint_translation = translate_mat4((vec4){0.0, middle_arm_dimensions.y, 0.0, 1.0});

  // CREATE UPPER ARM //
  upper_arm_dimensions = (vec4){1.3, 5.0, 1.3, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_upper_arm = mm_mult(translate_mat4((vec4){0.0, upper_arm_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(upper_arm_dimensions)); // scale, translate center of mass

  // CREATE WRIST //
  wrist_dimensions = (vec4){3.0, 1.5, 1.5, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_wrist = mm_mult(scale_mat4(wrist_dimensions), rotate_z_mat4(90.0));      // rotate, scale
  ctm_c_wrist = translate_mat4((vec4){0.0, upper_arm_dimensions.y, 0.0, 1.0}); // translate on top of the middle arm
  wrist_translation = translate_mat4((vec4){0.0, upper_arm_dimensions.y, 0.0, 1.0});

  // CREATE FINGER 1 //
  finger_dimensions = (vec4){0.8, 3.0, 0.8, 1.0};
  init_cylinder(positions, colors, SQUARES);
  ctm_finger_1 = mm_mult(translate_mat4((vec4){-wrist_dimensions.x / 4.0, finger_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(finger_dimensions)); // scale, translate center of mass

  // CREATE FINGER 2 //
  init_cylinder(positions, colors, SQUARES);
  ctm_finger_2 = mm_mult(translate_mat4((vec4){wrist_dimensions.x / 4.0, finger_dimensions.y / 2.0, 0.0, 1.0}), scale_mat4(finger_dimensions)); // scale, translate center of mass

  update_ctms();
}

void init_cylinder(vec4 *positions, vec4 *colors, int squares)
{

  float percent_rotation = 360.0 / squares;
  int start_index = curr_index;

  vec4 currColor = rand_color(); // get random color

  // FIRST TRIANGLE
  positions[curr_index] = (vec4){-0.5, -0.5, 0.5, 1.0};
  colors[curr_index++] = currColor;

  positions[curr_index] = mv_mult(rotate_y_mat4(-percent_rotation), (vec4)positions[0]);
  colors[curr_index++] = currColor;

  positions[curr_index] = mv_mult(translate_mat4((vec4){0.0, 1.0, 0.0, 1.0}), (vec4)positions[0]);
  colors[curr_index++] = currColor;

  // SECOND TRIANGLE
  positions[curr_index] = mv_mult(translate_mat4((vec4){0.0, 1.0, 0.0, 1.0}), (vec4)positions[0]);
  colors[curr_index++] = currColor;

  positions[curr_index] = mv_mult(rotate_y_mat4(-percent_rotation), (vec4)positions[0]);
  colors[curr_index++] = currColor;

  positions[curr_index] = mv_mult(mm_mult(rotate_y_mat4(-percent_rotation), translate_mat4((vec4){0.0, 1.0, 0.0, 1.0})), (vec4)positions[0]);
  colors[curr_index++] = currColor;

  // LOOP LOOP LOOP
  int remaining_indices = squares * 6;
  for (int i = 0; i < remaining_indices; i++)
  {
    positions[curr_index] = mv_mult(rotate_y_mat4(-percent_rotation), (vec4)positions[curr_index - 6]);
    colors[curr_index++] = currColor;
  }

  // HAT (TOP CAP)
  currColor = rand_color();
  int multiple = 6;
  for (int i = 0; i < squares; i++)
  {
    positions[curr_index] = positions[3 + (multiple * i)];
    colors[curr_index++] = currColor;

    positions[curr_index] = positions[3 + (multiple * (i + 1))];
    colors[curr_index++] = currColor;

    positions[curr_index] = (vec4){0.0, 0.5, 0.0, 1.0};
    colors[curr_index++] = currColor;
  }

  // SHOES(BOTTOM CAP)
  currColor = rand_color();
  for (int i = 0; i < squares; i++)
  {
    positions[curr_index] = positions[0 + (multiple * (i + 1))];
    colors[curr_index++] = currColor;

    positions[curr_index] = positions[0 + (multiple * i)];
    colors[curr_index++] = currColor;

    positions[curr_index] = (vec4){0.0, -0.5, 0.0, 1.0};
    colors[curr_index++] = currColor;
  }

  printf("num vertices: %d\n", curr_index);
}

void init_buffers(int _n)
{
  n = _n;

  positions = (vec4 *)malloc(sizeof(vec4) * n); // get pointer to position array
  cylinder_pos = n * sizeof(vec4);              // get size of position array

  colors = (vec4 *)malloc(sizeof(vec4) * n); // get pointer to colors array
  cylinder_cols = n * sizeof(vec4);          // get size of colors array
}

void frustum(float left, float right, float bottom, float top, float near,
             float far)
{
  projection =
      (mat4){{(-2.0f * near) / (right - left), 0.0f, 0.0f, 0.0f},
             {0.0f, (-2.0f * near) / (top - bottom), 0.0f, 0.0f},
             {(left + right) / (right - left), (bottom + top) / (top - bottom),
              (near + far) / (far - near), -1.0f},
             {0.0f, 0.0f, -((2.0f * near * far) / (far - near)), 0.0f}};
}

void zoom(float new_zoom_level)
{
  if (new_zoom_level < 1.0f)
  {
    zoom_level = new_zoom_level;
    frustum(-1.0f + zoom_level, 1.0f - zoom_level, -1.0f + zoom_level,
            1.0f - zoom_level, -1.0f, -100.0f);
  }
}

void updateLightPosition()
{
  // // Convert spherical to cartesian coordinates around world origin
  // float phi_rad = lightPhi * M_PI / 180.0;
  // float theta_rad = lightTheta * M_PI / 180.0;

  // // Calculate position relative to world origin
  // lightPosition.x = lightRadius * sin(phi_rad) * cos(theta_rad);
  // lightPosition.y = lightRadius * cos(phi_rad);
  // lightPosition.z = lightRadius * sin(phi_rad) * sin(theta_rad);
  // lightPosition.w = 1.0; // Important for position

  // // Update shader uniform
  // glUniform4fv(lightPosition_loc, 1, (GLfloat *)&lightPosition);
  // glutPostRedisplay();
}

void init()
{
  GLuint program = initShader("vshader.glsl", "fshader.glsl");
  glUseProgram(program);

  // Compile shaders and load into graphics card
  GLuint vao;
#ifdef __APPLE__
  glGenVertexArraysAPPLE(1, &vao);
  glBindVertexArrayAPPLE(vao);
#else
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
#endif

  // Locate memory in RAM. Create buffer in graphics
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      cylinder_pos + cylinder_cols, NULL,
      GL_STATIC_DRAW); // Allocate memory in graphics card. Typically modify
                       // the number of bytes to allocate (second argument).
  glBufferSubData(GL_ARRAY_BUFFER, 0, cylinder_pos,
                  positions); // Copy data from positions into allocated
                              // space in graphics card. 0 is the offset.
  glBufferSubData(GL_ARRAY_BUFFER, cylinder_pos, cylinder_cols, colors);

  GLuint vPosition = glGetAttribLocation(
      program, "vPosition"); // Tell graphics card which bytes are positions
                             // via vshader.
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(0));

  GLuint vColor = glGetAttribLocation(
      program, "vColor"); // Tell graphics card which bytes are textures.
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                        (GLvoid *)(cylinder_pos));

  projection_location = glGetUniformLocation(program, "projection");
  model_view_location = glGetUniformLocation(program, "model_view");

  glEnable(GL_DEPTH_TEST); // enable depth test
  glClearColor(
      0.0, 0.0, 0.0,
      1.0); // color used to clear screen before drawing (background color)
  glDepthRange(1, 0);
}

void look_at(vec4 new_eye, vec4 new_at, vec4 new_up)
{

  vec4 z_prime = norm(sub_v(new_eye, new_at));
  vec4 x_prime = norm(cross(new_up, z_prime));
  vec4 y_prime = norm(cross(z_prime, x_prime));

  // printf("n: \t");
  // print_v(n);
  // printf("u: \t");
  // print_v(u);
  // printf("v: \t");
  // print_v(v);

  mat4 r = (mat4){
      {x_prime.x, y_prime.x, z_prime.x, 0}, {x_prime.y, y_prime.y, z_prime.y, 0}, {x_prime.z, y_prime.z, z_prime.z, 0}, {0, 0, 0, 1}};

  mat4 t = translate_mat4(negate(new_eye));
  model_view = mm_mult(r, t);

  eye = new_eye;
  at = new_at;
  up = new_up;
}

void display(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_LINE);

  glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
  glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);

  // DRAW FLOOR
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_floor);
  glDrawArrays(GL_TRIANGLES, floor_idx, 438);

  // DRAW BASE
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_base_f);
  glDrawArrays(GL_TRIANGLES, base_idx, 438);

  // DRAW LOWER JOINT
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_lower_joint_f);
  glDrawArrays(GL_TRIANGLES, lower_joint_idx, 438);

  // DRAW LOWER ARM
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_lower_arm_f);
  glDrawArrays(GL_TRIANGLES, lower_arm_idx, 438);

  // DRAW MIDDLE JOINT
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_middle_joint_f);
  glDrawArrays(GL_TRIANGLES, middle_joint_idx, 438);

  // DRAW MIDDLE ARM
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_middle_arm_f);
  glDrawArrays(GL_TRIANGLES, middle_arm_idx, 438);

  // DRAW UPPER JOINT
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_upper_joint_f);
  glDrawArrays(GL_TRIANGLES, upper_joint_idx, 438);

  // DRAW UPPER ARM
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_upper_arm_f);
  glDrawArrays(GL_TRIANGLES, upper_arm_idx, 438);

  // DRAW WRIST
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_wrist_f);
  glDrawArrays(GL_TRIANGLES, wrist_idx, 438);

  // DRAW FINGER 1
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_finger_1_f);
  glDrawArrays(GL_TRIANGLES, wrist_idx, 438);

  // DRAW FINGER 2
  glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctm_finger_2_f);
  glDrawArrays(GL_TRIANGLES, wrist_idx, 438);

  glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{

  if (key == 'q')
  {
    exit(0);
  }
  else if (key == 'g') // rotate base left
  {
    base_delta -= 5.0;
    base_rotation = rotate_y_mat4(base_delta);
  }
  else if (key == 'h')
  { // rotate base right
    base_delta += 5.0;
    base_rotation = rotate_y_mat4(base_delta);
  }
  else if (key == 't')
  {
    if (lower_joint_delta > -180.0)
    {
      lower_joint_delta -= 5.0;
      lower_joint_rotation = rotate_x_mat4(lower_joint_delta);
    }
  }
  else if (key == 'y')
  {
    if (lower_joint_delta < 180.0)
    {
      lower_joint_delta += 5.0;
      lower_joint_rotation = rotate_x_mat4(lower_joint_delta);
    }
  }
  else if (key == 'u')
  {
    if (middle_joint_delta > -180.0)
    {
      middle_joint_delta -= 5.0;
      middle_joint_rotation = rotate_x_mat4(middle_joint_delta);
    }
  }
  else if (key == 'i')
  {
    if (middle_joint_delta < 180.0)
    {
      middle_joint_delta += 5.0;
      middle_joint_rotation = rotate_x_mat4(middle_joint_delta);
    }
  }
  else if (key == 'j')
  {
    if (upper_joint_delta > -180.0)
    {
      upper_joint_delta -= 5.0;
      upper_joint_rotation = rotate_x_mat4(upper_joint_delta);
    }
  }
  else if (key == 'k')
  {
    if (upper_joint_delta < 180.0)
    {
      upper_joint_delta += 5.0;
      upper_joint_rotation = rotate_x_mat4(upper_joint_delta);
    }
  }
  else if (key == 'o') // rotate base left
  {
    wrist_delta -= 5.0;
    wrist_rotation = rotate_y_mat4(wrist_delta);
  }
  else if (key == 'p')
  { // rotate base right
    wrist_delta += 5.0;
    wrist_rotation = rotate_y_mat4(wrist_delta);
  }
  else if (key == '=')
  {

    base_delta = 0.0;
    base_rotation = rotate_y_mat4(base_delta);

    lower_joint_delta = 0.0;
    lower_joint_rotation = rotate_x_mat4(lower_joint_delta);

    middle_joint_delta = 0.0;
    middle_joint_rotation = rotate_x_mat4(middle_joint_delta);

    upper_joint_delta = 0.0;
    upper_joint_rotation = rotate_x_mat4(upper_joint_delta);
  }

  update_ctms();
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    float x_click = (x * 2.0 / 511.0) - 1;
    float y_click = (-y * 2.0 / 511.0) + 1;
    float z_click = sqrt(1 - pow(x_click, 2) - pow(y_click, 2));
    drag_to = (vec4){x_click, y_click, z_click, 0.0f};
  }

  glutPostRedisplay();
}

void motion(int x, int y)
{
  drag_from = drag_to;
  float x_drag = (x * 2.0 / 511.0) - 1;
  float y_drag = (-y * 2.0 / 511.0) + 1;
  float z_drag = sqrt(1 - pow(x_drag, 2) - pow(y_drag, 2));
  drag_to = (vec4){x_drag, y_drag, z_drag, 0.0f};

  vec4 axis = norm(cross(drag_from, drag_to));
  float angle = acos(dot(drag_from, drag_to)) * drag_speed * -1.0f;
  if (!isnan(angle) && !v_isnan(axis))
  {
    // printf("Click: \t");
    // print_v(drag_from);
    // printf("Drag: \t");
    // print_v(drag_to);
    // printf("At: \t");
    // print_v(at);
    // printf("Eye: \t");
    // print_v(eye);
    // printf("Axis: \t");
    // print_v(axis);
    // printf("Angle: %f\n", angle);

    // target_at = rotate_v(at, eye, axis, angle);
    // at = target_at;

    // printf("New At: \t");
    // print_v(at);
  }

  glutPostRedisplay();
}

void print_controls()
{
  printf("\nControls:\n");
  printf("[g][h]: Rotate base\n");
  printf("[t][y]: Rotate lower joint\n");
  printf("[u][i]: Rotate middle joint\n");
  printf("[j][k]: Rotate upper joint\n");
  printf("[=]: Reset arm\n");
  printf("[c]: Print Controls\n");
  printf("[q]: Quit\n");
}

int main(int argc, char **argv)
{
  srand(time(0));
  glutInit(&argc, argv);

  // double frame buffer (memory space for writing to screen)
  // double to write to one while the other is pending, increasing performance
  // Depth detection. If one fragment has depth greater than another, don't draw
  // it.
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(512, 512); // use square, power of 2 preferred
  glutInitWindowPosition(
      100,
      100); // top-left corner of window relative to top-left corner of screen
  glutCreateWindow("Project 4");
#ifndef __APPLE__
  glewInit();
#endif

  int total_vertices = 438 * 11;
  init_buffers(total_vertices);
  init_arm();
  print_controls();
  look_at(eye, at, up);

  zoom(zoom_level);
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();

  return 0;
}