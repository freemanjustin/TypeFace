#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <getopt.h>


#define TRUE 1
#define FALSE 0


typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

    typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

typedef struct{

  unsigned char  *data;  // image data
  int   h;  // height
  int   w;  // width
  int   n;  // number of color components

}image;

typedef struct{

  char*   input_file;
  char*   output_file;

  image  in_image;  // image data struct
  image  out_image;  // output colormap sample image
  image  font_gray;
  image  scaled;//
  image  cs;    // rgb color shifted

  int   block_size;
  int   gray_value;
  int   alpha_blend;
  float alpha;

  int haveInput;
  int haveOutput;
  int haveAlpha;

}e;



// prototypes
void abort_(const char * s, ...);
float GetPixelGray(float r, float g, float b);
void block_average(e *E);
void get_approx_grayscale(e *E, int gray_value);
//hsv rgb2hsv(rgb in);
//rgb hsv2rgb(hsv in);

void get_command_line_arg_as_string(char **variable, char *cli_input);
void get_command_line_arg_as_int(int *variable, char *cli_input);

void print_usage();
void get_cli_args(e *E, int argc, char *argv[]);

