#include <iostream>
#include <vector>
#include <map>
#include "mnist_reader.hpp"
#include "mnist_utils.hpp"
#include "bitmap.hpp"
#include <sstream>
#include <math.h>
#define MNIST_DATA_DIR "../mnist_data"
int main(int argc, char* argv[]) {
    //Read in the data set from the files
    mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset =
    mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_DIR);
    //Binarize the data set (so that pixels have values of either 0 or 1)
    mnist::binarize_dataset(dataset);
    //There are ten possible digits 0-9 (classes)
    int numLabels = 10;
    //There are 784 features (one per pixel in a 28x28 image)
    int numFeatures = 784;
    //Each pixel value can take on the value 0 or 1
    int numFeatureValues = 2;
    //image width
    int width = 28;
    //image height
    int height = 28;
    //image to print (these two images were randomly selected by me with no particular preference)
    int trainImageToPrint = 50;
    int testImageToPrint = 5434;
    // get training images
    std::vector<std::vector<unsigned char>> trainImages = dataset.training_images;
    // get training labels
    std::vector<unsigned char> trainLabels = dataset.training_labels;
    // get test images
    std::vector<std::vector<unsigned char>> testImages = dataset.test_images;
    // get test labels
    std::vector<unsigned char> testLabels = dataset.test_labels;

    /*
     *
     *Determine the probabilities from the training images
     *
     */

    //Determine prior probabilies
    std::cout << "PRIOR PROBABILITIES" << std::endl;
    int imageDigits [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double priorProbabilities [] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < trainLabels.size(); ++i)
    {
        imageDigits[static_cast<int>(trainLabels[i])]++;
    }

    double total = 0;
    for (int i = 0; i < numLabels; ++i)
    {
        priorProbabilities[i] += ((double) imageDigits[i]) / 60000.0;
        total += priorProbabilities[i];
        std::cout << priorProbabilities[i] << std::endl;
    }

    std::cout << total << "\n" << std::endl;



    //Determine P(Fj = 1| C = c)
    std::cout << "P(Fj = 1| C = c)" << std::endl;
    int numFeaturesGivenC[numLabels][numFeatures];
    double percentFeatureGivenC[numLabels][numFeatures];

    //initialize arrays
    for (int i = 0; i < numLabels; ++i)
    {
        for (int j = 0; j < numFeatures; ++j)
        {
            numFeaturesGivenC[i][j] = 0;
            percentFeatureGivenC[i][j] = 0;
        }
    }

    //find number of digit c where pixel Fj is white
    for (int i = 0; i < trainImages.size(); ++i)
    {
        for (int j = 0; j < numFeatures; ++j)
        {
            if (static_cast<int>(trainImages[i][j]) == 1)
            {
                int classValue = trainLabels[i];
                numFeaturesGivenC[classValue][j]++;
            }
        }
    }

    //find P(Fj = 1| C = c)
    for (int i = 0; i < numLabels; ++i)
    {
        for (int j = 0; j < numFeatures; ++j)
        {
           percentFeatureGivenC[i][j] = ((double)numFeaturesGivenC[i][j] + 1.0) / ((double)imageDigits[i] + 2.0);
        }
    }


    /*
     *
     *Evaluate performance on test set
     *
     */

    //For each test image Ti compute the probability that it belongs to each class
    
    
    double denominatorForC = 1.0;
    int totalCorrect = 0;

    for (int i = 0; i < numFeatures; ++i)
    {
        denominatorForC *= 0.5;
    }

    for (int i = 0; i < testImages.size(); ++i)
    {
        double probabilityOfClass[numLabels] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        for (int c = 0; c < numLabels; ++c)
        {
            double numeratorForC = priorProbabilities[c];
            for (int j = 0; j < numFeatures; ++j)
            {
                double tempProb = 1.0;
                if (static_cast<int>(testImages[i][j]) == 1)
                {
                    tempProb = percentFeatureGivenC[c][j];
                }
                else {
                    tempProb = 1.0 - percentFeatureGivenC[c][j];
                }
                numeratorForC *= tempProb;
            }
            probabilityOfClass[c] = numeratorForC / denominatorForC;
        }


        double max = probabilityOfClass[0];
        int index = 0;
        for (int c = 1; c < numLabels; ++c)
        {
            if(probabilityOfClass[c] > max) {
                max = probabilityOfClass[c];
                index = c;
            }
        }

        //std::cout << "Predicted: " << index << std::endl;
        //std::cout << "Actual: " << static_cast<int>(testLabels[i]) << std::endl;

        //check correct
        if (index == static_cast<int>(testLabels[i])) 
        {
            totalCorrect++;
        } 
    }

    double percentage = (double)totalCorrect / testImages.size();
    percentage *= 100;
    std::cout << "Percent correct: " << percentage << std::endl;
    
    


    //Classify Ti as belonging to class ci that maximizes the following expression:
    //Maxc (Sum (logPl(Fj= tij | C = c) + logP(C = c) ) )
    
    totalCorrect = 0;

    std:: cout << "\n";
    std:: cout << "Maxc (Sum (logPl(Fj= tij | C = c) + logP(C = c) ) )" << std::endl;
    for (int i = 0; i < testImages.size(); ++i)
    {
        double probabilityOfClassPart2[numLabels] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        for (int c = 0; c < numLabels; ++c)
        {
            for (int j = 0; j < numFeatures; ++j)
            {
                double tempProb = 1.0;
                if (static_cast<int>(testImages[i][j]) == 1)
                {
                    tempProb = percentFeatureGivenC[c][j];
                }
                else {
                    tempProb = 1.0 - percentFeatureGivenC[c][j];
                }

                tempProb = log(tempProb);
                probabilityOfClassPart2[c] += tempProb;
            }
            probabilityOfClassPart2[c] += log(priorProbabilities[c]);
        } 

        double max = probabilityOfClassPart2[0];
        int index = 0;
        for (int c = 1; c < numLabels; ++c)
        {
            if(probabilityOfClassPart2[c] > max) {
                max = probabilityOfClassPart2[c];
                index = c;
            }
        }

        //std::cout << "Predicted: " << index << std::endl;
        //std::cout << "Actual: " << static_cast<int>(testLabels[i]) << std::endl;

        //check correct
        if (index == static_cast<int>(testLabels[i])) 
        {
            totalCorrect++;
        }    
    }

    percentage = (double)totalCorrect / testImages.size();
    percentage *= 100;
    std::cout << "Percent correct: " << percentage << std::endl;
    













    //print out one of the training images
    for (int f=0; f<numFeatures; f++) {
        // get value of pixel f (0 or 1) from training image trainImageToPrint
        int pixelIntValue = static_cast<int>(trainImages[trainImageToPrint][f]);
        if (f % width == 0) {
            std::cout<<std::endl;
        }
        std::cout<<pixelIntValue<<" ";
    }
    std::cout<<std::endl;
    // print the associated label (correct digit) for training image trainImageToPrint
    std::cout<<"Label: "<<static_cast<int>(trainLabels[trainImageToPrint])<<std::endl;
    //print out one of the test images
    for (int f=0; f<numFeatures; f++) {
        // get value of pixel f (0 or 1) from training image trainImageToPrint
        int pixelIntValue = static_cast<int>(testImages[testImageToPrint][f]);
        if (f % width == 0) {
            std::cout<<std::endl;
        }
        std::cout<<pixelIntValue<<" ";
    }
    std::cout<<std::endl;
    // print the associated label (correct digit) for test image testImageToPrint
    std::cout<<"Label: "<<static_cast<int>(testLabels[testImageToPrint])<<std::endl;
    std::vector<unsigned char> trainI(numFeatures);
    std::vector<unsigned char> testI(numFeatures);
    for (int f=0; f<numFeatures; f++) {
        int trainV = 255*(static_cast<int>(trainImages[trainImageToPrint][f]));
        int testV = 255*(static_cast<int>(testImages[testImageToPrint][f]));
        trainI[f] = static_cast<unsigned char>(trainV);
        testI[f] = static_cast<unsigned char>(testV);
    }
    std::stringstream ssTrain;
    std::stringstream ssTest;
    ssTrain << "../output/train" <<trainImageToPrint<<"Label"<<static_cast<int>(trainLabels[trainImageToPrint])<<".bmp";
    ssTest << "../output/test" <<testImageToPrint<<"Label"<<static_cast<int>(testLabels[testImageToPrint])<<".bmp";
    Bitmap::writeBitmap(trainI, 28, 28, ssTrain.str(), false);
    Bitmap::writeBitmap(testI, 28, 28, ssTest.str(), false);
    return 0;
}

