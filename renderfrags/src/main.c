#include "cmappr.h"

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


int offset( int x, int y, int z ) {
    return ( z * 256 * 256 ) + ( y * 256 ) + x ;
}


float GetPixelGray(float r, float g, float b)
{
    float grayScale;

	//grayScale = (( (r) * 0.3 + (g) * 0.59 + (b) * 0.11) / 3.0) * 255.0;
    grayScale = (( (r) * 0.3 + (g) * 0.59 + (b) * 0.11) ) * 255.0;
    //grayScale = ( r*0.3 + g * 0.59 + b* 0.11);
	//printf("grayscale = %f\n", grayScale);
    return grayScale;
}


void block_average(e *E){
    int x,y;
    int i,j;

    int block_size;
    int count;
    
    double  sum_r, sum_g, sum_b;
    double  ave_r, ave_g, ave_b;
    
    //for (y=0; y<height; y++){
    //    for (x=0; x<width; x++){
    for (y=E->block_size; y<=E->in_image.h-E->block_size; y+=E->block_size){
        for (x=E->block_size; x<=E->in_image.w-E->block_size; x+=E->block_size){

            sum_r = 0.0;
            sum_g = 0.0;
            sum_b = 0.0;
            count = 0;
            for(i=y;i<y+E->block_size;i++){
                for(j=x;j<x+E->block_size;j++){
                    //printf("y = %d, x = %d, i=%d, j = %d\n", y,x,i,j);

                    sum_r += (double)E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0];
                    sum_g += (double)E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1];
                    sum_b += (double)E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2];

                    count++;
                }
            }


            // get the average for this block
            ave_r = sum_r/(double)(count);
            ave_g = sum_g/(double)(count);
            ave_b = sum_b/(double)(count);
            
        
            //printf("count = %d, bs^2 = %d, a_r = %d, a_g = %d, a_b = %d\n", count, (2*block_size)*(2*block_size),(int)ave_r, (int)ave_g, (int)ave_b);
            //exit(1);
            // now fill in this block
            for(i=y-E->block_size;i<y+E->block_size;i++){
                for(j=x-E->block_size;j<x+E->block_size;j++){

                    E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] = ave_r;
                    E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] = ave_g;
                    E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] = ave_b;
                    E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = 255;

                }
            }

        }
    }
}



void get_approx_grayscale(e *E, int gray_value){
    int x,y;
    int i,j;
    int sub_i, sub_j;
    int out_i, out_j;

    int count;
    int block_count;
    
    float   this_gray;
    float   sum_gray;
    float   average_gray;

    int     how_many;
    char    outfilename[128];
    
    //for (y=0; y<height; y++){
    //    for (x=0; x<width; x++){
    

    // scan over this input image in block_size by block_size chuncks
    // and calculate the average grayscale value of this block_average
    // if the average grayscale of this block is equal to input gray_val
    // then copy this block to the output image and keep looking for other blocks that match

    //for (y=E->block_size; y<=E->in_image.h-block_size; y+=E->block_size){
    //    for (x=E->block_size; x<=E->in_image.w-block_size; x+=E->block_size){
    block_count = 0;
    how_many = 0;
    for (y=0; y<=E->in_image.h-E->block_size; y+=E->block_size/16){
        for (x=0; x<=E->in_image.w-E->block_size; x+=E->block_size/16){
            sum_gray = 0.0;
            count = 0;
            sub_i = y;
            sub_j = x;
            for(i=y;i<y+E->block_size;i++){
                for(j=x;j<x+E->block_size;j++){
                    //printf("y = %d, x = %d, i=%d, j = %d\n", y,x,i,j);

                    // for color images
                    /*
                    this_gray = GetPixelGray( 
                                    (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0]/255.0,
                                    (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1]/255.0, 
                                    (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2]/255.0);
                    */
                    // for type render (already grayscaled)
                    this_gray = (float) E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0];
                    //printf("block %d: pixel this_gray = %f\n", block_count, this_gray);
                    sum_gray += this_gray;
                    count++;
                }
            }


            // get the average for this block
            average_gray = sum_gray/(float)(count);
            printf("block %d: average_gray = %f, target gray_value = %d\n", block_count, average_gray, gray_value);

            //if( ((int)average_gray >= gray_value - 5) || ((int)average_gray <= gray_value + 5) ){
            if( (int) average_gray == gray_value) {
                //printf("count = %d, bs^2 = %d, a_r = %d, a_g = %d, a_b = %d\n", count, (2*block_size)*(2*block_size),(int)ave_r, (int)ave_g, (int)ave_b);
                //exit(1);
                // now fill in this block
                out_i = 0;
                out_j = 0;
                for(i=sub_i;i<sub_i+E->block_size;i++){
                    for(j=sub_j;j<sub_j+E->block_size;j++){

                        E->out_image.data[(out_i * E->out_image.w  + out_j) * E->out_image.n + 0] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 0];
                        E->out_image.data[(out_i * E->out_image.w  + out_j) * E->out_image.n + 1] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 1];
                        E->out_image.data[(out_i * E->out_image.w  + out_j) * E->out_image.n + 2] = E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 2];
                        E->out_image.data[(out_i * E->out_image.w  + out_j) * E->out_image.n + 3] = 255.0;//E->in_image.data[(i * E->in_image.w  + j) * E->in_image.n + 3];
                        out_j++;
                    }
                    out_j = 0;
                    out_i++;
                }
                //return;
                // write this out to disk and then keep looking
                sprintf(outfilename,"%s_%03d_%04d.png", E->output_file, E->gray_value, how_many);
                stbi_write_png(outfilename, E->out_image.w, E->out_image.h, E->out_image.n, (const void *)E->out_image.data, 0);
                how_many++;
                if( (E->gray_value == 0) || (E->gray_value == 255))
                    return;

            }
        }
        block_count++;
    }
}

// test case
void subset(e *E){

    int i,j;
    int sub_i, sub_j;
    int out_i, out_j;
    int count;

    sub_i = 0;
    sub_j = 0;

    
    // should extract the top 128x128 pixels from the image

    printf("test subset: block_size = %d\n", E->block_size);
    count = 0;
    out_i = 0;
    out_j = 0;
    for (i = sub_i; i < sub_i + E->block_size; i++){
        for (j = sub_j; j < sub_j+E->block_size; j++){

            /*
            printf("input pixel %d (%d, %d, %d, %d)\n", count, 
                    E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 0],
                    E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 1],
                    E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 2],
                    E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 3]);
            */
            E->out_image.data[(out_i * E->out_image.w + out_j) * E->out_image.n + 0] = E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 0];
            E->out_image.data[(out_i * E->out_image.w + out_j) * E->out_image.n + 1] = E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 1];
            E->out_image.data[(out_i * E->out_image.w + out_j) * E->out_image.n + 2] = E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 2];
            E->out_image.data[(out_i * E->out_image.w + out_j) * E->out_image.n + 3] = E->in_image.data[(i * E->in_image.w + j) * E->in_image.n + 3];
            out_j++;
            count++;
        }
        out_j = 0;
        out_i++;
    }


}

int main(int argc, char **argv)
{
    e   *E;
    int i,j;
    int mode;
    

    // malloc E;
    E = malloc(sizeof(e));
    if(E==NULL){
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }

    // parse command line arguments
	if(argc < 4){
		fprintf(stderr,"args: [input file] [output file] [gray level]\n");
        exit(1);
	}
	else{
        get_command_line_arg_as_string(&E->input_file, argv[1]);
		get_command_line_arg_as_string(&E->output_file, argv[2]);
		get_command_line_arg_as_int(&E->gray_value, argv[3]);
	}

    printf("working on %s. generating %d average graylevel image\n", E->input_file, E->gray_value);
    //read_png_file(argv[1]);
    // read input image
    E->in_image.data = stbi_load(E->input_file, &E->in_image.w, &E->in_image.h, &E->in_image.n, 0);

    printf("input image data: width = %d, height = %d, components = %d\n", 
    E->in_image.w, E->in_image.h, E->in_image.n);
  

    // set the run mode:
    // mode == 0 means we are doing color averaging - this will just color average the pixels and write the average out to file
    //          mostly just for testing.
    // mode == 1 find approximate subimage that matches a specified grayscale value - this is the main work function 
    mode = 1;
    E->block_size = 16; // 16x16 blocks for averaging
    if(mode == 0){
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
        
            // set the output image dimensions and the block size
            // if we are just color averaging then set the output image size to the original size
            block_average(E);
    }
    else if(mode == 1){
        // setup the output image
        E->block_size = 96; // will make a 96x95 pixel size fragment

        E->out_image.w = E->block_size;
        E->out_image.h = E->block_size;
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
        
            // test image subset
            //subset(E);

            // set the output image dimensions and the block size
            // if we are just color averaging then set the output image size to the original size
            // 0 is black
            // 255 is white
            get_approx_grayscale(E, E->gray_value);


    }
    // write output image
    //stbi_write_png(E->output_file, E->out_image.w, E->out_image.h, E->out_image.n, (const void *)E->out_image.data, 0);

    return 0;
}
