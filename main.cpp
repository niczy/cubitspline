#include <iostream>
#include <algorithm> 
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath> 
#include <vector>
#include "spline.h"
using namespace std;
// member function

/*
CYield Curve

Private
int last
int first
int count
maxTerm
vector <int> term
char  curr

Public
Double GetDiscountFactor()
Double GetOneMonthVolatility()

CYieldCurve(curr, maxTerm)
*/

class CYieldCurve {
  private:
    tk::spline s;
    vector<double> X;

  public:
    CYieldCurve(std::vector<double> _X, std::vector<double> Y) {
      s.set_points(_X,Y); // currently it is required that X is already sorted
      X = _X;
    }

    double getSmootedYield(int date) {
      return s(date);
    }

    double getDiscountFactor(int month) {
      return exp(-getSmootedYield(0) * month / 12);
    }

    double getHistoricalVolatility(int T) {
      double lnSum = 0;
      for (int i = 1; i < X.size(); i++) {
        lnSum += pow(log(s(X[i])/s(X[i-1])), 2);
      }
      return sqrt(X.size()* lnSum / T);
    }

    double getSpeedOfMeanReversion() {
      return 0.2;
    }
};


/* Yield Curve
This block will read daily historical yield curves. It will smooth each of them it using cubic splines. For each
term (and each historical date), two types of output will be created: smoothed yield and smoothed discount factor. It will also estimate the historical (absolute) volatility of the smoothed one-month yield, and the speed of mean-reversion alpha of the one-month yield. In a first time, we will assume that the one-month yield
follows non standard Brownian motion. You will then hard code alpha = 0.2. use cubic spline interpolation (source code I have found online).
 
 methodology online:
http://www.treasury.gov/resource-center/data-chart-center/interest-rates/Pages/yieldmethod.aspx */
int main(int argc, char** argv) {
   
  int TOTAL_TIME = 11;
  string MATU[] = {"1 Mo", "3 Mo", "6 Mo", "1 Yr", "2 Yr", "3 Yr", "5 Yr", "7 Yr", "10 Yr", "20 Yr", "30 Yr"};
  int month[] = {1, 3, 6, 12, 24, 36, 60, 84, 120, 240, 360};
  vector<vector<double> > X(TOTAL_TIME), Y(TOTAL_TIME);
  string market;
  string inputFileName = "USTREASURY-YIELD.csv";

  // Read data from data.csv
  ifstream infile(inputFileName);
  string line;
  getline(infile, line);

  int count = 0;
  while (getline(infile, line)) {
    count--;
    int commaIdx = line.find(",");
    line = line.substr(commaIdx+1);
    try {
      for (int i = 0; i < TOTAL_TIME; i++) {
        double value = stod(line);
        X[i].push_back(count);
        Y[i].push_back(value);
        line = line.substr(line.find(",") + 1);
      }
    } catch (...) {
    }
  }
  for (int i = 0; i < TOTAL_TIME; i++) {
    reverse(X[i].begin(), X[i].end());
    reverse(Y[i].begin(), Y[i].end());
  }
  infile.close();
  for (int i = 0 ; i < TOTAL_TIME; i++) {
    CYieldCurve *curve = new CYieldCurve(X[i], Y[i]);
    cout << "Yield for "  << MATU[i] << " at today is " << curve->getSmootedYield(0) <<
      " discount factor is " << curve->getDiscountFactor(month[i]) << endl;
  }

  // Caculate historical volatility.
  double sum = 0.0;
  for (int i = 0; i < Y[0].size(); i++) {
    sum += Y[0][i];
  }
  double avg = sum / Y[0].size();
  double s = 0.0;
  for (int i = 0; i < Y[0].size(); i++) {
    s += pow(Y[0][i] - avg, 2);
  }
  s /= Y[0].size() - 1; 
  s *= sqrt(22);
  cout << "historical (absolute) volatility of the smoothed one-month yield is " << s << endl;
  cout << "speed of mean-reversion is 0.2" << endl;

  return EXIT_SUCCESS;
}
