#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average;
    // loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate the average color (rgb) in the pixel
            average = round(((double) image[i][j].rgbtBlue + (double) image[i][j].rgbtRed + (double) image[i][j].rgbtGreen) / 3);
            // set red, green, and blue (rgb) of the pixel to be the average
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int opposite_index;
    int red_temp;
    int blue_temp;
    int green_temp;
    // loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            opposite_index = width - j - 1;
            // switch each pixel and its opposite counterpart, do nothing if the pixel is on the middle
            if (j < opposite_index)
            {
                // fill tempory holder with the content of the left pixel
                red_temp = image[i][j].rgbtBlue;
                blue_temp = image[i][j].rgbtRed;
                green_temp = image[i][j].rgbtGreen;
                // fill left pixel with the content of right pixel
                image[i][j].rgbtBlue = image[i][opposite_index].rgbtBlue;
                image[i][j].rgbtRed = image[i][opposite_index].rgbtRed;
                image[i][j].rgbtGreen = image[i][opposite_index].rgbtGreen;
                // fill right pixel with the content of left pixel
                image[i][opposite_index].rgbtBlue = red_temp;
                image[i][opposite_index].rgbtRed = blue_temp;
                image[i][opposite_index].rgbtGreen = green_temp;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of image called image_copy
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
            image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    // loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // revert counters to 0
            double sum_red = 0;
            double sum_blue = 0;
            double sum_green = 0;
            double count = 0;

            // calculate blur using image_copy
            if (j - 1 >= 0)
            {
                // right
                sum_red = sum_red + image_copy[i][j - 1].rgbtRed;
                sum_blue = sum_blue + image_copy[i][j - 1].rgbtBlue;
                sum_green = sum_green + image_copy[i][j - 1].rgbtGreen;
                count ++;
                if (i - 1 >= 0)
                {
                    // right up
                    sum_red = sum_red + image_copy[i - 1][j - 1].rgbtRed;
                    sum_blue = sum_blue + image_copy[i - 1][j - 1].rgbtBlue;
                    sum_green = sum_green + image_copy[i - 1][j - 1].rgbtGreen;
                    count ++;
                }
                if (i + 1 < height)
                {
                    // right down
                    sum_red = sum_red + image_copy[i + 1][j - 1].rgbtRed;
                    sum_blue = sum_blue + image_copy[i + 1][j - 1].rgbtBlue;
                    sum_green = sum_green + image_copy[i + 1][j - 1].rgbtGreen;
                    count ++;
                }
            }
            if (j + 1 < width)
            {
                // left
                sum_red = sum_red + image_copy[i][j + 1].rgbtRed;
                sum_blue = sum_blue + image_copy[i][j + 1].rgbtBlue;
                sum_green = sum_green + image_copy[i][j + 1].rgbtGreen;
                count ++;
                if (i - 1 >= 0)
                {
                    // left up
                    sum_red = sum_red + image_copy[i - 1][j + 1].rgbtRed;
                    sum_blue = sum_blue + image_copy[i - 1][j + 1].rgbtBlue;
                    sum_green = sum_green + image_copy[i - 1][j + 1].rgbtGreen;
                    count ++;
                }
                if (i + 1 < height)
                {
                    // left down
                    sum_red = sum_red + image_copy[i + 1][j + 1].rgbtRed;
                    sum_blue = sum_blue + image_copy[i + 1][j + 1].rgbtBlue;
                    sum_green = sum_green + image_copy[i + 1][j + 1].rgbtGreen;
                    count ++;
                }
            }
            if (i - 1 >= 0)
            {
                // up
                sum_red = sum_red + image_copy[i - 1][j].rgbtRed;
                sum_blue = sum_blue + image_copy[i - 1][j].rgbtBlue;
                sum_green = sum_green + image_copy[i - 1][j].rgbtGreen;
                count ++;
            }
            if (i + 1 < height)
            {
                // down
                sum_red = sum_red + image_copy[i + 1][j].rgbtRed;
                sum_blue = sum_blue + image_copy[i + 1][j].rgbtBlue;
                sum_green = sum_green + image_copy[i + 1][j].rgbtGreen;
                count ++;
            }
            sum_red = sum_red + image_copy[i][j].rgbtRed;
            sum_blue = sum_blue + image_copy[i][j].rgbtBlue;
            sum_green = sum_green + image_copy[i][j].rgbtGreen;
            count ++;

            // apply blur to image
            image[i][j].rgbtRed = round(sum_red / count);
            image[i][j].rgbtBlue = round(sum_blue / count);
            image[i][j].rgbtGreen = round(sum_green / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of image called image_copy
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j].rgbtRed = image[i][j].rgbtRed;
            image_copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    // loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialize counters
            double sum_red_gx = 0;
            double sum_blue_gx = 0;
            double sum_green_gx = 0;

            double sum_red_gy = 0;
            double sum_blue_gy = 0;
            double sum_green_gy = 0;

            double red_edge = 0;
            double blue_edge = 0;
            double green_edge = 0;

            // ...
            // calculate gx and gy
            if (j - 1 >= 0)
            {
                // left
                sum_red_gx = sum_red_gx + (-2 * image_copy[i][j - 1].rgbtRed);
                sum_blue_gx = sum_blue_gx + (-2 * image_copy[i][j - 1].rgbtBlue);
                sum_green_gx = sum_green_gx + (-2 * image_copy[i][j - 1].rgbtGreen);

                sum_red_gy = sum_red_gy + (0 * image_copy[i][j - 1].rgbtRed);
                sum_blue_gy = sum_blue_gy + (0 * image_copy[i][j - 1].rgbtBlue);
                sum_green_gy = sum_green_gy + (0 * image_copy[i][j - 1].rgbtGreen);
                // left up
                if (i - 1 >= 0)
                {
                    sum_red_gx = sum_red_gx + (-1 * image_copy[i - 1][j - 1].rgbtRed);
                    sum_blue_gx = sum_blue_gx + (-1 * image_copy[i - 1][j - 1].rgbtBlue);
                    sum_green_gx = sum_green_gx + (-1 * image_copy[i - 1][j - 1].rgbtGreen);

                    sum_red_gy = sum_red_gy + (-1 * image_copy[i - 1][j - 1].rgbtRed);
                    sum_blue_gy = sum_blue_gy + (-1 * image_copy[i - 1][j - 1].rgbtBlue);
                    sum_green_gy = sum_green_gy + (-1 * image_copy[i - 1][j - 1].rgbtGreen);
                }
                // left down
                if (i + 1 < height)
                {
                    sum_red_gx = sum_red_gx + (-1 * image_copy[i + 1][j - 1].rgbtRed);
                    sum_blue_gx = sum_blue_gx + (-1 * image_copy[i + 1][j - 1].rgbtBlue);
                    sum_green_gx = sum_green_gx + (-1 * image_copy[i + 1][j - 1].rgbtGreen);

                    sum_red_gy = sum_red_gy + (1 * image_copy[i + 1][j - 1].rgbtRed);
                    sum_blue_gy = sum_blue_gy + (1 * image_copy[i + 1][j - 1].rgbtBlue);
                    sum_green_gy = sum_green_gy + (1 * image_copy[i + 1][j - 1].rgbtGreen);
                }
            }
            if (j + 1 < width)
            {
                //right
                sum_red_gx = sum_red_gx + (2 * image_copy[i][j + 1].rgbtRed);
                sum_blue_gx = sum_blue_gx + (2 * image_copy[i][j + 1].rgbtBlue);
                sum_green_gx = sum_green_gx + (2 * image_copy[i][j + 1].rgbtGreen);

                sum_red_gy = sum_red_gy + (0 * image_copy[i][j + 1].rgbtRed);
                sum_blue_gy = sum_blue_gy + (0 * image_copy[i][j + 1].rgbtBlue);
                sum_green_gy = sum_green_gy + (0 * image_copy[i][j + 1].rgbtGreen);
                // right up
                if (i - 1 >= 0)
                {
                    sum_red_gx = sum_red_gx + (1 * image_copy[i - 1][j + 1].rgbtRed);
                    sum_blue_gx = sum_blue_gx + (1 * image_copy[i - 1][j + 1].rgbtBlue);
                    sum_green_gx = sum_green_gx + (1 * image_copy[i - 1][j + 1].rgbtGreen);

                    sum_red_gy = sum_red_gy + (-1 * image_copy[i - 1][j + 1].rgbtRed);
                    sum_blue_gy = sum_blue_gy + (-1 * image_copy[i - 1][j + 1].rgbtBlue);
                    sum_green_gy = sum_green_gy + (-1 * image_copy[i - 1][j + 1].rgbtGreen);
                }
                // right down
                if (i + 1 < height)
                {
                    sum_red_gx = sum_red_gx + (1 * image_copy[i + 1][j + 1].rgbtRed);
                    sum_blue_gx = sum_blue_gx + (1 * image_copy[i + 1][j + 1].rgbtBlue);
                    sum_green_gx = sum_green_gx + (1 * image_copy[i + 1][j + 1].rgbtGreen);

                    sum_red_gy = sum_red_gy + (1 * image_copy[i + 1][j + 1].rgbtRed);
                    sum_blue_gy = sum_blue_gy + (1 * image_copy[i + 1][j + 1].rgbtBlue);
                    sum_green_gy = sum_green_gy + (1 * image_copy[i + 1][j + 1].rgbtGreen);
                }
            }
            // up
            if (i - 1 >= 0)
            {
                sum_red_gx = sum_red_gx + (0 * image_copy[i - 1][j].rgbtRed);
                sum_blue_gx = sum_blue_gx + (0 * image_copy[i - 1][j].rgbtBlue);
                sum_green_gx = sum_green_gx + (0 * image_copy[i - 1][j].rgbtGreen);

                sum_red_gy = sum_red_gy + (-2 * image_copy[i - 1][j].rgbtRed);
                sum_blue_gy = sum_blue_gy + (-2 * image_copy[i - 1][j].rgbtBlue);
                sum_green_gy = sum_green_gy + (-2 * image_copy[i - 1][j].rgbtGreen);
            }
            // down
            if (i + 1 < height)
            {
                sum_red_gx = sum_red_gx + (0 * image_copy[i + 1][j].rgbtRed);
                sum_blue_gx = sum_blue_gx + (0 * image_copy[i + 1][j].rgbtBlue);
                sum_green_gx = sum_green_gx + (0 * image_copy[i + 1][j].rgbtGreen);

                sum_red_gy = sum_red_gy + (2 * image_copy[i + 1][j].rgbtRed);
                sum_blue_gy = sum_blue_gy + (2 * image_copy[i + 1][j].rgbtBlue);
                sum_green_gy = sum_green_gy + (2 * image_copy[i + 1][j].rgbtGreen);
            }

            // ...
            // agreggate gx and gy
            red_edge = sqrt(pow(sum_red_gx, 2) + pow(sum_red_gy, 2));
            blue_edge = sqrt(pow(sum_blue_gx, 2) + pow(sum_blue_gy, 2));
            green_edge = sqrt(pow(sum_green_gx, 2) + pow(sum_green_gy, 2));

            // change pixel
            // for red
            if (red_edge < 0)
            {
                image[i][j].rgbtRed = 0;
            }
            else if (red_edge > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(red_edge);
            }
            // for blue
            if (blue_edge < 0)
            {
                image[i][j].rgbtBlue = 0;
            }
            else if (blue_edge > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(blue_edge);
            }
            // for green
            if (green_edge < 0)
            {
                image[i][j].rgbtGreen = 0;
            }
            else if (green_edge > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(green_edge);
            }
        }
    }
    return;
}