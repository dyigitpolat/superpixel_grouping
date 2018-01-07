//you
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/plot.hpp>
#include <vector>
#include <string> 
#include <inttypes.h>
#include <opencv2/ximgproc.hpp>

//me
#include "functions.hpp"


int main( int argc, char** argv)
{
    Mat img, seg_img;
    img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    //preprocess
    GaussianBlur( img, seg_img, Size( 7, 7), 0, 0 );

    //SLIC
    Ptr<SuperpixelSLIC> myslic;
    myslic = createSuperpixelSLIC(seg_img, SLICO, 10, 220.0f);
    myslic->iterate(30);
    cv::Mat labels(seg_img.size(), CV_32SC1);
    myslic->getLabels(labels);
    Mat out;
    myslic->getLabelContourMask(out, false);

    //split channels
    std::vector<Mat> bgr; //destination array
    split(img,bgr); //split source  

    //count
    int superpixels = myslic->getNumberOfSuperpixels();
    printf("%d\n", superpixels);

    //merge
    merge_labels(labels, img, bgr, superpixels,1);
    merge_labels(labels, img, bgr, superpixels,2);
    merge_labels(labels, img, bgr, superpixels,3);
    merge_labels(labels, img, bgr, superpixels,4);
    merge_labels(labels, img, bgr, superpixels,5);
    
    Mat new_out = contour_at_sevgilim( labels);
    //POST
    for( int i = 0; i < img.rows; i++)
    {
        for( int j = 0; j < img.cols; j++)
        {
            if( new_out.row(i).at<uint8_t>(j))
            {
                bgr[0].row(i).at<uint8_t>(j) = 0;
                bgr[1].row(i).at<uint8_t>(j) = 0;
                bgr[2].row(i).at<uint8_t>(j) = 0;
            }
            else
            {
                Scalar s = random_color(labels.row(i).at<int32_t>(j));
                bgr[0].row(i).at<uint8_t>(j) = s[0];
                //bgr[1].row(i).at<uint8_t>(j) = s[1];
                //bgr[2].row(i).at<uint8_t>(j) = s[2];
            }
        }
    }


    //merge channels
    merge(bgr, img);
    

    //output
    myshow("asd", img);
    waitKey(0);
    return 0;
}
