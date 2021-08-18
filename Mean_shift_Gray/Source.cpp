#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<vector>
#include<algorithm>
#include  <random>
#include  <iterator>
using namespace cv;
using namespace std;


/***********        Random  selection *************/
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

/***********************************************************/
/***********************************************************/
/***********************************************************/
int main()
{
    Mat img_main = imread("D:/CV/small.png");
    Mat img = imread("D:/CV/small.png");
    Mat img_gray_seg = imread("D:/CV/small.png");
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    cvtColor(img, img_gray_seg, COLOR_BGR2GRAY);
    cout << img.rows << "  " << img.cols;


    /************************************************************
    /********        Feature Space     *******************/
    vector< int> features, visited;  int R, G, B, color, pt, visit = 0, new_mean, old_mean = 0, s = 0;

    for (int i = 0; i < img_gray.rows; i++)
    {
        for (int j = 0; j < img_gray.cols; j++)
        {
            color = img_gray.at<uchar>(i, j);
            features.push_back(color);
        }
    }


    /************************************************************
    /********        Visited     *******************/
    for (int i = 0; i < features.size(); i++)
        visited.push_back(0);




    /************************************************************/
    /********        Mean      *******************/
    old_mean = *select_randomly(features.begin(), features.end());
    std::vector<int>::iterator it;
    sort(features.begin(), features.end());



    /**********************************************************************/
    /**********************************************************************/
    /**********************************************************************/
    while (visit < features.size())
    {
        s = 0; int i = 0;
        vector< int> index, cluster;

        /****    Cluster      ******/
        while ((i < features.size()))
        {
            pt = sqrt(pow((features[i] - old_mean), 2));
            cout << "  pt: " << pt << endl;
            if ((pt <= 50) && (visited[i] == 0))
            {
                cluster.push_back(features[i]);
                index.push_back(i);
            }
        }

        /*******************************************************/
        for (int i = 0; i < cluster.size(); i++)
            s = s + cluster[i];
        new_mean = s / cluster.size();

        /*******************************************************/
        cout << "**********************************************" << endl;
        cout << " diff : " << abs(new_mean - old_mean) << endl;
        if ((abs(new_mean - old_mean)) <= 70)
        {
            for (int i = 0; i < cluster.size(); i++)
            {
                visited[index[i]] = new_mean;
                visit++;
            }
            for (int i = 0; i < visited.size(); i++)
            {
                if (visited[i] == 0)
                {
                    old_mean = features[i];
                    break;
                }
            }
        }
        else
            old_mean = new_mean;

    }


    /****************************************************************/
    /****************************************************************/
    for (int i = 0; i < visited.size(); i++)
    {
        cout << visited[i] << "  ";
    }

    /****************************************************************/
    for (int i = 0; i < img_gray.rows; i++)
    {
        for (int j = 0; j < img_gray.cols; j++)
        {
            color = img_gray_seg.at<uchar>(i, j);
            it = std::find(features.begin(), features.end(), color);
            img_gray_seg.at<uchar>(i, j) = visited[it - features.begin()];
        }
    }



    /****************************************************************/
    /****************************************************************/
    imshow("Image", img_gray);
    imshow("Image_seg", img_gray_seg);
    imshow("Image_main", img_main);
    waitKey(0);

}