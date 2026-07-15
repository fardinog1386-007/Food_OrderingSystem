#ifndef DB_H
#define DB_H
#include <iostream>
#include <string>
#include "sqlite3.h"
class Customer;
using namespace std;

enum class ROLE
{
    Customer = 1,
    ResturantBoss,
    Admin
};

class UserDB
{
private:
    sqlite3* sq;
public:
    UserDB(sqlite3* db) : sq{db} {}
    bool Register(string user, string pass, ROLE role)
    {
        string r = "";
        if (role == ROLE::Customer) r = "Customer";
        else if (role == ROLE::ResturantBoss) r = "ResturantBoss";
        else if (role == ROLE::Admin) r = "Admin";
        
        string str = "INSERT INTO Users (Username, Password, Role, Points, Level) VALUES ('" + user + "', '" + pass + "', '" + r + "', 0, 'Normal');";
        if (sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK)
        {
            return true;
        }
        return false;
    }
    bool Login(string user, string pass)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT * FROM Users WHERE Username = '" + user + "' AND Password = '" + pass + "';";
        bool success = false;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                success = true;
            }
        }
        sqlite3_finalize(stmt);
        return success;
    }
    string Getrole(string user)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Role FROM Users WHERE Username = '" + user + "';";
        string role = "None";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        return role;
    }
    int GetuserID(string user)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT id FROM Users WHERE Username = '" + user + "';";
        int id = -1;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                id = sqlite3_column_int(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return id;
    }
    int ActiveMembers()
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Role, COUNT(*) FROM Users GROUP BY Role;";
        int total_count = 0;
        
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW) 
            {
                const unsigned char* role = sqlite3_column_text(stmt, 0);
                int count = sqlite3_column_int(stmt, 1);
                
                if (role != nullptr) 
                {
                    cout << role << ": " << count << endl;
                }
                total_count += count;
            }
        }
        sqlite3_finalize(stmt);
        return total_count;
    }
};

class ResturantDB
{
private:
    sqlite3* sq;
public:
    ResturantDB(sqlite3* db) : sq{db} {}
    void AddResturant(int id, string name, string exp, string address, string phone, int open, int time)
    {
        string str = "INSERT INTO Resturants (ID, Name, Address, OpenSTATUS, Explain, Time, PhoneNumber) VALUES (" 
                    + to_string(id) + ", '" + name + "', '" + address + "', " + to_string(open) + ", '" + exp + "', " + to_string(time) + ", '" + phone + "');";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }

void ShowAllResturant()
{
    sqlite3_stmt* stmt;
    string str = "SELECT ID, Name, Address, OpenSTATUS, Time, PhoneNumber FROM Resturants WHERE Accept = 1;";
    if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        cout << "ID | Name | Address | Status | Time | Phone\n";
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int status = sqlite3_column_int(stmt, 3);
            int time = sqlite3_column_int(stmt, 4);
            string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            
            cout << id << " | " << name << " | " << address << " | " << (status ? "Open" : "Closed") << " | " << time << " | " << phone << endl;
        }
    }
    sqlite3_finalize(stmt);
}
    bool IsRestaurantOpen(int restID)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT OpenSTATUS FROM Resturants WHERE ID = " + to_string(restID) + ";";
        int status = 0;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                status = sqlite3_column_int(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return status == 1;
    }
    void ShowRequest()
    {
        sqlite3_stmt* stmt;
        string str = "SELECT ID, Name FROM Resturants WHERE Accept = 0;";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int id = sqlite3_column_int(stmt, 0);
                string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                cout << "ID: " << id << " | Name: " << name << endl;
            }
        }
        sqlite3_finalize(stmt);
    }
    void AcceptRest(int id)
    {
        string str = "UPDATE Resturants SET Accept = 1 WHERE ID = " + to_string(id) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }
};

class MenuDB
{
private:
    sqlite3* sq;
public:
    MenuDB(sqlite3* db) : sq{db} {}
    void AddFooditem(string name, string explain, double price, string type, int cooktime, int restID , string soldout)
    {
        string str = "INSERT INTO Menu (Name, Explain, price, Soldout, Type, Cooking_Time, RestID) VALUES ('" + name + "', '" + explain + "', " + to_string(price) + ", '"+soldout+"', '" + type + "', " + to_string(cooktime) + ", " + to_string(restID) + ");";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }

    void AddDrinkitem(string name, string explain, double price, string type, double volume, int restID , string soldout)
    {
        string str = "INSERT INTO Menu (Name, Explain, price, Soldout, Type, Volume, RestID) VALUES ('" + name + "', '" + explain + "', " + to_string(price) + ", '"+soldout+"', '" + type + "', " + to_string(volume) + ", " + to_string(restID) + ");";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }
    void showMenu(int restid)
{
    sqlite3_stmt* stmt;
    string str = "SELECT ID, Name, Explain, price, Type , Soldout FROM Menu WHERE RestID = " + to_string(restid) + ";";
    if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        cout << "\n==== MENU ====\n";
        bool hasItems = false;
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            hasItems = true;
            int id = sqlite3_column_int(stmt, 0);
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string exp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double price = sqlite3_column_double(stmt, 3);
            string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            string soldout = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            cout << id << " | " << name << " | " << exp << " | " << price << "$ | " << type << " | Vaziat:" << soldout << endl;
        }
        if (!hasItems)
        {
            cout << "Hich Itemi peida nashod.\n";
        }
    }
    sqlite3_finalize(stmt);
}
    void DeleteItem(int itemID, int restID)
    {
        string str = "DELETE FROM Menu WHERE ID = " + to_string(itemID) + " AND RestID = " + to_string(restID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        cout << "Item Ba Mofaghariat Hazf Shod.\n";
    }

    void SetSoldOut(int itemID, int restID)
    {
        string str = "UPDATE Menu SET Soldout = 'Namojod' WHERE ID = " + to_string(itemID) + " AND RestID = " + to_string(restID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        cout << "Vaziat Item Be Namojod Taghir Yaft.\n";
    }
    void SetUNSoldOut(int itemID, int restID)
    {
        string str = "UPDATE Menu SET Soldout = 'Mojod' WHERE ID = " + to_string(itemID) + " AND RestID = " + to_string(restID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        cout << "Vaziat Item Be Mojod Taghir Yaft.\n";
    }
    string getsoldout(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Soldout FROM Menu WHERE ID = " + to_string(id) + ";";
        string soldout = "";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                soldout = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        return soldout;
    }
    string Getname(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Name FROM Menu WHERE ID = " + to_string(id) + ";";
        string name = "";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        return name;
    }
    string Getexplain(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Explain FROM Menu WHERE ID = " + to_string(id) + ";";
        string exp = "";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                exp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        return exp;
    }
    double GetPrice(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT price FROM Menu WHERE ID = " + to_string(id) + ";";
        double price = 0;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                price = sqlite3_column_double(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return price;
    }
    double GetVolume(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Volume FROM Menu WHERE ID = " + to_string(id) + ";";
        double volume = 0;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                volume = sqlite3_column_double(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return volume;
    }
    int GetCookTime(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Cooking_Time FROM Menu WHERE ID = " + to_string(id) + ";";
        int time = 0;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                time = sqlite3_column_int(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return time;
    }
    string GetType(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Type FROM Menu WHERE ID = " + to_string(id) + ";";
        string type = "";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        return type;
    }
};

class OrderDB
{
private:
    sqlite3* sq;
public:
    OrderDB(sqlite3* db) : sq{db} {}
    void order(int custID, int restID, double price)
    {
        string str = "INSERT INTO Orders (CustomerID, ResturantID, Price, Stat) VALUES (" + to_string(custID) + ", " + to_string(restID) + ", " + to_string(price) + ", 'Dar hale barresi');";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }
    void ShowOrderForRest(int restID)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT ID, CustomerID, Price, Stat FROM Orders WHERE ResturantID = " + to_string(restID) + ";";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int id = sqlite3_column_int(stmt, 0);
                int custID = sqlite3_column_int(stmt, 1);
                double price = sqlite3_column_double(stmt, 2);
                string stat = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                cout << "OrderID: " << id << " | " << "CustomerID: " << custID << " | " << "Price: " << price << " | " << "Status: " << stat << endl;
            }
        }
        sqlite3_finalize(stmt);
    }
    void ShowOrderForCustomer(int customerid)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT ID, ResturantID, Price, Stat FROM Orders WHERE CustomerID = " + to_string(customerid) + ";";
        
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            cout << "\n======= ORDER HISTORY =======" << endl;
            cout << "OrderID | RestaurantID | Total Price | Status" << endl;
            cout << "--------------------------------------------" << endl;
            
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int orderId = sqlite3_column_int(stmt, 0);
                int restId = sqlite3_column_int(stmt, 1);
                double price = sqlite3_column_double(stmt, 2);
                
                const unsigned char* text = sqlite3_column_text(stmt, 3);
                string stat = text ? reinterpret_cast<const char*>(text) : "Unknown";
                
                cout << "OrderID:" << orderId << " | ResturantID: " << restId << " | Price:" << price << " | Status: " << stat << endl;
            }
        }
        sqlite3_finalize(stmt);
    }
    void UpdateOrderStatus(int orderID, string newStatus) 
    {
        string str = "UPDATE Orders SET Stat = '" + newStatus + "' WHERE ID = " + to_string(orderID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        cout << "Vaziat Sefaresh " << orderID << " Be " << newStatus << " Taghir Yaft\n";
    }
    double ShowAllResturantSellInfo()
    {
        sqlite3_stmt* stmt;
        string str = "SELECT ResturantID, COUNT(*), SUM(Price) FROM Orders GROUP BY ResturantID;";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int restid = sqlite3_column_int(stmt, 0);
                int ordercount = sqlite3_column_int(stmt, 1);
                double income = sqlite3_column_double(stmt, 2);
                cout << "ResturantID: " << restid << " | Total Orders: " << ordercount << " | Total Income: " << income << endl;
            }
        }
        sqlite3_finalize(stmt);
        return 0.0;
    }
};
class CustomerDB
{
private:
    sqlite3 *sq;
public:
    CustomerDB(sqlite3* db) : sq{db} {}
    Customer LoadCustomer(int id)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT id, Username, Password, Points, Level FROM Users WHERE id = " + to_string(id) + ";";
        int points = 0;
        string level = "Normal";
        string username = "", password = "";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                const unsigned char* u = sqlite3_column_text(stmt, 1);
                const unsigned char* p = sqlite3_column_text(stmt, 2);
                username = u ? reinterpret_cast<const char*>(u) : "";
                password = p ? reinterpret_cast<const char*>(p) : "";
                points = sqlite3_column_int(stmt, 3);
                const unsigned char* lv = sqlite3_column_text(stmt, 4);
                if (lv) 
                {
                    level = reinterpret_cast<const char*>(lv);
                }
            }
        }
        sqlite3_finalize(stmt);
        return Customer(id, username, password, "Customer", points, level);
    }
    void SaveCustomer(Customer &c , int custID)
    {
        string str = "UPDATE Users SET Points = " + to_string(c.getpoint()) +
                     ", Level = '" + c.getlevel()->getlevel() + "' WHERE id = " + to_string(custID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }
    void AchiveNextLevel(int custID , string NewLevel , string OldLevel)
    {
        string str = "INSERT INTO LevelHistory(CustomerID, OldLevel , NewLevel) VALUES ("+to_string(custID)+", "+OldLevel+", "+NewLevel+");";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    }
    void ShowLevelHistory(int custID)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT OldLevel, NewLevel, ChangeDate FROM LevelHistory WHERE CustomerID = " + to_string(custID) + " ORDER BY ID;";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            cout << "\n===== YOUR LEVEL HISTORY =====\n";
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                string oldL = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                string newL = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                cout << oldL << " -> " << newL << " | " << date << endl;
            }
        }
        sqlite3_finalize(stmt);
    }

    void ShowAllLevelHistory()
    {
        sqlite3_stmt* stmt;
        string str = "SELECT CustomerID, OldLevel, NewLevel FROM LevelHistory ORDER BY ID DESC;";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            cout << "\n===== LEVEL CHANGE LOG (ALL CUSTOMERS) =====\n";
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int custID = sqlite3_column_int(stmt, 0);
                string oldL = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                string newL = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                cout << "CustomerID: " << custID << " | " << oldL << " -> " << newL << endl;
            }
        }
        sqlite3_finalize(stmt);
    }
    void ShowLevelReport()
    {
            sqlite3_stmt* stmt;
            string str = "SELECT Level, COUNT(*) FROM Users WHERE Role='Customer' GROUP BY Level;";
            if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
            {
                cout << "\n===== LOYALTY LEVEL REPORT =====\n";
                while (sqlite3_step(stmt) == SQLITE_ROW)
                {
                    const unsigned char* lv = sqlite3_column_text(stmt, 0);
                    string level = lv ? reinterpret_cast<const char*>(lv) : "Normal";
                    int count = sqlite3_column_int(stmt, 1);
                    cout << level << ": " << count << " customer(s)" << endl;
                }
            }
            sqlite3_finalize(stmt);
        }
        void UpdateLastOrderDate(int custID)
        {
            string str = "UPDATE Users SET LastOrderDate = strftime('%Y-%m-%d','now') WHERE id = " + to_string(custID) + ";";
            sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        }
        void CheckDowngrade(int custID)
        {
            sqlite3_stmt* stmt;
        string str = "SELECT Level, Points, "
                    "CAST(julianday('now', 'localtime') - julianday(LastOrderDate) AS INTEGER) "
                    "FROM Users WHERE id = " + to_string(custID) + " AND LastOrderDate IS NOT NULL;";
        
        string currentLevel = "Normal";
        int currentPoints = 0;
        int daysPassed = 0;
        bool processDowngrade = false;

        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                const unsigned char* lv = sqlite3_column_text(stmt, 0);
                currentLevel = lv ? reinterpret_cast<const char*>(lv) : "Normal";
                currentPoints = sqlite3_column_int(stmt, 1);
                daysPassed = sqlite3_column_int(stmt, 2);
                if (daysPassed >= 30 && currentLevel != "Normal")
                {
                    processDowngrade = true;
                }
            }
        }
        sqlite3_finalize(stmt);
        if (processDowngrade)
        {
            string newLevel = "Normal";
            int newPoints = 0;
            if (currentLevel == "VIP")
            {
                newLevel = "Gold";
                newPoints = 301;
            }
            else if (currentLevel == "Gold")
            {
                newLevel = "Silver";
                newPoints = 101;
            }
            else if (currentLevel == "Silver")
            {
                newLevel = "Normal";
                newPoints = 0;
            }
            string updateQuery = "UPDATE Users SET Level = '" + newLevel + "', Points = " + to_string(newPoints) + 
                                "WHERE id = " + to_string(custID) + ";";
            sqlite3_exec(sq, updateQuery.c_str(), nullptr, nullptr, nullptr);

            string historyQuery = "INSERT INTO LevelHistory(CustomerID, OldLevel, NewLevel) VALUES (" + 
                                to_string(custID) + ", '" + currentLevel + "', '" + newLevel + "');";
            sqlite3_exec(sq, historyQuery.c_str(), nullptr, nullptr, nullptr);

            cout << "\n Because of not be active for 30 days Your level Downgrade to: " << newLevel << " \n" << endl;
        }
    }
    void AdminSetLevel(int custID, string newLevel)
    {
        Customer c = LoadCustomer(custID);
        string oldLevel = c.getlevel()->getlevel();
        string str = "UPDATE Users SET Level = '" + newLevel + "' WHERE id = " + to_string(custID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        AchiveNextLevel(custID, newLevel, oldLevel);
        cout << "Level updated: " << oldLevel << " -> " << newLevel << endl;
    }

    void AdminSetPoints(int custID, int points)
    {
        string str = "UPDATE Users SET Points = " + to_string(points) + " WHERE id = " + to_string(custID) + ";";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        cout << "Points updated.\n";
    }
    void UseCoupon(int custID)
    {
    string str = "UPDATE Copon SET Count = Count - 1 WHERE CustomerID = " + to_string(custID) + " AND Count > 0 LIMIT 1);";
    sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
    cout << "Yek adad Copon ba movafaghariat sabt va az hesab shoma kasr shod.\n";
    }
    void setBadges(int custID, string badge)
{
    sqlite3_stmt* stmt;
    string str = "SELECT Badges FROM Users WHERE id = " + to_string(custID) + ";";
    string badges = "";
    if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const unsigned char* b = sqlite3_column_text(stmt, 0);
            if (b) badges = reinterpret_cast<const char*>(b);
        }
    }
    sqlite3_finalize(stmt);

    if (badges.find(badge) != string::npos)
    {
        return;
    }
    else
    {
        badges += "\n" + badge;
    }
    string upd = "UPDATE Users SET Badges = '" + badges + "' WHERE id = " + to_string(custID) + ";";
    sqlite3_exec(sq, upd.c_str(), nullptr, nullptr, nullptr);
}
    string GetBadges(int custID)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT Badges FROM Users WHERE id = " + to_string(custID) + ";";
        string badges = "None";
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                const unsigned char* b = sqlite3_column_text(stmt, 0);
                if (b && reinterpret_cast<const char*>(b)[0] != '\0')
                    badges = reinterpret_cast<const char*>(b);
            }
        }
        sqlite3_finalize(stmt);
        return badges;
    }
    void MonthlyCopon(int custID , string lvl)
    {
        sqlite3_stmt* stmt;
        string checkStr = "SELECT COUNT(*) FROM Copon WHERE CustomerID = " + to_string(custID) +
                           " AND Month = strftime('%Y-%m','now','localtime');";
        int alreadyGiven = 0;
        if (sqlite3_prepare_v2(sq, checkStr.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                alreadyGiven = sqlite3_column_int(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);

        if (alreadyGiven > 0)
        {
            return;
        }

        int count = 0;
        if (lvl == "Silver")
        {
            count = 1;
        }
        else if (lvl == "Gold")
        {
            count = 1;
        }
        else if (lvl == "VIP")
        {
            count = 3;
        }
        else
        {
            return;
        }
        string str = "INSERT INTO Copon(CustomerID , Month , Count) VALUES ("+to_string(custID)+" , strftime('%Y-%m' , 'now' , 'localtime'), "+to_string(count)+");";
        sqlite3_exec(sq, str.c_str(), nullptr, nullptr, nullptr);
        cout << "You received " << count << " special coupon(s) this month!\n";
    }
    int GetCopon(int custID)
    {
        sqlite3_stmt* stmt;
        string str = "SELECT COALESCE(SUM(Count), 0) FROM Copon WHERE CustomerID = " + to_string(custID) + ";";
        int total = 0;
        if (sqlite3_prepare_v2(sq, str.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                total = sqlite3_column_int(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
        return total;
    }
    int IncrementOrderCount(int custID)
    {
        string upd = "UPDATE Users SET OrderCount = COALESCE(OrderCount,0) + 1 WHERE id = " + to_string(custID) + ";";
        sqlite3_exec(sq, upd.c_str(), nullptr, nullptr, nullptr);

        sqlite3_stmt* stmt;
        string q = "SELECT OrderCount FROM Users WHERE id = " + to_string(custID) + ";";
        int cnt = 0;
        if (sqlite3_prepare_v2(sq, q.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
                cnt = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
        return cnt;
    }
};
inline void CreateTable(sqlite3* db)
{
    char* erormess = 0;
    const char* Users = "CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY AUTOINCREMENT, Username TEXT UNIQUE, Password TEXT, Role TEXT, "
    "Points INTEGER DEFAULT 0 , Level TEXT DEFAULT 'Normal', LastOrderDate TEXT, Badges TEXT, OrderCount INTEGER DEFAULT 0);";
    const char* Resturants = "CREATE TABLE IF NOT EXISTS Resturants (ID INTEGER PRIMARY KEY, Name TEXT, Address TEXT, OpenSTATUS INTEGER,Explain TEXT ,"
    "Time INTEGER, PhoneNumber TEXT, Accept INTEGER DEFAULT 0);";
    const char* Menu = "CREATE TABLE IF NOT EXISTS Menu(ID INTEGER PRIMARY KEY, Name TEXT, Explain TEXT, price REAL, Soldout TEXT, Type TEXT, Cooking_Time INTEGER, Volume REAL," 
    "RestID INTEGER,FOREIGN KEY (RestID) REFERENCES Resturants(id));";
    const char* Orders = "CREATE TABLE IF NOT EXISTS Orders(ID INTEGER PRIMARY KEY AUTOINCREMENT, CustomerID INTEGER, ResturantID INTEGER, Price REAL, Stat TEXT);";
    const char* Copon = "CREATE TABLE IF NOT EXISTS Copon(ID INTEGER PRIMARY KEY AUTOINCREMENT, CustomerID INTEGER , Month TEXT , Count INTEGER);";
    const char* LevelHistory = "CREATE TABLE IF NOT EXISTS LevelHistory(ID INTEGER PRIMARY KEY AUTOINCREMENT,CustomerID INTEGER, OldLevel TEXT , NewLevel TEXT)";
    sqlite3_exec(db, Users, nullptr, nullptr, &erormess);
    sqlite3_exec(db, Resturants, nullptr, nullptr, &erormess);
    sqlite3_exec(db, Menu, nullptr, nullptr, &erormess);
    sqlite3_exec(db, Orders, nullptr, nullptr, &erormess);
    sqlite3_exec(db, LevelHistory, nullptr, nullptr, &erormess);
    sqlite3_exec(db, Copon, nullptr, nullptr, &erormess);

}

#endif