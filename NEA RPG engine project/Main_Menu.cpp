#include "Main_Menu.h" 

void Main_Menu::construct_buttons(sf::RenderWindow &window, sf::View &view) //calls Button::full_construct() for each button
{
    login_button.button_full_construct("Login", sf::Vector2i(0, 0), view, window);
    sign_up_button.button_full_construct("Sign Up", sf::Vector2i(0, 0), view, window);
    settings_button.button_full_construct("Settings", sf::Vector2i(0, 0), view, window);
    exit_button.button_full_construct("Exit", sf::Vector2i(0, 0), view, window);
    play_button.button_full_construct("Play Project", sf::Vector2i(0, 0), view, window);
    edit_button.button_full_construct("Edit Project", sf::Vector2i(0, 0), view, window);
    log_out_button.button_full_construct("Log Out", sf::Vector2i(0, 0), view, window);
    mm_win.construct_button(window, view); //constructs the enter button used in mm_win
    
}

void Main_Menu::menu_layout(sf::RenderWindow &window, sf::View &view) //handles positioning each button, window and text element
{
    drawable_calculations::center_n_offset_text(mm_header, 0, -81, view);
    drawable_calculations::center_n_offset_sprite(sign_up_button.button_sprites[button_sprites::beginning], +5, -10, view); //when the beginning of a button is moved, the other components will move with it (this is the job of Button::render_button)
    login_button.setPos(66, 90);
    drawable_calculations::center_n_offset_sprite(settings_button.button_sprites[button_sprites::beginning], 0, sign_up_button.button_sprites[button_sprites::beginning].getGlobalBounds().height, view);
    drawable_calculations::center_sprite(exit_button.button_sprites[button_sprites::beginning], view);
    exit_button.setPos(107, 142);
   
    drawable_calculations::center_n_offset_sprite(edit_button.button_sprites[button_sprites::beginning], 4, -10, view);
    play_button.setPos(edit_button.button_sprites[button_sprites::beginning].getPosition().x - edit_button.get_size().x - 4, edit_button.button_sprites[button_sprites::beginning].getPosition().y);
    drawable_calculations::center_sprite(log_out_button.button_sprites[button_sprites::middle], view);
    log_out_button.setPos(88, 142);
}

void Main_Menu::handle_events(sf::RenderWindow& window, sf::View& view, sf::Event &event) //handles program operations that happen in SFML's event loop, such as mouse clicking
{
    window.setKeyRepeatEnabled(false);
    if (User_details[details::Username] == "") //if there is no username, the user is logged out
    {
        if (what_button == -1) //what_button = 0 when the user is at the first part of the menu
        {
            if (login_button.make_functional(window, view, event))
            {
                what_button = button_pressed::login;
            }
            else if (sign_up_button.make_functional(window, view, event))
            {
                what_button = button_pressed::sign_up;
            }
            else if (settings_button.make_functional(window, view, event))
            {
                std::cout << "This is the settings button\n"; //settings are not implemented and will remain that way during the prototype's development
            }
            else if (exit_button.make_functional(window, view, event))
            {
                exit(EXIT_SUCCESS); //closes the program
            }
        }
        switch (what_button)
        {
        case button_pressed::sign_up:
            mm_win.set_title("Sign Up");
            mm_win.handle_events(window, event, what_button);

            if (mm_win.enter_button.make_functional(window, view, event)) //if the enter button has been pressed
            {
                what_button = 0;
                for (int i = 0; i != sizeof(mm_fields) / sizeof(mm_fields[0]); i++)
                {
                    User_details[i] = mm_fields[i].getText(); //gets entered text from text field
                    mm_fields[i].clear(); //clears text fields
                }
                register_account(); //responsible for registering a user into their account, uses adapted code from the first development iteration
                User_details[details::Password] = ""; //these are not needed after registering
                User_details[details::Password_confirmation] = ""; //so it is best to clear them
            }
            else if (what_button == -1) //what_button will also be 0 when the "x" button on the widnow is pressed
            {
                for (int i = 0; i != sizeof(mm_fields) / sizeof(mm_fields[0]); i++)
                {
                    mm_fields[i].clear(); //clears text fields without getting text from them
                }
            }
            mm_fields[details::Username].allow_text_entering(window, event); //allows text to be entered into the field
            mm_fields[details::Password].allow_censored_text_entering(window, event); //allows text to be entered into a field while also censoring it, this is useful for when a user enters personal data such as their passwords
            mm_fields[details::Password_confirmation].allow_censored_text_entering(window, event);
            break;
        case button_pressed::login:
            mm_win.set_title("Log In");
            mm_win.handle_events(window, event, what_button);

            if (mm_win.enter_button.make_functional(window, view, event)) //if the enter button has been pressed
            {
                what_button = -1;
                for (int i = 0; i != (sizeof(mm_fields) / sizeof(mm_fields[0])) - 1; i++)
                {
                    User_details[i] = mm_fields[i].getText(); //gets entered text from text fields
                    mm_fields[i].clear(); //clears the text fields
                }
                log_in(); //responsible for logging a user into their account, uses adapted code from the first development iteration
            }
            else if (what_button == -1) //what_button will also be 0 when the "x" button on the widnow is pressed
            {
                for (int i = 0; i != (sizeof(mm_fields) / sizeof(mm_fields[0])) - 1; i++)
                {
                    mm_fields[i].clear(); //clears text fields without getting text from them
                }
            }
            mm_fields[details::Username].allow_text_entering(window, event);
            mm_fields[details::Password].allow_censored_text_entering(window, event);
            break;
        }
    }
    else //if the username string is not empty, the user is logged in
    {
        if (what_button == -1) //the play and edit buttons will have their respective functions implemented in development iteration 3
        {
            if (play_button.make_functional(window, view, event))
            {
                std::cout << "This is the play button\n";
            }
            else if (edit_button.make_functional(window, view, event))
            {
                int new_or_existing = MessageBoxA(NULL, "Are you creating a new project?", "Project Editor", MB_YESNOCANCEL | MB_ICONQUESTION);
                switch (new_or_existing)
                {
                case IDYES:
                    what_button = button_pressed::go_to_editor; 
                    break; 
                case IDNO:
                    what_button = button_pressed::go_to_editor;
                    break; //as project saving has not been imlemented, clicking the "yes" or "no" button does the same thing
                }
            }
            else if (settings_button.make_functional(window, view, event))
            {
                std::cout << "This is the settings button\n";
            }
            else if (log_out_button.make_functional(window, view, event))
            {
                User_details[details::Username] = ""; //makes username string empty, thus logging the user out
            }
        }
    }
    window.setKeyRepeatEnabled(true);
}

void Main_Menu::render_menu(sf::RenderWindow &window, sf::View &view) //handles drawing menu elements and positioning that needs to happen in the main loop
{
    if (User_details[details::Username] == "") //if there is no username, the user is logged out
    {
        login_button.override_size(66); //the value of 66 is chosen to keep button size consistent with the user interface mockup, and account for the size difference between the old "Retroville NC" font used in the UI mockups and the new "Press Start K" font (the old override size for buttons was 62)
        sign_up_button.override_size(66);
        settings_button.override_size(66);
        exit_button.override_size(66);
        drawable_calculations::center_n_offset_sprite(settings_button.button_sprites[button_sprites::middle], 0, settings_button.button_sprites[button_sprites::middle].getGlobalBounds().height, view);
        window.draw(mm_header);
        login_button.render_button(window, view);
        sign_up_button.render_button(window, view);
        settings_button.button_sprites[button_sprites::beginning].setPosition(settings_button.button_sprites[button_sprites::middle].getPosition().x - settings_button.button_sprites[button_sprites::beginning].getGlobalBounds().width, settings_button.button_sprites[button_sprites::middle].getPosition().y); //as the settings button is reused, we need to change it's position here unlike the other buttons whose position is set once
        settings_button.render_button(window, view);
        exit_button.render_button(window, view);
        switch (what_button) //handles drawing the sign up and login windows
        {
        case button_pressed::sign_up: 
            mm_win.draw_ui_window(window, view);
            mm_fields[details::Username].set_field_position(mm_win.window_sprite.getPosition().x + 5, mm_win.window_sprite.getPosition().y + 15);
            mm_fields[details::Password].set_field_position(mm_fields[details::Username].getPos().x, mm_fields[details::Username].getPos().y + mm_fields[details::Username].getGlobalBounds().height + 17);
            mm_fields[details::Password_confirmation].set_field_position(mm_fields[details::Password].getPos().x, mm_fields[details::Password].getPos().y + mm_fields[details::Password].getGlobalBounds().height + 17);
            for (int i = 0; i != sizeof(mm_fields) / sizeof(mm_fields[0]); i++)
            {
                mm_fields[i].render_field(window); //iterates through each field and renders them
            }
            break;
        case button_pressed::login:
            mm_win.draw_ui_window(window, view);
            mm_fields[details::Username].set_field_position(mm_win.window_sprite.getPosition().x + 5, mm_win.window_sprite.getPosition().y + 15);
            mm_fields[details::Password].set_field_position(mm_fields[details::Username].getPos().x, mm_fields[details::Username].getPos().y + mm_fields[details::Username].getGlobalBounds().height + 17);
            mm_fields[details::Password_confirmation].set_field_position(-200, -200); //sends the password confirmation field far to the left of the screen where the user can see or access it (as the window can only extend across the positive x-axis)
            for (int i = 0; i != sizeof(mm_fields) / sizeof(mm_fields[0]) - 1; i++) //iterates through each field and renders them
            {
                mm_fields[i].render_field(window);
            }
            break;
        default:
            break;
        }
    }
    else //displays the menu when the user has logged in
    {
        mm_header.setString("Welcome!");
        logged_in_as.setString("Logged in as:");
        drawable_calculations::center_text(logged_in_as, view);
        display_username.setString(User_details[details::Username]);
        logged_in_as.setPosition(logged_in_as.getPosition().x, mm_header.getPosition().y + mm_header.getGlobalBounds().height + 2);
        drawable_calculations::center_text(display_username, view);
        display_username.setPosition(display_username.getPosition().x, logged_in_as.getPosition().y + logged_in_as.getGlobalBounds().height + 2);
        play_button.override_size(104); 
        edit_button.override_size(104);
        settings_button.override_size(104); 
        log_out_button.override_size(104);
        drawable_calculations::center_n_offset_sprite(settings_button.button_sprites[button_sprites::beginning], (-settings_button.button_sprites[button_sprites::middle].getGlobalBounds().width / 2) - 2, edit_button.button_sprites[button_sprites::beginning].getGlobalBounds().height, view);
        
        play_button.render_button(window, view);
        edit_button.render_button(window, view);
        settings_button.render_button(window, view);
        log_out_button.render_button(window, view);
        window.draw(mm_header);
        window.draw(logged_in_as);
        window.draw(display_username);
    }
}

void Main_Menu::register_account() //responsible for registering a user into their account, uses adapted code from the first development iteration
{
    std::fstream User_data;
    User_data.open("User data.csv", std::fstream::app | std::fstream::in); //opens the file for appending
    std::string special_chars = "_-$.@#%!=+()<>/[]{};:^&?"; //client feedback has been taken and more special characters have been added
    if (User_data.is_open())
    {
        while (true) // while loop allows early exit if certain first checked conditions arent true, saving time if we initially know a password or username is invalid
        {
            if (!(User_details[details::Password] == User_details[details::Password_confirmation])) //check that initial password and password confirmation match
            {
                MessageBoxA(NULL, "Error: initial password and re-entered passwords do not match", "Account registration unsuccessful", MB_OK | MB_ICONSTOP);
                User_details[details::Username] = "";
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
                MessageBoxA(NULL, ("Password must contain 2 numbers and 1 special character (any one of the following: " + special_chars + ")").c_str(), "Account registration unsuccessful", MB_OK | MB_ICONSTOP);
                User_details[details::Username] = "";
                break;
            }
            else
            {
                bool found = false; //flag that checks for identical user IDs
                std::string gen_user_id;
                while (true)
                {
                    std::string line; //gets the entire line from the csv file
                    std::string buffer; //this is a buffer to use for getting each line of the csv file, as we dont dont know yet if what is recieved is a username or hashed password
                    std::string current_user_ids; //client feedback has been taken and the program will now generate user IDs
                    std::stringstream hex_stream;
                    hex_stream << std::hex << (rand() % 0xFFFF + 1) << (rand() % 0xFF + 1); //The compiler I am using will not allow hex values greater than 2 bytes, so to get 6 digits i streamed an extra byte to hex stream
                    gen_user_id = (hex_stream.str()); //casts the std::stringstream hex_stream to a normal std::string
                    found = false;
                    User_data.clear(); //clears the fstream state, so that we can read our file after it has already reached the end (eof)
                    User_data.seekg(std::ios::beg); //sets the filestream pointer to the beginning of the file
                    while (std::getline(User_data, line)) //iterates through each line in the csv file
                    {
                        std::istringstream l(line); //converts line into an istringstream so that it can be used in std::getline()
                        for (int j = 0; std::getline(l, buffer, ','); j++)
                        {
                            if (j % 3 == 0) //usernames come before passwords, thus an odd iterator means we are collecting a username
                            {
                                if (User_details[details::Username] == buffer)
                                {
                                    MessageBoxA(NULL, "This Username already exists in our database, try another one.", "Failed To Resgister New Account", MB_OK | MB_ICONERROR);
                                    User_details[details::Username] = "";
                                    break;
                                }
                            }
                            else if (j % 3 == 2) 
                            {
                                current_user_ids = buffer;
                                if (current_user_ids == gen_user_id)
                                {
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (!found)
                    {
                        break;
                    }
                }
                User_data.clear(); //clears the fstream state, so that we can read our file after it has already reached the end (eof)
                User_data.seekp(std::ios::beg); //sets the filestream pointer to the beginning of the file
                if (User_details[details::Username] != "")
                {
                    User_data << User_details[details::Username] << "," << hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_224, User_details[details::Password]) << "," << gen_user_id << "\n"; //writes username and hashed password to user details csv file
                    User_data.close(); //clsoes and applies changes made to the csv file
                    MessageBoxA(NULL, "successfully written to database...", "Success!", MB_OK | MB_ICONINFORMATION);
                }
                break;
            }
        }
    }
}

void Main_Menu::log_in()
{
    std::fstream User_data;
    User_data.open("User data.csv", std::fstream::in); //opens user data csv file for reading and writing
    User_details[details::Password] = hashpp::get::getHash(hashpp::ALGORITHMS::SHA2_224, User_details[details::Password]); //turns the password the user is trying to validate into a hash value
    if (User_data.is_open())
    {
        bool details_found_flag = false;
        std::string csv_User_details[2]; //a secondary buffer to place each individual's user details (each line of the csv file) into while we compare and validate the input user details with those stored in the csv file
        std::string buffer; //this is a buffer to use for getting each line of the csv file, as we dont dont know yet if what is recieved is a username or hashed password
        std::string line; //gets the entire line from the csv file
        for (int i = 0; std::getline(User_data, line); i++) //iterates through each line in the csv file
        {
            std::istringstream l(line); //converts line into an istringstream so that it can be used in std::getline()
            for (int j = 0; std::getline(l, buffer, ','); j++)
            {
                if (j % 3 == 0) //usernames come before passwords, thus an odd iterator means we are collecting a username
                {
                    csv_User_details[details::Username] = buffer;
                }
                else if (j % 3 == 1) //passwords come after usernames, thus an even iterator means we are collecting a hashed password
                {
                    csv_User_details[details::Password] = buffer;
                }
            }
            if (User_details[details::Username] == csv_User_details[details::Username]
                && User_details[details::Password] == csv_User_details[details::Password])
            { //if the input username and associated hashed password matches with those stored in the csv file, the user has signed in successfully
                MessageBoxA(NULL, ("Successfully logged in as " + User_details[details::Username]).c_str(), "Success!", MB_OK | MB_ICONINFORMATION); 
                details_found_flag = true; //ensures that the program doesnt outut the "username of password incorrect" message
                break;
            }
        }
        if (!details_found_flag)
        {
            MessageBoxA(NULL, ("Username or password incorrect."), "Login Failed.", MB_OK | MB_ICONERROR);
            User_details[details::Username] = "";
        } //ensures that the "Username or password incorrect message" only appears once, after the validation process is finished
        User_data.close();
    }
    else MessageBoxA(NULL, ("Unable to locate or open user database file"), "File Could Not Be Opened", MB_OK | MB_ICONERROR);
}