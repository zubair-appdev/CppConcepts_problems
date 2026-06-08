#include <iostream>

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

int main()
{
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
    return 0;
}
