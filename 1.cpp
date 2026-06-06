#include<iostream>
#include<string>
#include "1.h"
#include"Database.h"
using namespace std;
void SabadKharid::EditSabad(int id)
{
    bool find = false;
    for (int i = 0; i < orders.size(); i++)
    {
        if (id == orders[i].getorderid())
        {
            orders.erase(orders.begin()+i);
            cout << "Edit Succesfully\n";
            find = true;
            break;
        }
    }
    if (find == false)
    {
        cout << "Invalid OrderId" << endl;
    }
}
void SabadKharid::showSabad()
{
    cout << "______ SABAD KHARID ________" <<endl;
    for (int i = 0; i < orders.size(); i++)
    {
        orders[i].printOrder();
    }
}
void SabadKharid::Finalize(OrderDB &db , int custID , int RestID)
{
    if (orders.empty())
    {
        cout << "Sabad Kharid Khalie" << endl;
    }
    double totalprice = 0;
    for (auto &order : orders)
    {
        totalprice += order.getprice();
    }
    db.order(custID , RestID , totalprice);
    orders.clear();
    cout << "Etelaat ba movafaghit sabt shod." << endl;
}
void Order::addfood(Menuitem* x)
{
    items.push_back(x);
    Price += x->getprice();
}
int Order::getorderid()
{
    return orderID;
}
void Order::printOrder()
{
    cout << "Order Id: " << orderID << endl;
    for (int i = 0; i < items.size(); i++)
    {
        items[i]->displayinfo();
    }
    cout << "Total Price: " << Price << endl;
}
double Menuitem::getprice()
{
    return price;
}
void Food::displayinfo() override
{
    cout << "Food: " << name << " | Price: " << price << " | Explanation: " << explaination << " | Time to ready: " << cooking_time << " Minute" << endl;
}
void Drink::displayinfo() override
{
    cout << "Drink: " << name << " | Price: " << price << " | Explanation: " << explaination << " | Volume: " << V << endl;
}
Resturant::Resturant(int ID , string Name , Address add ,bool op , int Time , string phone): id{ID} , name{Name}, address{add}, openorclose{op} , time{Time} , phone_number{phone}
{
    cout << "Resturant Added Succesfully" << endl;
}
void Resturant::Showinfo(string s)
{
    cout << s << endl;
}
void Resturant::additem(Menuitem *item)
{
    menu.push_back(item);
}
void Resturant::showMenu()
{
    cout << "==== MENU ====" << endl;
    for (auto item : menu)
    {
        item->displayinfo();
    }
}
Resturant::~Resturant()
{
    for (auto item : menu)
    {
        delete item;
    }
}
void Customer::showportal() override
{
    cout << "1_ View restruants\n" << "2_ order history\n" << "3_ Sabad Kharid" << "4_ Log out\n";
}
void ResturantBoss::showportal() override
{
    cout << "1_ Edit Resturant\n" << "2_ See and edit Menu\n" << "3_Order List\n" <<"4_ Log out\n";
}
void Admin::showportal() override
{
    cout << "1_Request Lists\n" << "2_ resturant information\n" << "3_ Log out\n";
}