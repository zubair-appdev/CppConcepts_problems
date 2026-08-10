#include <iostream>
#include <vector>
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

int main()
{
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
