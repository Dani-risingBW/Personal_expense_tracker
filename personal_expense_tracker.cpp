#include <iostream> 
#include <string>
#include <vector> 
#include <ctime> 
#include <iomanip>
#include <chrono>
#include <format>
#include <fstream>
#include <algorithm>
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

void addItem(vector<Expense>& vec, fstream& file){
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
    //switching to persistent storage: Using I/O operations 
    //Make sure file is open
    //writing to file now....
    //Must make sure the pointer of the file is at the end
    file.open("records.csv", ios::out | ios::app) ;
    if (!file) {
        std::cerr << "Failed to open file for writing.\n";
    }
    file.seekp(0, ios::end);
    file << cat << "," << des << "," << time << "," << price << endl;
    file.close();
};

void printItem(const vector<string>& vec){
    cout << vec[0] << " " << vec[1] << " " << vec[2] << " $" << vec[3] << endl;
}; 

void printCatalog(vector<Expense>& vec, fstream& file){
  /* USES VECTOR TO PRINT
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
*/

    //Will hold the one line in the file 
    string line;
    //make sure file points to beginning
    //Opens the file to read
    file.open("records.csv", ios::in);
    file.seekg(0, ios::beg);
    if (!file) {
        std::cerr << "Failed to open file for reading.\n";
    }
    //Read the file line by line
    while(getline(file,line)){
        stringstream ss(line);
        string cell;

        //Read each cell (column) in the line separated by commas
        while(getline(ss,cell,',')){
            cout << cell << "\t"; //output each cell separated by a tab
        }

        cout << endl; //starts a new line at end of row
    }
    file.close();

}; 

void searchItem(vector<Expense>& vec, fstream& file){
    string targetDate; 
    bool found = false; 
    cout << "\nEnter the date of the log you would like to find for(MM/DD/YYYY):" << endl;
    cin >> targetDate; 

    while(!checkDate(targetDate)){
        cout << "Please enter a valid date using this format: (MM/DD/YYYY)" << endl; 
        cin >> targetDate; 
    }
    
    //for every element in the vector it checks the targetDate. If it is true, then 
    //it will print every log under that date
/*  for(int i = 0; i < vec.size(); i++){
        if(vec[i].getDate() == targetDate){
            printItem(vec[i]);  
            found = true; 
        }
    }
    if(!found){
        cout << "No logs were found." << endl;
    }
*/
    //searches for log in file
    string line;

    //opens file to read
    file.open("records.csv", ios::in);
    if (!file) {
        std::cerr << "Failed to open file for writing.\n";
    }
    //makes sure file points to beginning
    file.seekg(0, ios::beg);

    while(getline(file,line)){
        stringstream ss(line);
        string cell;
        vector<string> row;

        while(getline(ss,cell,',')){
            row.push_back(cell); //turns each individual item into an element to search 
        }
        
        //checks if target value matches the date column(should be the 3 column)
        if(row[2] == targetDate){
            cout << "\nMatch found:" << endl;
            printItem(row); 
            found = true;
        }
    }
    if(!found){
        cout << "No matched found." << endl;
    }
    file.close();
}; 

void deleteItem(vector<Expense>& vec, fstream& file){
    string targetDate; 
    cout << "\nEnter the date of the log you would like to search for(MM/DD/YYYY):" << endl;
    cin >> targetDate; 

    while(!checkDate(targetDate)){
        cout << "Please enter a valid date using this format: (MM/DD/YYYY)" << endl; 
        cin >> targetDate; 
    }

    //creates a temp array for all the targeted dates 
    vector<Expense> temp; 
    //char choice = 'y'; 
    bool exitLoops = false; 
/*
    for(int i  = 0; i < vec.size() && !exitLoops; i++ ){
        if(vec[i].getDate() == targetDate){
            //printItem(vec[i]);
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
*/

   //creates a temperary file and opens records file
   ofstream tempFile("temp.csv");
   if(!tempFile.is_open()){
       cerr << "Could not open file." << endl;
   }
   file.open("records.csv", ios::in);
   if (!file) {
        std::cerr << "Failed to open file for writing.\n";
    }

    //searches for log in file
    string line;
    //makes sure file points to beginning
    file.seekg(0, ios::beg);

    while(getline(file,line)){
        stringstream ss(line);
        string cell;
        vector<string> row;


        while(getline(ss,cell,',')){
            row.push_back(cell); //turns each individual item into an element to search 
        }
        for(string e: row){
            cout << e << " ";
        }
        cout << "size: " << row.size() << endl;
 

        //checks if target value matches the date column(should be the 3 column)
        if(row[2] == targetDate && !exitLoops){
            printItem(row);
            cout << "Is this what you would like to delete? (Y or N)" << endl; 
            char deleteChoice;
            cin >> deleteChoice; 
            if(tolower(deleteChoice) == 'y'){
                exitLoops = true;
                cout << "Item deleted from log.\n" << endl;
            }
            else{
                tempFile << line << endl;
            }
        }
        else{
            tempFile << line << endl;
        }
    }
  
    tempFile.close(); // Always close the temp file first

    file.close();     // Close original file before replacing

    if (!exitLoops) {
        cout << "No logs were found under " << targetDate << ".\n" << std::endl;
        remove("temp.csv"); // No change, so delete temp
    } else {
        remove("records.csv");        // Replace with your actual filename
        rename("temp.csv", "records.csv");
        cout << "Log updated successfully.\n" << std::endl;
    }   
}; 

void sortByPrice(fstream& file, int num){
   //holds each line in the file 
    string line;

    //creates a temperary file and opens records file
   ofstream tempFile("temp.csv");
   if(!tempFile.is_open()){
       cerr << "Could not open file." << endl;
   }
    //opens file to read
    file.open("records.csv", ios::in);
    if (!file) {
        std::cerr << "Failed to open file for writing.\n";
    }
    //makes sure file points to beginning
    file.seekg(0, ios::beg);

    //build a matrix manually
    vector<vector<string>> fileMatrix;

    //read each line into the file
    while(getline(file,line)){
        string cell;
        stringstream ss(line);
        vector<string> row; 
        while(getline(ss,cell,',')){
            row.push_back(cell);
        }
        fileMatrix.push_back(row); //adds vector row to another vector
    }
    if(num == 3){//compares string as doubles to be correct interpret data
       sort(fileMatrix.begin() + 1, fileMatrix.end(), [num](const vector<string>& a, const vector<string>& b){
            return stod(a[num]) < stod(b[num]);
        }); 
    }
    else{
        sort(fileMatrix.begin() + 1, fileMatrix.end(), [num](const vector<string>& a, const vector<string>& b){
            return a[num] < b[num];
        });
    }

    file.close(); 

    //adds matrix to new file
    for(size_t r = 0; r < fileMatrix.size(); r++){
        for (size_t i = 0; i < fileMatrix[r].size(); ++i) {
            tempFile << fileMatrix[r][i];
            if (i != fileMatrix[r].size() - 1) 
                tempFile << ",";
        }
        tempFile << "\n";
    }

    tempFile.close();
    remove("records.csv");   // Replace with your actual filename
    rename("temp.csv", "records.csv");
};

/*void sortByCategory(fstream& file, int num){
    //holds each line in the file 
    string line;

    //opens file to read
    file.open("records.csv", ios::in);
    if (!file) {
        std::cerr << "Failed to open file for writing.\n";
    }
    //makes sure file points to beginning
    file.seekg(0, ios::beg);
};*/

void printMenu(){
    cout << "A - Add a Log" << endl;
    cout << "F - Find a Log" << endl;
    cout << "D - Delete a Log" << endl;
    cout << "S - Sort Log" << endl;
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

//This is a main function
int main(){
    //Opening statements 
    cout << "Welcome to Green FootPrint, \n"
        "an online personal expense tracker where you can log any and every expenditure.\n" << endl; 

    vector<Expense> log;  
    //opens or creates a file
    fstream myFile("records.csv", ios::in | ios::out | ios::app); 
    if(!myFile){
        cerr << "Error opening file!" << endl;
        return 1;

    }
    //Adds column headers if file is created
    if(!myFile.is_open())
        myFile << "Category, Description, Date, Price" << endl;
    
    myFile.close();
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
            addItem(log, myFile); 
        }
        else if(tolower(choice[0]) == 'f'){
            searchItem(log, myFile);
        }
        else if(tolower(choice[0]) == 'd'){
            deleteItem(log, myFile);
        }
        else if(tolower(choice[0]) == 'p'){
            printCatalog(log, myFile);   
        }
        else if(tolower(choice[0]) == 's'){
            cout << "\nWill you like to sort by price or category:" << endl;
            //cout << "\nWill you like to sort by price or category(p for price or c for category):" << endl;
            getline(cin, choice);
            while(tolower(choice[0]) != 'p' && tolower(choice[0]) != 'c'){
                cout << "\nPlease enter a valid choice: " << endl;
                getline(cin, choice);  
            }
            if(tolower(choice[0]) == 'p'){
                sortByPrice(myFile,3);
            }
            else
                sortByPrice(myFile,0);
            
        }

    }while(tolower(choice[0]) != 'q');

    myFile.close();//closes file
}

