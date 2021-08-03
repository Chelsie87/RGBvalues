//Chelsie Yan
//CSIT 
//Assignement 2
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main() {

	//variables
	string header;
	string p;
	int width = 0;
	int height = 0;
	int color_values = 0;

	//declare input image and outputs
	ifstream input("input.ppm", ios_base::in | ios_base::binary);
	ofstream output_1("output_1.ppm", ios_base::out | ios_base::binary);
	ofstream output_2("output_2.ppm", ios_base::out | ios_base::binary);
	ofstream output_3("output_3.ppm", ios_base::out | ios_base::binary);
	ofstream bargraph("bargraph.ppm", ios_base::out | ios_base::binary);

	//get header
	getline(input, header);

	//get the width, height, and amount of color channels
	string s_p, s_w, s_h, s_c;
	s_p = header.substr(0, 2);
	s_w = header.substr(3, 3);
	s_h = header.substr(7, 3);
	s_c = header.substr(11, 3);

	width = stoi(s_w);
	height = stoi(s_h);
	color_values = stoi(s_c);

	//use pointers to create 3 matrixcies
	//matrix for first channel (Red) 
	int** in_matrix_ch1 = new int* [height];
	for (int i = 0; i < height; ++i)
		in_matrix_ch1[i] = new int[width];

	//matrix for second channel (Green)
	int** in_matrix_ch2 = new int* [height];
	for (int i = 0; i < height; ++i)
		in_matrix_ch2[i] = new int[width];

	//matrix for thrid channel (Blue)
	int** in_matrix_ch3 = new int* [height];
	for (int i = 0; i < height; ++i)
		in_matrix_ch3[i] = new int[width];

	char r, g, b;

	//get the colors from input image + store in the matricies
	for (int i_h = 0; i_h < height; i_h++) {
		for (int i_w = 0; i_w < width; i_w++) {

			input.get(r);
			input.get(g);
			input.get(b);
			in_matrix_ch1[i_h][i_w] = int(r);
			in_matrix_ch2[i_h][i_w] = int(g);
			in_matrix_ch3[i_h][i_w] = int(b);

		}
	}
	//------------------------------------------------------------------------------
	//1a - top left quadrant
	//divide by 2 to get top left quadrant
	int second_width = width / 2;
	int second_height = height / 2;

	//write out new header
	output_1 << s_p << " " << second_width << " " << second_height << " " << s_c << endl;

	//set it so the matricies only go up till the pixels you want to keep in the output
	//384x256
	for (int i_h = 0; i_h < second_height; i_h++) {
		for (int i_w = 0; i_w < second_width; i_w++) {

			output_1 << (char)in_matrix_ch1[i_h][i_w];
			output_1 << (char)in_matrix_ch2[i_h][i_w];
			output_1 << (char)in_matrix_ch3[i_h][i_w];
		}
	}

	//------------------------------------------------------------------------------
	//1b - center of image
	//192
	int b_width = second_width / 2;
	//128
	int b_height = second_height / 2;

	//get the starting (1) and stopping (2) pixels for width and height
	//192
	int c_1_w = b_width;
	//576
	int c_2_w = second_width + b_width;
	//128
	int c_1_h = b_height;
	//384
	int c_2_h = second_height + b_height;

	//get the exact widths and heights for the header so there are no errors
	int b_header_witdh = c_2_w - c_1_w;
	int b_header_height = c_2_h - c_1_h;

	output_2 << s_p << " " << b_header_witdh << " " << b_header_height << " " << s_c << endl;

	//use the start and stop values above
	for (int i_h = c_1_h; i_h < c_2_h; i_h++) {
		for (int i_w = c_1_w; i_w < c_2_w; i_w++) {

			output_2 << (char)in_matrix_ch1[i_h][i_w];
			output_2 << (char)in_matrix_ch2[i_h][i_w];
			output_2 << (char)in_matrix_ch3[i_h][i_w];
		}
	}

	//------------------------------------------------------------------------------
	//1c - reverse the image (I assume flip vertically and horizontally)
	output_3 << s_p << " " << width << " " << height << " " << s_c << endl;

	for (int i_h = height - 1; i_h > -1; i_h--) {
		for (int i_w = width - 1; i_w > -1; i_w--) {

			output_3 << (char)in_matrix_ch1[i_h][i_w];
			output_3 << (char)in_matrix_ch2[i_h][i_w];
			output_3 << (char)in_matrix_ch3[i_h][i_w];
		}
	}

	//------------------------------------------------------------------------------
	//2
	//To hold all 3 values (0-255)
	int height2 = 768;
	int width2 = 1050;

	//keep track of which value from -128-127
	int counter = -128;
	//keep track of how many of n values per color
	int red_freq = 0;
	int green_freq = 0;
	int blue_freq = 0;

	//new matricies so others keep same values
	int** bar1 = new int* [height2];
	for (int i = 0; i < height2; ++i)
		bar1[i] = new int[width2];

	//matrix for second channel (Green)
	int** bar2 = new int* [height2];
	for (int i = 0; i < height2; ++i)
		bar2[i] = new int[width2];

	//matrix for thrid channel (Blue)
	int** bar3 = new int* [height2];
	for (int i = 0; i < height2; ++i)
		bar3[i] = new int[width2];

	//Set the new matrix to full white
	for (int i_h = 0; i_h < height2; i_h++) {
		for (int i_w = 0; i_w < width2; i_w++) {

			bar1[i_h][i_w] = 255;
			bar2[i_h][i_w] = 255;
			bar3[i_h][i_w] = 255;
		}
	}

	//go through whole image (by file values)
	//When you get to 128, that would be the 256 value so you would stop
	while (counter != 128) {
		for (int i_h = 0; i_h < height; i_h++) {
			for (int i_w = 0; i_w < width; i_w++) {

				//if the int value of input image R matrix == -128, -127,...,117
				if (in_matrix_ch1[i_h][i_w] == counter) {
					red_freq++;
				}

				if (in_matrix_ch2[i_h][i_w] == counter) {
					green_freq++;
				}

				if (in_matrix_ch3[i_h][i_w] == counter) {
					blue_freq++;
				}
			}

		}

		//We need to make sure the counter is positive for inputing/outputing the matricies
		unsigned int positive_counter = 0;
		//This would make (counter = -128 === positive_counter = 0), and so forth all the way to counter = 127 === positive_counter = 255
		positive_counter = counter + 128;

			//divide the frequency in half in case it is too large
			if (red_freq > 16) {
				red_freq = red_freq / 16;
			}
			if (green_freq > 16) {
				green_freq = green_freq / 16;
			}
			if (blue_freq > 16) {
				blue_freq = blue_freq / 16;
			}

			//in the bargraph ppm, we update the matricies 
			//The positive_counter will be the height, while the freq will be the width
			//Then it will start again for the next horizontal pixel line down
			//Set the other values to 0 so the focus is only on the respective matrix
			//Red matrix 
			for (int j = 0; j < red_freq; j++) {
				bar1[positive_counter][j] = positive_counter;
				bar2[positive_counter][j] = 0;
				bar3[positive_counter][j] = 0;
			}

			//Green
			for (int j = 0; j < green_freq; j++) {
				bar1[positive_counter + 256][j] = 0;
				bar2[positive_counter + 256][j] = positive_counter;
				bar3[positive_counter + 256][j] = 0;
			}

			//Blue
			for (int j = 0; j < blue_freq; j++) {
				bar1[positive_counter + 512][j] = 0;
				bar2[positive_counter + 512][j] = 0;
				bar3[positive_counter + 512][j] = positive_counter;
			}

			//inc the counter to continue down 1 pixel for the output
			counter++;
			//reset the frequencies for the next line
			red_freq = 0;
			green_freq = 0;
			blue_freq = 0;
	}

		//output for barcode.ppm
 		bargraph << s_p << " " << width2 << " " << height2 << " " << s_c << endl;

		for (int i_h = 0; i_h < height2; i_h++) {
			for (int i_w = 0; i_w < width2; i_w++) {

				bargraph << (char)bar1[i_h][i_w];
				bargraph << (char)bar2[i_h][i_w];
				bargraph << (char)bar3[i_h][i_w];
			}
		}

	system("pause");
	return 0;
	
}