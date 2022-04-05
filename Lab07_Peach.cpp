//Student: Matthew Peach
//Program Status: Complete
//
//Description:
//     I challenged myself by trying to create splitable receipts that can all
// recieve custom tip amounts. I was able to make the initial one evenly splitable
// (where the user selects the number of splits). This was much more difficult than I expected, so I
// didn't go on to make every one splitable after the initial one or splitable into custom
// amounts. Managing multiple menus was also more difficult than I thought it would be, especially with
// C++ requiring so much input checking (can't just 'try-except'). I did learn a lot from this though - 
// like that you can't initalize static class variables inside of the class
// and how to use initalizer lists for objects.


#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;


class Receipt{
private:
                             // You can't initalize static class variables to a default value, apparently.
    static int numReceipts; // I had to initialize it outside of the class, but still in the scope of the class (using ' :: '
                           // to make it work - I have no idea what the difference is between that and just initializing it here)
    
    int receiptNumber;
    float totalWithNoTip;
    float taxRate;
    float tip; // On the untaxed total, as in the instructions


public:
    Receipt(float total, float tax = 0.085, float gratuity = 0.00) : totalWithNoTip{total}, taxRate{tax}, tip{gratuity} { // Using an intializer list
        ++numReceipts;
        this->receiptNumber = numReceipts; // Marking this Reciept object with a unique Reciept#
    }
    static int getNumReceipts(){
        return numReceipts;
    }
    int getReceiptNumber(){
        return receiptNumber;
    }
    float getTip(){
        return tip;
    }
    float getTotalWithNoTip(){
        return totalWithNoTip;
    }
    float getTotalWithNoTaxesOrTip(){
        return totalWithNoTip/(1 + taxRate);
    }
    float getTaxesDue(){
        return totalWithNoTip - totalWithNoTip/(1 + taxRate);
    }
    float getFinalTotal(){
        return totalWithNoTip + tip;
    }
    void setTip(float tipPercentage){
        this->tip = getTotalWithNoTaxesOrTip() * (tipPercentage/100);
    }
    void printThisReceipt();
    

};

// Static Attribute Initialization
int Receipt::numReceipts {0};


// Function Prototypes
float floatConvertPercentage(string userInput);
float floatConvert(string userInput);
int posIntegerConvert(string userInput);
int receiptSelection(vector <Receipt> &ReceiptVector);
vector <float> splitDollarAmount(float dollars, int divisions);


int main(){



string anotherTable {"Unset"};
string taxRateRaw {"Unset"};

    float taxRate {0.00};
    // Tax rate shouldn't change between tables - it's also only input once in the lab example
    do{
       cout << "What is the tax rate here? Enter the percentage (i.e. \"8.5 %\" = 8.5 or simply put 8.5%)" << endl
            << "Tax Rate (%): ";
        getline(cin, taxRateRaw); // getline(cin, inputVariable) allows spaces to be used
       cin.clear(); // just in case
        cout << endl;
        taxRate = (floatConvertPercentage(taxRateRaw))/100;
        if (taxRate == 0){
            cout << "Please enter a tax rate higher than zero." << endl;
        }
    } while(taxRate <= 0.00);



// Most of main is in this do-while loop so that you can keep inputting information for more tables
// the receipt numbers remain unique between loops since numReceipts is static.
// It is indented a bit strangely with "do" on the same "level" as main, but I feel it looks better this way
// since there's so much in the do-while loop and lines start moving pretty far over.
do{
    // Variables reinitalized every time
    string totalRaw {"Unset"};
    float total {0.00}; // total of all table's purchases
    
    vector <Receipt> ReceiptVector; // Will hold all Receipt objects
    int selection {0}; // Selects a Reciept from the vector
    string yesOrNo {"Unset"}; // Choice of splitting your reciept or not
    string splitSelectionRaw {"Unset"};
    int splitSelection {0}; // Choice of how many even splits for the reciept
    
    vector <float> splitOptionVector; // Temporarily holds the even split bill totals before 
                                      // they are used to initialize new Reciept Objects
    string tipOrExitRaw {"Unset"};
    int tipOrExit {-1}; // User chooses what to do with their reciepts
    
    string tipPercentRaw{"Unset"};
    float tipPercent {0.00}; // User choice of tip percentage
 
    bool totallyDone {false}; // Used to exit the final loop if the user enters '3'
    
    
    do{
        cout << "Thank you for dining with us today! What was the total of your entire table's purchases? (including taxes)" << endl
             << "We can split the bill in just a moment." << endl
             << "Total in purchases for my table: $";
        getline(cin, totalRaw); 
        cin.clear();
        cout << endl;
        total = floatConvert(totalRaw);
    } while(total < 0.00);
    
    // Creating the first Receipt and putting it in the Receipt Vector
    ReceiptVector.push_back(Receipt(total, taxRate));
    ReceiptVector[0].printThisReceipt();
    
    //======================== Bill Split? (Yes/No) ===========================================================================================================
    do{
        cout << "Would you like to evenly split this bill? (y/n)" << endl
             << "My Answer: ";
        getline(cin, yesOrNo);
        cin.clear();
        if (yesOrNo == "N" || yesOrNo == "n"){
            cout << "Ok, we'll leave it all together." << endl;
        }
        
        //============================ Number of Splits Selection ==============================================================================================
        else if(yesOrNo == "Y" || yesOrNo == "y"){
            do{
                cout << "How many ways would you like it split? (use whole numbers or '1' if you changed your mind)" << endl
                     << "My Answer: ";
                getline(cin, splitSelectionRaw);
                cin.clear();
                splitSelection = posIntegerConvert(splitSelectionRaw);
            }while(splitSelection <= 0);
            
            //============= Creation of new Reciept Objects ====================================================================================================
            splitOptionVector = splitDollarAmount(ReceiptVector[0].getFinalTotal(), splitSelection); //Will split the total into smaller amounts
            ReceiptVector.clear(); //Erase the original
            for (int i = 0; i < splitOptionVector.size(); i++){
                ReceiptVector.push_back(Receipt(splitOptionVector[i], taxRate));
            }
        }
        else{
            cout << "Please use 'y' or 'n' as a response." << endl;
        }
    }while(yesOrNo != "n" && yesOrNo != "N" && yesOrNo != "Y" && yesOrNo != "y");
    
    //======================== Menus =============================================================================================================================
    do{
        //================= Display all Receipts =====================================================================================================================
        selection = receiptSelection(ReceiptVector);
    
        //================ Selections for an Individual Receipt ======================================================================================================
        do{
            ReceiptVector[selection].printThisReceipt();
           cout << "Options: 1.) Leave a Tip" << endl
                << "         2.) Go Back" << endl
                << "         3.) I'm all done! Exit and print all receipts." << endl
                << "My Selection: ";
            getline(cin, tipOrExitRaw);
            cin.clear();
            tipOrExit = posIntegerConvert(tipOrExitRaw);
        }while(tipOrExit != 1 && tipOrExit != 2 && tipOrExit != 3);
        
        //======================= Setting a Tip ==================================================================================================================
        if (tipOrExit == 1){
            do{
                //ReceiptVector[selection].printThisReceipt();
                cout << endl << "What percentage would you like to tip? (calculated with the subtotal)" << endl
                    << "Tip Amount (Percentage): ";
                getline(cin, tipPercentRaw);
                cin.clear();
                tipPercent = floatConvertPercentage(tipPercentRaw);
            }while(tipPercent < 0);
            
            ReceiptVector[selection].setTip(tipPercent); //Can safely use the .setTip method
        }
        //==================== Finally Exit ======================================================================================================================
        if(tipOrExit == 3){
            for (int i = 0; i < ReceiptVector.size(); i++){
                ReceiptVector[i].printThisReceipt();
            }
            totallyDone = true;
        }
    }while(totallyDone == false);
    
    do{
        cout << "Thank you for dining with us today, we hope to see you had a good interaction with the payment system!" << endl << endl
             << "Do you want to get receipts for another table? (y/n): ";
        getline(cin, anotherTable);
        cin.clear();
    }while(anotherTable != "Y" && anotherTable != "y" && anotherTable != "N" && anotherTable != "n");
    cout << endl << "=================================================" << endl
                 << "                 Next Table                      " << endl
                 << "=================================================" << endl;
}while(anotherTable == "Y" || anotherTable == "y");    
    
    
    cout << endl << endl << "Thanks again, bye!" << endl << endl;
    
    return 0;
}

float floatConvertPercentage(string userInput){
    //Initialized to count the decimals in userInput
    int decimalCount {0};
    
    //==============================================================================================
    //         NEW CHECKS to account for spaces and fixing a bug I found
    //==============================================================================================
    if (userInput.empty()){
        cout << "\nInput failed! Please enter something!" << endl;
        return -1.00;
    }
    if (userInput.length() == 1 && !isdigit(userInput[0]) ){ //Triggers if user input one character that isn't even number
    cout << "\nInput failed! You need to include numbers." << endl; // (you could previously crash the program by inputting '.' or spaces)
    return -1.00;
    }   
    
    //==============================================================================================
    //First, all commas are removed by using this loop =============================================
    //(Commas are acceptable input for high numbers)
    // ************(Newly added ability to remove '%' signs)******************
    for (int i {0}; i < userInput.length(); i++){
        if (userInput[i] == ',' || userInput[i] == '%' || userInput[i] == ' '){
            userInput.erase(i,1); // erase(index to start erasure, number of characters to erase)
        }
         
    }
    //==============================================================================================
    
    //==============================================================================================
    //Second, the user input is checked for having > 1 decimal
    //(Having > 1 decimal will make the input invalid)
    for (int i {0}; i < userInput.length(); i++){
        if (userInput[i] == '.'){
            decimalCount++;
        }
    }
    if (decimalCount > 1){
        cout << "\nInput failed! Please enter a number with at most ONE decimal." << endl;
        return -1.00;
    }
    //==============================================================================================
    
    //==============================================================================================
    //Finally, check to make sure everything in the user input is a number or the decimal
    //  and see if the user thought they could give a negative entry and correct it
    for (int i {0}; i < userInput.length(); i++){
        // New "if" conditin to give a specific error message if the user thought
        //   a negative entry was acceptable
        if (userInput[0] == '-'){
            cout << "\nInput failed! Please enter a positive number." << endl;
            return -1.00;
        }
        if ( !isdigit(userInput[i]) && userInput[i] != '.'){
            cout << "\nInput failed! Please only use numbers (a decimal, commas, and % are acceptable)." << endl;
            return -1.00;
        }
    }
    //It should now be safe to convert the userInput to a float without error
    //   "string to float"
    return std::stof(userInput);
}

//Same as above, but no percentages allowed
float floatConvert(string userInput){
    //Initialized to count the decimals in userInput
    int decimalCount {0};
    
    //==============================================================================================
    //         NEW CHECKS to account for spaces and fixing a bug I found
    //==============================================================================================
    if (userInput.empty()){
        cout << "\nInput failed! Please enter something!" << endl;
        return -1.00;
    }
    
    if (userInput.length() == 1 && !isdigit(userInput[0]) ){ // Triggers if user input one character that isn't even number
        cout << "\nInput failed! You need to include numbers." << endl; // (you could previously crash the program by inputting '.' or spaces)
        return -1.00;
   } 
    //==============================================================================================
    
    //First, all commas are removed by using this loop =============================================
    //(Commas are acceptable input for high numbers)
    // ************(Newly added ability to remove '%' signs)******************
    for (int i {0}; i < userInput.length(); i++){
        if (userInput[i] == ',' || userInput[i] == ' '){
            userInput.erase(i,1); // erase(index to start erasure, number of characters to erase)
        }
    }
    //==============================================================================================
    
    //Second, the user input is checked for having > 1 decimal =====================================
    //(Having > 1 decimal will make the input invalid)
    for (int i {0}; i < userInput.length(); i++){
        if (userInput[i] == '.'){
            decimalCount++;
        }
    }
    if (decimalCount > 1){
        cout << "\nInput failed! Please enter a number with at most ONE decimal." << endl;
        return -1.00;
    }
    //==============================================================================================
    
    //Finally, check to make sure everything in the user input is a number or the decimal
    //  and see if the user thought they could give a negative entry and correct it
    for (int i {0}; i < userInput.length(); i++){
        // New "if" conditin to give a specific error message if the user thought
        //   a negative entry was acceptable
        if (userInput[0] == '-'){
            cout << "\nInput failed! Please enter a positive number." << endl;
            return -1.00;
        }
        if ( !isdigit(userInput[i]) && userInput[i] != '.'){
            cout << "\nInput failed! Please only use numbers or a decimal." << endl;
            return -1.00;
        }
    }
    //It should now be safe to convert the userInput to a float without error
    //   "string to float"
    return std::stof(userInput);
}

// Also used this in another weekly assignment, but took away the ability to erase commas this time since the input shouldn't be that large
int posIntegerConvert(string userInput){
    if (userInput.empty()){
        cout << "\nInput failed! Please enter something!" << endl;
        return -1;
    }
    
    // Since this function only makes sure an integer is returned, only whole numbers are allowed.
    for (int i {0}; i < userInput.length(); i++){
        if (userInput[0] == '-'){
            cout << "\nInput failed! Please enter a positive number (exclude the '+')." << endl;
            return -1;
        }
        if ( !isdigit(userInput[i]) ){
            cout << "\nInput failed! Please only use numbers." << endl;
            return -1;
        }
    }
    // userInput should be able to convert to int now without error, if it skips the previous two "if" statements
    return std::stoi(userInput);
}


int receiptSelection(vector <Receipt> &ReceiptVector){
    string userSelectionRaw {"Unset"};
    int userSelectionValid {-1};
    cout << endl << "===========================" << endl
                 << "|      Your Reciepts      |" << endl
                 << "===========================" << endl;
    do{
        
        for (int i = 0; i < ReceiptVector.size(); i++){
            cout << std::right << std::setw(2) << i + 1 << ".) Receipt #" << ReceiptVector[i].getReceiptNumber() << ": " << ReceiptVector[i].getFinalTotal() << endl;  
        }
        cout << "\nChoose a Receipt to view and leave a tip (use the number to the left not the \"Receipt #\")" << endl
             << "My Selection: ";
        getline(cin, userSelectionRaw);
        cin.clear();
        userSelectionValid = posIntegerConvert(userSelectionRaw);
    
    // Loops if the selection is invalid due to not using a numeric character,
    //  putting in a 0, or putting in a number one greater than the number of Receipts in the vector
    }while(userSelectionValid < 1 || userSelectionValid > ReceiptVector.size());
    
    return userSelectionValid - 1; // -1 to line up with the vector positions
}

vector <float> splitDollarAmount(float dollars, int divisions){
    vector <float> splitAmountsVector; // Will hold the new split amounts
    float newAmountsEvenlySplit = dollars/divisions;
    string newAmountsEvenlySplitString = std::to_string(newAmountsEvenlySplit);
    float newAmounts {0.00};
    int indexOfDecimal {0};
    int leftoverPennies {0};
    
    
    // Find where the decimal is
    for (int i = 0; i < newAmountsEvenlySplitString.length(); i++){
        if (newAmountsEvenlySplitString[i] == '.'){
            indexOfDecimal = i;
        }
    }
    //cout << indexOfDecimal << endl; //******** DEBUG LINE *********************
    
    // New amounts go here, with the less than a penny amounts "cut off"
    // then converted back to a float
    newAmounts = std::stof(newAmountsEvenlySplitString.substr(0, indexOfDecimal + 3)); // newAmounts = evenly split values from the first number to the decimal + 2 more numbers
    for (int i = 1; i <= divisions; i++){
        splitAmountsVector.push_back(newAmounts); // Should contain "divisions" amount of values
    } 
    
    leftoverPennies = round((dollars - (newAmounts * divisions)) * 100); //Actual number of pennies
    
    if (leftoverPennies > 0){
        for(int i = 0; i < splitAmountsVector.size(); i++){
            if (leftoverPennies == 0){
                break;
            }
            leftoverPennies--; // Subtract a leftover penny
            splitAmountsVector[i] += 0.01; // Add a penny
        }
    }
    // Just giving the leftover pennies to whoever
    splitAmountsVector[0] += (leftoverPennies/100);
    
    return splitAmountsVector;
}

// ================================ METHODS ================================================================================================================

// This took a lot of trial and error to format correctly
void Receipt::printThisReceipt(){
    cout << std::fixed << std::setprecision(2);
    cout << "================================" << endl 
         << "|    Receipt #" << std::left << std::setw(17) << getReceiptNumber() << "|" << endl
         << "================================" << endl
         << "| Subtotal: $" << std::left << std::setw(18) << getTotalWithNoTaxesOrTip()  << "|" << endl
         << "| Taxes: $" << std::left << std::setw(21) << getTaxesDue() << "|" << endl
         << "| Tip:   $" << std::left << std::setw(21) << getTip() << "|" << endl
         << "| Total: $" << std::left << std::setw(21) << getFinalTotal()  << "|" << endl
         << "================================" << endl;
}