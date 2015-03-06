#include <iostream>
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
   
  int n;
  cout << "Enter the number of historical yield:" <<endl;
  cin >> n;
  std::vector<double> X, Y;
  cout << "Enter " << n << " historical yield in the format of: date yield" << endl;
  for (int i = 0; i < n; i++) {
    double date, yield;
    cin >> date >> yield;
    X.push_back(date);
    Y.push_back(yield);
  }
  //X[0]=0.1; X[1]=0.4; X[2]=1.2; X[3]=1.8; X[4]=2.0;
  //Y[0]=0.1; Y[1]=0.7; Y[2]=0.9; Y[3]=1.1; Y[4]=1.9;
   
  CYieldCurve *curve = new CYieldCurve(X, Y);
   
  int date = 4;
   
  cout << "Historica(absolute) volatility of smoothed one-month yield " << curve->getHistoricalVolatility(30) << endl;
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
