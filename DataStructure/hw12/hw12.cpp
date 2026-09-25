#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <algorithm> // 用于排序 (Extra Credit)

using namespace std;

// ============================================================================
// 1. Money Class Definition & Implementation (Provided in money.cpp)
// ============================================================================
class Money
{
public:
    friend Money operator+(const Money &amount1, const Money &amount2);
    friend Money operator-(const Money &amount1, const Money &amount2);
    friend Money operator-(const Money &amount);
    friend bool operator==(const Money &amount1, const Money &amount2);
    friend bool operator<(const Money &amount1, const Money &amount2);
    
    Money(long dollars, int cents);
    Money(long dollars);
    Money();

    double get_value() const;
    void set_value(long cents_in);
    void set_value(double amount);

    friend istream &operator>>(istream &ins, Money &amount);
    friend ostream &operator<<(ostream &outs, const Money &amount);

private:
    long all_cents;
};

int digit_to_int(char c);

Money::Money(long dollars, int cents)
{
    if (dollars * cents < 0)
    {
        cout << "Illegal values for dollars and cents.\n";
        exit(1);
    }
    all_cents = dollars * 100 + cents;
}

Money::Money(long dollars) : all_cents(dollars * 100) {}
Money::Money() : all_cents(0) {}

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
    all_cents = static_cast<long>(amount * 100);
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
    return (amount1.all_cents < amount2.all_cents);
}

istream &operator>>(istream &ins, Money &amount)
{
    char one_char, decimal_point, digit1, digit2;
    long dollars;
    int cents;
    bool negative = false;

    ins >> one_char;
    if (one_char == '-')
    {
        negative = true;
        ins >> one_char; // read '$'
    }

    ins >> dollars >> decimal_point >> digit1 >> digit2;
    if (one_char != '$' || decimal_point != '.' || !isdigit(digit1) || !isdigit(digit2))
    {
        cout << "Error: Illegal format for money input. Example format: $100.50 or -$100.50\n";
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

ostream &operator<<(ostream &outs, const Money &amount)
{
    long positive_cents = labs(amount.all_cents);
    long dollars = positive_cents / 100;
    long cents = positive_cents % 100;

    if (amount.all_cents < 0)
        outs << "-$" << dollars << '.';
    else
        outs << "$" << dollars << '.';

    if (cents < 10)
        outs << '0';
    outs << cents;

    return outs;
}

// ============================================================================
// 2. Check Class Definition & Implementation (Homework Requirement)
// ============================================================================
class Check
{
private:
    int number;
    Money amount;
    bool cashed;
public:
    //constructors
    Check();
    Check(int num, Money amt, bool is_cashed);

    //accessors
    int get_number() const;
    Money get_amount() const;
    bool is_cashed() const;

    //mutators
    void set_number(int num);
    void set_amount(Money amt);
    void set_cashed(bool is_cashed);

    //input&output
    void input();
    void output() const;
    
};

Check::Check():number(0),amount(0),cashed(false){}
Check::Check(int num, Money amt, bool is_cashed):number(num),amount(amt),cashed(is_cashed){}

int Check::get_number() const {return number;}
Money Check::get_amount() const {return amount;}
bool Check::is_cashed() const {return cashed;}

void Check::set_number(int num){number=num;}
void Check::set_amount(Money amt){amount=amt;}
void Check::set_cashed(bool is_cashed){cashed=is_cashed;}

void Check::input()
{
    int flag;
    cout << "Enter check number: ";
    cin >> number;
    cout << "Enter check amount(e.g.,$50.25)";
    cin >> amount;
    cout << "Has the check been cashed?(1 for Yes, 0 for No): ";
    cin >> flag;
    cashed =(flag==1);
}

void Check::output() const
{
    cout << "Check #" << number << ": " << amount <<" [" << (cashed?"Cashed":"Uncashed") << "]";
}

bool compareChecks(const Check &c1, const Check &c2)
{
    return c1.get_number() < c2.get_number();
}

const int Max_Checks=100;
const int Max_Deposits=100;

int main()
{
    Money old_balance;
    cout << "====checkbook balacing program====\n";
    cout << "Enter the old account balance(e.g.,$1000.00): ";
    cin >> old_balance;

    //1. Read Deposits
    int num_deposits=0;
    Money deposits[Max_Deposits];
    Money total_deposits(0);

    cout << "\n---Deposit Entry---\n";
    cout << "How many deposits would you like to enter? \n";
    cin >> num_deposits;

    for(int i =0;i<num_deposits;++i)
    {
        cout << "Enter deposits #" << (i+1) << " amount (e.g.,$100.00): " << '\n';
        cin >> deposits[i];
        total_deposits = total_deposits + deposits[i]; 
    }

    //2. Read Checks
    int num_checks=0;
    Check checks[Max_Checks];

    cout << "\n--- Check Entry ---\n";
    cout << "How many checks would you like to enter? \n";
    cin >> num_checks;

    for(int i=0;i<num_checks;i++)
    {
        cout << "Enter checks #" << (i+1) << '\n';
        checks[i].input();
    }

    //3. calculate totals for cashed and uncashed checks
    Money cashed_sum(0);
    Money uncashed_sum(0);

    for(int i;i<num_checks;i++)
    {
        if (checks[i].is_cashed())
            cashed_sum = cashed_sum+checks[i].get_amount();
        else
            uncashed_sum=uncashed_sum+checks[i].get_amount();
    }
    
    //4. calculate balances
    Money bank_balance = old_balance + total_deposits - cashed_sum;
    Money actual_balance = bank_balance - uncashed_sum;
    Money difference = uncashed_sum;

    //5.sort checks by check number
    sort(checks, checks+num_checks,compareChecks);

    //6.display output reports
    cout << "\n===summary report===\n";

    cout << "---cashed checks---\n";
    bool has_cashed=false; //to check if there is a cashed check
    for(int i=0;i<num_checks;i++)
    {
        if(checks[i].is_cashed()){
            checks[i].output();
            cout << '\n';
            has_cashed=true;
        }
    }
    if (!has_cashed) cout << "No cashed check\n";
    cout << "Total Cashed Checks Amount: " << cashed_sum << '\n';

    cout << "---uncashed checks---\n";
    bool has_uncashed=false;
    for(int i;i<num_checks;i++)
    {
        if(!checks[i].is_cashed())
        {
            checks[i].output();
            cout<< endl;
            has_uncashed=true;
        }
    }
    if(!has_uncashed) cout << "No uncashed checks\n";
    cout << "Total Uncashed Checks Amount: " << uncashed_sum << '\n';

    cout <<"---Balance Calculations---\n";
    cout << "Total Deposits:                " << total_deposits << endl;
    cout << "New Bank Balance:              " << bank_balance << endl;
    cout << "Actual Account Balance:        " << actual_balance << endl;
    cout << "Difference (Outstanding):      " << difference << endl;
    cout << "=============================\n";

    return 0;
}


