
// just putting this here
// has nothing to do with the content
// but this is the command to use when making movies on lust
//
// avconv -r 20 -i window%04d.png -b:v 10000k test.mp4

#ifndef STB_HEADERS
#define STB_HEADERS

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#endif
#include "cmappr.h"

// globals
// should abstract these out
// helvetica_96
// original - 17 gray levels
#define ASIZE 17
float levels[ASIZE] = {0.0,16.0,32.0,48.0,64.0,80.0,96.0,112.0,128.0,144.0,160.0,176.0,192.0,208.0,224.0,240.0,256.0};
int   amount[ASIZE] = {26,589,498,447,530,507,491,479,479,496,433,462,499,497,460,111,26};

// even less images - 9 gray levels
//float levels[] = {0.0,32.0,64.0,96.0,128.0,160.0,192.0,224.0,256.0};
//int   amount[] = {26,498,530,491,479,433,499,460,26};

//char  letters[] = "abcdefghijklmnopqrstuvwxyz";

//int how_many = 9;
int how_many = ASIZE;

void abort_(const char * s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}


void get_command_line_arg_as_string(char **variable, char *cli_input){

	*variable = malloc((strlen(cli_input)+1)*sizeof(char));
	strncpy(*variable, cli_input, strlen(cli_input));
	// fix the string
	*(&(*variable)[strlen(cli_input)]) = '\x0';
}

void get_command_line_arg_as_int(int *variable, char *cli_input){

	sscanf(cli_input,"%d",variable);
}

float randInRange(float min, float max)
{
  if (min==max) return 0.0;
  else return (float)( min + (rand() / (float) (RAND_MAX + 1.0) * (max - min)));
}

int offset( int x, int y, int z ) {
    return ( z * 256 * 256 ) + ( y * 256 ) + x ;
}

int midpoint(int low, int high){

  return  (((unsigned int)low + (unsigned int)high) >> 1);

}

int find_closest_int(int *A, int key, int imin, int imax)
{

  // continue searching while [imin,imax] is not empty
  while (imax >= imin){
    // calculate the midpoint for roughly equal partition
    int imid = midpoint(imin, imax);
    if(A[imid] == key)
      // key found at index imid
      return imid;
    // determine which subarray to search
    else if (A[imid] < key)
      // change min index to search upper subarray
      imin = imid + 1;
    else
      // change max index to search lower subarray
      imax = imid - 1;
  }
  // key was not found, return closest
  if (imax <= 0) return 0;
  if (imin >= imax) return imax;
  if ((key - A[imax]) < (A[imin] - key)) return imax;
  return imin;
}

int find_closest(float *A, float key, int imin, int imax)
{
  // continue searching while [imin,imax] is not empty
  while (imax >= imin){
    // calculate the midpoint for roughly equal partition
    int imid = midpoint(imin, imax);
    if(A[imid] == key)
      // key found at index imid
      return imid;
    // determine which subarray to search
    else if (A[imid] < key)
      // change min index to search upper subarray
      imin = imid + 1;
    else
      // change max index to search lower subarray
      imax = imid - 1;
  }
  // key was not found, return closest
  if (imax <= 0) {
    return 0;
  }
  if (imin >= imax){
      if( fabs(A[imin]-key) > (fabs(A[imax]-key))  )
        return imax;
      else
        return imin;
  }
  if ((key - A[imax]) < (A[imin] - key)) {
      return imax;
  }
  return imin;
}

float GetPixelGray(float r, float g, float b)
{
    float grayScale;

    grayScale = (( (r) * 0.3 + (g) * 0.59 + (b) * 0.11) ) * 255.0;
    
    return grayScale;
}

void block_average(e *E){
    int x,y;
    int i,j;
    int sub_i, sub_j;
    int out_i, out_j;

    int block_size;
    int count;

    float   this_gray;
    float   sum_gray;
    float   average_gray;

    int     closest;
    char    fname[128];
    int     which_one[18];

    float rnum;

    // randomly assign gray scale font images for patching in later
    for(i=0;i<how_many;i++){
        which_one[i] = (int)randInRange(0, amount[i]);
    }

    for (y=0; y<=E->in_image.h-E->block_size; y+=E->block_size){
        for (x=0; x<=E->in_image.w-E->block_size; x+=E->block_size){
            // grayscale and average this block
            sum_gray = 0.0;
            count = 0;
            sub_i = y;
            sub_j = x;
            for(i=y;i<y+E->block_size;i++){
                for(j=x;j<x+E->block_size;j++){
                    this_gray = GetPixelGray(
                                    (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0]/255.0,
                                    (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1]/255.0,
                                    (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2]/255.0);

                    sum_gray += this_gray;
                    count++;
                }
            }


            // get the average for this block
            average_gray = sum_gray/(float)(count);
            closest = find_closest(levels, (int)average_gray, 0, how_many-1);
            printf("average_gray = %d, closest is %d (value = %f)\n", (int)average_gray, find_closest(levels, (int)average_gray, 0, 16), levels[find_closest(levels, (int)average_gray, 0, how_many-1)]);

            // now fill in this block with a corresponding gray level fragment

            // randomly pick original image or grayscale type image
            //rnum = randInRange(0.0, 1.0);
            //if(rnum < 0.85){     // grayscale font image
            if(average_gray > 0.0){
                // load up an equivalent grayscale image

                // randomly selected a gray level image to use
                // original
                sprintf(fname,"helvetica_96/%03d/%04d.png", (int)levels[closest], (int)randInRange(0, amount[closest]) );

                // inverting the grayscale value
                //sprintf(fname,"helvetica_96/%03d/%04d.png", (int)levels[ (how_many-1) - closest], (int)randInRange(0, amount[(how_many-1) - closest]) );

                // constant image for gray level:
                //sprintf(fname,"helvetica_96/%03d/%04d.png", (int)levels[closest], which_one[closest] );

                printf("adding: %s\n", fname);
                E->font_gray.data = stbi_load(fname, &E->font_gray.w, &E->font_gray.h, &E->font_gray.n, 0);
                out_i = 0;
                out_j = 0;
            
                for(i=sub_i;i<sub_i+E->block_size;i++){
                    for(j=sub_j;j<sub_j+E->block_size;j++){

                        // copy slices of the original image to the destination
                        // this didn't work as well as I wanted...
                        /*
                        //&& (out_i >32 && out_i < 64)
                        if( (out_j <20) ){  // copy a slice of the original image
                            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] =      E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0];
                            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1];
                            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2];
                            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 3];
                        }
                        */
                        //else{
                            //printf("sub_i = %d, sub_j = %d\n", sub_i, sub_j);fflush(stdout);
                            if( (E->alpha_blend == TRUE) && // only alpha blend on white pixels
                            ( (E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 0]
                               + E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 1]
                               + E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 3]) ==
                               765) ){
                        
                                // color = alpha * src + (1 - alpha) * dest
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] =
                                    E->alpha*E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 0] + (1.0-E->alpha)*E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0];
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] =
                                    E->alpha*E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 1] + (1.0-E->alpha)*E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1];
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] =
                                    E->alpha*E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 2] + (1.0-E->alpha)*E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2];
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = 
                                    E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 3];

                            }
                            else{   // splat - return an image made up of only the fragments
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] = E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 0];
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] = E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 1];
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] = E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 2];
                                E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = E->font_gray.data[(out_i * E->font_gray.w  + out_j) * E->font_gray.n + 3];
                            }
                        //}
                        out_j++;
                    }
                    out_j = 0;
                    out_i++;
                }
            }
            else{
                // copy original block to output block
                for(i=sub_i;i<sub_i+E->block_size;i++){
                    for(j=sub_j;j<sub_j+E->block_size;j++){
                        //printf("sub_i = %d, sub_j = %d\n", sub_i, sub_j);fflush(stdout);
                        E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0];
                        E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1];
                        E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2];
                        E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 3];
                    }
                }
            }

        }
    }
}




int main(int argc, char **argv)
{
    e   *E;
    int i,j;
    int mode;

    int colorShift;
    int sourceChannel;
    int targetChannel;

    // malloc E;
    E = malloc(sizeof(e));
    if(E==NULL){
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }

    // parse command line arguments
	if(argc < 3){
        print_usage();
		//fprintf(stderr,"args: [input file] [output file]\n");
        exit(1);
	}
	else{
        get_cli_args(E, argc, argv);
        //get_command_line_arg_as_string(&E->input_file, argv[1]);
		//get_command_line_arg_as_string(&E->output_file, argv[2]);
	}

    // init random number generator
    srand(time(NULL));

    printf("working on %s...\n", E->input_file);
    //read_png_file(argv[1]);
    // read input image
    E->in_image.data = stbi_load(E->input_file, &E->in_image.w, &E->in_image.h, &E->in_image.n, 0);

    printf("input image data: width = %d, height = %d, components = %d\n",
    E->in_image.w, E->in_image.h, E->in_image.n);

    //E->alpha_blend = TRUE;
    //E->alpha = 0.55;
    
    
    E->block_size = 96; // 96x96 blocks for averaging
    printf("block_size is %d\n", E->block_size);
    // setup the output image
    E->out_image.w = E->in_image.w;
    E->out_image.h = E->in_image.h;
    E->out_image.n = 4;
    E->out_image.data = (unsigned char*)malloc(E->out_image.w*E->out_image.h*E->out_image.n*sizeof(unsigned char));

    // fill the image with black
    for(i=0;i<E->out_image.h;i++){
        for(j=0;j<E->out_image.w;j++){
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] = 0;
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] = 0;
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] = 0;
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = 0;
        }
    }

    // approximate the source image by fragments
    //printf("alpha blend = %d, value = %f\n", E->alpha_blend, E->alpha);
    block_average(E);

    // rescale image if it is too big
    // resize to 256x256 to improve color extraction performance
    // if we do this then we need to change the kmeans stuff below...
    if(E->out_image.w > 192000){
        E->scaled.w = E->out_image.w*0.25;
        E->scaled.h = E->out_image.h*0.25;
        E->scaled.n = E->out_image.n;

        E->scaled.data = (unsigned char *) malloc(E->scaled.w*E->scaled.h*E->scaled.n*sizeof(unsigned char));


        stbir_resize_uint8((const void *)E->out_image.data, E->out_image.w, E->out_image.h , 0,
                                    (const void *)E->scaled.data, E->scaled.w, E->scaled.h, 0, E->scaled.n);
        stbi_write_png(E->output_file, E->scaled.w, E->scaled.h, E->scaled.n, (const void *)E->scaled.data, 0);
    }
    else{
        stbi_write_png(E->output_file, E->out_image.w, E->out_image.h, E->out_image.n, (const void *)E->out_image.data, 0);
    }
    
    return 0;
}
