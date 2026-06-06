#ifndef DB_H
#define DB_H
#include<iostream>
#include<string>
#include"sqlite3.h"
#include"1.h"
using namespace std;
enum class ROLE
{
    Customer = 1,
    ResturantBoss,
    Admin
};
void CreateTable(sqlite3* db)
{
    char* erormess = 0;
    const char* Users = "CREATE TABLE IF NOT EXISTS Users (""id INTEGER PRIMARY KEY AUTOINCREMENT," "Username TEXT UNIQUE," "Password TEXT," "Role TEXT"");";
    const char* Resturants = "CREATE TABLE IF NOT EXISTS Resturants (""ID INTEGER PRIMARY KEY," "Name TEXT," "Address TEXT," "OpenSTATUS INTEGER," "Time INTEGER," "PhoneNumber TEXT," "Accept INTEGER DEFUALT 0"");";
    const char* Menu = "CREATE TABLE IF NOT EXISTS Menu(""ID INTEGER PRIMARY KEY," "Name TEXT," "Explain TEXT," "price REAL," "Soldout TEXT," "Type TEXT," "Cooking_Tme INTEGER," "Volume REAL,""RestID INTEGER," "FOREIGN KEY (resturantID) REFERENCES Resturants(id)"");";
    const char* Orders = "CREATE TABLE IF NOT EXISTS Orders(""ID INTEGER PRIMARY KEY AUTOINCREMENT," "CustomerID INTEGER," "ResturantID INTEGER," "Price REAL," "Stat TEXT"");";
    sqlite3_exec(db , Users , nullptr , nullptr , &erormess);
    sqlite3_exec(db , Resturants , nullptr , nullptr , &erormess);
    sqlite3_exec(db , Menu , nullptr , nullptr , &erormess);
    sqlite3_exec(db , Orders , nullptr , nullptr , &erormess);
}
class UserDB
{
private:
    sqlite3* db;
public:
    UserDB(sqlite3* _db): db{_db} {}
    bool Login(string username , string password)
    {
        string str = "SELECT * FROM Users WHERE USERNAME = '" +username+"' AND PASSWORD = '"+password+"';";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db , str.c_str() , -1 , &stmt , nullptr))
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }
    bool Register(string username , string password , ROLE R)
    {
        char* erromess = 0;
        string str = "INSERT INTO Users VALUES('"+username+"' , '"+password+"');";
        int exit = sqlite3_exec(db , str.c_str() , nullptr ,0 , &erromess);
        if (exit == SQLITE_OK)
        {
            return true;
        }
        cout << "Eror: " << erromess << endl;
        sqlite3_free(erromess);
        return false;
    }
    string Getrole(string username)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Role FROM Users WHERE Username = '"+username+"';";
        if (sqlite3_prepare_v2(db , str.c_str() , -1 , &stmt , nullptr))
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                const unsigned char* roletext = sqlite3_column_text(stmt , 0);
                string role = reinterpret_cast<const char*>(roletext);
                sqlite3_finalize(stmt);
               return role;
            }
        }
    }
};
class ResturantDB
{
private:
    sqlite3* db;
public:
    ResturantDB(sqlite3* _db) : db{_db}{}
    void addResturant(Resturant * res , int id , string name, Address add , int time , string phone , int accept)
    {
        string str = "INSERT INTO Resturant(id , name , add ,time , phone , accept) VALUES ("+to_string(id)+" , '"+name+"', '"+add.getAddress()+"' , "+to_string(time)+" , '"+phone+"' , "+to_string(accept)+");";
        sqlite3_exec(db , str.c_str() , nullptr , nullptr , nullptr);
    }
    void acceptRest(int resturantid)
    {
        string str = "UPDATE Resturant SET accept = 1 WHERE ID = "+to_string(resturantid)+";";
    }
    void ShowAcceptedRest()
    {
        string str = "SELECT * FROM Resturants WHERE accepted = 1;";
    }
};
class MenuDB
{
private:
    sqlite3 * db;
public:
    MenuDB(sqlite3* _db):db{_db} {}
    void addTOMenu(int restid ,int id , string name , string explain , double price , bool ItsAvailable)
    {
        string str = "INSERT INTO MENU(resturant_id, id , name , explain , price, availble) VALUES ("+to_string(restid)+","+to_string(id)+" , '"+name+"' , '"+explain+"' , "+to_string(price)+" , "+to_string(ItsAvailable)+");";
        sqlite3_exec(db , str.c_str() , nullptr , nullptr , nullptr);
    }
};
class OrderDB
{
private:
    sqlite3 * sq;
public:
    OrderDB(sqlite3 *_sq): sq{_sq}{}
    void order(int orederID , int customerID , int resturantID ,double total)
    {
        string str = "INSERT INTO Orders(ID , cusid , restid , price , stat) VALUES ("+to_string(orederID)+" , "+to_string(customerID)+" , "+to_string(resturantID)+" , "+to_string(total)+" , 'Pending');";
        sqlite3_exec(sq , str.c_str() , nullptr , nullptr , nullptr);
    }
    void orderhistory(int orederID ,int customerID, int resturantID ,double total)
    {
        string str = "SELECT ID , ResturantID ,Price , Stat FROM Orders "
    }
};
#endif