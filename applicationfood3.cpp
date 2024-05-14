#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <conio.h>

using namespace std;

struct MenuItem {
    string name;
    double price;
};

struct User {
    string loginId;
    string password;
};

double getOverallRating() {
    double overallRating;
    cout << "Enter your overall rating for the order (1 to 5): ";
    cin >> overallRating;

    if (overallRating < 1 || overallRating > 5) {
        cout << "Invalid rating. Please enter a rating between 1 and 5.\n";
        return getOverallRating();
    }

    return overallRating;
}

void displayMenu(const MenuItem* menu, int numItems) {
    cout << "\n\tMENU:";
    cout << setw(20) << left << "\nItem" << "Price";
    cout << setfill('-') << setw(30) << "\n" << setfill(' ');
    cout << endl;
    for (int i = 0; i < numItems; ++i) {
        cout << setw(20) << left << menu[i].name << "$" << menu[i].price << "\n";
    }
    cout << setfill('-') << setw(30) << "\n" << setfill(' ');
    cout << endl;
}

bool createUser(User* users, int& numUsers) {
	system("cls");
    string loginId, password;

    cout << "\n\n\t\t\t ====== Create Your Account ======\n";
    cout << "\n\n\t\tEnter your Username: ";
    cin >> loginId;

    for (int i = 0; i < numUsers; ++i) 
	{
        if (users[i].loginId == loginId) {
            cout << "\t\tLogin ID already taken. Please choose another.\n";
            return false;
        }
    }

    cout << "\t\tCreate Password: ";
    cin >> password;

    users[numUsers].loginId = loginId;
    users[numUsers].password = password;
    ++numUsers;

    ofstream userFile("G:\\2nd semester\\PF Lab\\C++\\Project\\users.txt", ios::app);
    if (userFile.is_open()) {
        userFile << loginId << " " << password << endl;
        userFile.close();
    } else {
        cout << "Unable to open user file for writing.\n";
        return false;
    }

    cout << "\n\t\tAccount created successfully!\n";
    return true;
}

bool loginUser(const User* users, int numUsers, string& loginId, string& password) {
	system("cls");
	cout << "\n\n\t\t\t\t ====== LOGGING IN ======\n";
    cout << "\n\n\t\t\tEnter Login ID: ";
    cin >> loginId;
    cout << "\t\t\tEnter Password: ";
    cin >> password;

    for (int i = 0; i < numUsers; ++i) {
        if (users[i].loginId == loginId && users[i].password == password) {
            cout << "Login successful!\n";
            return true;
        }
    }

    cout << "Invalid login credentials. Please try again.\n";
    return false;
}

double processOrder(const MenuItem* menu, const int* quantities, int numItems) {
    double totalBill = 0.0;

    ofstream orderFile("G:\\2nd semester\\PF Lab\\C++\\Project\\order.txt");  

    orderFile << "Your Order:\n";
    orderFile << setw(20) << left << "Item" << setw(20) << left << "Quantity" << setw(20) << left << "Total\n";
    orderFile << setfill('-') << setw(60) << "\n" << setfill(' ');
    orderFile << endl;

    for (int i = 0; i < numItems; ++i) {
        if (quantities[i] > 0) {
            double itemTotal = quantities[i] * menu[i].price;
            totalBill += itemTotal;
            orderFile << setw(20) << left << menu[i].name << setw(20) << left << quantities[i] << "$" << itemTotal << endl;
        }
    }

    orderFile << setfill('-') << setw(60) << "\n" << setfill(' ');
    orderFile << endl;

    orderFile << left << "\nTotal=" << "$" << totalBill << endl;
    orderFile << setfill('-') << setw(60) << "\n" << setfill(' ');

    orderFile.close();  

    return totalBill;
}


void Menu() {
    const int MAX_ITEMS = 50;   
    MenuItem menu[MAX_ITEMS];   
    int numItems = 0;           

    ifstream readMenuFromFile("G:\\2nd semester\\PF Lab\\C++\\Project\\menu.txt");

    if (!readMenuFromFile.is_open()) {
        cout << "Unable to open menu file" << endl;
        return; 
    }

    while (numItems < MAX_ITEMS && readMenuFromFile >> menu[numItems].name >> menu[numItems].price) {
        ++numItems;
    }
    readMenuFromFile.close();

    displayMenu(menu, numItems);

    int* quantities = new int[numItems] {0};

    char addMore;
    cout << "PLACE YOUR ORDER\n";
    int itemNumber, quantity;
    do {
        cout << "Enter item number: ";
        cin >> itemNumber;
        cout << "Enter quantity of that item number: ";
        cin >> quantity;

        if (itemNumber >= 1 && itemNumber <= numItems && quantity >= 0) {
            quantities[itemNumber - 1] += quantity;
        } else {
            cout << "Invalid input. Please try again.\n";
        }

        cout << "Add more items to your order? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    system("cls");

    double totalBill = processOrder(menu, quantities, numItems);
    cout << "\nThank you for your order! Your total bill is: $" << totalBill << endl;

    double overallRating = getOverallRating();
    cout << "Thank you for your overall rating of " << overallRating << "!\n";

    if (overallRating <= 3) {
        cout << "We appreciate your feedback. We will improve our service.\n";
    } else {
        cout << "Thank you for your positive feedback!\n";
    }

    delete[] quantities;
}

void handleAuthentication() {
    const int MAX_USERS = 100;  
    User users[MAX_USERS];      
    int numUsers = 0;           

    ifstream readUserFile("users.txt");
    if (readUserFile.is_open()) {
        while (numUsers < MAX_USERS && readUserFile >> users[numUsers].loginId >> users[numUsers].password) {
            ++numUsers;
        }
        readUserFile.close();
    } else {
        cout << "Unable to open user file for reading.\n";
        return; 
    }

    int choice;
    do {
        cout << "\n\t\t\tFOOD ORDER AND BILLING SYSTEM";
        cout << "\n\n\n\t\t\t      ====== ACCOUNT MENU ======";
        cout << "\n\n\t\t\t 1. Create Account";
        cout << "\n\t\t\t 2. Log In";
        cout << "\n\t\t\t 3. Exit";
        cout << "\n\n\t\t\t Enter your choice (1-3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (createUser(users, numUsers)) {
                    cout << "\nPress Enter to continue \n";
                    while (getche() != '\r')
                        ;
                    system("cls");
                    Menu();
                    return;  
                }
                break;

            case 2:
                if (loginUser(users, numUsers, users[numUsers].loginId, users[numUsers].password)) {
                    cout << "\nPress Enter to continue \n";
                    while (getche() != '\r')
                        ;
                    system("cls");
                    Menu();
                    return;  
                }
                break;

            case 3:
            	system("cls");
                cout << "Exiting program.\n";
                break;
                
            default:
                cout << "\n\tInvalid choice. Please enter a number between 1 and 3.\n";
        }
    } while (choice != 3);
}

int main() {
    system("cls");
    system("COLOR 70");

    handleAuthentication();

    return 0;
}

