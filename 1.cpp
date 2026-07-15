#include <iostream>
#include <string>
#include "1.h"
#include "Database.h"
#include<ctime>
#include <cmath>
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
void SabadKharid::Finalize(OrderDB &db ,CustomerDB &custDB , Customer &cust, int custID , int RestID , int use)
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
    LoyaltyPoints* lvl = cust.getlevel();
    string oldLevelName = lvl->getlevel();
    double newtotalPrice = totalprice - totalprice * lvl->discount();
    double deliveryprice = lvl->deliveryprice(10);
    double Finalprice = newtotalPrice + deliveryprice;
    int point = static_cast<int>(totalprice * lvl->zarib());
    double couponDiscount = 0;
    int availableCoupons = custDB.GetCopon(custID); 
    if (availableCoupons > 0)
    {
        int choice = 0; 
        cout << "Shoma " << availableCoupons << " Copon darid. Mayelid az chand copon (1$) baraye in sefaresh estefade konid? (0 baraye adam estefade): ";
        cin >> choice;
        while (choice < 0 || choice > availableCoupons)
        {
            cout << "You Don't have Enough Coupon or entered an invalid number. Please Choose another Number: ";
            cin >> choice;
        }
        if (choice > 0)
        {
            couponDiscount = choice * 1.0;
            if (Finalprice >= couponDiscount) 
            {
                Finalprice -= couponDiscount;
            } 
            else 
            {
                choice = static_cast<int>(ceil(Finalprice));
                couponDiscount = Finalprice;
                Finalprice = 0;
                cout << "Tavajoh: Faghat " << choice << " copon baraye rayegan shodan sefaresh kafi bod va masraf shod.\n";
            }
            for (int i = 0; i < choice; i++)
            {
                custDB.UseCoupon(custID);
            }
        }
    }
    cust.addpoint(totalprice);
    custDB.SaveCustomer(cust , use);
    custDB.UpdateLastOrderDate(custID);
    string neq = cust.getlevel()->getlevel();
    db.order(custID , RestID , Finalprice);
    orders.clear();
    cout << "Sefareshe Shoma Sabt Shod.\n";
    if (oldLevelName != neq)
    {
        custDB.AchiveNextLevel(custID , neq ,  lvl->getlevel());
    }
    int orderc = custDB.IncrementOrderCount(custID);
    if (orderc >= 6)
    {
        custDB.setBadges(custID, "FrequentBuyer");
    }
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    if (ltm != nullptr && (ltm->tm_hour >= 23 || ltm->tm_hour < 7))
    {
        custDB.setBadges(custID, "NightCustomer");
    }

    cout << "\n================ FACTOR ================\n";
    cout << "Base Price:        " << totalprice << endl;
    cout << "Membership Level:  " << lvl->getlevel() << endl;
    cout << "Discount Applied:  " << " (" << (lvl->discount()*100) << "%)" << endl;
    cout << "Delivery Fee:      " << deliveryprice << endl;
    cout << "-----------------------------------------\n";
    cout << "Final Total:       " << Finalprice << endl;
    cout << "Points Earned:     " << point << endl;
    cout << "Total Points Now:  " << cust.getpoint() << endl;
    cout << "Badges:            " << custDB.GetBadges(custID) << endl;
    cout << "==========================================\n";

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
void Customer::addpoint(double orderprice)
{
    points += static_cast<int>(orderprice * level->zarib());
    checkLevelUpgrade();
}
void Customer::checkLevelUpgrade()
{
    if (points >= 100 && points < 300 && level->getlevel() == "Normal")
    {
        delete level;
        level = new Silver;
        cout << "You upgraided to Silver Level!" << endl;
    }
    else if (points >= 300 && points < 700 && level->getlevel() == "Silver")
    {
        delete level;
        level = new Gold;
        cout << "You upgraided to Gold Level!" << endl;
    }
    if (points >= 700 && level->getlevel() == "Gold")
    {
        delete level;
        level = new VIP;
        cout << "You upgraided to VIP Level!" << endl;
    }
}