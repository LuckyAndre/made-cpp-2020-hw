#pragma once
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

namespace task {

vector<double> operator + (const vector<double> &a, const vector<double> &b) {
    vector<double> c;
    for (size_t i = 0; i < a.size(); ++ i) {
        c.push_back(a[i] + b[i]);
    }
    return c;
}


vector<double> operator - (const vector<double> &a, const vector<double> &b) {
    vector<double> c;
    for (size_t i = 0; i < a.size(); ++ i) {
        c.push_back(a[i] - b[i]);
    }
    return c;
}

// скалярное произведение
double operator * (const vector<double> &a, const vector<double> &b) {
    int c = 0;
    for (size_t i = 0; i < a.size(); ++ i) {
        c += a[i] * b[i];
    }
    return c;
}

// векторное произведение
vector<double> operator % (const vector<double> &a, const vector<double> &b) {
    vector<double> c;

    c.push_back(a[1] * b[2] - b[1] * a[2]);
    c.push_back(- (a[0] * b[2] - b[0] * a[2]));
    c.push_back(a[0] * b[1] - b[0] * a[1]);

    return c;
}


// коллинеарность
bool operator || (const vector<double> &a, const vector<double> &b) {
    bool c;
    double cos, sin, len_a, len_b;

    len_a = pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2);
    len_b = pow(b[0], 2) + pow(b[1], 2) + pow(b[2], 2);

    cos = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / sqrt(len_a) / sqrt(len_b);
    sin = sqrt(1 - pow(cos, 2));

    if (sin == 0) {
        c = true;
    } else {
        c = false;
    }
    return c;
}


// сонаправленность
bool operator && (const vector<double> &a, const vector<double> &b) {
    bool c;
    double cos, len_a, len_b;

    len_a = pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2);
    len_b = pow(b[0], 2) + pow(b[1], 2) + pow(b[2], 2);

    cos = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / sqrt(len_a) / sqrt(len_b);

    if (cos == 1) {
        c = true;
    } else {
        c = false;
    }
    return c;
}


// потоковый ввод (double)
istream & operator >> (istream &in, vector<double> &a) {
    int amount;
    double x;

    in >> amount;
    for (int i = 0; i < amount; ++ i) {
        in >> x;
        a.push_back(x);
    }
    return in;
}


// потоковый вывод (double)
ostream & operator << (ostream &out, const vector<double> &a) {
    for (auto element : a)
        out << element << " ";
    out << endl;
    return out;
}


vector<double> reverse (const vector<double> &a) {
    vector<double> b;

    for (size_t i = a.size(); i > 0; -- i){
        b.push_back(a[i - 1]);
    }
    return b;
}


// поэлементное побитовое или
vector<int> operator | (const vector<int> &a, const vector<int> &b) {
    vector<int> c;
    for (size_t i = 0; i < a.size(); ++i) {
        c.push_back(a[i] | b[i]);
    }
    return c;
}

// поэлементное побитовое и
vector<int> operator & (const vector<int> &a, const vector<int> &b) {
    vector<int> c;
    for (size_t i = 0; i < a.size(); ++i) {
        c.push_back(a[i] & b[i]);
    }
    return c;
}


}  // namespace task
