# Blurring-BMP-Files
The project has two blurring files, one that blurs a BMP image sequentially, and the other that uses multithreading to blur the image.
 The user must give 4 arguments for the sequential blur file (blur1.c): 
    call-to-obj-file imagetoblur.bmp newblurredimage.bmp boxsizeforblur 
 The user must give 3 arguments for the multithreading blur file (blur2.c): 
    call-to-obj-file imagetoblur.bmp newblurredimage.bmp boxsizeforblur numberofthreads
    
 These two files make use of the QDBMP library for working with the BMP files.
 
 This was done as part of CIT595 at Upenn, with Prof. Chris Murphy. 
