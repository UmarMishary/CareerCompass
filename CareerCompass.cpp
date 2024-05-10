#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>
#include <thread>
#include <ctype.h>
#include <cctype>
#include <chrono>
#include <windows.h> // For Windows systems (modify for other OS)
#include <filesystem>
using namespace std;

struct Question {
    string question;
    char personalityType; // E (Extrovert), I (Introvert), A (Ambivert)
    int extrovertWeight; // Weight for Extrovert answer (Yes/No)
    int introvertWeight; // Weight for Introvert answer (Yes/No)
};

struct UserProfile {
    string name;
    string email;
    string accountType; // Can be "Job Seeker", "Employer", etc.
};

const int MIN_USERNAME_LENGTH = 8;
const int MAX_USERNAME_LENGTH = 20;
const int MIN_PASSWORD_LENGTH = 8;

map<string, UserProfile> userProfiles; // Map username to user profile

bool getUserYesNoAnswer(const string& prompt) {
    char answer;
    do {
        cout << prompt << " (y/n): ";
        cin >> answer;
    } while (tolower(answer) != 'y' && tolower(answer) != 'n');
    return tolower(answer) == 'y';
}

bool IsLoggedIn(){
    int ch;
    string username, password;
    string un, pw; // comparison strings
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    while (true) {
        if (_kbhit()) {  // Check if a key is pressed
            ch = _getch();

            if (ch == 13) {  // Enter key pressed (password complete)
                cout << endl;
                break;
            } else if (ch == 8) {  // Backspace key pressed (remove last character)
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";  // Move cursor back and erase the asterisk
                }
            } else if (ch == ' '){
                if (isalnum(ch)){
                    password.push_back(ch);
                }
            } else {
                password.push_back(ch);
                cout << "*";  // Mask the character with an asterisk
            }
        }
    }

    ifstream read(username + ".txt"); // ifstream reads a file
    getline(read, un); // reads the username
    getline(read, pw); // reads the password

    if (un == username && pw == password){
        return true;
    } else {
        return false;
    }
}

void test_worker() {
    // Define question bank (example)
    vector<Question> questions = {
        {"Do you enjoy spending time with large groups of people?", 'E', 1, -1},  // Extrovert +1, Introvert -1
        {"Do you find yourself needing to be alone to recharge after social interaction?", 'I', -1, 1}, // Introvert +1, Extrovert -1
        {"Do you feel comfortable in both social and solitary situations?", 'A', 0, 0}, // No weight for Ambivert
        {"Would you describe yourself as someone who takes initiative and enjoys challenges?", 'E', 1, 0}, // Extrovert +1, Introvert 0
        {"Do you prefer to plan things out in advance rather than act spontaneously?", 'I', 1, 0}, // Introvert +1, Extrovert 0
        {"Do you often initiate conversations with strangers?", 'E', 1, -1},  // Extrovert +1, Introvert -1 (Social initiation)
        {"Do you get drained by spending a long time in crowded places?", 'I', -1, 1}, // Introvert +1, Extrovert -1 (Social stimulation)
        {"Do you find it easy to express your emotions openly?", 'E', 1, 0}, // Extrovert +1, Ambivert neutral (Emotional expression)
        {"Do you carefully consider all options before making a decision?", 'I', 1, 0}, // Introvert +1, Ambivert neutral (Decision-making style)
        {"Do you get energized by helping others and being involved in social activities?", 'E', 1, 0}, // Extrovert +1, Ambivert neutral (Social motivation)
        {"Would you describe yourself as someone who is easily bored?", 'E', 1, 0}, // Extrovert +1, Ambivert neutral (Need for stimulation)
        {"Do you find it difficult to say no to requests from others?", 'I', 1, 0}, // Introvert +1, Ambivert neutral (Assertiveness)
        {"Do you enjoy trying new things and taking risks?", 'E', 1, 0}, // Extrovert +1, Ambivert neutral (Openness to experience)
        {"Do you find it important to have a plan and routine in your daily life?", 'I', 1, 0}, // Introvert +1, Ambivert neutral (Structure preference)
        {"Do you get satisfaction from helping others achieve their goals?", 'E', 1, 0}, // Extrovert +1, Ambivert neutral (Social focus)
        {"Would you describe yourself as a good listener?", 'I', 0, 1}, // Introvert +1 for active listening (Ambivert neutral)
        {"Do you often find yourself reflecting on your thoughts and feelings?", 'I', 1, 0}, // Introvert +1 (Self-reflection)
        {"Would you consider yourself to be a creative or imaginative person?", 'E', 0, 0}, // Ambivert neutral (Can be present in both)
        {"Do you feel comfortable speaking your mind in a group setting?", 'E', 1, 0}, // Extrovert +1, Ambivert neutral (Confidence)
        {"Do you enjoy spending time in nature and peaceful environments?", 'I', 1, 0}, // Introvert +1 (Need for quiet time)
        {"Do you find yourself energized by being around people?", 'E', 1, -1},  // Extrovert +1, Introvert -1 (Social stimulation)
        {"Do you prefer to follow a set routine or be spontaneous?", 'I', 1, -1}, // Introvert +1 (Structure preference), Extrovert -1 (Dislike for routine)
        {"Would you describe yourself as a quick decision-maker?", 'E', 1, 0},  // Extrovert +1 (Action-oriented), Ambivert neutral        {"Would you rather take a solo trip or travel with a group?", 'I', -1, 1}, // Introvert -1 (Need for solitude), Extrovert +1 (Social travel)
        {"Do you find it easy to adapt to new situations?", 'E', 1, 0},  // Extrovert +1 (Flexibility), Ambivert neutral 
        {"Would you describe yourself as a trusting person?", 'E', 1, 0},  // Extrovert +1 (Openness to others), Ambivert neutral
        {"Do you enjoy helping others and offering support?", 'E', 1, 0},  // Extrovert +1 (Social focus), Ambivert neutral
        {"Do you tend to analyze situations before taking action?", 'I', 1, 0},  // Introvert +1 (Reflective nature), Ambivert neutral
    };

    // Shuffle questions for randomization (optional)
    random_shuffle(questions.begin(), questions.end());

    int extrovertScore = 0;
    int introvertScore = 0;

    TEST:
    cout << "Welcome To The Personality Test!" << endl;
    cout << "Are You Ready For The Test?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No " << endl;
    cout << "Enter Your Choice: ";
    int option;
    cin >> option;
    if (option == 1) {
        cout << "Let's Begin!" << endl;
    } else if (option == 2){
        cout << "Press Enter When Your Ready! ^-^";
        cin.ignore();
        cin.get();
        goto TEST;
    } else {
        cout << "Please Enter A Valid Option" << endl;
        goto TEST;
    }

    for (const Question& question : questions) {
        cout << question.question << endl;
        bool answer = getUserYesNoAnswer("Yes Or No");

        // Update scores based on answer and weight
        extrovertScore += answer ? question.extrovertWeight : 0;
        introvertScore += answer ? question.introvertWeight : 0;
    }

    // Analyze results (based on score difference)
    char dominantType;
    int scoreDifference = extrovertScore - introvertScore;
    if (scoreDifference > 0) {
        dominantType = 'E';
    } else if (scoreDifference < 0) {
        dominantType = 'I';
    } else {
        dominantType = 'A';
    }

    cout << "\nBased On Your Answers, You Seem To Be More of a(n): " << dominantType << "." << endl;
    cout << "E = Extrovert\tI = Introvert\tA = Ambivert" << endl;

    if (dominantType == 'E' || dominantType == 'A'){
        cout << "Congratulations! You Have Passed The Test." << endl;
        cout << "Now You Can Work With Us!" << endl;
    } else {
        cout << "Sorry! You Didn't Passed The Test. Wanna Re-Test?" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cout << "Enter Your Choice: ";
        int choice;
        cin >> choice;

        if (choice == 1){
            goto TEST;
        } else {
            cout << "Thank You For Your Time!" << endl;
            cout << "Have A Nice Day!" << endl << endl;
            cout << "Press Enter To Continue...";
            cin.ignore();
            cin.get();
            goto ENDTEST;
        }
    }
    ENDTEST:
    cout << endl;
}

void ChangePassword() {
    Error:
    string username, currentPassword, newPassword, confirmPassword;
    string name, email, accountType;
    int specialCharCount = 0;
    bool hasUpperCase = false, hasLowerCase = false, hasDigit = false;

    cout << "Enter Username: ";
    cin >> username;

    ifstream read(username + ".txt");
    if (!read.is_open()) {
        cerr << "Username Doesn't Exists. Please Try Again." << endl;
        goto Error;
    }

    // Read all data from the file
    getline(read, username);
    getline(read, currentPassword);
    getline(read, name);
    getline(read, email);
    getline(read, accountType);
    read.close();

    cout << "Enter Current Password: ";
    // Consider using getch() or a similar method to hide password input
    newPassword = "";
    char ch;
    while (true) {
        if (_kbhit()) {  // Check if a key is pressed
            ch = _getch();

            if (ch == 13) {  // Enter key pressed (password complete)
                cout << endl;
                break;
            } else if (ch == 8) {  // Backspace key pressed (remove last character)
                if (!newPassword.empty()) {
                    char deletedChar = newPassword.back();
                    newPassword.pop_back();
                    cout << "\b \b";  // Move cursor back and erase the asterisk
                    if (ispunct(deletedChar)) {
                        specialCharCount--; // Decrement only if it was a special character
                    }
                    // Update hasUpperCase flag after backspace
                    if (hasUpperCase && newPassword.empty()) {
                        hasUpperCase = false;
                    }
                }
            } else if (ch == ' '){
                if (isalnum(ch)){
                    newPassword.push_back(ch);
                }
            } else {
                newPassword.push_back(ch);
                cout << "*";  // Mask the character with an asterisk
            }
        }
    }
    cout << endl;

    if (currentPassword != newPassword) {
        cerr << "Incorrect Current Password. Please Try Again." << endl;
        goto Error;
    }

    // Get new password and confirmation
    cout << "Enter Your New Password (Must Be More Than " << MIN_PASSWORD_LENGTH << " Characters And Contain At Least One Uppercase Letter, Lowercase Letter, Digit, And Special Character)" << endl;
    cout << "New Password: ";
    while (newPassword.empty() || newPassword.length() < MIN_PASSWORD_LENGTH || specialCharCount < 1 || !hasUpperCase || !hasLowerCase || !hasDigit) {
        char ch;
        while (true) {
            if (_kbhit()) {  // Check if a key is pressed
            ch = _getch();

                if (ch == 13) {  // Enter key pressed (password complete)
                    cout << endl;
                    break;
                } else if (ch == 8) {  // Backspace key pressed (remove last character)
                    if (!newPassword.empty()) {
                        char deletedChar = newPassword.back();
                        newPassword.pop_back();
                        cout << "\b \b";  // Move cursor back and erase the asterisk                                if (ispunct(deletedChar)) {
                        specialCharCount--; // Decrement only if it was a special character
                    }
                    // Update hasUpperCase flag after backspace
                    if (hasUpperCase && newPassword.empty()) {
                        hasUpperCase = false;
                    }
                } else if (ch == ' ') {
                    if (isalnum(ch)) {
                        newPassword.push_back(ch);
                        // Don't echo the character for better security
                    }
                } else if (ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '^' || ch == '&' || ch == '*') {
                    // Check for specific special characters
                    specialCharCount++;
                    newPassword.push_back(ch);
                    cout << "*";  // Mask the character with an asterisk
                    // Don't echo the character for better security
                } else {
                    if (isupper(ch)) {
                        hasUpperCase = true;
                    } else if (islower(ch)) {
                        hasLowerCase = true;
                    } else if (isdigit(ch)) {
                        hasDigit = true;
                    }
                    newPassword.push_back(ch);
                    cout << "*";  // Mask the character with an asterisk
                }
            }
        } if (newPassword.length() < MIN_PASSWORD_LENGTH) {
            cout << "Password Is Too Short. Please Try Again." << endl;
            cout << "Password: ";
            newPassword.clear(); // Clear password to avoid accumulating invalid characters
        } else if (specialCharCount < 1) {
            cout << "Password Must Contain At Least One Of Special Characters. Please Try Again." << endl;
            cout << "Password: ";
            newPassword.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;                
            hasDigit = false;
        } else if (!hasUpperCase) {
            cout << "Password Must Contain At Least One Uppercase Letter. Please Try Again." << endl;
            cout << "Password: ";
            newPassword.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;
            hasDigit = false;
        } else if (!hasLowerCase) {
            cout << "Password Must Contain At Least One Lowercase Letter. Please Try Again." << endl;
            cout << "Password: ";
            newPassword.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;
            hasDigit = false;
        } else if (!hasDigit) {
            cout << "Password Must Contain At Least One Digit. Please Try Again.";
            cout << "Password: ";
            newPassword.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;
            hasDigit = false;
        }
    }
    PasswordError:
    cout << "Confirm Your New Password: ";
    confirmPassword = "";
    while (true) {
        if (_kbhit()) {  // Check if a key is pressed
            ch = _getch();

            if (ch == 13) {  // Enter key pressed (password complete)
                cout << endl;
                break;
            } else if (ch == 8) {  // Backspace key pressed (remove last character)
                if (!confirmPassword.empty()) {
                    confirmPassword.pop_back();
                    cout << "\b \b";  // Move cursor back and erase the asterisk
                }
            } else if (ch == ' ') {
                if (isalnum(ch)) {
                    confirmPassword.push_back(ch);
                }
            } else {
                confirmPassword.push_back(ch);
                cout << "*";  // Mask the character with an asterisk
            }
        }
    }
    cout << endl;

    if (newPassword != confirmPassword) {
        cerr << "Passwords Do Not Match. Please Try Again." << endl;
        goto PasswordError;
    }

    // Update password in the file
    ofstream write(username + ".txt");
    if (!write.is_open()) {
        cerr << "Username Doesn't Exists. Please Try Again." << endl << endl;
        goto PasswordError;
    }
    
    write << username << endl;
    write << newPassword << endl;
    write << name << endl;
    write << email << endl;
    write << accountType << endl;
    write.close();

    cout << endl << "Password Changed Successfully!" << endl;
}

void availableJobs(){
    string user_profile, pasword, resume;
    string test;
    int ans = 0, job_categories, job_listings, training_course;
    string pdfPath = "Calculus II.pdf", filePath = "You.mp4";
    cout << "JOB CATEGORIES" << endl;
    cout << "------------------------------------------------------------------" << endl;
    do {
        cout << "What Job Category Is Your Choice?" << endl;
        cout << "1. Education\t2. Design\t3. Customer Services" << endl;
        cout << "Enter Your Choice: ";
        cin >> job_categories;
        if (job_categories == 1){
            cout << "JOB LISTING" << endl;
            cout << "You Choosed Education!" << endl;
            do {
                cout << "What Job Do You Want To Apply?" << endl;
                cout << "1. Teacher\t2. Tutor\t3. Professor" << endl;
                cout << "Enter Your Choice: ";
                cin >> job_listings; 
                if (job_listings == 1) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else if (job_listings == 2) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else if (job_listings == 3) {
                    cout << "Do You Want To Join Training Course First?" << endl;
                    cout << "1. Yes\t" << endl;
                    cout << "2. No\t" << endl;
                    do {
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else {
                    cout << "Invalid Input! Please Enter Valid Option!" << endl;
                }
            } while (job_listings < 1 || job_listings > 3);
        } else if (job_categories == 2) {
            cout << "JOB LISTING" << endl;
            cout << "You Choosed Design!" << endl;
            do {
                cout << "What Job Do You Want To Apply?" << endl;
                cout << "1. Graphic Designer\t2. Animator\t3. Web Designer" << endl;
                cout << "Enter Your Choice: ";
                cin >> job_listings; 
                if (job_listings == 1) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else if (job_listings == 2) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else if (job_listings == 3) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else {
                    cout << "Invalid Input! Please Enter Valid Option!" << endl;
                }
            } while (job_listings < 1 || job_listings > 3);
        } else if (job_categories == 3) {
            cout << "JOB LISTING" << endl;
            cout << "You Choosed Customer Services!" << endl;
            do {
                cout << "What Job Do You Want To Apply?" << endl;
                cout << "1. Account Manager\t2. Live Chat Support Specialist\t3. Email Support Specialist" << endl;
                cout << "Enter Your Choice: ";
                cin >> job_listings; 
                if (job_listings == 1) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else if (job_listings == 2) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            system("pause");
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else if (job_listings == 3) {
                    do {
                        cout << "Do You Want To Join Training Course First?" << endl;
                        cout << "1. Yes\t" << endl;
                        cout << "2. No\t" << endl;
                        cout << "Enter Your Choice: ";
                        cin >> training_course;
                        if (training_course == 1) {
                            cout << "Here Are Some Lessons For You!" << endl;
                            cout << "file.pdf -- video.mp4" << endl;
                            // Use ShellExecute to open the PDF with default viewer
                            ShellExecute(GetDesktopWindow(), NULL, pdfPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            ShellExecute(GetDesktopWindow(), NULL, filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
                            cout << "In A Week, You Will Be An Online Test To Approve You To Become Our Worker.  Goodluck!" << endl;
                            sleep(60);
                            test_worker();
                            goto ENDTEST;
                        } else if (training_course == 2) {
                            cout << "Please Submit Your Resume Here!" << endl;
                            cout << "Resume (Copy & Paste Here!): ";
                            cin >> resume;
                            cout << "Your Resume Will Be Reviewed Between 12-24 Hours From Now. Please Be Patient!" << endl;
                            sleep(12);
                            cout << "Congratulations! Your Resume Has Been Approved!" << endl;
                            cout << "Now You Can Work With Us!" << endl;
                            goto ENDTEST;
                        } else {
                            cout << "Invalid Input! Please Enter Valid Option!" << endl;
                        }
                    } while (training_course != 1 || training_course != 2);
                } else {
                    cout << "Invalid Input! Please Enter Valid Option!" << endl;
                }
            } while (job_listings < 1 || job_listings > 3);
        } else {
            cout << "Invalid Input! Please Enter Valid Option!" << endl;
        }
    } while (job_categories < 1 || job_categories > 3);
    ENDTEST:
    cout << endl;
}

void ForgotPassword() {
    Error:
    string username, new_password, currentPassword, name, email, accountType;
    int specialCharCount = 0;
    bool hasUpperCase = false, hasLowerCase = false, hasDigit = false;
    cout << "Enter Username: ";
    cin >> username;
    ifstream read(username + ".txt"); // ifstream reads a file
    if (!read.is_open()) {
        cerr << "Username Doesn't Exists. Please Try Again." << endl;
        goto Error;
    }

    // Read all data from the file
    getline(read, username);
    getline(read, currentPassword);
    getline(read, name);
    getline(read, email);
    getline(read, accountType);
    read.close();

    // Get new password and confirmation
    cout << "Enter Your New Password (Must Be More Than " << MIN_PASSWORD_LENGTH << " Characters And Contain At Least One Uppercase Letter, Lowercase Letter, Digit, And Special Character)" << endl;
    cout << "New Password: ";
    while (new_password.empty() || new_password.length() < MIN_PASSWORD_LENGTH || specialCharCount < 1 || !hasUpperCase || !hasLowerCase || !hasDigit) {
        char ch;
        while (true) {
            if (_kbhit()) {  // Check if a key is pressed
            ch = _getch();

                if (ch == 13) {  // Enter key pressed (password complete)
                    cout << endl;
                    break;
                } else if (ch == 8) {  // Backspace key pressed (remove last character)
                    if (!new_password.empty()) {
                        char deletedChar = new_password.back();
                        new_password.pop_back();
                        cout << "\b \b";  // Move cursor back and erase the asterisk                                if (ispunct(deletedChar)) {
                        specialCharCount--; // Decrement only if it was a special character
                    }
                    // Update hasUpperCase flag after backspace
                    if (hasUpperCase && new_password.empty()) {
                        hasUpperCase = false;
                    }
                } else if (ch == ' ') {
                    if (isalnum(ch)) {
                        new_password.push_back(ch);
                        // Don't echo the character for better security
                    }
                } else if (ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '^' || ch == '&' || ch == '*') {
                    // Check for specific special characters
                    specialCharCount++;
                    new_password.push_back(ch);
                    cout << "*";  // Mask the character with an asterisk
                    // Don't echo the character for better security
                } else {
                    if (isupper(ch)) {
                        hasUpperCase = true;
                    } else if (islower(ch)) {
                        hasLowerCase = true;
                    } else if (isdigit(ch)) {
                        hasDigit = true;
                    }
                    new_password.push_back(ch);
                    cout << "*";  // Mask the character with an asterisk
                }
            }
        } if (new_password.length() < MIN_PASSWORD_LENGTH) {
            cout << "Password Is Too Short. Please Try Again." << endl;
            cout << "Password: ";
            new_password.clear(); // Clear password to avoid accumulating invalid characters
        } else if (specialCharCount < 1) {
            cout << "Password Must Contain At Least One Of Special Characters. Please Try Again." << endl;
            cout << "Password: ";
            new_password.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;                
            hasDigit = false;
        } else if (!hasUpperCase) {
            cout << "Password Must Contain At Least One Uppercase Letter. Please Try Again." << endl;
            cout << "Password: ";
            new_password.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;
            hasDigit = false;
        } else if (!hasLowerCase) {
            cout << "Password Must Contain At Least One Lowercase Letter. Please Try Again." << endl;
            cout << "Password: ";
            new_password.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;
            hasDigit = false;
        } else if (!hasDigit) {
            cout << "Password Must Contain At Least One Digit. Please Try Again.";
            cout << "Password: ";
            new_password.clear(); // Clear password to avoid accumulating invalid characters
            specialCharCount = 0; // Reset special character count
            hasUpperCase = false;
            hasLowerCase = false;
            hasDigit = false;
        }
    }
    string confirmPassword;
    PasswordError:
    cout << "Confirm Your New Password: ";
    confirmPassword = "";
    while (true) {
        char ch;
        if (_kbhit()) {  // Check if a key is pressed
            ch = _getch();

            if (ch == 13) {  // Enter key pressed (password complete)
                cout << endl;
                break;
            } else if (ch == 8) {  // Backspace key pressed (remove last character)
                if (!confirmPassword.empty()) {
                    confirmPassword.pop_back();
                    cout << "\b \b";  // Move cursor back and erase the asterisk
                }
            } else if (ch == ' ') {
                if (isalnum(ch)) {
                    confirmPassword.push_back(ch);
                }
            } else {
                confirmPassword.push_back(ch);
                cout << "*";  // Mask the character with an asterisk
            }
        }
    }
    cout << endl;

    if (new_password != confirmPassword) {
        cerr << "Passwords Do Not Match. Please Try Again." << endl;
        goto PasswordError;
    }

    // Update password in the file
    ofstream write(username + ".txt");
    if (!write.is_open()) {
        cerr << "Username Doesn't Exists. Please Try Again." << endl << endl;
        goto PasswordError;
    }
    write << username << endl;
    write << new_password << endl;
    write << name << endl;
    write << email << endl;
    write << accountType << endl;
    write.close();

    cout << endl << "Password Changed Successfully!" << endl;
}

int main() {
    string username;
    int choice;
    Loop:
    do {
        cout << endl;
        cout << "CareerCompass: Job Seeker App" << endl;
        cout << "Main Menu: " << endl;
        cout << "------------------------------------------------------------------" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Forgot Password" << endl; // Last Project Maybe
        cout << "0. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << endl;
            cout << "REGISTER" << endl;
            cout << "------------------------------------------------------------------" << endl;
            REGISTER:
            string username, password, name, email, accountType;
            int specialCharCount = 0;
            bool hasUpperCase = false, hasLowerCase = false, hasDigit = false;
            cout << "Enter Your Username (Must Be Between " << MIN_USERNAME_LENGTH << " To " << MAX_USERNAME_LENGTH << " Characters): ";
            while (username.empty() || username.length() < MIN_USERNAME_LENGTH || username.length() > MAX_USERNAME_LENGTH) {
                cin >> username;
                if (userProfiles.count(username) > 0) {
                    cerr << "Username Already Exists. Please Choose A Different One." << endl;
                    goto REGISTER;
                }
                if (username.length() < MIN_USERNAME_LENGTH) {
                    cout << "Username Is Too Short. Please Try Again: ";
                } else if (username.length() > MAX_USERNAME_LENGTH) {
                    cout << "Username Is Too Long. Please Try Again: ";
                }
            }
            cout << "Enter Your Password (Must Be More Than " << MIN_PASSWORD_LENGTH << " Characters and contain at least one uppercase letter, lowercase letter, digit, and special character)" << endl;
            cout << "Password: ";
            while (password.empty() || password.length() < MIN_PASSWORD_LENGTH || specialCharCount < 1 || !hasUpperCase || !hasLowerCase || !hasDigit) {
                char ch;
                while (true) {
                    if (_kbhit()) {  // Check if a key is pressed
                        ch = _getch();

                        if (ch == 13) {  // Enter key pressed (password complete)
                            cout << endl;
                            break;
                        } else if (ch == 8) {  // Backspace key pressed (remove last character)
                            if (!password.empty()) {
                                char deletedChar = password.back();
                                password.pop_back();
                                cout << "\b \b";  // Move cursor back and erase the asterisk
                                if (ispunct(deletedChar)) {
                                    specialCharCount--; // Decrement only if it was a special character
                                }
                                // Update hasUpperCase flag after backspace
                                if (hasUpperCase && password.empty()) {
                                    hasUpperCase = false;
                                }
                            }
                        } else if (ch == ' ') {
                            if (isalnum(ch)) {
                                password.push_back(ch);
                                // Don't echo the character for better security
                            }
                        } else if (ch == '!' || ch == '@' || ch == '#' || ch == '$' || ch == '%' || ch == '^' || ch == '&' || ch == '*') {
                            // Check for specific special characters
                            specialCharCount++;
                            password.push_back(ch);
                            cout << "*";  // Mask the character with an asterisk
                            // Don't echo the character for better security
                        } else {
                            if (isupper(ch)) {
                                hasUpperCase = true;
                            } else if (islower(ch)) {
                                hasLowerCase = true;
                            } else if (isdigit(ch)) {
                                hasDigit = true;
                            }
                            password.push_back(ch);
                            cout << "*";  // Mask the character with an asterisk
                        }
                    }
                } if (password.length() < MIN_PASSWORD_LENGTH) {
                    cout << "Password Is Too Short. Please Try Again." << endl;
                    cout << "Password: ";
                    password.clear(); // Clear password to avoid accumulating invalid characters
                } else if (specialCharCount < 1) {
                    cout << "Password Must Contain At Least One Of Special Characters. Please Try Again." << endl;
                    cout << "Password: ";
                    password.clear(); // Clear password to avoid accumulating invalid characters
                    specialCharCount = 0; // Reset special character count
                    hasUpperCase = false;
                    hasLowerCase = false;
                    hasDigit = false;
                } else if (!hasUpperCase) {
                    cout << "Password Must Contain At Least One Uppercase Letter. Please Try Again." << endl;
                    cout << "Password: ";
                    password.clear(); // Clear password to avoid accumulating invalid characters
                    specialCharCount = 0; // Reset special character count
                    hasUpperCase = false;
                    hasLowerCase = false;
                    hasDigit = false;
                } else if (!hasLowerCase) {
                    cout << "Password Must Contain At Least One Lowercase Letter. Please Try Again." << endl;
                    cout << "Password: ";
                    password.clear(); // Clear password to avoid accumulating invalid characters
                    specialCharCount = 0; // Reset special character count
                    hasUpperCase = false;
                    hasLowerCase = false;
                    hasDigit = false;
                } else if (!hasDigit) {
                    cout << "Password Must Contain At Least One Digit. Please Try Again.";
                    cout << "Password: ";
                    password.clear(); // Clear password to avoid accumulating invalid characters
                    specialCharCount = 0; // Reset special character count
                    hasUpperCase = false;
                    hasLowerCase = false;
                    hasDigit = false;
                }
            }

            cout << "Enter Your Full Name: ";
            cin.ignore(); // Ni Untuk Mengelakkan function getline terus masuk ke cout << email
            getline(cin, name); 

            cout << "Enter Your Email: ";
            cin >> email;

            int option;
            do {
                cout << "Select Account Type (1 - Job Seeker, 2 - Employer): ";
                cin >> option;
                if (option == 1) {
                    accountType = "Job Seeker";
                } else if (option == 2) {
                    accountType = "Employer";
                } else {
                    cerr << "Invalid Choice. Please Enter A Valid Option." << endl;
                }
            } while (option != 1 && option != 2);
            cout << endl;

            ofstream file;
            file.open(username + ".txt");
            file << username << endl << password << endl << name << endl << email << endl << accountType << endl;
            file.close();
            cout << "You Have Registered Successfully!" << endl;
            cout << "Now Please Login With Your Account." << endl;
        } else if (choice == 2) {
            cout << endl;
            cout << "LOGIN" << endl;
            cout << "------------------------------------------------------------------" << endl;
            Login:
            bool status = IsLoggedIn();
            if (!status){
                cout << endl;
                cout << "Invalid Login! Try Checking Your Username And Password Again." << endl;
                goto Login;
            } else {
                cout << endl;
                cout << "Successfully Logged In!" << endl;
                cout << endl;

                int choiceTwo, choiceThree, choiceFour;
                cout << "WELCOME BACK!"  << username << endl;
                do {
                    DASHBOARD:
                    cout << endl;
                    cout << "DASHBOARD" << endl;
                    cout << "------------------------------------------------------------------" << endl;
                    cout << "1. Available Jobs" << endl;
                    cout << "2. Payoff" << endl;
                    cout << "3. Accounts" << endl;
                    cout << "4. Settings" << endl; 
                    cout << "5. About Us" << endl;
                    cout << "0. Log Out" << endl;
                    cout << "Enter Your Choice: ";
                    cin >> choiceTwo;
                    cout << endl;
                
                    if (choiceTwo == 0){
                        cout << "You Have Successfully Logged Out!" << endl;
                        goto Loop;
                    } else if (choiceTwo == 1) {
                        availableJobs();
                        goto DASHBOARD;
                    } else if (choiceTwo == 2) {
                        do {
                            PAYOFF:
                            cout << endl;
                            cout << "PAYOFF" << endl;
                            cout << "------------------------------------------------------------------" << endl;
                            cout << "1. Payoff Details" << endl;
                            cout << "2. Modify Payoff" << endl;
                            cout << "0. Back" << endl;
                            cout << "Enter Your Choice: ";
                            cin >> choiceThree;
                            cout << endl;

                            if (choiceThree == 1) {
                                cout << "PAYOFF DETAILS" << endl;
                                cout << "------------------------------------------------------------------" << endl;
                                double amount = 100.0;
                                string currency = "MYR";
                                string description = "Salary Payment";
                                cout << "Amount: " << amount << endl;
                                cout << "Currency: " << currency << endl;
                                cout << "Description: " << description << endl << endl;
                                cout << "0. Back" << endl;
                                do {
                                    cout << "Enter Your Choice: ";
                                    cin >> choiceFour;
                                    if (choiceFour == 0) {
                                        goto PAYOFF;
                                    } else {
                                        cout << "Invalid Choice! Please Enter A Valid Option.";
                                        cout << endl;
                                    }
                                } while (choiceFour != 0);
                            } else if (choiceThree == 2) {
                                cout << "MODIFY PAYOUT" << endl;
                                cout << "------------------------------------------------------------------" << endl;
                                cout << "Enter Payout Category: ";
                                string newCategory;
                                cin.ignore(); // Consume newline character
                                getline(cin, newCategory);
                                cout << endl;
                                cout << "0. Back" << endl;
                                do {
                                    cout << "Enter Your Choice: ";
                                    cin >> choiceFour;
                                    if (choiceFour == 0) {
                                        goto PAYOFF;
                                    } else {
                                        cout << "Invalid Choice! Please Enter A Valid Option.";
                                        cout << endl;
                                    }
                                } while (choiceFour != 0);
                            } else if (choiceThree == 0) {
                                goto DASHBOARD;
                            } else {
                                cout << "Invalid Choice! Please Enter A Valid Option.";
                                cout << endl;
                            }
                        } while (choiceThree < 0 || choiceThree > 2);
                    } else if (choiceTwo == 3) {
                        do {
                            cout << "ACCOUNTS" << endl;
                            cout << "------------------------------------------------------------------" << endl;
                            cout << "1. Account Details" << endl;
                            cout << "0. Back" << endl;
                            cout << "Enter Your Choice: ";
                            cin >> choiceThree;
                            cout << endl;

                            if (choiceThree == 1) {
                                do {
                                    cout << "ACCOUNT DETAILS" << endl;
                                    cout << "------------------------------------------------------------------" << endl;
                                    cout << "1. Payment History" << endl;
                                    cout << "0. Back" << endl;
                                    cout << "Enter Your Choice: ";
                                    cin >> choiceFour;

                                    if (choiceFour == 1) {
                                        cout << "PAYMENT HISTORY" << endl;
                                        cout << "------------------------------------------------------------------" << endl;
                                        cout << "0. Back" << endl;
                                        cout << "Enter Your Choice: ";
                                        int choiceFive;
                                        do {
                                            cin >> choiceFive;
                                            if (choiceFive == 0){
                                                cout << endl;
                                                system("exit");
                                            } else {
                                                cout << "Invalid Choice! Please Enter A Valid Option.";
                                                cout << endl;
                                            }
                                        } while (choiceFive != 0);
                                    } else if (choiceFour == 0) {
                                        cout << endl;
                                        system("exit");
                                    } else {
                                        cout << "Invalid Choice! Please Enter A Valid Option.";
                                        cout << endl;
                                    }
                                } while (choiceFour != 0);
                            } else if (choiceThree == 0) {
                                goto DASHBOARD;
                            } else {
                                cout << "Invalid Choice! Please Enter A Valid Option.";
                                cout << endl;
                            }
                        } while (choiceThree != 0);
                    } else if (choiceTwo == 4) {
                        SETTINGS:
                        do {
                            cout << "SETTINGS" << endl;
                            cout << "------------------------------------------------------------------" << endl;
                            cout << "1. Change Password" << endl;
                            cout << "2. Profile" << endl;
                            cout << "0. Back" << endl;
                            cout << "Enter Your Choice: ";
                            cin >> choiceThree;
                            cout << endl;

                            if (choiceThree == 1) {
                                cout << "CHANGE PASSWORD" << endl;
                                cout << "------------------------------------------------------------------" << endl;
                                ChangePassword();
                                cout << endl;
                                goto SETTINGS;
                            } else if (choiceThree == 2) {
                                cout << "PROFILE" << endl;
                                cout << "------------------------------------------------------------------" << endl;
                                //
                            } else if (choiceThree == 0) {
                                goto DASHBOARD;
                            } else {
                                cout << "Invalid Choice! Please Enter A Valid Option.";
                                cout << endl;
                            }
                        } while (choiceThree < 0 || choiceThree > 2);
                    } else if (choiceTwo == 5) {
                        do {
                            cout << "ABOUT US" << endl;
                            cout << "------------------------------------------------------------------" << endl;
                            cout << "..." << endl;
                            //
                            cout << "0. Back" << endl;
                            cout << "Enter Your Choice: ";
                            cin >> choiceThree;

                            if (choiceThree == 0){
                                goto DASHBOARD;
                            } else {
                                cout << "Invalid Choice! Please Enter A Valid Option." << endl;
                                cout << endl;
                            }
                        } while (choiceThree != 0);
                    } else {
                        cout << "Invalid Choice! Please Enter A Valid Option." << endl;
                        cout << endl;
                    }
                } while (choiceTwo < 0 || choiceTwo > 5);
            }
        } else if (choice == 3) {
            ForgotPassword();
            cout << endl;
        } else if (choice == 0) {
            cout << "Exiting . . ." << endl;
            return 1;
        } else {
            cout << "Invalid Choice! Please Enter A Valid Option." << endl;
            cout << endl;
        }
    } while (choice != 0);
    return 0;
}