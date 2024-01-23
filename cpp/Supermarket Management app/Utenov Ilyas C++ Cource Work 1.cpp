/**
  * File:    Utenov Ilyas C++ Cource Work 1.cpp
  *
  * Author:  Utenov Ilyas (ilyas2.utenov@live.uwe.ac.uk)
  * Date:     13/03/2023
  * Partner:  I worked alone
  * Course:   UWEBIC IY469 Programming in C/C++
  *
  * Summary of File:
  *
  * This code is a C++ console application that enables the user to maintain a supermarket database.
  * It has capabilities to load and save data from a text file, add and remove items from database, edit items,
  * print low in stock itmes and items about to expire, search for items by their name or type, and exit the application.
  * The program has a menu-driven interface that allows users to perform these
  * operations until they choose to exit.
  *
  */

  //I confirm that this assignment is my own work. Where I have referred to academic sources, I have provided in-text citations and included the sources in the final reference list.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <tuple>

/*
struct Date

Summary of the Date struct :
  The Date struct is used to store information about a date.

Members :
  day : an integer variable to store the day of a date.
  month : an integer variable to store the moth of a date.
  year : an integer variable to store the year of a date.


Description :
  The Item struct is used to store and access the attributes of an item.
  The struct provides a convenient way to group these attributes together and manipulate them as a single entity.
*/

struct Date {
	int day;
	int month;
	int year;
};

/*
std::istream& operator>>(std::istream& in, Date& date)
	and
std::ostream& operator<<(std::ostream& out, Date& date)
	and
std::istream& operator>>(std::istream& in, Item& item)
	and
std::ostream& operator<<(std::ostream& out, Item& item)

Summary of the operator overload :
  Operator overloading specifies the way how to input or output structures or classes.

Parameters : Items: a structure of item object
			 Date: a structure of a date object

Return value : Nothing

Description :
  This allows to output and input structures more efficient.
*/

std::istream& operator>>(std::istream& in, Date& date) {
	in >> date.day;
	in.ignore();
	in >> date.month;
	in.ignore();
	in >> date.year;
	return in;
}
std::ostream& operator<<(std::ostream& out, Date& date) {
	out << date.day << "/" << date.month << "/" << date.year;
	return out;
}

/*
bool operator<(const Date a, const Date b)

Summary : This describes the way how two Date structures should be compared

Parameters : Date: a structure of a date object

Reurn value : true or false

Description : This allows to compare Date structures correctly
*/


bool operator<(const Date a, const Date b) {
	return std::tie(a.year, a.month, a.day) < std::tie(b.year, b.month, b.day);
}

time_t now = time(0);
tm *ltm = localtime(&now);
Date CurrentDate = { ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year };

/*
struct Item

Summary of the Item struct :
  The Item struct is used to store information about an item.

Members :
  name : a string variable to store the name of an item.
  type : a string variable to store the type of an item.
  date : a date struct to store the best before date of an item.
  price : a double variable to store the price of an item.
  quantity : an integer variable to store the quantity of an item.


Description :
  The Item struct is used to store and access the attributes of an item.
  The struct provides a convenient way to group these attributes together and manipulate them as a single entity.
*/

struct Item { // struct holds the product information
	std::string name;
	Date date;
	int quantity;
	float price;
	std::string type;
};

std::istream& operator>>(std::istream& in, Item& item) {
	in >> item.name >> item.date >> item.quantity >> item.price >> item.type;
	return in;
}
std::ostream& operator<<(std::ostream& out, Item& item) {
	out << item.name << " " << item.date << " " << item.quantity << " " << item.price << " " << item.type;
	return out;
}

/*
std::vector<Item> ReadFile(const std::string& name)

Summary of the ReadFile(const std::string& name) function :
  The ReadFile function reads the SupermarketDatabase.txt file and puts all the items from it into a Item vector.

Parameters : name: a string variable that provides source of database

Return value : Nothing

Description :
  This function takes data from the database and stores it.
*/

std::vector<Item> ReadFile(const std::string& name) {
	std::vector<Item> products;
	std::ifstream infile(name);
	if (!infile) {
		std::cerr << "We can't open your file" << name << std::endl;
		return products;
	}
	Item item;
	while (infile >> item.name)
	{
		infile >> item.date >> item.quantity >> item.price >> item.type;
		products.push_back(item);
		infile.clear();
	}
	infile.close();
	return products;
}
std::vector<Item> shop = ReadFile("SupermarketDatabase.txt");

/*
void WriteFile(const std::string& name)
Summary of the WriteFile(const std::string& name) :
  The WriteFile function saves the current state of Item vector to the database

Parameters : name: a string varible that provides source of database

Return value :  Nothing

Description:
  This function saves any changes to database.
*/

void WriteFile(const std::string& name) {
	std::ofstream out;
	out.open(name);
	if (out.is_open()) {
		int num = shop.size();
		for (int i = 0; i <= shop.size() - 1; ++i) {
			out << shop[i].name << " " << shop[i].date << " " << shop[i].quantity << " " << shop[i].price << " " << shop[i].type;
			out << std::endl;
		}
	}
}

/*
void ListAllItems(const std::vector <Item>& shop)

Summary of the void ListAllItems(const std::vector <Item>& shop) :
  The void ListAllItems function lists all items stored in the database

Parameters : Item: a vector of item objects

Return value: Nothing

Description :
  This function prints out all the data from the database
*/

void ListAllItems(const std::vector <Item>& shop) {
	int num = 1;
	for (auto i : shop) {
		std::cout << "Item number " << num << " " << i << std::endl;
		num++;
	}
}

/*
void AddItem(std::vector <Item>& shop)

Summary of the AddItem(std::vector <Item>& shop) function :
  The AddItem function adds a new item to the database.

Parameters : Item: a vector of Item objects

Return value : Nothing

Description :
  This function gets from user aspects of an item, checks for it validity adds it to Item struct,
  adds it to the vector and then to the database.
*/


void AddItem(std::vector <Item>& shop) {
	Item item;
	std::cout << "Enter the name of an item:" << std::endl;
	std::cin >> item.name;
	std::cout << "Enter the type of an item:" << std::endl;
	std::cin >> item.type;
	std::cout << "Enter the best before date of an item:" << std::endl << "Day:";
	std::cin >> item.date.day;
	std::cout << std::endl << "Month:";
	std::cin >> item.date.month;
	std::cout << std::endl << "Year:";
	std::cin >> item.date.year;
	std::cout << "Enter the price of an item:" << std::endl;
	std::cin >> item.price;
	std::cout << "Enter the quantity of an item:" << std::endl;
	std::cin >> item.quantity;
	shop.push_back(item);
	std::cout << "Item added successfully.";
}

/*
void EditItem(std::vector <Item>& shop)

Summary of the EditItem(std::vector <Item>& shop) function :
  The EditItem function modifies the existing item in the database by changing its aspects.
Parameters : Item: a vector of Item objects

Return value : Nothing

Description :
  This function asks a user for a number of an item and new parameters for an item.
  Then it replaces existing parameters in the vector and saves the changes in the database.
*/

void EditItem(std::vector <Item>& shop) {
	ListAllItems(shop);
	std::cout << "Write the number of item which you want to edit" << std::endl;
	int num;
	while (true) {
		std::cin >> num;
		if (num > 1 && num < shop.size() + 1) break;
		else std::cout << "We don't have the item with this number" << std::endl;
	}
	std::cout << "Write here new attributes of this item" << std::endl;
	Item item;
	std::cout << "Enter the name of an item:" << std::endl;
	std::cin >> item.name;
	std::cout << "Enter the type of an item:" << std::endl;
	std::cin >> item.type;
	std::cout << "Enter the best before date of an item:" << std::endl << "Day:";
	std::cin >> item.date.day;
	std::cout << std::endl << "Month:";
	std::cin >> item.date.month;
	std::cout << std::endl << "Year:";
	std::cin >> item.date.year;
	std::cout << "Enter the price of an item:" << std::endl;
	std::cin >> item.price;
	std::cout << "Enter the quantity of an item:" << std::endl;
	std::cin >> item.quantity;
	shop[num - 1] = item;
	std::cout << "Item edited succesfully.";
}

/*
void DeleteItem(std::vector<Item>& shop)

Summary of the DeleteItem(std::vector <Item>& shop) function :
  The DeleteItem function removes an item from the database.

Parameters : Item: a vector of item objects

Return value : Nothing

Description :
  This function takes a number of item from the user and deletes an object from the vector and the database.
*/

void DeleteItem(std::vector <Item>& shop) {
	ListAllItems(shop);
	std::cout << "Write the number of item which you want to delete" << std::endl;
	int num;
	while (true) {
		std::cin >> num;
		if (num > 0 && num < shop.size() + 1) break;
		else std::cout << "We don't have the item with this number" << std::endl;
	}
	shop.erase(shop.begin() + num - 1);
	std::cout << "Your item was succesfully deleted";
}

/*
void LowInStock(std::vector <Item>& shop)

Summary of the LowInStock(std::vector <Item>& shop) function :
  The LowInStock function lists all the items with quantity of 10 or less

Parameters : Item: a vector of item objects

Return value : Nothing

Description :
  This function prints out all the items with quantity of 10 or less
*/

void LowInStockItems(const std::vector <Item>& shop) {
	int quantity = 10;
	for (auto i : shop) {
		if (i.quantity <= quantity) std::cout << i << std::endl;
	}
}

/*
void AboutToExpireItems(std::vector <Item>& shop)

Summary of AboutToExpireItems(std::vector <Item>& shop) function :
  The AboutToExpireItems function prints out the items that already are or within 2 days to be expired.

Parameters : Item: a vector of item objects

Return value : Nothing

Description :
  This function lists all items which are within 2 days before end of best before date or already expired.
*/

void AboutToExpireItems(std::vector <Item>& shop) {
	for (auto i : shop) {
		auto j = i;
		j.date.day = i.date.day - 3;
		if (j.date < CurrentDate) {
			std::cout << i << std::endl;
		}
	}
}

/*
void Search(std::vector <Item>& shop)

Summary of the Search(std::vector <Item>& shop) function :
  The Search function browses thru database to find items by name or type defined by the user.

Parameters : Item: a vector of item objects

Return value : Nothing

Description :
  This function finds and lists all items from the database with either the same name or type,
  the method and values defined be the user.
*/

void Search(std::vector <Item>& shop) {
	std::cout << "Do you wanna search by name, or by type" << std::endl;
	std::cout << "1) Name" << std::endl;
	std::cout << "2) Type" << std::endl;
	int num;
	std::cin >> num;
	std::string line;
	std::cin >> line;
	if (num == 1) {
		for (auto i : shop) if (i.name == line) std::cout << i << std::endl;
	}
	else {
		for (auto i : shop) if (i.type == line) std::cout << i << std::endl;

	}
}

/*
void Quit()

Summary of the Quit() function :
  The Quit function stops the programm

Parameters : none

Return value : Nothing

Description :
  This function stops the programm
*/

void Quit() {
	exit(0);
}

/*
void Direction(std::vector <Item>& shop, int operation)

Summary of the Direction(std::vector <Item>& shop, int operation) function :
  The Direction function directs the programm to the function chosen be the user.

Parameters : Item: a vector of item objects
		   : operanion: an integer variable defined by the user

Return value : Nothing

Description :
  This function calls different functions according to user input
*/

void Direction(std::vector <Item>& shop, int operation) {
	if (operation == 1) ListAllItems(shop);
	else if (operation == 2) AddItem(shop);
	else if (operation == 3) EditItem(shop);
	else if (operation == 4) DeleteItem(shop);
	else if (operation == 5) LowInStockItems(shop);
	else if (operation == 6) AboutToExpireItems(shop);
	else if (operation == 7) Search(shop);
	else if (operation == 8) Quit();
}

/*
void Operations(const std::vector <std::string>& operations)

Summary of the Operations(const std::vector <std::string>& operations) function :
  The Operations function prints all available operations to the user.

Parameters : operations: a vector containing all operations

Return value : Nothing

Description :
  This function lists all functions to the user that he can chose.
*/

void Operations(const std::vector <std::string>& operations) {
	int num = 1;
	for (auto i : operations) {
		std::cout << num << ") " << i << std::endl;
		num++;
	}
}

/* int main()
Summary of the int main() :
The main function displays the avalible operations and gets from the user which operation to run.

Parameters : none

Return value : Nothing

Description: This function is the first one to be called, it declares available
for the user operations and checks for valid input from the user, besides to calling up other major functions.
*/

int main() {
	std::vector <std::string> operations{ "All Items","Add Item", "Edit Item", "Remove Item", "Low In stock items", "Items about to expire", "Search", "End the session" };

	while (true) {
		Operations(operations);

		try {
			int operation;
			std::cin >> operation;
			if (operation == 7) { break; }
			operation > 0 && operation < 9 ? Direction(shop, operation) : throw(operation);
		}
		catch (int WrongOperation) {
			std::cout << "We don't have this type of operation please choose the new one" << std::endl;
			system("cls");

		}
		system("cls");
		WriteFile("SupermarketDatabase.txt");
	}
	return 0;
}
/*
Replit mirror:
https://replit.com/@1ukrainech1/Super-Market-Database-Management#main.cpp
*/