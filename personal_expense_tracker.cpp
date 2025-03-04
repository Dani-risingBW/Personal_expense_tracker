#include <iostream> 
#include <string>
#include <vector> 
#include <ctime> 
#include <iomanip>
#include <chrono>
#include <format>
using namespace std; 

class Expense{
    private: 
        string category; 
        string description; 
        string date;
        double cost; 
    public: 
        //getter functions 
        //initalizing and declaring  
        string getCategory() const{ return category;}
        string getDescription() const{ return description;}
        string getDate() const{return date;}
        double getCost() const{return cost;}
        
        //constructor
        Expense(string c, string d, string time, double money){
            category = c; 
            description = d; 
            date = time; 
            cost = money; 
            
        }

    
};

//I will define the function so that I can call this function from whereever
//the definition is 
bool isValidInput(string&); 
bool checkDate(string);

void addItem(vector<Expense>& vec){
    //gathers class attributes 
    string cat, des, time; 
    double price; 

    cout << "Please create your own category for your log: " << endl;
    getline(cin, cat); 

    cout << "\nWhat is the description of this expense(short sentence): " << endl;
    getline(cin,des); 

    //asks user if they want to store the current date or past date
    cout << "Was this a past expense?(Y or N): " << endl;
    string date; 
    cin >> date; 
    //checks the validity
    bool checkValidity = isValidInput(date);   
    while(!checkValidity){
        cout << "Please enter a valid answer. Y for yes and n for no: " << endl; 
        cin >> date; 
        checkValidity = isValidInput(date); 
    }
    //stores the past date in MM/DD/YYYY format && checks it 
    if(date == "y"  || date == "Y"){
        cout << "Please enter the date in this format (MM/DD/YYYY):" << endl; 
        cin >> time;
        //add variable to this function 
        while(!checkDate(time)){
            cout << "Please enter a valid date using this format: (MM/DD/YYYY)" << endl; 
            cin >> time; 
        }
    }
    else{
        // Get the current time from the system clock
        auto now = chrono::system_clock::now();
    
        // Convert to time_t
        time_t t = chrono::system_clock::to_time_t(now);

        // Convert to tm (local time)
        tm* local_time = localtime(&t);

        // Format the date as MM/DD/YYYY using std::format (C++20)
        ostringstream oss;
        oss << (local_time->tm_mon + 1) << '/' << local_time->tm_mday << '/' << (local_time->tm_year + 1900);
        time = oss.str();

        //to save time I will add a zero in the month
        time = "0" + time;
        
    }

    //price 
    cout << "Enter the price of this purchase(no need to add the $ symbol): " << endl; 
    cin >> price;

    //finally adds the log to a vector
    vec.push_back(Expense(cat, des, time, price)); 

};

void printItem(Expense x){
    cout << x.getDate() << " " << x.getCategory() << " $" << x.getCost() << " " << x.getDescription() << endl; 
}; 

void printCatalog(vector<Expense>& vec){
    if(vec.size() == 0){
        cout << "No expenses logged yet." << endl;
    }
    else{
        cout << " " << endl;
        for(int i = 0; i < vec.size(); i++){
            cout << vec[i].getDate() << " " << vec[i].getCategory() << " $" << vec[i].getCost() << " " << vec[i].getDescription() << endl; 
        }
    }
    cout << " " << endl;
}; 

void searchItem(vector<Expense>& vec){
    string targetDate; 
    bool found = false; 
    cout << "\nEnter the date of the log you would like to search for(MM/DD/YYYY):" << endl;
    cin >> targetDate; 

    while(!checkDate(targetDate)){
        cout << "Please enter a valid date using this format: (MM/DD/YYYY)" << endl; 
        cin >> targetDate; 
    }
    
    //for every element in the vector it checks the targetDate. If it is true, then 
    //it will print every log under that date
    for(int i = 0; i < vec.size() && !found; i++){
        if(vec[i].getDate() == targetDate){
            printItem(vec[i]);  
            found = true; 
        }
    }
    if(!found){
        cout << "No logs were found." << endl;
    }

}; 

void deleteItem(vector<Expense>& vec){
    string targetDate; 
    cout << "\nEnter the date of the log you would like to search for(MM/DD/YYYY):" << endl;
    cin >> targetDate; 

    while(!checkDate(targetDate)){
        cout << "Please enter a valid date using this format: (MM/DD/YYYY)" << endl; 
        cin >> targetDate; 
    }

    //creates a temp array for all the targeted dates 
    vector<Expense> temp; 
    char choice = 'y'; 
    bool exitLoops = false; 

    for(int i  = 0; i < vec.size() && !exitLoops; i++ ){
        if(vec[i].getDate() == targetDate){
            printItem(vec[i]);
            cout << "Is this what you would like to delete? (Y or N)" << endl;
            //temp.push_back(Expense(vec[i].getCategory(), vec[i].getDescription(), vec[i].getDate(), vec[i].getCost()));
            char deleteChoice;
            cin >> deleteChoice; 
            if(tolower(deleteChoice) == 'y'){
                vec.erase(vec.begin() + i);
                exitLoops = true;
            }

        }
    }


    if(!exitLoops){
        cout << "No logs were found under " << targetDate << "." << endl; 
    }
}; 

void printMenu(){
    cout << "A - Add a Log" << endl;
    cout << "S - Search a Log" << endl;
    cout << "D - Delete a Log" << endl;
    cout << "P - Print Expenses" << endl; 
    cout << "Q - Quit" << endl << endl;
    //cout << "Edit a Log" << endl;
};

bool isValidInput(string& userInput){
    char input = userInput[0];
    
    //checks to make sure one character is inputted
    if(userInput.length() != 1 || !isalpha(input)){ 
        return false; 
    }

    if(tolower(input) == 'a' || tolower(input) == 's' || tolower(input) == 'd' || tolower(input) == 'p' || tolower(input) == 'y' || tolower(input) == 'n' 
        || tolower(input) == 'q'){
        return true; 
    }
    return false; 
}; 

bool checkDate(string d){
    //checks for MM/DD/YYYY format
    if(d.length() != 10){
        return false; 
    }
    string day = d.substr(3,2);
    string month = d.substr(0,2);
    string year = d.substr(6,4);

    //checks month
    int size = month.length();
    for(int i = 0; i < size; i++){
        if(!(isdigit(month[i]))){
            return false; 
        }
    }
    if(stoi(month) < 1 || stoi(month) > 12){
        return false; 
    } 

    //checks day
    size = day.length();
    if(size != 2)
        return false;
    for(int i = 0; i < size; i++){
        if(!(isdigit(day[i]))){
            return false; 
        }
    }
    if(stoi(day) < 1 || stoi(day) > 31){
        return false; 
    }
    
    //checks year
    size = year.size();
    for(int i = 0; i < size; i++){
        if(!(isdigit(year[i]))){
            return false; 
        }
    }

    return true;

};

int main(){
    //Opening statements 
    cout << "Welcome to Green FootPrint, \n"
        "an online personal expense tracker where you can log any and every expenditure.\n" << endl; 

    vector<Expense> log;  

    string choice = "q"; 
    do{
        //prints menu
        printMenu();
        
        //asks user for a choice
        cout << "Please select from the above actions: " << endl;
        getline(cin, choice); 
        
        //Validates the choice entered 
        bool validInput = isValidInput(choice);
        while(!validInput){
            cout << "\nPlease enter a valid choice: " << endl;
            getline(cin, choice); 
            validInput = isValidInput(choice);
        }

        if(tolower(choice[0]) == 'a'){
            addItem(log); 
        }
        else if(tolower(choice[0]) == 's'){
            searchItem(log);
        }
        else if(tolower(choice[0]) == 'd'){
            deleteItem(log);
        }
        else if(tolower(choice[0]) == 'p'){
            printCatalog(log);        }
        

    }while(tolower(choice[0]) != 'q');
}
