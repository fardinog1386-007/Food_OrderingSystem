#ifndef PROJECT_H
#define PROJECT_H
#include<iostream>
#include<string>
#include<vector>
using namespace std;
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
    Drink(int _id , string _name , string _explain , double _price , bool _sold , int _V) : Menuitem(_id , _name , _explain , _price , _sold) , V{_V}{}
    virtual void displayinfo() override;
};
class Address
{
private:
    string city;
    string street;
    string house_number;
public:
    Address();
   Address(string c , string st , string h_num): city{c} , street{st} , house_number{h_num}{} // ثبت آدرس و نشان دادن پیام موفقیت
    string getAddress()
    {
        string add = city + "," + street + "," + house_number + "\n";
        return add;
    }
};
class Resturant
{
private:
    int id;
    string name;
    Address address;
    bool openorclose;
    int time;
    string phone_number;
    vector<Menuitem*> menu;
public:
    Resturant(int ID , string Name , Address add, bool op , int Time , string phone);
    void Showinfo(string s);
    void additem(Menuitem *item);
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
    virtual ~User();
};
class Customer : public User
{
public:
    Customer(int _id , string _username , string _password , string _role) : User(_id , _username , _password , "Customer"){}
    Customer();
    void showportal() override;
};
class ResturantBoss : public User
{
public:
    ResturantBoss(int _id , string _username , string _password , string _role) : User(_id , _username , _password , "ResturantBoss"){}
    ResturantBoss();
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
    double getprice()
    {
        return Price;
    }
    void printOrder();
    ~Order(){}
};
class SabadKharid
{
private:
    vector<Order> orders;
public:
    void showSabad();
    void EditSabad(int id);
    
    void Finalize(OrderDB &db , int custID , int RestID);
};
#endif