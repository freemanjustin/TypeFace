#include "cmappr.h"

void get_cli_args(e *E, int argc, char *argv[]){

  char  str[128];
  int             c;
  const char      *short_opt = "i:o:a:h";
  struct option   long_opt[] =
  {
     {"help",          no_argument,       NULL, 'h'},
     {"alpha",          required_argument, NULL, 'a'},
     {"output",          required_argument, NULL, 'o'},
     {"input",          required_argument, NULL, 'i'},
     {NULL,            0,                 NULL, 0  }
  };

  E->haveInput = FALSE;
  E->haveOutput = FALSE;
  E->haveAlpha = FALSE;

  while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1)
  {
    //printf("c = %c\n", c);
     switch(c)
     {
        case -1:       /* no more arguments */
        case 0:        /* long options toggles */
         break;

        case 'i':
          E->input_file = malloc((strlen(optarg)+1)*sizeof(char));
          strncpy(E->input_file, optarg, strlen(optarg));
          // fix the string
          E->input_file[strlen(optarg)] = '\x0';
          //printf("input file: %s\n", E->input_file);
          E->haveInput = 1;
          break;
        
        case 'o':
          E->output_file = malloc((strlen(optarg)+1)*sizeof(char));
          strncpy(E->output_file, optarg, strlen(optarg));
          // fix the string
          E->output_file[strlen(optarg)] = '\x0';
          //printf("output_file = %s\n", E->output_file);
          E->haveOutput = 1;
          break;

        case 'a':
          //E->tide_input = malloc((strlen(optarg)+1)*sizeof(char));
          strncpy(str, optarg, strlen(optarg));
          // fix the string
          str[strlen(optarg)] = '\x0';
          sscanf(str,"%f", &E->alpha);
          //printf("iterations = %d\n", E->iterations);
          E->haveAlpha = 1;
          E->alpha_blend = TRUE;
          break;

        case 'h':
          print_usage();
          exit(0);

        default:
          print_usage();
          exit(0);
     };
  };

  // check presence
  if(  E->haveInput == FALSE  ){
    fprintf(stderr,"no input file specified:\nI need one of these to run.\n");
    exit(1);
  }
  if(  E->haveOutput == FALSE ){
    fprintf(stderr,"no output file specified:\nI need one of these to run.\n");
    exit(1);
  }
  if(  E->haveAlpha == FALSE ){
    E->alpha_blend = FALSE;
  }
  
  
}



void print_usage(){
  printf("Usage: \n");
  printf("  -i, --input [filename]               input file [jpg, png]\n");
  printf("  -o, --output [filename]              output file [jpg, png]\n");
  printf("  -h, --alpha [float between 0.0 and 1.0]                alpha blend value\n");
  printf("\n");
}

