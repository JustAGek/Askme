#include<iostream>
#include<fstream>
#include<sstream>
#include<ctime>
#include<vector>
#include<string>
#include<unordered_map>
#include <random>
#include <chrono>
#include <windows.h>

using namespace std;


int generateUniqueID()
{
    int timestamp = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                         std::chrono::high_resolution_clock::now().time_since_epoch())
                                     .count() % 100000);
    int pid = getpid() % 10000;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(10000, 99999);

    return (timestamp * 10000 + pid + dist(rng)) % 2147483647;
}
/* I found it a problem to run a multiple session and using a variable to store the
the user_id or the question_id in order to increment it so I used a hashing function that can almost guaranteed
to provide me unique id.
*/


struct User
{
    int user_ID;
    bool QA;
    string email;
    string name;
    string user_name;
    string password;
};
struct Question
{
    int ID_from;
    int ID_to;
    int Question_ID;
    bool QA;
    vector<struct Question> children;
    string Question;
    int level;
    string Answer;
    bool AY;
};
struct User_manager
{
    int current_user_ID = -1;
    unordered_map<string, string> Sign_in_data;
    unordered_map<string, int> user_IDs;
    unordered_map<int, User> users;

    void list_users()
    {
        for(auto it = users.begin(); it!= users.end();)
        {
            cout << "ID: " << it->first << "\t" << "Name: " << it->second.name << endl;
            it++;
        }
    }
    void Log_out()
    {
        current_user_ID = -1;
    }
    void sign_in()
    {
        cout << "Enter your username and password: ";
        string user_name, password;
        cin >> user_name >> password;
        if (Sign_in_data[user_name] == password)
        {
            current_user_ID = user_IDs[user_name];
        }
        else cout << "Wrong Username or password, please try again" << endl;
    }
    void sign_up()
    {
        User temp;
        cout << "Enter your name: ";
        cin >> temp.name;
        cout << "Enter your email: ";
        cin >> temp.email;

        cout << "Enter your username: ";
        cin >> temp.user_name;
        if(Sign_in_data.count(temp.user_name))
        {
            cout << "This user name exist, try again" << endl;
            return;
        }
        cout << "Enter your password: ";
        cin >> temp.password;
        cout << "If your will accept Anymous Questions, Enter one else write zero: ";
        cin >> temp.QA;
        cout << "Done Successfully, Now sign in to access the Askme!" << endl;
        temp.user_ID = generateUniqueID();
        Sign_in_data[temp.user_name] = temp.password;
        user_IDs[temp.user_name] = temp.user_ID;
        users[temp.user_ID] = temp;
    }
    void fetchusers(string s)
    {
        {
            User temp;
            int space_pointer = 0;
            int end_point = s.find(',',space_pointer);
            temp.user_ID = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            end_point = s.find(',',space_pointer);
            temp.user_name = s.substr(space_pointer, end_point - space_pointer);
            space_pointer = end_point + 1;
            end_point = s.find(',',space_pointer);
            temp.password= s.substr(space_pointer, end_point - space_pointer);
            space_pointer = end_point + 1;
            end_point = s.find(',',space_pointer);
            temp.name = s.substr(space_pointer, end_point - space_pointer);
            space_pointer = end_point + 1;
            end_point = s.find(',',space_pointer);
            temp.email = s.substr(space_pointer, end_point - space_pointer);
            space_pointer = end_point + 1;
            end_point = s.find(',',space_pointer);
            temp.QA = stoi(s.substr(space_pointer, end_point - space_pointer));
            Sign_in_data[temp.user_name] = temp.password;
            user_IDs[temp.user_name] = temp.user_ID;
            users[temp.user_ID] = temp;
        }

    }
    void load()
    {
        string s;
        ifstream input("users.txt");
        while(getline(input,s)&& s!="")
        {
            fetchusers(s);
        }
        input.close();

    }
    void save_into(ofstream& output)
    {
        string s;
        for (auto& x : users)
        {
            s = "";
            s+=to_string(x.second.user_ID);
            s+=',';
            s+=x.second.user_name;
            s+=',';
            s+=x.second.password;
            s+=',';
            s+=x.second.name;
            s+=',';
            s+=x.second.email;
            s+=',';
            s+=to_string(x.second.QA);
            s+='\n';
            output << s;
        }
    }
    void save()
    {
        ofstream output("users.txt");
        save_into(output);
    }

};
struct Question_manager
{
    vector<Question> Questions;
    string s;
    int size = 0;
    int number_fetching(int index)
    {
        while (index < int(s.size()) && isdigit(s[index]))
        {
            index++;
        }
        return index;
    }
    void fetchReply(vector<Question> &a, int space_pointer,int level)
    {
        if (s[space_pointer] == ' ')
        {
            fetchReply(a.back().children,space_pointer+1,level+1);
        }
        else
        {
            Question temp;
            int end_point = number_fetching(space_pointer);
            temp.Question_ID = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            end_point = number_fetching(space_pointer);
            temp.ID_from = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            end_point = number_fetching(space_pointer);
            temp.ID_to = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            end_point = number_fetching(space_pointer);
            temp.QA = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            end_point = number_fetching(space_pointer);
            int ss = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            temp.Question = s.substr(space_pointer, ss);
            space_pointer += ss+1;
            end_point = number_fetching(space_pointer);
            ss = stoi(s.substr(space_pointer, end_point - space_pointer));
            space_pointer = end_point + 1;
            temp.Answer = s.substr(space_pointer, ss);
            space_pointer += ss+1;
            end_point = number_fetching(space_pointer);
            temp.AY = stoi(s.substr(space_pointer, end_point - space_pointer));
            temp.level = level;
            a.push_back(temp);
            size++;

        }

    }

    void load()
    {
        ifstream input("questions.txt");
        while(getline(input,s)&& s!="")
        {
            fetchReply(Questions, 0,0);
        }
        input.close();

    }
    void save_into(vector<Question> root,ofstream& output)
    {
        for (auto& x : root)
        {
            s="";
            for(int i = 0; i < x.level; i++)
                s+= " ";
            s+=to_string(x.Question_ID);
            s+=',';
            s+=to_string(x.ID_from);
            s+=',';
            s+=to_string(x.ID_to);
            s+=',';
            s+=to_string(x.QA);
            s+=',';
            s+=to_string(x.Question.size());
            s+='#';
            s+=x.Question;
            s+=',';
            s+=to_string(x.Answer.size());
            s+='#';
            s+=x.Answer;
            s+=',';
            s+=to_string(x.AY);
            s+='\n';
            output << s;
            save_into(x.children,output);
        }

    }
    void save()
    {
        ofstream output("questions.txt");
        save_into(Questions,output);
    }
    int answer_question(vector<Question> &root,int user_ID,int Qid, string ans)
    {
        for(auto &x: root)
        {
            if(x.Question_ID == Qid)
            {
                if(x.ID_to == user_ID)
                {
                    x.Answer = ans;
                    x.AY = 1;
                    return 1;
                }
                else if(x.ID_to != user_ID)
                {
                    return 0;
                }
            }
            if (x.children.empty() != true)
            {
                if (answer_question(x.children, user_ID, Qid,ans))
                    return 1;
            }
        }
        return -1;
    }
    void questions(vector<Question> &root,int id, int ft)
    {
        if (ft == 1)
        {
            for(auto &x: root)
            {
                if(x.ID_to == id)
                {
                    cout << "Question ID(" << x.Question_ID << ") ";
                    if(x.QA == 0) cout << "From User(" << x.ID_from << "):";
                    cout << "\t Q: " << x.Question << endl;
                    if (x.AY == 0) cout << "\t A: Not Answered Yet!" << endl;
                    else cout << "\t A: " << x.Answer << endl;
                }
                if(x.children.size() != 0) questions(x.children,id,ft);
            }
        }
        else if (ft == 2)
        {
            for(auto &x: root)
            {
                if(x.ID_from == id)
                {
                    cout << "Question ID(" << x.Question_ID << ") ";
                    cout << "To User(" << x.ID_to << "):";
                    cout << "\t Q: " << x.Question << endl;
                    if (x.AY == 0) cout << "\t A: Not Answered Yet!" << endl;
                    else cout << "\t A: " << x.Answer << endl;
                }
                if(x.children.size() != 0) questions(x.children,id,ft);
            }
        }
        else if (ft == 3)
        {
            for(auto &x: root)
            {
                for(int i = 0; i < x.level; i++)
                    cout << " ";
                cout << "Question ID(" << x.Question_ID << ")";

                if(x.QA == 0)
                {
                    for(int i = 0; i < x.level; i++)
                        cout << " ";
                    cout << "From User(" << x.ID_from << ") ";
                }
                for(int i = 0; i < x.level; i++)
                    cout << " ";
                cout << "To User(" << x.ID_to << "):" << endl;
                for(int i = 0; i < x.level; i++)
                    cout << " ";
                cout << "\t Q: " << x.Question << endl;
                for(int i = 0; i < x.level; i++)
                    cout << " ";
                if (x.AY == 0) cout << "\t A: Not Answered Yet!" << endl;
                else cout << "\t A: " << x.Answer << endl;

                if(x.children.size() != 0) questions(x.children,id,ft);
            }
        }

    }

    int delete_question(vector<Question> &root, int id, int user_Id)
    {
        for (auto it = root.begin(); it != root.end(); )
        {
            if (it->Question_ID == id)
            {
                if(it->ID_from == user_Id || it->ID_from == user_Id)
                {
                    it = root.erase(it);
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
            if (it->children.empty() != true)
            {
                if (delete_question(it->children, id, user_Id))
                    return 1;
            }
            it++;
        }
        return 0;
    }
    int ask_question(vector<Question> &root, int stat, int from,int to,string t,bool QA, int level = 0)
    {
        if (stat == -1)
        {
            Question temp;
            temp.ID_from = from;
            temp.ID_to = to;
            temp.QA = QA;
            temp.level = 0;
            temp.Question = t;
            temp.Question_ID = generateUniqueID();
            Questions.push_back(temp);
            return 1;
        }
        else
        {
            for (auto it = root.begin(); it != root.end(); )
            {
                if(it->ID_to != to && it->Question_ID == stat)
                    return -1;
                if(it->ID_to == to && it->Question_ID == stat)
                {
                    Question temp;
                    temp.ID_from = from;
                    temp.ID_to = to;
                    temp.QA = QA;
                    temp.level = 0;
                    temp.Question = t;
                    temp.Question_ID = generateUniqueID();
                    Questions.push_back(temp);
                    return 1;
                }
                if (it->children.empty() != true)
                {
                    if (ask_question(it->children, stat, from,to,t,QA, level + 1))
                        return 1;
                }
                it++;
            }
        }
        return 0;
    }
    void Questions_To_Me(int id)
    {
        questions(Questions,id,1);
    }
    void Questions_From_Me(int id)
    {
        questions(Questions,id,2);
    }
    void Feed(int id)
    {
        questions(Questions,id,3);
    }
};
struct askme
{
    User_manager user_manager;
    Question_manager question_manager;
    void user_menu()
    {
        int state;
        cout << "---------------User Menu---------------" << endl;
        while(true)
        {
            question_manager.load();
            cout << "Menu:" << endl;
            cout << "\t 1: Print Questions To Me" << endl;
            cout << "\t 2: Print Questions From Me" << endl;
            cout << "\t 3: Answer Question" << endl;
            cout << "\t 4: Delete Question" << endl;
            cout << "\t 5: Ask Question" << endl;
            cout << "\t 6: List System Users" << endl;
            cout << "\t 7: Feed" << endl;
            cout << "\t 8: Logout" << endl;
            cout << endl;
            cout << "Enter an option: ";
            cin >> state;
            if(state == 1)
                question_manager.Questions_To_Me(user_manager.current_user_ID);
            else if (state == 2)
                question_manager.Questions_From_Me(user_manager.current_user_ID);
            else if (state == 3)
            {
                int QID;
                string ans;
                cout << "Enter the question ID you want to answer: ";
                cin >> QID;
                cout << "What's your answer?: ";
                cin >> ans;
                int state = question_manager.answer_question(question_manager.Questions,user_manager.current_user_ID,QID,ans);
                if(state == 1) cout << "Answer successfully" << endl;
                else if (state == 0) cout << "This Question isn't asked to You!" << endl;
                else cout << "This Question don't exist" << endl;
            }
            else if (state == 4)
            {
                int QID;
                cout << "Enter the question ID you want to delete: ";
                cin >> QID;
                int state = question_manager.delete_question(question_manager.Questions,user_manager.current_user_ID,QID);
                if(state == 1)
                    cout << "Question removed successfully" << endl;
                else if (state == -1)
                    cout << "unauthorized delete: The question must be either asked to you or from you to delete" << endl;
                else cout << "This Question don't exist" << endl;

            }
            else if (state == 5)
            {
                int user_id;
                cout << "Enter user ID you want to ask: ";
                cin >> user_id;
                if (user_manager.users.count(user_id))
                {
                    if(user_id == user_manager.current_user_ID)
                        cout << "You can't ask yourself duh!" << endl;
                    else
                    {
                        int QA;
                        int stats;
                        string que;
                        if(user_manager.users[user_id].QA == 0)
                            cout << "Note: Anonymous questions aren't allowed for this user" << endl;
                        else
                        {
                            cout << "Type one to ask anonymously and 0 for non-anonymously: ";
                            cin >> QA;
                        }
                        cout << "For thread question: Enter Question ID or -1 for new question: ";
                        cin >> stats;
                        cout << "What do you want to ask?: ";
                        cin >> que;
                        int stat = question_manager.ask_question(question_manager.Questions,stats,user_manager.current_user_ID,user_id,que,QA,0);
                        if(stat == 1)
                        {
                            cout << "Question asked successfully" << endl;
                        }
                        else if (stat == -1) cout << "This Question isn't asked for that user!!" << endl;
                        else cout << "This Question don't exist" << endl;
                    }
                }
                else cout << "This user don't exist!" << endl;

            }
        else if (state == 6)
            user_manager.list_users();
        else if (state == 7)
            question_manager.Feed(user_manager.current_user_ID);
        else if (state == 8)
        {
            user_manager.current_user_ID = -1;
            break;
        }
        else
        {
            cout << "ERROR: invalid number....Try again" << endl;
        }
        question_manager.save();
    }
    }
    void start_menu()
    {
        cout << "---------------HELLO---------------" << endl;
        cout << "WELCOME TO THE ASKME PLATFORM" << endl;
        int status;
        while(true)
        {
            user_manager.load();
            cout << "Menu:" << endl;
            cout << "\t 0: Exit" << endl;
            cout << "\t 1: Login" << endl;
            cout << "\t 2: Sign Up" << endl;
            cout << endl;
            cout << "Enter an option: ";
            cin >> status;
            if(status == 0)
            {
                cout << "See You later!" << endl;
                break;
            }
            else if(status == 1)
            {
                user_manager.sign_in();
            }
            else if(status == 2)
            {
                user_manager.sign_up();
            }
            else cout << "ERROR: invalid number....Try again" << endl;
            if(user_manager.current_user_ID != -1)
                user_menu();
            user_manager.save();
        }
    }
    void start()
    {
        start_menu();
    }
};

int main()
{
    askme project;
    project.start();
    return 0;
}




