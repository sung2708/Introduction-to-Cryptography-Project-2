#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class BigInt {
private:
  vector<int> digits;
  bool isNegative;

  void removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
      digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) {
      isNegative = false;
    }
  }

public:
  BigInt() : digits(1, 0), isNegative(false) {}

  BigInt(const string &value) {
    isNegative = false;
    string tempValue = value;

    if (tempValue[0] == '-') {
      isNegative = true;
      tempValue = tempValue.substr(1);
    }

    if (tempValue == "0") {
      isNegative = false;
      digits.push_back(0);
      return;
    }

    digits.reserve(tempValue.size());
    for (int i = tempValue.size() - 1; i >= 0; --i) {
      if (!isdigit(tempValue[i])) {
        throw invalid_argument("Invalid character in input string");
      }
      digits.push_back(tempValue[i] - '0');
    }

    removeLeadingZeros();
  }

  BigInt(int value) {
    isNegative = (value < 0);
    value = abs(value);

    if (value == 0) {
      isNegative = false;
      digits.push_back(0);
    } else {
      while (value > 0) {
        digits.push_back(value % 10);
        value /= 10;
      }
    }
  }
  string toString() const {
    string result = isNegative ? "-" : "";
    for (int i = digits.size() - 1; i >= 0; --i) {
      result += (digits[i] + '0');
    }
    return result;
  }

  int toInt() const {
    int result = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
      result = result * 10 + digits[i];
    }
    return isNegative ? -result : result;
  }

  BigInt add(const BigInt &other) const {
    BigInt result;
    result.digits.resize(max(digits.size(), other.digits.size()), 0);
    int carry = 0;
    for (size_t i = 0; i < result.digits.size(); ++i) {
      int sum = carry;
      if (i < digits.size())
        sum += digits[i];
      if (i < other.digits.size())
        sum += other.digits[i];
      carry = sum / 10;
      result.digits[i] = sum % 10;
    }
    if (carry) {
      result.digits.push_back(carry);
    }
    result.isNegative = isNegative;
    return result;
  }

  BigInt subtract(const BigInt &other) const {
    BigInt result;
    result.digits.resize(digits.size(), 0);
    int borrow = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
      int diff =
          digits[i] - borrow - (i < other.digits.size() ? other.digits[i] : 0);
      if (diff < 0) {
        diff += 10;
        borrow = 1;
      } else {
        borrow = 0;
      }
      result.digits[i] = diff;
    }
    result.removeLeadingZeros();
    result.isNegative = isNegative;
    return result;
  }

  BigInt multiply(const BigInt &other) const {
    BigInt result;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    for (size_t i = 0; i < digits.size(); ++i) {
      for (size_t j = 0; j < other.digits.size(); ++j) {
        result.digits[i + j] += digits[i] * other.digits[j];
        if (result.digits[i + j] >= 10) {
          result.digits[i + j + 1] += result.digits[i + j] / 10;
          result.digits[i + j] %= 10;
        }
      }
    }
    result.removeLeadingZeros();
    result.isNegative = isNegative != other.isNegative;
    return result;
  }

  BigInt operator%(const BigInt &mod) const {
    if (mod == BigInt("0")) {
      throw invalid_argument("Modulo by zero");
    }

    BigInt dividend = *this;
    dividend.isNegative = false;
    BigInt divisor = mod;
    divisor.isNegative = false;

    BigInt quotient = dividend / divisor;
    BigInt product = quotient * divisor;
    BigInt remainder = dividend - product;

    remainder.removeLeadingZeros();
    return remainder;
  }

  BigInt operator+(const BigInt &other) const {
    if (isNegative == other.isNegative) {
      return add(other);
    } else {
      BigInt temp = other;
      temp.isNegative = !temp.isNegative;
      return subtract(temp);
    }
  }

  BigInt operator-(const BigInt &other) const {
    if (isNegative != other.isNegative) {
      BigInt temp = other;
      temp.isNegative = !temp.isNegative;
      return add(temp);
    }

    if (*this < other) {
      BigInt result = other.subtract(*this);
      result.isNegative = !isNegative;
      return result;
    }

    return subtract(other);
  }

  BigInt operator*(const BigInt &other) const { return multiply(other); }

  BigInt operator/(const BigInt &other) const {
    if (other == BigInt("0")) {
      throw invalid_argument("Division by zero");
    }

    BigInt dividend = *this;
    BigInt divisor = other;
    dividend.isNegative = false;
    divisor.isNegative = false;

    BigInt quotient;
    BigInt current;

    for (int i = dividend.digits.size() - 1; i >= 0; --i) {
      current.digits.insert(current.digits.begin(), dividend.digits[i]);
      current.removeLeadingZeros();

      int x = 0;
      int left = 0, right = 10;
      while (left <= right) {
        int mid = (left + right) / 2;
        BigInt temp = divisor * BigInt(to_string(mid));
        if (temp <= current) {
          x = mid;
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }

      quotient.digits.insert(quotient.digits.begin(), x);
      current = current.subtract(divisor * BigInt(to_string(x)));
    }

    quotient.removeLeadingZeros();
    quotient.isNegative = isNegative != other.isNegative;
    return quotient;
  }

  bool operator<(const BigInt &other) const {
    if (digits.size() != other.digits.size()) {
      return digits.size() < other.digits.size();
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
      if (digits[i] != other.digits[i]) {
        return digits[i] < other.digits[i];
      }
    }
    return false;
  }

  bool operator==(const BigInt &other) const {
    return digits == other.digits && isNegative == other.isNegative;
  }

  bool operator>=(const BigInt &other) const { return !(*this < other); }

  bool operator<=(const BigInt &other) const {
    return (*this < other) || (*this == other);
  }

  bool operator>(const BigInt &other) const {
    return !(*this < other) && !(*this == other);
  }

  bool operator!=(const BigInt &other) const { return !(*this == other); }
};

BigInt convertHexToDec(const string &hex) {
  vector<int> hexValue = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  vector<char> hexChar = {'0', '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  BigInt decValue("0");

  for (int i = 0; i < hex.size(); i++) {
    int index = 0;
    for (int j = 0; j < hexChar.size(); j++) {
      if (hex[i] == hexChar[j]) {
        index = j;
        break;
      }
    }
    decValue = decValue * BigInt("16") + BigInt(to_string(hexValue[index]));
  }

  return decValue;
}

string convertDecToHex(BigInt dec) {
  string hex;
  while (dec >= BigInt("16")) {
    BigInt remainder = dec % BigInt("16");
    dec = dec / BigInt("16");
    hex = "0123456789ABCDEF"[remainder.toInt()] + hex;
  }
  hex = "0123456789ABCDEF"[dec.toInt()] + hex;
  return hex;
}

BigInt modPow(BigInt base, BigInt exp, BigInt mod) {
  BigInt result("1");
  base = base % mod;

  while (exp > BigInt("0")) {
    if (exp % BigInt("2") == BigInt("1")) {
      result = (result * base) % mod;
    }
    exp = exp / BigInt("2");
    base = (base * base) % mod;
  }

  return result;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    return 1;
  }

  ifstream inputFile(argv[1]);
  ofstream outputFile(argv[2]);

  if (!inputFile.is_open() || !outputFile.is_open()) {
    cerr << "Error opening files!" << endl;
    return 1;
  }

  string pHex, gHex, aHex, bHex;
  inputFile >> pHex >> gHex;
  inputFile >> aHex;
  inputFile >> bHex;

  BigInt pDec = convertHexToDec(pHex);
  BigInt gDec = convertHexToDec(gHex);
  BigInt aDec = convertHexToDec(aHex);
  BigInt bDec = convertHexToDec(bHex);

  // BigInt A = modPow(gDec, aDec, pDec);
  BigInt B = modPow(gDec, bDec, pDec);
  BigInt k = modPow(B, aDec, pDec);

  outputFile << convertDecToHex(k);

  return 0;
}
