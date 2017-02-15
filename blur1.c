/**************************************************************

	The program reads an BMP image file and creates a new
	image that is the negative of the input file.

**************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include "qdbmp.c"

/* Creates a negative image of the input bitmap file */
int main( int argc, char* argv[] )
{
	UCHAR	r, g, b;
	UINT	width, height;
	UINT	x, y;
	BMP*	bmp;
	int i, j;
	/* Check arguments */
	if ( argc != 4 )
	{
		fprintf( stderr, "Usage: %s <input file> <output file>\n", argv[ 0 ] );
		return 0;
	}

	/* Read an image file */
	bmp = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stdout, -1 );
	char* temp = argv[3];
	int box = atoi(temp);
	printf("%d\n", box);

	width = BMP_GetWidth( bmp );
	height = BMP_GetHeight( bmp );
	

	for ( x = 0 ; x < width; ++x )
	{
		for ( y = 0 ; y < height; ++y ) {
			int x_low = (x - box);
			int y_low = (y - box); //the dimension of the starting edge of the box

			int totalr = 0;
			int totalg = 0;
			int totalb = 0;
			int totalboxes = 0;

			int xlength = x_low+2*box+1; //the length of the box
			int ylength = y_low+2*box+1;


			for (i = x_low; i < xlength; ++i) {
					for (j = y_low; j < ylength; ++j) {

						if (i < 0 || j < 0 || i >= width || j>= height) {
							//ignore

						} else {
					totalboxes++;

						/* Get pixel's RGB values */
					BMP_GetPixelRGB( bmp, i, j, &r, &g, &b );
					totalr = totalr + r;

					totalb = totalb + b;
					totalg = totalg + g;

					}
					}
					}
					int red = totalr/totalboxes;
					int blue = totalb/totalboxes;
					int green = totalg/totalboxes;

					BMP_SetPixelRGB( bmp, x, y, red, green, blue);
		}
	}

	/* Save result */
	BMP_WriteFile( bmp, argv[ 2 ] );
	BMP_CHECK_ERROR( stdout, -2 );


	/* Free all memory allocated for the image */
	BMP_Free( bmp );

	return 0;
}
