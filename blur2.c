/**************************************************************

	The program reads an BMP image file and creates a new
	image that is the negative of the input file.

**************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include "qdbmp.c"
#include <pthread.h>

void* blur_two(void* start);
void* blur_one(void* start);

int	width, height;
BMP*	bmp;

int length1;
int box;
pthread_mutex_t lock1, lock2;
/* Creates a negative image of the input bitmap file */
int main( int argc, char* argv[] )
{

	int i;
	int j;

	/* Check arguments */
	if ( argc != 5 )
	{
		fprintf( stderr, "Usage: %s <input file> <output file>\n", argv[ 0 ] );
		return 0;
	}

char* threads = argv[4];
int num_thread = atoi(threads);
pthread_t thread[num_thread];
int start[num_thread];
void* start_p[num_thread];

if (num_thread < 1) {
	printf("Invalid request. The program is terminating.\n");
	return -1;
}
	/* Read an image file */

	bmp = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stdout, -1 );
	char* temp = argv[3];
	box = atoi(temp);

	/* Get image's dimensions */
	width = BMP_GetWidth( bmp );
	height = BMP_GetHeight( bmp );
	length1 = width/num_thread;

	if (width%num_thread == 0) {

		for(i = 0; i < num_thread; i++) {

			start[i] = (width/num_thread) * i;

			start[i] = start[i];
			start[i] = start[i] - 1; //trying to fix the lines that show up when blurring, does not work
  		start_p[i] = &start[i];

			pthread_create(&thread[i], NULL, &blur_one, (void*) start_p[i]);

			}
		}
		//if the boxes are not even, the first number-1 boxes are even size, the last one takes care of the remainder
		else {
 for (i = 0; i < num_thread-1; ++i) {

	 	start[i] =(width/num_thread) *i;
		start[i] = start[i] - 1;
			start_p[i] = &start[i];

		pthread_create(&thread[i], NULL, &blur_one, (void*) start_p[i]);
 		}
		start[i]= start[i-1] + length1;
		start[i] = start[i] - 1;

		start_p[i] = &start[i];
		pthread_create(&thread[i], NULL, &blur_two, (void*) start_p[i]);
		}


		for (i = 0; i < num_thread; i++) {
			pthread_join(thread[i], NULL);
		}


	/* Save result */
	BMP_WriteFile( bmp, argv[ 2 ] );
	BMP_CHECK_ERROR( stdout, -2 );


	/* Free all memory allocated for the image */
	BMP_Free( bmp );

	return 0;
}


/*
* when the blur box size is evenly divided
*/
void* blur_one(void* start) {
UCHAR	r, g, b;
int x, y;
int i, j;
int start_pixel = *(int*)start;
int move_length= start_pixel + length1;

	for ( x = start_pixel ; x < move_length; ++x ) {
		for ( y = 0 ; y < height; ++y ) {
			int x_low = (x - box);
			int y_low = (y - box);

			int totalr = 0;
			int totalg = 0;
			int totalb = 0;
			int totalboxes = 0;

			int xlength = x_low+2*box+1;
			int ylength = y_low+2*box+1;


			for (i = x_low; i < xlength; ++i) {
					for (j = y_low; j < ylength; ++j) {

						if (i < 0 || j < 0 || i >= width || j>= height) {
							//ignore

						}
						else {
					totalboxes++;

					pthread_mutex_lock(&lock1);
					BMP_GetPixelRGB( bmp, i, j, &r, &g, &b );
					pthread_mutex_unlock(&lock1);
					totalr = totalr + r;

					totalb = totalb + b;
					totalg = totalg + g;

						}
					}
				}
					int red = totalr/totalboxes;
					int blue = totalb/totalboxes;
					int green = totalg/totalboxes;
					pthread_mutex_lock(&lock2);
					BMP_SetPixelRGB( bmp, x, y, red, green, blue);
						pthread_mutex_unlock(&lock2);

		}
	}

}

//this takes care of the remainder of the pixels
void* blur_two(void* start) {
	UCHAR	r, g, b;
	int x, y;
	int i, j;
	int start_pixel = *(int*)start;


	for ( x = start_pixel ; x < width; ++x ) {
		for ( y = 0 ; y < height; ++y ) {
			int x_low = (x - box);
			int y_low = (y - box);

			int totalr = 0;
			int totalg = 0;
			int totalb = 0;
			int totalboxes = 0;

			int xlength = x_low+2*box+1;
			int ylength = y_low+2*box+1;

			for (i = x_low; i < xlength; ++i) {
					for (j = y_low; j < ylength; ++j) {

						if (i < 0 || j < 0 || i >= width || j>= height) {
							//ignore

						}
						else {
					totalboxes++;

				//	pthread_mutex_lock(&lock1);  - commented out because helgrind becomes too slow
					BMP_GetPixelRGB( bmp, i, j, &r, &g, &b );
					//pthread_mutex_unlock(&lock1);  - commented out because helgrind becomes too slow
					totalr = totalr + r;

					totalb = totalb + b;
					totalg = totalg + g;

						}
					}
				}
					int red = totalr/totalboxes;
					int blue = totalb/totalboxes;
					int green = totalg/totalboxes;

					//pthread_mutex_lock(&lock2);  - commented out because helgrind becomes too slow
					BMP_SetPixelRGB( bmp, x, y, red, green, blue);
					//pthread_mutex_unlock(&lock2);  - commented out because helgrind becomes too slow
			}
			}
		}
