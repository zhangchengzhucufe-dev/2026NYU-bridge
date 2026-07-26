// This is the definition for the class Money.
// Values of this type are amounts of money in U.S. currency.
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

using namespace std;
class Money
{
public:
    friend Money operator+(const Money &amount1, const Money &amount2);
    // Returns the sum of the values of amount1 and amount2.
    friend Money operator-(const Money &amount1, const Money &amount2);
    // Returns amount1 minus amount2.
    friend Money operator-(const Money &amount);
    // Returns the negative of the value of amount.
    friend bool operator==(const Money &amount1, const Money &amount2);
    // Returns true if amount1 and amount2 have the same value; false otherwise.
    friend bool operator<(const Money &amount1, const Money &amount2);
    // Returns true if amount1 is less than amount2; false otherwise.
    Money(long dollars, int cents);
    // Initializes the object so its value represents an amount with
    // the dollars and cents given by the arguments. If the amount
    // is negative, then both dollars and cents should be negative.
    Money(long dollars);
    // Initializes the object so its value represents $dollars.00.
    Money();
    // Initializes the object so its value represents $0.00.
    double get_value() const;
    // Returns the amount of money recorded in the data portion of the calling
    // object.
    void set_value(long cents_in);
    // Sets all_cents to cents_in
    void set_value(double amount);
    // Sets all_cents to equivalent integer version of amount
    friend istream &operator>>(istream &ins, Money &amount);
    // Overloads the >> operator so it can be used to input values of type
    // Money. Notation for inputting negative amounts is as in − $100.00.
    // Precondition: If ins is a file input stream, then ins has already been
    // connected to a file.

    friend ostream &operator<<(ostream &outs, const Money &amount);
    // Overloads the << operator so it can be used to output values of type
    // Money. Precedes each output value of type Money with a dollar sign.
    // Precondition: If outs is a file output stream, then outs has already been
    // connected to a file.
private:
    long all_cents;
};

int digit_to_int(char c);
// Used in the definition of the overloaded input operator >>.
// Precondition: c is one of the digits '0' through '9'.
// Returns the integer for the digit; for example, digit_to_int('3') returns 3.
Money::Money(long dollars, int cents)
{
    if (dollars * cents < 0) // If one is negative and one is positive
    {
        cout << "Illegal values for dollars and cents.\n";
        exit(1);
    }
    all_cents = dollars * 100 + cents;
}
Money::Money(long dollars) : all_cents(dollars * 100)
{
    // Body intentionally blank.
}
Money::Money() : all_cents(0)
{
    // Body intentionally blank.
}
double Money::get_value() const
{
    return (all_cents * 0.01);
}

void Money::set_value(long cents_in)
{
    all_cents = cents_in;
}

void Money::set_value(double amount)
{
    all_cents = amount * 100;
}

Money operator+(const Money &amount1, const Money &amount2)
{
    Money temp;
    temp.all_cents = amount1.all_cents + amount2.all_cents;
    return temp;
}

bool operator==(const Money &amount1, const Money &amount2)
{
    return (amount1.all_cents == amount2.all_cents);
}

Money operator-(const Money &amount1, const Money &amount2)
{
    Money temp;
    temp.all_cents = amount1.all_cents - amount2.all_cents;
    return temp;
}
Money operator-(const Money &amount)
{
    Money temp;
    temp.all_cents = -amount.all_cents;
    return temp;
}

bool operator<(const Money &amount1, const Money &amount2)
{
    if (amount1.all_cents < amount2.all_cents)
        return true;
    return false;
}

istream &operator>>(istream &ins, Money &amount)
{
    char one_char, decimal_point,
        digit1, digit2; // digits for the amount of cents
    long dollars;
    int cents;
    bool negative; // set to true if input is negative.
    ins >> one_char;
    if (one_char == '-')
    {
        negative = true;
        ins >> one_char; // read '$'
    }
    else
        negative = false;
    // if input is legal, then one_char == '$'
    ins >> dollars >> decimal_point >> digit1 >> digit2;
    if (one_char != '$' || decimal_point != '.' || !isdigit(digit1) || !isdigit(digit2))
    {
        cout << "Error illegal form for money input\n";
        exit(1);
    }
    cents = digit_to_int(digit1) * 10 + digit_to_int(digit2);
    amount.all_cents = dollars * 100 + cents;
    if (negative)
        amount.all_cents = -amount.all_cents;
    return ins;
}

int digit_to_int(char c)
{
    return (static_cast<int>(c) - static_cast<int>('0'));
}
// Uses cstdlib and iostream:
ostream &operator<<(ostream &outs, const Money &amount)
{
    long positive_cents, dollars, cents;
    positive_cents = labs(amount.all_cents);
    dollars = positive_cents / 100;
    cents = positive_cents % 100;

    if (amount.all_cents < 0)
        outs << "− $" << dollars << '.';
    else
        outs << "$" << dollars << '.';

    if (cents < 10)
        outs << '0';
    outs << cents;

    return outs;
}


class Check
{
private:
    long all_cents;
    bool cashed;
    int num;
public:
    Check(bool cashed, int num, long dollars, int cents);
    Check(bool cashed, int num, long dollars);
    void set_value(double amount)
    {
        all_cents = amount * 100;
    }
    

};

void Check::set_value(double amount)
{
    all_cents = amount * 100;
}


int main()
{
    int capacity = 16;

    return 0;
}