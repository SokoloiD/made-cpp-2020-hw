#pragma once
#include <iostream>
#include <vector>


namespace task {
const double EPSILON = 1.0E-12;

std::vector<int> operator+(const std::vector<int> &a,
                           const std::vector<int> &b) {
  std::vector<int> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i] + b[i]);
  }
  return c;
}
std::vector<double> operator+(const std::vector<double> &a,
                              const std::vector<double> &b) {
  std::vector<double> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i] + b[i]);
  }
  return c;
}

std::vector<int> operator+(const std::vector<int> &a) {
  std::vector<int> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i]);
  }
  return c;
}
std::vector<double> operator+(const std::vector<double> &a) {
  std::vector<double> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i]);
  }
  return c;
}

std::vector<int> operator-(const std::vector<int> &a,
                           const std::vector<int> &b) {
  std::vector<int> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i] - b[i]);
  }
  return c;
}

std::vector<double> operator-(const std::vector<double> &a,
                              const std::vector<double> &b) {
  std::vector<double> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i] - b[i]);
  }
  return c;
}

std::vector<int> operator-(const std::vector<int> &a) {
  std::vector<int> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(-a[i]);
  }
  return c;
}

std::vector<double> operator-(const std::vector<double> &a) {
  std::vector<double> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(-a[i]);
  }
  return c;
}

int operator*(const std::vector<int> &a, const std::vector<int> &b) {
  int scalar_mul = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    scalar_mul += a[i] * b[i];
  }
  return scalar_mul;
}
double operator*(const std::vector<double> &a, const std::vector<double> &b) {
  double scalar_mul = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    scalar_mul += a[i] * b[i];
  }
  return scalar_mul;
}

std::vector<int> operator%(const std::vector<int> &a,
                           const std::vector<int> &b) {
  std::vector<int> c;
  c.push_back(a[1] * b[2] - a[2] * b[1]);
  c.push_back(a[2] * b[0] - a[0] * b[2]);
  c.push_back(a[0] * b[1] - a[1] * b[0]);
  return c;
}
std::vector<double> operator%(const std::vector<double> &a,
                              const std::vector<double> &b) {
  std::vector<double> c;
  c.push_back(a[1] * b[2] - a[2] * b[1]);
  c.push_back(a[2] * b[0] - a[0] * b[2]);
  c.push_back(a[0] * b[1] - a[1] * b[0]);
  return c;
}

void reverse(std::vector<int> &a) {

  for (size_t i = 0; i < a.size() / 2; ++i) {
    std::swap(a[i], a[a.size() - i - 1]);
  }
}
void reverse(std::vector<double> &a) {

  for (size_t i = 0; i < a.size() / 2; ++i) {
    std::swap(a[i], a[a.size() - i - 1]);
  }
}

std::vector<int> operator|(const std::vector<int> &a,
                           const std::vector<int> &b) {
  std::vector<int> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i] | b[i]);
  }
  return c;
}
std::vector<int> operator&(const std::vector<int> &a,
                           const std::vector<int> &b) {
  std::vector<int> c;
  for (size_t i = 0; i < a.size(); ++i) {
    c.push_back(a[i] & b[i]);
  }
  return c;
}

bool operator||(const std::vector<int> &a, const std::vector<int> &b) {
  double coef = 1.;
  size_t aZeroCnt = 0, bZeroCnt = 0;

  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i] && b[i]) {
      coef = double(a[i]) / double(b[i]);
      break;
    } else {
      if (a[i]) {
        aZeroCnt++;
      } else {
        bZeroCnt++;
      }
    }
  }

  if (aZeroCnt == a.size() || bZeroCnt == b.size()) {
    return true; 
  }

 
  for (size_t i = 0; i < a.size(); i++) {
    if (fabs(double(b[i]) * coef - double(a[i])) > EPSILON) {
      return false;
    }
  }
  return true;
}

bool operator||(const std::vector<double> &a, const std::vector<double> &b) {
  double coef = 1.;
  size_t aZeroCnt = 0, bZeroCnt = 0;

  for (size_t i = 0; i < a.size(); ++i) {
    if (std::fabs(a[i]) > EPSILON && std::fabs(b[i]) > EPSILON) {
      coef = a[i] / b[i];
      break;
    } else {
      if (std::fabs(a[i]) <= EPSILON) {
        aZeroCnt++;
      } else {
        bZeroCnt++;
      }
    }
  }

  if (aZeroCnt == a.size() || bZeroCnt == b.size()) {
    return true;
  }

  for (size_t i = 0; i < a.size(); i++) {
    if (std::fabs(b[i] * coef - a[i]) > EPSILON) {
      return false;
    }
  }
  return true;
}

bool operator&&(const std::vector<int> &a, const std::vector<int> &b) {
  double coef = 1.;
  size_t aZeroCnt = 0, bZeroCnt = 0;

  for (size_t i = 0; i < a.size(); ++i) {
    if (a[i] && b[i]) {
      coef = double(a[i]) / double(b[i]);
      break;
    } else {
      if (a[i]) {
        aZeroCnt++;
      } else {
        bZeroCnt++;
      }
    }
  }

  if (aZeroCnt == a.size() || bZeroCnt == b.size()) {
    return true;
  }
  if (coef < 0) {
    return false; 
  }

  
  for (size_t i = 0; i < a.size(); i++) {
    if (abs(double(b[i]) * coef - double(a[i])) > EPSILON) {
      return false;
    }
  }
  return true;
}
bool operator&&(const std::vector<double> &a, const std::vector<double> &b) {
  double coef = 1.;
  size_t aZeroCnt = 0, bZeroCnt = 0;

  for (size_t i = 0; i < a.size(); ++i) {
    if (fabs(a[i]) > EPSILON && fabs(b[i]) > EPSILON) {
      coef = a[i] / b[i];
      break;
    } else {
      if (abs(a[i]) <= EPSILON) {
        aZeroCnt++;
      } else {
        bZeroCnt++;
      }
    }
  }

  if (aZeroCnt == a.size() || bZeroCnt == b.size()) {
    return true;
  }

  if (coef < 0) {
    return false; 
  }

 
  for (size_t i = 0; i < a.size(); i++) {
    if (fabs(b[i] * coef - a[i]) > EPSILON) {
      return false;
    }
  }
  return true;
}

std::istream &operator>>(std::istream &in, std::vector<int> &a) {
  size_t vSize;
  in >> vSize;
  a.resize(vSize);

  for (size_t i = 0; i < vSize; ++i) {
    in >> a[i];
  }
  return in;
}
std::istream &operator>>(std::istream &in, std::vector<double> &a) {
  size_t vSize;

  in >> vSize;
  a.resize(vSize);

  for (size_t i = 0; i < vSize; ++i) {
    in >> a[i];
  }
  return in;
}
std::ostream &operator<<(std::ostream &out, const std::vector<int> &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    out << a[i] << " ";
  }
  out << "\n";
  return out;
}
std::ostream &operator<<(std::ostream &out, const std::vector<double> &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    out << a[i] << " ";
  }
  out << "\n";
  return out;
}

} // namespace task
