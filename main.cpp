#include <iostream>
#include <string>
#include "1.h"
#include "Database.h"
#include<limits>
using namespace std;

int main()
{
    sqlite3* db;
    if (sqlite3_open("Service_food.db", &db) != SQLITE_OK)
    {
        cout << "Error opening database!" << endl;
        return 1;
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
        if (choice == 3) break;
        
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
            
            if (User.Login(user, password))
            {
                cout << "Login Successfully.\n";
                string role = User.Getrole(user);
                int iD = User.GetuserID(user);
                
                if (role == "Customer")
                {
                    SabadKharid sabad;
                    
                    while (true)
                    {
                        cout << "1_ Show Restaurants\n2_ Show Sabad\n3_ Edit Sabad\n4_ Order History\n5_ Finalize Order\n6_ Logout\n";
                        cin >> choice;
                        if (choice == 6) break;
                        else if (choice == 1)
                        {
                            restu.ShowAllResturant();
                            cout << "Enter Restaurant ID to see menu: ";
                            int restid;
                            cin >> restid;
                            if (!restu.IsRestaurantOpen(restid))
                            {
                                cout << "Khatar! In Resturant Baste Ast va nemitavanid Menu ra bebinid.\n";
                                continue;
                            }
                            menu.showMenu(restid);
                            int static orderCounter = 1; 
                            Order singleOrder(orderCounter, iD, restid); 
                            bool hasOrderedAnything = false;
                            double money = 0;
                            while (true)
                            {
                                cout << "Enter Item ID to add to order (or -1 to skip): ";
                                int itemid;
                                cin >> itemid;
                                
                                if (itemid == -1)
                                {
                                    break;
                                }

                                string name = menu.Getname(itemid);
                                string exp = menu.Getexplain(itemid);
                                double price = menu.GetPrice(itemid);
                                string type = menu.GetType(itemid);
                                string soldout = menu.getsoldout(itemid);
                                
                                Menuitem* item = nullptr;
                                if (soldout == "Namojod")
                                {
                                    cout << "This Food Is SoldOut";
                                    continue;
                                }
                                
                                else if (soldout == "Mojod")
                                {
                                    money += price;
                                    if (type == "Food")
                                    {
                                        int cook = menu.GetCookTime(itemid);
                                        item = new Food(itemid, name, exp, price, false, cook);
                                    }
                                    else if (type == "Drink")
                                    {
                                        double vol = menu.GetVolume(itemid);
                                        item = new Drink(itemid, name, exp, price, false, vol);
                                    }
                                }
                                if (item != nullptr)
                                {
                                    singleOrder.addfood(item); 
                                    hasOrderedAnything = true;
                                    cout << "Added to Sabad.\n";
                                    cout << "TotalMoney: " << money << endl;
                                }
                            }
                            if (hasOrderedAnything)
                            {
                                sabad.addOrder(singleOrder);
                                orderCounter++;
                                cout << "---> Whole order successfully added to your shopping cart! <---\n";
                            }
                        }  
                    
                        else if (choice == 2)
                        {
                            sabad.showSabad();
                            cout << " | Status: Pending\n";
                        }
                        else if (choice == 3)
                        {
                            cout << "Enter Order ID to remove: ";
                            int orderid;
                            cin >> orderid;
                            sabad.EditSabad(orderid);
                        }
                        else if (choice==4)
                        {
                            order.ShowOrderForCustomer(iD);
                        }
                        
                        else if (choice == 5)
                        {
                            cout << "Enter Restaurant ID to finalize: ";
                            int restid;
                            cin >> restid;
                            sabad.Finalize(order, iD, restid);
                        }
                    }
                }
                else if (role == "ResturantBoss")
                {
                    while (true)
                    {
                        cout <<"1_ Add Restaurant\n2_ Add Item to Menu\n3_ Show Orders\n4_ Update Order Status\n5_ ShowMenu\n6_ Delete Item\n7_ Make Item Namojod\n 8_Make Item mojod\n9_ Logout\n";
                        cin >> choice;
                        if (choice == 9) break;
                        
                        if (choice == 1)
                        {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            string name, addr, phone , Explain;
                            int open, time;
                            cout << "Enter Name: "; 
                            getline(cin , name);
                            cout << endl;
                            cout << "Enter Address: "; 
                            getline(cin , addr);
                            cout << endl;
                            cout << "Enter Phone: "; 
                            getline(cin , phone);
                            cout << endl;
                            cout << "Resrurant Explain: ";
                            getline(cin , Explain);
                            cout << endl;
                            cout << "Open Status (1 for open, 0 for close): "; 
                            cin >> open;
                            cout << endl;
                            cout << "Delivery Time: "; 
                            cin >> time;
                            cout << endl;
                            restu.AddResturant(iD, name, Explain ,addr, phone, open, time);
                        }
                        else if (choice == 2)
                        {
                           
                            cout << "What Item Do you Want To Add?(1_FOOD  2_DRINK)\n";
                            int newchoice;
                            cin >> newchoice;
                            if (newchoice == 1)
                            {
                                int cooktime;
                                string name, exp , vaziat;
                                double price;
                                /*cout << "Enter Item ID: "; 
                                cin >> itemid;
                                cout << endl;*/
                                cout << "Enter Cooking Time: "; 
                                cin >> cooktime;
                                cout << endl;
                                cout << "Enter Price: "; 
                                cin >> price;
                                cout << endl;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Enter Name: "; 
                                getline(cin , name);
                                cout << endl;
                                cout << "Enter Explanation: "; 
                                getline(cin , exp);
                                cout << endl;
                                int y = 0;
                                while (y < 1 || y>=3)
                                {
                                    cout << "Vaziat Food:(1_Mojod  2_Namojod)";
                                    cin >> y;
                                }
                                if (y == 1)
                                {
                                    vaziat = "Mojod";
                                }
                                else if (y==2)
                                {
                                    vaziat = "Namojod";
                                }
                                menu.AddFooditem(name, exp, price, "Food", cooktime, iD , vaziat);
                            }
                            else if (newchoice == 2)
                            {
                                string name, exp , vaziat;
                                double price, vol;
                                /*cout << "Enter Item ID: "; 
                                cin >> itemid;
                                cout << endl;*/
                                cout << "Enter Volume: "; 
                                cin >> vol;
                                cout << endl;
                                cout << "Enter Price: "; 
                                cin >> price;
                                cout << endl;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Enter Name: "; 
                                getline(cin , name);
                                cout << endl;
                                cout << "Enter Explanation: "; 
                                getline(cin , exp);
                                cout << endl;
                                int y = 0;
                                while (y < 1 || y>=3)
                                {
                                    cout << "Vaziat Food:(1_Mojod  2_Namojod)";
                                    cin >> y;
                                }
                                if (y == 1)
                                {
                                    vaziat = "Mojod";
                                }
                                else if (y==2)
                                {
                                    vaziat = "Namojod";
                                }
                                menu.AddDrinkitem(name, exp, price, "Drink", vol, iD , vaziat);
                            }
                            
                        }
                        else if (choice == 3)
                        {
                            order.ShowOrderForRest(iD);
                        }
                        else if (choice == 4)
                        {
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            int orderid;
                            string stat;
                            cout << "Enter Order ID: "; 
                            cin >> orderid;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Enter New Status: "; 
                            getline(cin , stat);
                            cout << endl;
                            order.UpdateOrderStatus(orderid, stat);
                        }
                        else if (choice == 5)
                        {
                            menu.showMenu(iD);
                        }
                        else if (choice == 6)
                        {
                            int itemid;
                            cout << "Enter Item ID to Delete: ";
                            cin >> itemid;
                            menu.DeleteItem(itemid, iD);
                        }
                        else if (choice == 7)
                        {
                            int itemid;
                            cout << "Enter Item ID to make Namojod: ";
                            cin >> itemid;
                            menu.SetSoldOut(itemid, iD);
                        }
                        else if (choice == 8)
                        {
                            int itemid;
                            cout << "Enter Item ID to make Mojod: ";
                            cin >> itemid;
                            menu.SetUNSoldOut(itemid, iD);
                        }
                    }
                }
                else if (role == "Admin")
                {
                    while (true)
                    {
                        cout << "1_ Show Register Request\n2_ Show sell info\n3_ Active Members\n4_ Logout\n";
                        cin >> choice;
                        if (choice == 4) break;
                        
                        if (choice == 1)
                        {
                            restu.ShowRequest();
                            cout << "Please Enter ID of restaurant to Accept: ";
                            int id;
                            cin >> id;
                            restu.AcceptRest(id);
                        }                        
                        else if (choice == 2)
                        {
                            order.ShowAllResturantSellInfo();
                        }
                        else if (choice == 3)
                        {
                            cout << "Active Users Count: " << User.ActiveMembers() << endl;
                        }
                    }
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
            
            while (true)
            {
                cout << "Please Enter your Role:\n1_ Customer\n2_ ResturantManager\n3_ Admin\n";
                cin >> role;
                if (role < 1 || role > 3)
                {
                    cout << "Wrong choice Please Try Again.\n";
                    continue;
                }
                break;
            }
            
            ROLE Role = static_cast<ROLE>(role);
            if(User.Register(user, password, Role))
            {
                cout << "Register Successful." << endl;
                continue;
            }
            else
            {
                cout << "Register Failed! (Username might be taken)" << endl;
            }
        }
    }
    sqlite3_close(db);
    return 0;
}
// gcc -c sqlite3.c -o sqlite3.o
// g++ main.cpp 1.cpp sqlite3.o -o FoodService.exe -std=c++11