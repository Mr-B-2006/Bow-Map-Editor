//THIS COMPONENT OF THE SYSTEM IS FINISHED, RUN THE TESTS (AND ALSO MAYBE SPLIT IT UP INTO ACCOUNT CREATION SYSTEM AND LOGIN SYSTEM)
#include <iostream> 
#include <fstream> //file handling library
#include <SFML/Graphics.hpp> //SFML graphics library (unused but will be used later in development): https://www.sfml-dev.org/ 
#include "hashpp.h" //Header for hashing: https://github.com/D7EAD/HashPlusPlus
#include <Windows.h> //header for interacting with the Windows OS
#include <sstream>
#include <string.h> 

enum details //allows numbers to have a named identifier in the source code, does not have an effect on the final compiled machine code
{
    Username = 0, Password = 1, Password_confirmation = 2
};
/*
int main()  
{
    std::string Logged_in_as = "Not currently logged into an account.";
    std::fstream User_data; //initialises the fstream file object
    while (true) //main program loop 
    {
        system("cls"); //sends the windows cmd command "cls" to the windows terminal runnning the program
        std::cout << "Welcome!\n\n" << Logged_in_as << "\nSelect:\n  Register: 1\n  Log In: 2\n  Exit: 3\n"; //basic CLI to select whether the user will Register an account to the system or log into an existing account
        int inp; //initialise the integer input we use for selection
        std::cin >> inp;
        switch (inp)
        {
        case 1: //Register account
            User_data.open("User data.csv", std::fstream::app); //opens the file for appending
            { //variables are declared in a scope, so that they can be declared in the switch statement as otherwise, the execution may jump over the varible's initilisation (which leads to the code not compiling)
                std::string User_details[3];
                std::string prompts[3] = { "Enter Username", "Enter Password", "Re-enter Password" };
                std::string special_chars = "$%^&"; //not allowing the £ sign to be used due to encoding issues
                std::cin.ignore(); //ignores unwanted "newline" ("\n") characters in the buffer, that will skip any user input prompt
                while (true) 
                {
                    for (int i = 0; i != (sizeof(prompts) / sizeof(prompts[0])); i++) //avoiding code repition by using a for loop
                    {
                        std::cout << prompts[i] << ":   \n"; //iterates through each input prompt
                        std::getline(std::cin, User_details[i]); //puts each input in the corresponding array element
                    }
                    if (!(User_details[details::Password] == User_details[details::Password_confirmation])) //check that initial password and password confirmation match
                    {
                        std::cout << "Error: initial password and re-entered passwords do not match\n";
                        break;
                    }
                    else if (User_details[details::Username].find(",") != std::string::npos) //condition for whether "," exists in string, "," shouldnt be in usernames as it would  interefere with the csv formatting, "," can be used in passwords however, as passwords are hashed and thus "," will not end up in the csv file after hashing
                    {
                        std::cout << "Error: Username cannot contain a comma ','\n";
                        break;
                    }
                    else if (User_details[details::Username].find("\x9c") != std::string::npos) //due to encoding issues and lack of time to fix said encoding issues, the "£" sign cannot be entered
                    {
                        std::cout << "Error: Username cannot contain a pound sign '\x9c'\n";
                        break;
                    }
                    int validation_flag = 0; //the value of this flag variable increases by 1 when certain conditions are met (such as digit found in the password string or special character found in the password string)
                    //validation rules: 1 special character and 2 numerical characters
                    for (int i = 0; i != (special_chars.length()); i++) //search for each special character
                    {
                        if (User_details[details::Password].find(special_chars[i]) != std::string::npos) //searches for each special character
                        {
                            validation_flag += 1; //marks that the password has a valid amount of special characters
                            break; //if we already find one special character, there is no point in continuing to search for more, so we break the loop, furthermore, this prevents usernames with special characters and no numbers being accepted
                        }
                    }
                    if (validation_flag != 0) //if validation_flag = 0, then there are no special characters, thus we can skip checking for digits in the string as we know the password has already failed validation
                    {
                        for (int i = 0; i != (User_details[details::Password].length()); i++) //iterate through each character in the unhashed password string to find any digits
                        {
                            if (isdigit(static_cast<unsigned char>(User_details[details::Password][i]))) //the current character is casted as an unsigned char as the isdigit() function cannot take in negative char values (if it did, an assertion error would occur)
                            {
                                validation_flag += 1; //increases the validation_flag value when a special character is found
                                if (validation_flag == 3) //once the value of validation_flag has reached 3, we know that g
                                {
                                    break; //break out of validation loop
                                }
                            }
                        }
                    }
                    if (validation_flag != 3) //one or more of the validation rules must not have been met if the vlaue of validation_flag is not equal to 3
                    {
                        std::cout << "Password must contain 2 numbers and 1 special character (any one of the following: " << special_chars << ")\n";
                    }
                    else
                    {
                        std::cout << "successfully written to database...\n";
                        break;
                    }
                }
                User_data << User_details[details::Username] << "," << hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_224, User_details[details::Password]) << "\n"; //writes username and hashed password to user details csv file
                User_data.close();
                Sleep(3000); //provides a 3 second delay so that the user has enough time to read previously outputted messages
            } //in the GUI iteration of this program, Sleep() will not be used as Sleep() completely halts the program for the amount of time given by the parameter, which can cause user frustration as they will think the program is being slow
            break; //(especially because windows will report the program as "not responding") furthermore, the user will not be able to interact with the program while the program is "sleeping", in this instance I will use a more appropriate function for this, such as the built-in SFML sf::Clock.getElapsedTime()
        case 2: //signing into account
            {
                User_data.open("User data.csv", std::fstream::in); //opens user data csv file for reading and writing
                std::string User_details[2];
                std::string prompts[2] = { "Enter Username", "Enter Password" };
                std::cin.ignore();       
                for (int i = 0; i != (sizeof(prompts) / sizeof(prompts[0])); i++) //avoiding code repition by using a for loop
                {
                    std::cout << prompts[i] << ":   \n";
                    std::getline(std::cin, User_details[i]); //like before, we are iterating through prompts and putting each respective input string into an array
                } //end of user input
                User_details[details::Password] = hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_224, User_details[details::Password]); //turns the password the user is trying to validate into a hash value
                if (User_data.is_open())
                {
                    bool details_found_flag = false;
                    std::string csv_User_details[2]; //a buffer to place each individual's user details (each line of the csv file) into while we compare and validate the input user details with those stored in the csv file
                    std::string buffer; //this is a buffer to use for getting each line of the csv file, as we dont dont know yet if what is recieved is a username or hashed password
                    std::string line; //gets the entire line from the csv file
                    for (int i=0;std::getline(User_data, line);i++) //iterates through each line in the csv file
                    {   
                        std::istringstream l(line); //converts line into an istringstream so that it can be used in std::getline()
                        for (int j=0;std::getline(l, buffer, ',');j++)
                        {
                            if (j % 2 == 0) //usernames come before pass    words   , thus an odd iterator means we are collecting a username
                            {
                                csv_User_details[details::Username] = buffer;
                            }
                            else //passwords come after usernames, thus an even iterator means we are collecting a hashed password
                            {
                                csv_User_details[details::Password] = buffer;
                            }
                        }
                        if (User_details[details::Username] == csv_User_details[details::Username] 
                            && User_details[details::Password] == csv_User_details[details::Password])
                        { //if the input username and assosiated hashed password matches with those stored in the csv file, the user has signed in successfully
                            Logged_in_as = "Logged in as: " + User_details[details::Username]; //allows the username of the logged in user to be displayed in the main menu, letting whoever is currently in front of the computer know what user is logged in
                            std::cout << "Successfully logged in as " << User_details[details::Username];
                            details_found_flag = true; //ensures that the program doesnt outut the "username of password incorrect" message
                            Sleep(1000);
                            break;
                        }
                    }
                    if (!details_found_flag)
                    {
                        std::cout << "Username or password incorrect.\n";
                    } //ensures that the "Username or password incorrect message" only appears once, after the validation process is finished
                    User_data.close();
                    Sleep(3000);
                }
                else std::cout << "file could not be opened";
            }
            break;
        case 3: //exit
            return 0;
            break;
        default: //handles invalid inputs
            std::cout << "Not a valid option. Please Re-enter a number";
            Sleep(1000);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //handles erroneous string data being inputted
            break;
        }
        
    }
}*/
