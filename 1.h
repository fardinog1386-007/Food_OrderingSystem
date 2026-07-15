#ifndef PROJECT_H
#define PROJECT_H
#include <iostream>
#include <string>
#include <vector>
class OrderDB;
class CustomerDB;
using namespace std;

class OrderDB;
class LoyaltyPoints
{
public:
    virtual string getlevel() = 0;
    virtual int nextlevel() = 0;
    virtual double zarib() = 0;
    virtual double discount()= 0;
    virtual double deliveryprice(double pricee) = 0;
    virtual ~LoyaltyPoints() {}
};
class Normal : public LoyaltyPoints
{
public:
    string getlevel() override
    {
        return "Normal";
    }
    int nextlevel() override
    {
        return 100;
    }
    double zarib() override
    {
        return 1;
    }
    double discount() override
    {
        return 0;
    }
    double deliveryprice(double pricee) override
    {
        return pricee;
    }
};
class Silver : public LoyaltyPoints
{
public:
    string getlevel() override
    {
        return "Silver";
    }
    int nextlevel() override
    {
        return 300;
    }
    double zarib() override
    {
        return 1.2;
    }
    double discount() override
    {
        return 0.05;
    }
    double deliveryprice(double pricee) override
    {
        return pricee * 0.90;
    }
};
class Gold : public LoyaltyPoints
{
public:
    string getlevel() override
    {
        return "Gold";
    }
    int nextlevel() override
    {
        return 700;
    }
    double zarib() override
    {
        return 1.5;
    }
    double discount() override
    {
        return 0.10;
    }
    double deliveryprice(double pricee) override
    {
        return pricee * 0.5;
    }
};
class VIP : public LoyaltyPoints
{
public:
    string getlevel() override
    {
        return "VIP";
    }
    int nextlevel() override
    {
        return 999999;
    }
    double zarib() override
    {
        return 2;
    }
    double discount() override
    {
        return 0.15;
    }
    double deliveryprice(double pricee) override
    {
        return 0;
    }
};
class Menuitem
{
protected:
    int id;
    string name;
    string explaination;
    double price;
    bool soldout_or_no;
public:
    Menuitem(int ID, string n , string ex ,double p , bool sold) : id{ID} , name{n} , explaination{ex} , price{p} , soldout_or_no(sold){}
    virtual void displayinfo() = 0;
    double getprice();
    virtual ~Menuitem(){};
};

class Food : public Menuitem
{
private:
    int cooking_time;
public:
    Food(int _id , string _name , string _explain , double _price , bool _sold , int _cooktime) : Menuitem(_id , _name , _explain , _price , _sold) , cooking_time{_cooktime}{}
    virtual void displayinfo() override;
};

class Drink : public Menuitem
{
private:
    double V;
public:
    Drink(int _id , string _name , string _explain , double _price , bool _sold , double _V) : Menuitem(_id , _name , _explain , _price , _sold) , V{_V}{}
    virtual void displayinfo() override;
};

class Resturant
{
private:
    int id;
    string name;
    string address;
    bool openorclose;
    int time;
    string phone_number;
    vector<Menuitem *> menu;
public:
    Resturant(int ID , string Name , string add ,bool op , int Time , string phone);
    void Showinfo(string s);
    void additem(Menuitem * item);
    void showMenu();
    ~Resturant();
};

class User
{
protected:
    int id;
    string username;
    string password;
    string role;
public:
    User(int _id , string _username , string _password , string _role) : id{_id} , username{_username} , password{_password} , role{_role}{}
    virtual void showportal() = 0;
    virtual ~User(){}
};

class Customer : public User
{
private:
    int customerID;
    int points;
    LoyaltyPoints* level;
public:
    Customer(int _id , string _username , string _password , string _role , int point , string lev) : User(_id , _username , _password , "Customer") , points(point)
    {
        if (lev == "Silver")
        {
            level = new Silver();
        }
        else if (lev == "Gold")
        {
            level = new Gold;
        }
        else if (lev == "VIP")
        {
            level = new VIP;
        }
        else
        {
            level = new Normal;
        }
    }
    Customer(): User(0, "", "", "Customer"), customerID{0} {}
    void showportal() override;
    void addpoint(double orderprice);
    void checkLevelUpgrade();
    int getpoint()
    {
        return points;
    }
    LoyaltyPoints* getlevel()
    {
        return level;
    }
};

class ResturantBoss : public User
{
private:
    int bossID;
public:
    ResturantBoss(int _id , string _username , string _password , string _role) : User(_id , _username , _password , "ResturantBoss"), bossID{_id}{}
    ResturantBoss(): User(0, "", "", "ResturantBoss"), bossID{0} {};
    int getBossID();
    void showportal() override;
};

class Admin : public User
{
public:
    Admin(int _id , string _username , string _password , string _role) : User(_id , _username , _password , "Admin"){}
    Admin();
    void showportal() override;
};

class Order
{
private:
    int orderID;
    int customerid;
    int resturantid;
    vector<Menuitem *> items;
    double Price = 0;
    string Stat;
public:
    Order(int _orderID, int _customerid , int _resturantid) : orderID{_orderID} , customerid{_customerid}, resturantid{_resturantid} {}
    void addfood(Menuitem * x);
    int getorderid();
    double getprice();
    void printOrder();
    void setprice(double m);
};

class SabadKharid
{
private:
    vector<Order> orders;
public:
    void EditSabad(int id);
    void showSabad();
    void Finalize(OrderDB &db ,CustomerDB &custDB , Customer &cust, int custID , int RestID , int use);
    void addOrder(Order x) { orders.push_back(x); }
};

#endif