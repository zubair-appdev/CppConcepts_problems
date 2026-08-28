#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <thread>
#include <mutex>

#include <future>

using namespace std;

bool checkPalindrome(string name,int left,int right)
{
    if(left >= right)
    {
        return true;
    }

    if(name[left] != name[right])
    {
        return false;
    }

    return checkPalindrome(name,left+1,right-1);
}

bool findDuplicate(string s);

//Rule Of 5
class Buffer
{
    char *Adata;

public:
    Buffer(const char *data)
    {
        cout<<"ctor called"<<endl;
        Adata = new char[strlen(data)+1];
        strcpy(Adata,data);
    }

    Buffer(const Buffer &other)
    {
        cout<<"copy ctor called"<<endl;
        Adata = new char[strlen(other.Adata)+1];
        strcpy(Adata,other.Adata);
    }

    Buffer& operator=(const Buffer &other)
    {
        cout<<"copy Assn called"<<endl;

        if(this != &other)
        {
            cout<<"CALL"<<endl;
            delete[] Adata;
            Adata = new char[strlen(other.Adata)+1];
            strcpy(Adata,other.Adata);
        }

        return *this;

    }

    Buffer(Buffer &&other) noexcept
    {
        cout<<"Move ctor called"<<endl;
        Adata = other.Adata;
        other.Adata = nullptr;
    }

    Buffer& operator=(Buffer &&other) noexcept
    {
        cout<<"MOVE Assn called"<<endl;

        if(this != &other)
        {
            delete[] Adata;
            Adata = other.Adata;
            other.Adata = nullptr;
        }

        return *this;
    }

    ~Buffer()
    {
        cout<<"dtor called"<<endl;
        delete[] Adata;

    }

    void print()
    {
        if(Adata != nullptr)
            cout<<Adata<<" :data"<<endl;
        else
            cout<<"null data"<<endl;

    }
};

//Runtime polymorphism
class Animal
{
public:
    virtual void speak()
    {
        cout<<"Animal Speaks"<<endl;
    }

    virtual ~Animal()
    {
        cout<<"Animal dtor called"<<endl;
    }
};

class Cat : public Animal
{
public:
    void speak() override
    {
        cout<<"Cat Speaks"<<endl;
    }

    ~Cat()
    {
        cout<<"Cat dtor called"<<endl;

    }
};

//Composition
class Engine
{
public:
    void engineStart()
    {
        cout<<"engine is starting"<<endl;
    }

};

class Car
{
public:
    void start()
    {
        e1.engineStart();
        cout<<"Car is starting"<<endl;
    }
private:
    Engine e1;
};

//DI + Aggregation
class Energy
{
public:
    void jungleEnergy()
    {
        cout<<"Jungle's Energy"<<endl;
    }
};

class Lion
{
public:
    void kingOfJungle(Energy *energy)
    {
        ene1 = energy;
        ene1->jungleEnergy();
        cout<<"Lion is created"<<endl;
    }

private:
    Energy *ene1;
};

// Singleton
class Singleton{
public:
    static Singleton& getInstance()
    {
        static Singleton s;
        return s;
    }

    void method()
    {
        cout<<"singleton's method"<<endl;
    }
private:
    Singleton()
    {
        cout<<"Singleton private constructor"<<endl;
    }

    Singleton(const Singleton& other) = delete;
    Singleton& operator= (const Singleton& other) = delete;
};

void printEven(int num)
{
    long long sum = 0;

    cout<<this_thread::get_id()<<" :Even thread Id"<<endl;
    for(int i=0; i<num; i++)
    {
        if(i % 2 == 0)
        {
            sum += i;
        }
    }
}

void printOdd(int num)
{
    long long sum = 0;

    cout<<this_thread::get_id()<<" :Odd thread Id"<<endl;
    for(int i=0; i<num; i++)
    {
        if(i % 2 != 0)
        {
            sum += i;
        }
    }
}

int count = 0;
mutex mtx;

void increment()
{
    for(int i=0; i<1'00'000; i++)
    {
        lock_guard<mutex> myMutex(mtx);
        count++;
    }
}

void sleepFuncNormal()
{
    for(int i=0; i<5; i++)
    {
        cout<<"NORMAL ONE"<<endl;
        cout<<"Is it happening 5 times: "<<i<<endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }

}

void sleepFunc(promise<int> p)
{
    for(int i=0; i<5; i++)
    {
        cout<<"Is it happening 5 times: "<<i<<endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    p.set_value(999);
}


int main()
{
    //multithreading section
    auto start = chrono::steady_clock::now();
    thread t1(printEven,100'000'000);
    thread t2(printOdd,100'000'000);

    t1.join();
    t2.join();

    auto end = chrono::steady_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout<<"Multithread Time(ms): "<<duration.count()<<endl;

    // Normal Section
    auto start1 = chrono::steady_clock::now();
    printEven(100'000'000);
    printOdd(100'000'000);

    auto end1 = chrono::steady_clock::now();

    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1);

    cout<<"Normal Time(ms): "<<duration1.count()<<endl;

    //Race Condition
    thread t3(increment);
    thread t4(increment);

    t3.join();
    t4.join();

    cout<<count<<" :Shared variable count"<<endl;

    //Fire-and-forget thread detach()
    thread t5(sleepFuncNormal);
    t5.detach(); //Here if main finishes t5 can't complete it's 5 operations
    // See thread needs either join or detach

    // C++ don't have exact QFutureWatcher mechanism we need to
    // Implement it by using Conditional variable (Queue)
    // But we have promise and future with that also we can achieve some
    // But this thing is used when you want to get result from worker thread
    promise<int> p;
    future<int> f = p.get_future();

    thread t6(sleepFunc,move(p));

    int result5 = f.get();
    t6.join();
    cout<<result5<<" :result from thread"<<endl;


    // Singleton
    Singleton &s1 = Singleton::getInstance();
    s1.method();

    //DI + aggregation
    Energy e1;

    Lion l1;
    l1.kingOfJungle(&e1);

    Lion l2;
    l2.kingOfJungle(&e1);

    //Composition
    Car c1;
    c1.start();


    //Runtime polymorphism
    Animal *a = new Cat();
    a->speak();

    delete a;

    //Rule Of 5
    Buffer b1("zubair");
    b1.print();

    Buffer b2 = "Rahul";
    b2.print();

    b2 = b1;
    b2.print();
    b1.print();

    cout<<"###"<<endl;
    Buffer b3 = move(b2);
    b3.print();
    b2.print();

    b2 = move(b1);
    b2.print();

    // Problem 1 : Palindrome check using recursion
    string name = "malayalam";
    bool result = checkPalindrome(name,0,name.length()-1);

    if(result)
    {
        cout<<name<<" is palindrome"<<endl;
    }
    else
    {
        cout<<name<<" is not palindrome"<<endl;
    }

    // Problem 2 : Longest Substring Without Repeating Characters
    string str = "pwwkew";
    vector<string> subStr;
    string temp = "";
    int count = 0;
    int max = 0;
    string resultStr;

    for(size_t i=0; i<str.length(); i++)
    {
        temp = "";
        for(size_t j=i; j<str.length(); j++)
        {
            temp = temp+str[j];
            subStr.push_back(temp);
        }
    }

    for(auto sub : subStr)
    {
        bool result = findDuplicate(sub);

        if(result == true)
        {
            count = sub.length();

            if(count > max)
            {
                max = count;
                resultStr = sub;
            }

        }
    }

    cout<<resultStr<<" "<<resultStr.length()<<endl;

    // Problem 3 : Moving zeroes to end without disturbing order
    int nums[5] = {3,0,9,1,0};
    int size = sizeof (nums)/sizeof (nums[0]);

    int j = 0;

    for(int i=0; i<size; i++)
    {
        if(nums[i] != 0)
        {
            nums[j] = nums[i];
            j++;

        }
    }

    for(int i=j; i<size; i++)
    {
        nums[i] = 0;
    }


    //Results
    for(int i=0; i<size; i++)
    {
        cout<<nums[i]<<" ";
    }
    cout<<endl;

    // Problem 4 : finding frequency of letters
    string name1 = "madam";

    for (size_t i=0; i<name1.length(); i++)
    {
        bool alreadyDone = false;

        for(size_t k=0; k<i; k++)
        {
            if(name1[i] == name1[k])
            {
                alreadyDone = true;
                break;
            }
        }

        if(alreadyDone)
            continue;

        int count1 = 0;

        for (size_t j = 0; j < name1.length(); j++)
        {
            if (name1[i] == name1[j])
                count1++;
        }

        cout << name1[i] << " " << count1 << endl;
    }

    //Problem 5 : First Non Repeating Character and Repeating Character
    string place = "swiss";
    bool noUnique = false;

    for(size_t i=0; i<place.length(); i++)
    {
        int countPlace = 0;

        for(size_t j=0; j<place.length(); j++)
        {
            if(place[i] == place[j])
            {
                countPlace++;
            }
        }

//      For First Non Repeating Character
                if(countPlace == 1)
                {
                        noUnique = true;
                        cout<<place[i]<<" :First Non Repeating Character"<<endl;
                        break;
                }

//      For First Repeating Character
//        if(countPlace >= 2)
//        {
//            cout<<place[i]<<" :First Repeating Character"<<endl;
//            break;
//        }
    }

    if(!noUnique)
        cout<<"No First Non Repeating Characters"<<endl;

    // Problem 5 ended

    //Problem 6 — Remove Duplicate Characters
    string car = "SwiftDesire";
    string newCar;

    for(size_t i=0; i<car.length(); i++)
    {
        bool check = false;

        for(size_t j=0; j<car.length(); j++)
        {
           if(newCar[j] == car[i])
           {
               check = true;
               break;
           }
        }

        if(!check)
            newCar += car[i];
    }

    cout<<newCar<<" :newCar"<<endl;

    //Problem 7 — Palindrome using two pointers
    string palindrome = "abba";

    size_t jp = palindrome.length();
    cout<<jp/2<<" :jp"<<endl;
    bool checkPalin = true;

    for(size_t i=0; i<palindrome.length()/2; i++)
    {
        if(palindrome[i] != palindrome[jp-1])
        {
            checkPalin = false;
            break;
        }
        jp--;
    }

    if(checkPalin)
        cout<<palindrome<<" :is palindrome"<<endl;
    else
        cout<<palindrome<<" :is NOT palindrome"<<endl;


    //Problem 8 — Two Sum Using Two Pointers
    vector<int> v = {1, 2, 3, 4, 6, 8, 9};
    int targetVal = 17;

    int iterations = 0;

    int jv = v.size() - 1;

    for(size_t i=0; i<v.size(); i++)
    {

        if(v[i] + v[jv] > targetVal)
        {
            jv--;
        }

        if(v[i] + v[jv] == targetVal)
        {
            cout<<"TARGET FOUND "<<v[i]<<" "<<v[jv]<<endl;
            break;
        }

        iterations++;
    }

    cout<<iterations<<" :iterations"<<endl;

    //continue from here ... prove gpt is wrong here for above problem for two pointer.

    cout << "END OF MAIN" << endl;
    return 0;
}

bool findDuplicate(string s)
{
    for(size_t i=0; i<s.length(); i++)
    {
        for(size_t j=i+1; j<s.length(); j++)
        {
            if(s[i] == s[j])
            {
                return false;
            }
        }
    }
    return true;
}
