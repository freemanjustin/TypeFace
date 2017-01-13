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
