/*
	Задача на 09.02.2023:
		Переписать весь код с использованием контайнера Vector вместо указателей на массив. 
    Задача на 12.02.2023:
        Написал unit-тесты, и дошел до метода getStandardError()
*/
#include <iostream>
#include <cstdio>
#include <cmath>
#include <conio.h>
#include <vector>
#include <ctime>
#include <cassert>

using namespace std;

vector<double> getRegressionParameters(vector<double> independent, vector<double> dependent) {
    double sumIndVar(0), sumDepVar(0), sumMultiplication(0), sumSqIndVar(0);
    vector<double> regressionParameters(2, 0);
    for (int i = 0; i < independent.size(); i++) {
		sumIndVar += independent[i];
        sumSqIndVar += pow(independent[i], 2);
        sumDepVar += dependent[i];
        sumMultiplication += independent[i] * dependent[i];
	}
    regressionParameters[0] = ((sumDepVar * sumSqIndVar) - (sumIndVar * sumMultiplication)) / (independent.size() * sumSqIndVar - pow(sumIndVar, 2));
    regressionParameters[1] = (independent.size() * sumMultiplication - (sumDepVar * sumIndVar)) / (independent.size() * sumSqIndVar - pow(sumIndVar, 2));
    return regressionParameters;
}

class Statistics {
protected:
	vector<vector<double>> samples;
public:
	Statistics(vector<vector<double>> inputSamples);
	~Statistics();
	Statistics(Statistics & otherSample);
	Statistics & operator = (const Statistics & otherSample);
	void showSamples();
	vector<double> getAverage(vector<double> *average);
    vector<double> getDispersion(vector<double> *dispersion);
    vector<double> getStandardDeviation(vector<double> *standardDeviation);
    vector<double> getStandardError(vector<double> *standardError);
    double getTestF();
    vector<vector<double>> getTestT();
    vector<vector<double>> getCoefficientCorrelation();
    
};

Statistics::Statistics(vector<vector<double>> inputSamples) {
    samples = inputSamples;
    for (int i = 0; i < inputSamples.size(); i++) {
        samples[i] = inputSamples[i];
    }
}
Statistics::~Statistics() {
}
Statistics::Statistics(Statistics & otherSample) {
    samples = otherSample.samples;
    for (int i = 0; i < otherSample.samples.size(); i++) {
        samples[i] = otherSample.samples[i];
    }
}
Statistics & Statistics::operator = ( const Statistics & otherSample ) {
    samples = otherSample.samples;
    for (int i = 0; i < otherSample.samples.size(); i++) {
        samples[i] = otherSample.samples[i];
    }
    return *this;
}
void Statistics::showSamples() {
    cout << "Количество выборок: " << samples.size() << endl;
	cout << "Длина выборок: ";
    for (int i = 0; i < samples.size(); i++) {
        cout << samples[i].size() << " ";
    }
	cout << endl << "Выборки: " << endl;
    for (int i = 0; i < samples.size(); i++) {
        for (int j = 0; j < samples[i].size(); j++) {
            cout << samples[i][j] << " ";
        }
        cout << endl;
    }
}
vector<double> Statistics::getAverage(vector<double> *average) {
    std::vector<double>& pAverage = *average;
    for (int i = 0; i < samples.size(); i++) {
        pAverage.push_back(0.0);
        for (int j = 0; j < samples[i].size(); j++) {
            pAverage[i] += samples[i][j];
        }
        pAverage[i] /= samples[i].size();
    }
    return pAverage;
}
vector<double> Statistics::getDispersion(vector<double> *dispersion) {
	vector<double> average;
    getAverage(&average);
    vector<double>& pDispersion = *dispersion;
	double deviation;
    for (int i = 0; i < samples.size(); i++) {
        pDispersion.push_back(0.0);
        for (int j = 0; j < samples[i].size(); j++) {
            deviation = average[i] - samples[i][j];
            pDispersion[i] += deviation * deviation; 
        }
        pDispersion[i] /= samples[i].size() - 1;
    }
	return pDispersion;
}
vector<double> Statistics::getStandardDeviation(vector<double> *standardDeviation) {
    vector<double> dispersion;
    getDispersion(&dispersion);
    vector<double>& pStandardDeviation = *standardDeviation;
    for (int i = 0; i < dispersion.size(); i++) {
        pStandardDeviation.push_back(sqrt(dispersion[i]));
    }
    return pStandardDeviation;
}
vector<double> Statistics::getStandardError(vector<double> *standardError) {
    vector<double> standardDeviation;
    getStandardDeviation(&standardDeviation);
    for (int i = 0; i < standardDeviation.size(); i++) {
        standardError->push_back(standardDeviation[i]/sqrt(samples[i].size()));
    }
	return *standardError;
}
double Statistics::getTestF() {
    vector<double> average;
    getAverage(&average);
    double averageAllSample(0);
    for (int i = 0; i < average.size(); ++i) {
        averageAllSample += average[i];
    }
    averageAllSample /= average.size();
    double SSB(0);
    for (int i = 0; i < average.size(); ++i) {
        SSB += samples[i].size() * pow((average[i] - averageAllSample), 2);
    }
    double SSW(0);
    for (int i = 0; i < samples.size(); ++i) {
        for (int j = 0; j < samples[i].size(); ++j) {
            SSW += pow(samples[i][j] - average[i], 2);    
        }
    }
    int numberObjects(0);
    for (int i = 0; i < samples.size(); ++i) {
        numberObjects += samples[i].size();
    }
    double testF = ((SSB / (samples.size() - 1)) / (SSW / (numberObjects - samples.size())));
    return testF;
}
vector<vector<double>> Statistics::getTestT() {
    vector<double> average;
    getAverage(&average);
    vector<double> standardDeviation;
    getStandardDeviation(&standardDeviation);
    vector<vector<double>> testT(samples.size(), vector<double>(samples.size(), 0));
    for (int i = 0; i < samples.size(); ++i) {
        for (int j = 0; j < samples.size(); ++j) {
            double differenceAverage = abs(average[i] - average[j]);
            double iSquareAverageStandardDeviation = pow(standardDeviation[i], 2)/samples[i].size();
            double jSquareAverageStandardDeviation = pow(standardDeviation[j], 2)/samples[j].size();
            testT[i][j] = differenceAverage / (sqrt(iSquareAverageStandardDeviation + jSquareAverageStandardDeviation));
        }
    }
    return testT;
}
vector<vector<double>> Statistics::getCoefficientCorrelation() {
    vector<double> average;
    getAverage(&average);
    vector<vector<double>> coefficientCorrelation(samples.size(), vector<double>(samples.size(), 0));
    double iSquareDeviation(0), jSquareDeviation(0), sumDeviation(0);
    for (int i = 0; i < samples.size(); ++i) {
        for (int j = 0; j < samples.size(); ++j) {
            for (int k = 0; k < samples[i].size(); ++k) {
                sumDeviation += (average[i] - samples[i][k]) * (average[j] - samples[j][k]);
                iSquareDeviation += pow((average[i] - samples[i][k]), 2);
                jSquareDeviation += pow((average[j] - samples[j][k]), 2);
            }
            coefficientCorrelation[i][j] = sumDeviation / sqrt(iSquareDeviation * jSquareDeviation);
            iSquareDeviation = 0; 
            jSquareDeviation = 0;
            sumDeviation = 0;
        }
    }
    return coefficientCorrelation;
}


void TestStandartMethodsStatistics() {
    vector<double> testFirstSample = {31, 32, 33, 34, 35, 35, 40, 41, 42, 46};
    vector<double> testSecondSample = {7.8, 8.3, 7.6, 9.1, 9.6, 9.8, 11.8, 12.1, 14.7, 13.0};
    vector<double> testThirdSample = {3.7, 1.3, 4.6, 2.1, 7.6, 1.8, 11.0, 12.2, 11.7, 19.0};
    vector<double>& pTestFirstSample = testFirstSample;
    vector<double>& pTestSecondSample = testSecondSample;
    vector<double>& pTestThirdSample = testThirdSample;
    vector<vector<double>> testSamples = {testFirstSample, testSecondSample, testThirdSample};
    Statistics testObject(testSamples);
    vector<double> average;
    testObject.getAverage(&average);
    vector<double> dispersion;
    testObject.getDispersion(&dispersion);
    vector<double> standardDeviation;
    vector<double> correctStandardDeviation = {4.99889, 2.39898, 5.84218};
    testObject.getStandardDeviation(&standardDeviation);
    for (int i = 0; i < standardDeviation.size(); i++) {
        assert(abs(standardDeviation[i] - correctStandardDeviation[i]) < 0.0001);
    }
    vector<double> standardError;
    vector<double> correctStandardError = {1.58079, 0.758624, 1.84746};
    testObject.getStandardError(&standardError);
    for (int i = 0; i < standardError.size(); i++) {
        assert(abs(standardError[i] - correctStandardError[i]) < 0.0001);
    }
}
void TestTestStatistics() {
    vector<double> testFirstSample = {3, 1 ,2};
    vector<double> testSecondSample = {5, 3, 4};
    vector<double> testThirdSample = {7, 6, 5};
    vector<double>& pTestFirstSample = testFirstSample;
    vector<double>& pTestSecondSample = testSecondSample;
    vector<double>& pTestThirdSample = testThirdSample;
    vector<vector<double>> testSamples = {testFirstSample, testSecondSample, testThirdSample};
    Statistics testObject(testSamples);
    assert(testObject.getTestF() == 12);
    vector<vector<double>> testT = testObject.getTestT();
    double correctTestT[3][3] = {
                                {0, 2.44949, 4.89898},
                                {2.44949, 0, 2.44949},
                                {4.89898, 2.44949, 0}
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(abs(testT[i][j] - correctTestT[i][j]) < 0.0001);
        }
    }
    vector<vector<double>> coefficientCorrelation = testObject.getCoefficientCorrelation();
    double correctCoefficientCorrelation[3][3] = {
                                {1, 1, 0.5},
                                {1, 1, 0.5},
                                {0.5, 0.5, 1}
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(abs(correctCoefficientCorrelation[i][j] - coefficientCorrelation[i][j]) < 0.0001);
        }
        std::cout << endl;
    }
    assert(abs(getRegressionParameters(testFirstSample, testSecondSample)[1] - 1) < 0.0001);
    assert(abs(getRegressionParameters(testFirstSample, testSecondSample)[0] - 2) < 0.0001);
}

int main() {
	srand(time(nullptr));
    TestStandartMethodsStatistics();
    TestTestStatistics();
    return 0;
}