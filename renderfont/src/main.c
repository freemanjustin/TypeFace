/*

  renderfont will render characters from a ttf file to individual png files
  this is a preprocessing step for the Type Fragment Exemperiments conducted under
  the renderimage code. 

  usage:

  the text that is rendered is hard coded into the 
    char *text string 
  each character in this string is written out to a png file with the character name as output

  the font to use is harcoded below (search for *.ttf)

  on osx, the system fonts, like Helvetica, are stored in /System/Library/Fonts
  some fonts need to be unpacked from *.dfont format to .ttf format

  I used this code to do this: https://peter.upfold.org.uk/projects/dfontsplitter
  there is probably a github project just for this but i never bothered to look any further.


  the size of the output images is
    width * height 
  which is #defined below

  the font size is called
    line_height 
  which is a rather odd name


  to run:
    ./renderfont

  the output will appear in the same directory
  
*/


#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h" /* http://nothings.org/stb/stb_truetype.h */

#define width 1024   
#define height 1024
#define line_height 600

char buffer[24<<20];
unsigned char screen[height][width];

int main(int arg, char **argv)
{
   stbtt_fontinfo font;
   int i,j,ascent,baseline,ch=0;
   float scale, xpos=50;
   char *text = "abcdefghijklmnopqrstuvwxyz";
   unsigned char *outimage;

   int  characters;
   char fname_mono[128];
   char fname[128];


   float  r,g,b,a;

   // pick a taget color for the text
   r = 0.0;
   g = 0.0;
   b = 0.0;
   a = 1.0;


   if ((outimage = (unsigned char*) malloc(height*width*4*sizeof(unsigned char)) ) == NULL) {
        printf("failed to allocate memory for image \n"); exit(1);
    }

   //fread(buffer, 1, 1000000, fopen("Roboto.ttf", "rb"));
   
   fread(buffer, 1, 1000000, fopen("HelveticaBold.ttf", "rb"));
   stbtt_InitFont(&font, buffer, 0);

   scale = stbtt_ScaleForPixelHeight(&font, line_height);
   stbtt_GetFontVMetrics(&font, &ascent,0,0);
   baseline = (int) (ascent*scale);

   for(characters = 0; characters < 26; characters++){

     printf("%c.png\n", text[characters]);
     sprintf(fname, "%c.png", text[characters]);            // 4 channel image derived from 1 channel image
     sprintf(fname_mono, "%c_mono.png", text[characters]);  // 1 channel image

      // initialize the image color
      for (j=0; j < height; ++j) {
          for (i=0; i < width; ++i){
            /*
            image[ (j * width  + i) * 4 + 0] = r;
            image[ (j * width  + i) * 4 + 1] = g;
            image[ (j * width  + i) * 4 + 2] = b;
            image[ (j * width  + i) * 4 + 3] = 255;
            */
            screen[j][i] = 0;
            outimage[j * width * 4 + i * 4 + 0] = r;
            outimage[j * width * 4 + i * 4 + 1] = g;
            outimage[j * width * 4 + i * 4 + 2] = b;
            outimage[j * width * 4 + i * 4 + 3] = a;
          }
        }

        //stbi_write_png("out_init.png", width, height, 4, image, 0);

      /*
      while (text[ch]) {
          int advance,lsb,x0,y0,x1,y1;
          float x_shift = xpos - (float) floor(xpos);
          stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
          stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale,scale,x_shift,0, &x0,&y0,&x1,&y1);

          // original
          stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int) xpos + x0], x1-x0,y1-y0, width, scale,scale,x_shift,0, text[ch]);

          // note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
          // because this API is really for baking character bitmaps into textures. if you want to render
          // a sequence of characters, you really need to render each bitmap to a temp buffer, then
          // "alpha blend" that into the working buffer
          xpos += ( advance * scale);
          if (text[ch+1])
            xpos += scale*stbtt_GetCodepointKernAdvance(&font, text[ch],text[ch+1]);
          ++ch;
      }
      */

      
      int advance,lsb,x0,y0,x1,y1;
      float x_shift = xpos - (float) floor(xpos);
      stbtt_GetCodepointHMetrics(&font, text[characters], &advance, &lsb);
      stbtt_GetCodepointBitmapBoxSubpixel(&font, text[characters], scale,scale,x_shift,0, &x0,&y0,&x1,&y1);

      // original
      stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int) xpos + x0], x1-x0,y1-y0, width, scale,scale,x_shift,0, text[characters]);

      /* save out a 1 channel image */
      //stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
      stbi_write_png(fname_mono, width, height, 1, screen, width);

      // make monobitmap into a RGBA premultiplied aplpha image
      // the target is color multiplied by the monochrome value
      // we are just using the result from the monochrome image as the alpha value
      for (j=0; j < height; ++j) {
          for (i=0; i < width; ++i){
            a = (float)screen[j][i]/255.0;
            //printf("r = %f, g = %f, b =%f monochrome = %f\n", r, g, b,a);

            outimage[ j * width * 4 + i * 4 + 0] = (unsigned char)(r);
            //printf("r = %f, screen = %d, a = %f\n", r,screen[j][i],a );
            //printf("r*a = %f\n", r*a);
            //printf("j=%d, i=%d, image.r = %u\n", j,i,outimage[ j * width * 4 + i * 4 + 0]);
            outimage[ j * width * 4 + i * 4 + 1] = (unsigned char)(g);
            //printf("j=%d, i=%d, image.g = %u\n", j,i,outimage[j * width * 4 + i * 4 + 1]);
            outimage[ j * width * 4 + i * 4 + 2] = (unsigned char)(b);
            //printf("j=%d, i=%d, image.b = %u\n", j,i,outimage[ j * width * 4 + i * 4 + 2]);
            outimage[ j * width * 4 + i * 4 + 3] = (unsigned char)(a*255.0);
            //printf("j=%d, i=%d, image.a = %u\n", j,i,outimage[j * width * 4 + i * 4 + 3]);
          }
      }
      /* save out a 4 channel RGBA image */
      //stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
      stbi_write_png(fname, width, height, 4, outimage, 0);
   }

   return 0;
}
