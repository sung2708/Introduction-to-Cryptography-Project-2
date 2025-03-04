#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

class BigInt
{
private:
    std::vector<int> digits;
    bool isNegative;

    void removeLeadingZeros()
    {
        while (digits.size() > 1 && digits.back() == 0)
        {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0)
        {
            isNegative = false;
        }
    }

public:
    BigInt() : digits(1, 0), isNegative(false) {}

    BigInt(const std::string &value)
    {
        isNegative = false;
        std::string tempValue = value;
        if (tempValue[0] == '-')
        {
            isNegative = true;
            tempValue = tempValue.substr(1);
        }

        if (value == "0")
        {
            digits.push_back(0);
            return;
        }

        for (int i = value.size() - 1; i >= 0; --i)
        {
            digits.push_back(value[i] - '0');
        }

        removeLeadingZeros();
    }

    BigInt(int value)
    {
        isNegative = value < 0;
        value = std::abs(value);
        while (value > 0)
        {
            digits.push_back(value % 10);
            value /= 10;
        }
        if (digits.empty())
        {
            digits.push_back(0);
        }
    }

    std::string toString() const
    {
        std::string result = isNegative ? "-" : "";
        for (int i = digits.size() - 1; i >= 0; --i)
        {
            result += (digits[i] + '0');
        }
        return result;
    }

    int toInt() const
    {
        int result = 0;
        for (int i = digits.size() - 1; i >= 0; --i)
        {
            result = result * 10 + digits[i];
        }
        return isNegative ? -result : result;
    }

    BigInt add(const BigInt &other) const
    {
        BigInt result;
        result.digits.resize(std::max(digits.size(), other.digits.size()), 0);
        int carry = 0;
        for (size_t i = 0; i < result.digits.size(); ++i)
        {
            int sum = carry;
            if (i < digits.size())
                sum += digits[i];
            if (i < other.digits.size())
                sum += other.digits[i];
            carry = sum / 10;
            result.digits[i] = sum % 10;
        }
        if (carry)
        {
            result.digits.push_back(carry);
        }
        result.isNegative = isNegative;
        return result;
    }

    BigInt subtract(const BigInt &other) const
    {
        BigInt result;
        result.digits.resize(digits.size(), 0);
        int borrow = 0;
        for (size_t i = 0; i < digits.size(); ++i)
        {
            int diff = digits[i] - borrow - (i < other.digits.size() ? other.digits[i] : 0);
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.digits[i] = diff;
        }
        result.removeLeadingZeros();
        result.isNegative = isNegative;
        return result;
    }

    BigInt multiply(const BigInt &other) const
    {
        BigInt result;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i)
        {
            for (size_t j = 0; j < other.digits.size(); ++j)
            {
                result.digits[i + j] += digits[i] * other.digits[j];
                if (result.digits[i + j] >= 10)
                {
                    result.digits[i + j + 1] += result.digits[i + j] / 10;
                    result.digits[i + j] %= 10;
                }
            }
        }
        result.removeLeadingZeros();
        result.isNegative = isNegative != other.isNegative;
        return result;
    }

    BigInt operator%(const BigInt &mod) const
    {
        if (mod == BigInt("0"))
        {
            throw std::invalid_argument("Modulo by zero");
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

    BigInt operator+(const BigInt &other) const
    {
        if (isNegative == other.isNegative)
        {
            return add(other);
        }
        else
        {
            BigInt temp = other;
            temp.isNegative = !temp.isNegative;
            return subtract(temp);
        }
    }
    
    BigInt operator+=(const BigInt &other)
    {
        *this = *this + other;
        return *this;
    }
    BigInt operator-=(const BigInt &other)
    {
        *this = *this - other;
        return *this;
    }
    BigInt operator*=(const BigInt &other)
    {
        *this = *this * other;
        return *this;
    }
    BigInt operator/=(const BigInt &other)
    {
        *this = *this / other;
        return *this;
    }
    BigInt operator%= (const BigInt &other)
    {
        *this = *this % other;
        return *this;
    }
    BigInt operator-(const BigInt &other) const
    {
        if (isNegative != other.isNegative)
        {
            BigInt temp = other;
            temp.isNegative = !temp.isNegative;
            return add(temp);
        }

        if (*this < other)
        {
            BigInt result = other.subtract(*this);
            result.isNegative = !isNegative;
            return result;
        }

        return subtract(other);
    }

    BigInt operator*(const BigInt &other) const
    {
        return multiply(other);
    }

    BigInt operator/(const BigInt &other) const
    {
        if (other == BigInt("0"))
        {
            throw std::invalid_argument("Division by zero");
        }

        BigInt dividend = *this;
        BigInt divisor = other;
        dividend.isNegative = false;
        divisor.isNegative = false;

        BigInt quotient;
        BigInt current;

        for (int i = dividend.digits.size() - 1; i >= 0; --i)
        {
            current.digits.insert(current.digits.begin(), dividend.digits[i]);
            current.removeLeadingZeros();

            int x = 0;
            int left = 0, right = 10;
            while (left <= right)
            {
                int mid = (left + right) / 2;
                BigInt temp = divisor * BigInt(std::to_string(mid));
                if (temp <= current)
                {
                    x = mid;
                    left = mid + 1;
                }
                else
                {
                    right = mid - 1;
                }
            }

            quotient.digits.insert(quotient.digits.begin(), x);
            current = current.subtract(divisor * BigInt(std::to_string(x)));
        }

        quotient.removeLeadingZeros();
        quotient.isNegative = isNegative != other.isNegative;
        return quotient;
    }

    bool operator<(const BigInt &other) const
    {
        if (isNegative && !other.isNegative)
        {
            return true;
        }
        if (!isNegative && other.isNegative)
        {
            return false;
        }
        if (digits.size() != other.digits.size())
        {
            return digits.size() < other.digits.size();
        }
        for (int i = digits.size() - 1; i >= 0; --i)
        {
            if (digits[i] != other.digits[i])
            {
                return digits[i] < other.digits[i];
            }
        }

        return false;
    }

    bool operator==(const BigInt &other) const
    {
        return digits == other.digits && isNegative == other.isNegative;
    }

    bool operator>=(const BigInt &other) const
    {
        return !(*this < other);
    }

    bool operator<=(const BigInt &other) const
    {
        return (*this < other) || (*this == other);
    }

    bool operator>(const BigInt &other) const
    {
        return !(*this < other) && !(*this == other);
    }

    bool operator!=(const BigInt &other) const
    {
        return !(*this == other);
    }
};

BigInt convertHexToDec(const std::string &hex)
{
    std::vector<int> hexValue = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<char> hexChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    BigInt decValue("0");

    for (int i = 0; i < hex.size(); i++)
    {
        int index = 0;
        for (int j = 0; j < hexChar.size(); j++)
        {
            if (hex[i] == hexChar[j])
            {
                index = j;
                break;
            }
        }
        decValue = decValue * BigInt("16") + BigInt(std::to_string(hexValue[index]));
    }

    return decValue;
}

string convertDecToHex(BigInt dec)
{
    string hex;
    while (dec >= BigInt("16"))
    {
        BigInt remainder = dec % BigInt("16");
        dec = dec / BigInt("16");
        hex = "0123456789ABCDEF"[remainder.toInt()] + hex;
    }
    hex = "0123456789ABCDEF"[dec.toInt()] + hex;
    return hex;
}

BigInt modular_multiply(BigInt a, BigInt b, BigInt mod)
{
    BigInt result = 0;
    a %= mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            result = (result + a) % mod;
        }
        a = (BigInt("2") * a) % mod;
        b /= 2;
    }
    return result;
}

BigInt modular_exponentiation(BigInt base, BigInt exp, BigInt mod)
{
    BigInt result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = modular_multiply(result, base, mod);
        }
        base = modular_multiply(base, base, mod);
        exp /= 2;
    }
    return result;
}

BigInt modular_inverse(BigInt a, BigInt p)
{
    return modular_exponentiation(a, p - 2, p);
}

pair<BigInt, BigInt> add_points(
    BigInt x1, BigInt y1,
    BigInt x2, BigInt y2,
    BigInt a, BigInt p)
{
    BigInt lambda;
    if (x1 == x2 && y1 == y2)
    { 
        BigInt numerator = modular_multiply(BigInt("3") * x1 % p, x1, p) + a;
        BigInt denominator = (BigInt("2") * y1) % p;
        lambda = modular_multiply(numerator % p, modular_inverse(denominator, p), p);
    }
    else
    {
        BigInt numerator = (y2 + p - y1) % p;
        BigInt denominator = (x2 + p - x1) % p;
        lambda = modular_multiply(numerator, modular_inverse(denominator, p), p);
    }

    BigInt x3 = (modular_multiply(lambda, lambda, p) + p - x1 + p - x2) % p;
    BigInt y3 = (modular_multiply(lambda, (x1 + p - x3) % p, p) + p - y1) % p;

    return {x3, y3};
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    string pHex, aHex, bHex, xPHex, yPHex, xQHex, yQHex;
    inputFile >> pHex >> aHex >> bHex >> xPHex >> yPHex >> xQHex >> yQHex;

    BigInt p = convertHexToDec(pHex);
    BigInt a = convertHexToDec(aHex);
    BigInt b = convertHexToDec(bHex);
    BigInt xPDec = convertHexToDec(xPHex);
    BigInt yPDec = convertHexToDec(yPHex);
    BigInt xQDec = convertHexToDec(xQHex);
    BigInt yQDec = convertHexToDec(yQHex);

    auto R = add_points(xPDec, yPDec, xQDec, yQDec, a, p);
    BigInt xR = R.first;
    BigInt yR = R.second;

    outputFile << convertDecToHex(xR) << " " << convertDecToHex(yR) << std::endl;

    return 0;
}