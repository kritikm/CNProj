/*
Description:    C++ file to read questions from text files and extract the correct answer from the same
Author:         Nimish Agrawal
Created:        November 8, 2016

Reference for patch:
http://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g
*/

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace std;

// Add other parameters as per the requirements of the master file
class Questions
{
    public:
        int q_id;
//        bool used = false;
//        string question;
        char answer;
};

int random_number()
{
    return 123%7;
}

char fetch_next_question(int q_no, Questions ques)
{
    string q_no_character = patch::to_string(q_no);

    ifstream q_file;
    string q_file_name = q_no_character.append(".txt");
    q_file.open(q_file_name.c_str());

    string content;
    while(q_file >> content)
    {
        if(content[0] == 58)
            break;                      // 58 is ASCII for ':'
        int len = content.length();
        if(content[len-1] == 63)        // 63 is ASCII for '?'
        {
            cout<<" "<<content;
            cout<<endl;
            continue;
        }
        else if( content[len-1] == 46)  // 46 is ASCII for '.'
        {
            cout<<endl;
            cout<<content;
            continue;
        }
        cout<<" "<<content;
    }
    ques.answer = content[1];
    return ques.answer;
}

int main()
{
    int q_no;
    cin>>q_no;
    Questions ques;
    ques.answer = fetch_next_question(q_no, ques);
    cout<<"\nThe answer is "<<ques.answer;
    return 0;
}
