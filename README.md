# Final_project

### The project works by running the following line in the terminal:

./convolve "data/input_file" "kernels/deciered_effect.txt" "output/outputfile.pgm

### An example would be:

./convolve data/500x305.pgm kernels/blur.txt output/500x305_blur.pgm

Which will output a blurred version of the 500x305 file in the output folder.

If you want to get other filters just change the name of the file after kernels/

### You can then convert the pgm file to a viewable png file using (this requires imagemagick "sudo apt-get install imagemagick"):

convert output/500x305_blur.pgm output/500x305.png