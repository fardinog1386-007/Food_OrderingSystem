#include<iostream>
#include<string>
#include"1.h"
#include"Database.h"
using namespace std;

int main()
{
    sqlite3* db;
    if (sqlite3_open("Service_food.db" , &db) != SQLITE_OK)
    {
        cout << "Eror" << endl;
    }
    CreateTable(db);
    UserDB User(db);
    ResturantDB restu(db);
    MenuDB menu(db);
    OrderDB order(db);
    int choice;
    while (true)
    {
        cout << "_______ Welcome to Food Service App _________\n";
        cout << "1_ Login\n";
        cout << "2_ Register\n";
        cout << "3_ Exit\n";
        cin >> choice;
        if (choice == 1)
        {
            string user;
            string password;
            cout << "Please Enter your username: ";
            cin >> user;
            cout << endl;
            cout << "Please Enter your password: ";
            cin >> password;
            cout << endl;
            if (User.Login(user , password))
            {
                cout << "Login Successfully.\n";
                string role = User.Getrole(user);
                if (role == "Customer")
                {
                    Customer cus;
                    cus.showportal();
                    cin >> choice;
                    if (choice == 1)
                    {
                        restu.ShowAcceptedRest();
                    }
                    else if (choice == 2)
                    {
                        
                    }
                    
                }
                else if (role == "ResturantBoss")
                {
                    ResturantBoss rest;
                    rest.showportal();
                }
                else if (role == "Admin")
                {
                    Admin admin;
                    admin.showportal();
                }
            }
            else
            {
                cout << "Invalid Username or Password" << endl;
                continue;
            }
            
        }
        if (choice == 2)
        {
            string user;
            string password;
            int role = 0;
            cout << "Please Enter a username: ";
            cin >> user;
            cout << endl;
            cout << "Please Enter a password: ";
            cin >> password;
            cout << endl;
            while (role > 0 || role < 4)
            {
                cout << "Plaese Enter your Role:\n1_ Customer\n2_ ResturantManager\n3_ Admin\n";
                cin >> role;
                if (role > 0 || role < 4)
                {
                    cout << "Wrong choice Please Try Again.\n";
                    continue;
                }
            }
            ROLE Role = static_cast<ROLE>(role);
            if(User.Register(user , password , Role))
            {
                cout << "Register Succesfull." << endl;
                continue;
            }
        }
    }
    
}