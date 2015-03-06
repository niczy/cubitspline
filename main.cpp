#include <iostream>
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

    double getSmoothedDiscountFactor(int term, int date) {
      // TODO(shuwen): Define this.
      return 0.0;
    }

    double getHistoricalVolatility(int T) {
      double lnSum = 0;
      for (int i = 1; i < X.size(); i++) {
        lnSum += pow(log(s(X[i])/s(X[i-1])), 2);
      }
      return sqrt(X.size()* lnSum / T);
    }

    double getSpeedOfMeanReversion() {
      // TODO(shuwen): Define this.
      return 0.0;
    }
};


/* Yield Curve
This block will read daily historical yield curves. It will smooth each of them it using cubic splines. For each
term (and each historical date), two types of output will be created: smoothed yield and smoothed discount factor. It will also estimate the historical (absolute) volatility of the smoothed one-month yield, and the speed of mean-reversion alpha of the one-month yield. In a first time, we will assume that the one-month yield
follows non standard Brownian motion. You will then hard code alpha = 0.2. use cubic spline interpolation (source code I have found online).
 
 methodology online:
http://www.treasury.gov/resource-center/data-chart-center/interest-rates/Pages/yieldmethod.aspx */
int main(int argc, char** argv) {
   
  vector<double> X, Y;
  string market;
  string inputFileName = "data.in";
  string unit;
  
  while (true) {
    cout << "Choose US or CHN market, enter 'US' or CHN:" << endl; 
    cin >> market;
    if (market.compare("CHN") == 0) {
      unit = "￥";
      break;
    } else if (market.compare("US") == 0){
      unit = "$";
      break;
    } else {
      cout << "Input error!" << endl;
    }
  }
  

  // Read data from data.csv
  ifstream infile(inputFileName);
  string line;
  int count = 0;
  while (infile >> line) {
    count++;
    int commaIdx = line.find(",");
    string data = line.substr(0, commaIdx);
    try {
      double value = stod(line.substr(commaIdx + 1));
      X.push_back(count);
      Y.push_back(value);
    } catch (...) {
    }
  }
  cout << "Read " << count << " historical data from " << inputFileName << endl;
  infile.close();
   
  CYieldCurve *curve = new CYieldCurve(X, Y);
   
  int date = 4;
   
  cout << "Historica(absolute) volatility of smoothed one-month yield " << curve->getHistoricalVolatility(30) << unit << endl;
  while (true) {
    int term, date;
    cout << "Enter a term, or -1 to exit:"<<endl;
    cin >> term;
    if (term == -1) {
      cout << "Exiting!" << endl;
      break;
    }
    cout <<"and historical date:" << endl;
    cin >> date;
    cout << "Yield for date: " << date << " is " << curve->getSmootedYield(date) << endl;
    cout << "\n\n\n\n" << endl;
  }
   
  return EXIT_SUCCESS;
}
