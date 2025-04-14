#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

vector<vector<double>> calc_position(int n, double r) {
    double gap_theta = 2 * M_PI / n;
    vector<vector<double>> request;
    vector<double> theta;
    vector<double> x;
    vector<double> y;

    for (int i; i < n; i++) {
        theta.push_back(i * gap_theta);
        x.push_back(r * cos(theta[i]));
        y.push_back(r * sin(theta[i]));
    }
    request.push_back(x);
    request.push_back(y);
    request.push_back(theta);

    return request;
}

int main() {
    vector<vector<double>> result = calc_position(3, 5);

    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] << endl;

        }
    }
    return 0;
}

