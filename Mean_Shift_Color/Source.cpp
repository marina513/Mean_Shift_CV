#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<vector>
#include<algorithm>
#include  <random>
#include  <iterator>
#include <string> 
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
    Mat img_main = imread("D:/CV/background.png");
    Mat img = imread("D:/CV/background.png");
    Mat img_seg = imread("D:/CV/background.png");
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    cout << img.rows << "  " << img.cols;


    /************************************************************
    /********        Feature Space     *******************/
    vector< int>  visit, old_mean, new_mean;  int R, G, B, color, pt, visit_num = 0, s = 0;
    vector<vector<int>> features, visited, cluster;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            vector< int> feature;
            R = img.at<Vec3b>(i, j)[0];
            G = img.at<Vec3b>(i, j)[1];
            B = img.at<Vec3b>(i, j)[2];
            feature.push_back(R);
            feature.push_back(G);
            feature.push_back(B);
            features.push_back(feature);
        }
    }


    /******************************************/
    /************************************************************
    /********        Visited     *******************/
    for (int i = 0; i < 3; i++)
        visit.push_back(0);
    for (int i = 0; i < features.size(); i++)
        visited.push_back(visit);




    /************************************************************/
    /********        Mean      *******************/
    int num_begin = rand() % (features.size() - 0 + 1) + 0;
    old_mean = features[num_begin];

    std::vector<int>::iterator it;




    /**********************************************************************/
    /**********************************************************************/
    cout << endl << "begin segmentation" << endl;
    /**********************************************************************/
    while (visit_num < features.size())
    {
        s = 0;
        vector< int> index, feature, new_mean;
        vector < vector<int>> cluster;

        /****    Cluster      ******/
        for (int i = 0; i < features.size(); i++)
        {
            feature = features[i];

            pt = sqrt(pow((feature[0] - old_mean[0]), 2) + pow((feature[1] - old_mean[1]), 2) + pow((feature[2] - old_mean[2]), 2));

            visit = visited[i];
            if ((pt <= 150) && (visit[0] == 0) && (visit[1] == 0) && (visit[2] == 0))
            {
                cluster.push_back(feature);
                index.push_back(i);
            }
        }


        /*******************************************************/
        /*******************************************************/
        /*******************************************************/
        int R_mean = 0, G_mean = 0, B_mean = 0;
        for (int i = 0; i < cluster.size(); i++)
        {
            vector<int> c = cluster[i];
            R_mean = R_mean + c[0];
            G_mean = G_mean + c[1];
            B_mean = B_mean + c[2];
        }
        new_mean.push_back(R_mean / cluster.size());
        new_mean.push_back(G_mean / cluster.size());
        new_mean.push_back(B_mean / cluster.size());

        /*******************************************************/
        if (sqrt(pow((new_mean[0] - old_mean[0]), 2) + pow((new_mean[1] - old_mean[1]), 2) + pow((new_mean[2] - old_mean[2]), 2)) < 70)
        {
            for (int i = 0; i < cluster.size(); i++)
            {
                visited[index[i]] = new_mean;
                visit_num++;
            }
            for (int i = 0; i < visited.size(); i++)
            {
                if ((visited[i][0] == 0) && (visited[i][1] == 0) && (visited[i][2] == 0))
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

    cout << endl << "finish segmentation" << endl;
    vector<string> feature_string;

    /*********************************************************/

    for (int f = 0; f < features.size(); f++)
    {
        string R = to_string(features[f][0]);
        string G = to_string(features[f][1]);
        string B = to_string(features[f][2]);
        feature_string.push_back(R + G + B);
    }

    /*********************************************************/
    /*********************************************************/
    /****************************************************************/
    vector<string>::iterator it_s;

    for (int i = 0; i < img.rows; i++)
    {
        cout << "preparing at row: " << i << " ..." << endl;
        if (i == (img.rows) / 2)
            cout << "take a breath, you are in the middle of the way" << endl;

        for (int j = 0; j < img.cols; j++)
        {
            vector< int> feature;
            string R = to_string(img.at<Vec3b>(i, j)[0]);
            string G = to_string(img.at<Vec3b>(i, j)[1]);
            string B = to_string(img.at<Vec3b>(i, j)[2]);
            string find = R + G + B;

            it_s = std::find(feature_string.begin(), feature_string.end(), find);
            int f = it_s - feature_string.begin();
            img_seg.at<Vec3b>(i, j)[0] = visited[f][0];
            img_seg.at<Vec3b>(i, j)[1] = visited[f][1];
            img_seg.at<Vec3b>(i, j)[2] = visited[f][2];

        }
    }

    cout << endl << "show" << endl;
    /****************************************************************/
    /****************************************************************/
    imshow("Image_seg", img_seg);
    imshow("Image_main", img_main);
    waitKey(0);

}
