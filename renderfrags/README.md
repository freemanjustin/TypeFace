# renderfrags

renderfrags will render sub images from a source image in ```block_size x block_size``` blocks.

the block size is hardcoded in main() and I have used 96x96 blocks for the StreetType work. The fragment searching happens in the function get_approx_grayscale().

The ```get_approx_grayscale()``` function will search over the source image in ```block_size/16``` increments which produces a bunch of fragments. Keep in mind that the source image should be a reasonable size so that the fragment searching is successful (for example, if you set it up to fid larger fragments, then the source image should also be larger).

Sometimes, the search does not find a result. This usually happens when asking for all white or all black fragments. In theses cases I just make an all black or all white png image file of the correct fragment size and manually add it to the fragment set.

# run

To run

```
./renderfrags [input file] [output file] [gray level]
```
which will search over the ```input file``` for fragments which match the ```gray level``` and write them out as ```output file_%04d.png```. For a single run, depending on the block size and the source image size, you could get hunderds of fragments generated.

The bin directory includes my python script for generating the fragments for the TypeFace and StreetType projects.

# sample output

![output sample](https://raw.github.com/freemanjustin/TypeFace/master/renderfrags/bin/sample.png)

# contact
- email: freeman.justin@gmail.com
- instagram: ```__freej__```

