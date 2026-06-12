#include <iostream>
#include <string>
#include "1.h"
#include "Database.h"
using namespace std;

void SabadKharid::EditSabad(int id)
{
    bool find = false;
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (id == orders[i].getorderid())
        {
            orders.erase(orders.begin()+i);
            cout << "Edit Successfully\n";
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
    for (size_t i = 0; i < orders.size(); i++)
    {
        orders[i].printOrder();
    }
}
void SabadKharid::Finalize(OrderDB &db , int custID , int RestID)
{
    if (orders.empty())
    {
        cout << "Sabad Kharid Khalie" << endl;
        return;
    }
    double totalprice = 0;
    for (auto &order : orders)
    {
        totalprice += order.getprice();
    }
    db.order(custID , RestID , totalprice);
    orders.clear();
    cout << "Sefareshe Shoma Sabt Shod.\n";
}
double Menuitem::getprice()
{
    return price;
}
void Food::displayinfo()
{
    cout << "Food: " << name << " | Price: " << price << " | Explanation: " << explaination << " | Time to ready: " << cooking_time << " Minute" << endl;
}

void Drink::displayinfo()
{
    cout << "Drink: " << name << " | Price: " << price << " | Explanation: " << explaination << " | Volume: " << V << endl;
}

Resturant::Resturant(int ID , string Name , string add ,bool op , int Time , string phone): id{ID} , name{Name}, address{add}, openorclose{op} , time{Time} , phone_number{phone}
{
    cout << "Resturant Added Successfully" << endl;
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
void Customer::showportal()
{
    cout << "1_ View restaurants\n";
}
void Order::addfood(Menuitem *x)
{
    items.push_back(x);
    Price += x->getprice();
}

int Order::getorderid()
{
    return orderID;
}

double Order::getprice()
{
    return Price;
}

void Order::printOrder()
{
    cout << "OrderID: " << orderID << " | RestaurantID: " << resturantid << " | Price: " << Price;
}

void ResturantBoss::showportal() 
{
	cout << "--- Resturant Boss Portal ---" << endl;
}
int ResturantBoss::getBossID() { return bossID; }
Admin::Admin() : User(0, "", "", "Admin") {}
void Admin::showportal() 
{
	cout << "--- Admin Portal ---" << endl;
}