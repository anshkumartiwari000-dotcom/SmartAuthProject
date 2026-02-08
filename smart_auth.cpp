#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
using namespace std;

string hashPassword(string password){
unsigned long hash = 5381;
for(char c : password){
    hash = ((hash << 5) + hash) + c;

}
return to_string(hash);
}
struct User {
    string username;
    string passwordHash;
    string role;
    int failedAttempts;
    bool locked;

};

vector<User> loadUser(){
    vector<User> users;
    ifstream file("users.txt");
    string line;

    while (getline(file, line)){
        stringstream ss(line);
        User u;
        string lockedStr;

        getline(ss, u.username, '|');
        getline(ss, u.passwordHash, '|');
        getline(ss, u.role, '|');
        ss >> u.failedAttempts;
        ss.ignore();
        ss >> lockedStr;

        u.locked = (lockedStr == "1");
        users.push_back(u);
    }
    file.close();
    return users;
}
void saveUsers(vector<User>& users) {
    ofstream file("users.txt");
    for (auto& u : users){
        file << u.username << "|"
             << u.passwordHash << "|"
             << u.role << "|"
             << u.failedAttempts << "|"
             << (u.locked ? 1 : 0) << endl;
    
    }
    file.close();
}
int findUser(vector<User>& users, string username) {
    for (int i=0; i < users.size(); i++) {
        if (users[i].username == username)
            return i;
    }
     return -1;
}

void login(vector<User>& users) {
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout<< "password:";
    cin >> password;

    int index = findUser(users, username);
    if (index == -1) {
        cout << "User not found.\n";
        return ;
    }

    User& u = users[index];
     if (u.locked) {
        cout << "Account is locked.\n";
        return;
     }

     if (u.passwordHash == hashPassword(password)) {
        cout << "Login successful. Role: " << u.role << endl;
        u.failedAttempts = 0;
     } else {
        u.failedAttempts++;
        cout << "Wrong password. \n";

        if (u.failedAttempts >= 3) {
            u.locked = true;
            cout << "Account locked after 3 faild attempts.\n";
        }

     }
     saveUsers(users);
    }
    void unlockedUser(vector<User>& users) {
        string username;
        cout << "Enter username to unlock: ";
        cin >> username;

        int index = findUser(users, username);
        if (index == -1) {
            cout << "User not found. \n";
            return;
    }
     users[index].locked = false;
     users[index].failedAttempts = 0;
     saveUsers(users);
     cout << "User unlocked successfully.\n";
}
 int main() {
    vector<User> users = loadUser();
    int choice;

    do {
        cout << "\n1. Login\n2. Unlock User (Admin)\n3. Exit\nChoice: ";
        cin >> choice;
         switch (choice) {
        case 1:
            login(users);
            break;
        case 2:
            unlockedUser(users);
            break;
        case 3:
            cout << "Goodbye\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 3);
    return 0;
}