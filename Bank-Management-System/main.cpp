// my solution 98% 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// ==========================================
// CONSTANTS & GLOBALS
// ==========================================
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

// ==========================================
// ENUMS & STRUCTS
// ==========================================
enum class enMainMenueOptions {
	eShowClientList = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eShowTransactionsMenue = 6,
	eManageUsers = 7,
	eExit = 8
};

enum class enTransactionsMenueOptions {
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
	eMainMenue = 4
};

enum class enManageUsersMenueOptions {
	eListUsers = 1,
	eAddNewUser = 2,
	eDeleteUser = 3,
	eUpdateUser = 4,
	eFindUser = 5,
	eMainMenue = 6
};

enum enMainMenuePermissions {
	eAll = -1,
	pListClients = 1,
	pAddNewClient = 2,
	pDeleteClient = 4,
	pUpdateClients = 8,
	pFindClient = 16,
	pTranactions = 32,
	pManageUsers = 64
};

struct sClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

struct stUser {
	string UserName;
	string Password;
	int Permissions;
	bool MarkForDelete = false;
};

stUser CurrentUser;

// ==========================================
// FORWARD DECLARATIONS
// ==========================================

// 1. Entry Point & Authentication
void Login();
bool LoadUserInfo(string Username, string Password);

// 2. Main Menu & Navigation Flow
void ShowMainMenue();
void MainMenue(enMainMenueOptions Option);
void GoBackToMainMenu();

// 3. Client Management Screens & Actions
void ShowClientListScreen();
void AddNewClientScreen();
void DeleteClientScreen();
void UpdateClientScreen();
void FindClientScreen();
void AddClients();
void AddNewClient();
bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients);
bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients);
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients);

// 4. Transactions Screens & Actions
void ShowTransactionsMenue();
void TransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOption);
void GoBackToTransactionsMenue();
void ShowDepositScreen();
void ShowWithdrawScreen();
void ShowBalancesList();
bool DepositAmount(string AccountNumber, vector<sClient>& vClients);
bool Withdraw(string AccountNumber, vector<sClient>& vClients);

// 5. User Management Screens & Actions
void ShowManageUsersMenue();
void ManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption);
void GoBackToManageUsersMenue();
void ShowListUsersScreen();
void ShowAddNewUserScreen();
void ShowDeleteUserScreen();
void ShowUpdateUserScreen();
void ShowFindUserScreen();
void ShowAllUsersScreen();
void AddNewUsers();
void AddNewUser();
bool UpdateUserByUsername(string Username, vector<stUser>& vUsers);
bool DeleteUserByUsername(string Username, vector<stUser>& vUsers);
bool MarkUserForDeleteByUsername(string Username, vector<stUser>& vUsers);

// 6. Display & Printing Utilities
void PrintAllClientsData(vector<sClient> vClientsData);
void PrintClientRecord(sClient Client);
void PrintClientCard(sClient Client);
void PrintUserCard(stUser User);
void PrintUserRecordLine(stUser User);
void ShowAccessDeniedMessage();
void ShowEndScreen();

// 7. File & Database Operations
bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client);
bool FindUserByUsername(string Username, vector<stUser> vUsers, stUser& User);
bool FindUserByUsernameAndPassword(string Username, string Password, stUser& User);
vector<sClient> LoadCleintsDataFromFile(string FileName);
vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients);
vector<stUser> LoadUsersDataFromFile(string FileName);
vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser> vUsers);
void AddDataLineToFile(string FileName, string sDataLine);

// 8. Input Reading & Validation Utilities
short ReadNumber(short From, short To);
string ReadClientAccountNumber();
string ReadUserName();
sClient ReadNewClient();
sClient ChangeClientRecord(string AccountNumber);
stUser ReadNewUser();
stUser ChangeUserRecord(string Username);
int ReadPermissionsToSet();
double ReadDepositAmount();
double ReadWithdraw();
bool ClientExistsByAccountNumber(string AccountNumber, string FileName);
bool UserExistsByUsername(string Username, string FileName);
bool CheckAccessPermission(enMainMenuePermissions Permission);

// 9. Data Conversion & String Utilities
sClient ConvertLineToRecord(string Line, string Separator = "#//#");
string ConvertRecordToLine(sClient Client, string Seperator = "#//#");
stUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#");
string ConvertUserRecordToLine(stUser User, string Seperator = "#//#");
vector<string> SplitString(string S1, string Delim);


// ==========================================
// 1. ENTRY POINT & AUTHENTICATION
// ==========================================

int main() {

	Login();

	system("pause>0");
	return 0;
}

void Login() {

	bool LoginFaild = false;
	string Username, Password;

	do {
		system("cls");

		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";

		if (LoginFaild) {
			cout << "Invlaid Username/Password!\n";
		}

		cout << "Enter Username? ";
		cin >> Username;

		cout << "Enter Password? ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenue();
}

bool LoadUserInfo(string Username, string Password) {
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

// ==========================================
// 2. MAIN MENU & NAVIGATION FLOW
// ==========================================

void ShowMainMenue() {
	system("cls");
	cout << "============================================\n";
	cout << "\t\t Main Menu Screen \n";
	cout << "============================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "============================================\n";
	cout << "Choose what do you want to do? [1 to 8]? ";
	MainMenue((enMainMenueOptions)ReadNumber(1, 8));
}

void MainMenue(enMainMenueOptions Option) {

	switch (Option) {
	case enMainMenueOptions::eShowClientList:
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;

	case enMainMenueOptions::eAddNewClient:
		system("cls");
		AddNewClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenueOptions::eDeleteClient:
		system("cls");
		DeleteClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenueOptions::eUpdateClient:
		system("cls");
		UpdateClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenueOptions::eFindClient:
		system("cls");
		FindClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenueOptions::eShowTransactionsMenue:
		system("cls");
		ShowTransactionsMenue();
		break;

	case enMainMenueOptions::eManageUsers:
		system("cls");
		ShowManageUsersMenue();
		break;

	case enMainMenueOptions::eExit:
		system("cls");
		//ShowEndScreen();
		Login();
		break;
	}
}

void GoBackToMainMenu() {

	cout << "Press any key to go back to Main Menu...";
	system("pause > nul");
	ShowMainMenue();
}

// ==========================================
// 3. CLIENT MANAGEMENT SCREENS & ACTIONS
// ==========================================

void ShowClientListScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	PrintAllClientsData(vClients);
}

void AddNewClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	AddClients();
}

void DeleteClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "============================================\n";
	cout << "\t Delete Client Screen \n";
	cout << "============================================\n";

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void UpdateClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "============================================\n";
	cout << "\t Update Client Screen \n";
	cout << "============================================\n";

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void FindClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "============================================\n";
	cout << "\t Find Client Screen \n";
	cout << "============================================\n";

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
	}
}

void AddClients() {
	char AddMore = 'Y';
	do {
		system("cls");
		cout << "============================================\n";
		cout << "\t Add New Client Screen \n";
		cout << "============================================\n";
		cout << "Adding New Client :\n\n";

		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void AddNewClient() {

	sClient Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {

	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {
			for (sClient& C : vClients) {
				if (C.AccountNumber == AccountNumber) {
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient updated Successfully.";
			return true;
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {

	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(ClientsFileName, vClients);
			// Refresh Clients 
			vClients = LoadCleintsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients) {

	for (sClient& C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

// ==========================================
// 4. TRANSACTIONS SCREENS & ACTIONS
// ==========================================

void ShowTransactionsMenue() {

	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenu();
		return;
	}

	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";
	cout << "Choose what do you want to do? [1 to 4]? ";
	TransactionsMenueOption((enTransactionsMenueOptions)ReadNumber(1, 4));
}

void TransactionsMenueOption(enTransactionsMenueOptions TransactionsOption) {
	switch (TransactionsOption) {
	case enTransactionsMenueOptions::eDeposit: {
		system("cls");
		ShowDepositScreen();
		break;
	}
	case enTransactionsMenueOptions::eWithdraw: {
		system("cls");
		ShowWithdrawScreen();
		break;
	}
	case enTransactionsMenueOptions::eTotalBalances: {
		system("cls");
		ShowBalancesList();
		GoBackToTransactionsMenue();
		break;
	}
	case enTransactionsMenueOptions::eMainMenue: {
		system("cls");
		ShowMainMenue();
		break;
	}
	}
}

void GoBackToTransactionsMenue() {
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowTransactionsMenue();
}

void ShowDepositScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!ClientExistsByAccountNumber(AccountNumber, ClientsFileName)) {
		cout << "\nClient with [" << AccountNumber << "] dose not exists.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	DepositAmount(AccountNumber, vClients);
	system("pause>0");
	ShowTransactionsMenue();
}

void ShowWithdrawScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!ClientExistsByAccountNumber(AccountNumber, ClientsFileName)) {
		cout << "\nClient with [" << AccountNumber << "] dose not exists.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	Withdraw(AccountNumber, vClients);
	system("pause>0");
	ShowTransactionsMenue();
}

void ShowBalancesList() {

	vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	double TotalBalance = 0;

	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else {
		for (sClient Client : vClients) {
			cout << "| " << setw(15) << left << Client.AccountNumber;
			cout << "| " << setw(40) << left << Client.Name;
			cout << "| " << setw(12) << left << Client.AccountBalance;
			TotalBalance = TotalBalance + Client.AccountBalance;
			cout << endl;
		}
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\t\t\t\t Totale Balances = " << TotalBalance << endl;
}

bool DepositAmount(string AccountNumber, vector<sClient>& vClients) {

	sClient Client;
	sClient ClNewBalance;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		double Amount = ReadDepositAmount();

		cout << "\n\nAre you sure you want perform this transaction? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			for (sClient& C : vClients) {
				if (C.AccountNumber == AccountNumber) {
					C.AccountBalance = C.AccountBalance + Amount;
					ClNewBalance.AccountBalance = C.AccountBalance;
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nDeposit done Successfully.";
			cout << "\nNew Balance is : " << ClNewBalance.AccountBalance;
			return true;
		}
		else {
			ShowTransactionsMenue();
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
	return false;
}

bool Withdraw(string AccountNumber, vector<sClient>& vClients) {

	sClient Client;
	sClient ClNewBalance;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		double Amount = ReadWithdraw();

		while (Amount > Client.AccountBalance) {
			cout << "\nAmount Exceeds the balance,you withdraw up to : " << Client.AccountBalance << endl;
			Amount = ReadWithdraw();
		}

		cout << "\n\nAre you sure you want perform this transaction? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			for (sClient& C : vClients) {
				if (C.AccountNumber == AccountNumber) {
					C.AccountBalance = C.AccountBalance + (-Amount);
					ClNewBalance.AccountBalance = C.AccountBalance;
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nDeposit done Successfully.";
			cout << "\nNew Balance is : " << ClNewBalance.AccountBalance;
			return true;
		}
		else {
			ShowTransactionsMenue();
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
	return false;
}

// ==========================================
// 5. USER MANAGEMENT SCREENS & ACTIONS
// ==========================================

void ShowManageUsersMenue() {

	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers)) {
		ShowAccessDeniedMessage();
		GoBackToMainMenu();
		return;
	}

	system("cls");
	cout << "===========================================\n";
	cout << "\t\tManage Users Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "===========================================\n";
	cout << "Choose what do you want to do? [1 to 6]? ";
	ManageUsersMenueOption((enManageUsersMenueOptions)ReadNumber(1, 6));
}

void ManageUsersMenueOption(enManageUsersMenueOptions Option) {
	switch (Option) {
	case enManageUsersMenueOptions::eListUsers: {
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUsersMenue();
		break;
	}
	case enManageUsersMenueOptions::eAddNewUser: {
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUsersMenue();
		break;
	}
	case enManageUsersMenueOptions::eDeleteUser: {
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenue();
		break;
	}
	case enManageUsersMenueOptions::eUpdateUser: {
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersMenue();
		break;
	}
	case enManageUsersMenueOptions::eFindUser: {
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenue();
		break;
	}
	case enManageUsersMenueOptions::eMainMenue: {
		ShowMainMenue();
		break;
	}
	}
}

void GoBackToManageUsersMenue() {
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowManageUsersMenue();
}

void ShowListUsersScreen() {

	ShowAllUsersScreen();
}

void ShowAddNewUserScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tAdd New User Screen";
	cout << "\n-----------------------------------\n";
	AddNewUsers();
}

void ShowDeleteUserScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tDelete Users Screen";
	cout << "\n-----------------------------------\n";

	vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	DeleteUserByUsername(Username, vUsers);
}

void ShowUpdateUserScreen() {

	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Users Screen";
	cout << "\n-----------------------------------\n";

	vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	UpdateUserByUsername(Username, vUsers);
}

void ShowFindUserScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-----------------------------------\n";

	vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	stUser User;
	string Username = ReadUserName();

	if (FindUserByUsername(Username, vUsers, User))
		PrintUserCard(User);
	else
		cout << "\nUser with Username [" << Username << "] is not found!";
}

void ShowAllUsersScreen() {
	vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(40) << "Permissions";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vUsers.size() == 0) {
		cout << "\t\t\t\tNo Users Available In the System!";
	}
	else {
		for (stUser User : vUsers) {
			PrintUserRecordLine(User);
			cout << endl;
		}
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void AddNewUsers() {
	char AddMore = 'Y';
	do {
		cout << "Adding New User:\n\n";
		AddNewUser();
		cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void AddNewUser() {
	stUser User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

bool UpdateUserByUsername(string Username, vector<stUser>& vUsers) {
	stUser User;
	char Answer = 'n';

	if (FindUserByUsername(Username, vUsers, User)) {
		PrintUserCard(User);
		cout << "\n\nAre you sure you want update this User? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			for (stUser& U : vUsers) {
				if (U.UserName == Username) {
					U = ChangeUserRecord(Username);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);
			cout << "\n\nUser Updated Successfully.";
			return true;
		}
	}
	else {
		cout << "\nUser with Account Number (" << Username << ") is Not Found!";
		return false;
	}
	return false;
}

bool DeleteUserByUsername(string Username, vector<stUser>& vUsers) {
	if (Username == "Admin") {
		cout << "\n\nYou cannot Delete This User.";
		return false;
	}

	stUser User;
	char Answer = 'n';

	if (FindUserByUsername(Username, vUsers, User)) {
		PrintUserCard(User);
		cout << "\n\nAre you sure you want delete this User? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			MarkUserForDeleteByUsername(Username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);
			// Refresh Users
			vUsers = LoadUsersDataFromFile(UsersFileName);
			cout << "\n\nUser Deleted Successfully.";
			return true;
		}
	}
	else {
		cout << "\nUser with Username (" << Username << ") is Not Found!";
		return false;
	}
	return false;
}

bool MarkUserForDeleteByUsername(string Username, vector<stUser>& vUsers) {
	for (stUser& U : vUsers) {
		if (U.UserName == Username) {
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}


// ==========================================
// 6. DISPLAY & PRINTING UTILITIES
// ==========================================

void PrintAllClientsData(vector<sClient> vClientsData) {

	cout << "\n\t\t\t\tClients List (" << vClientsData.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "PIN Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClientsData.size() == 0) {
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else {
		for (sClient Client : vClientsData) {
			PrintClientRecord(Client);
			cout << endl;
		}
	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void PrintClientRecord(sClient Client) {

	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

void PrintClientCard(sClient Client) {

	cout << "\n\n---------------------------------\n";
	cout << "Client details :\n";
	cout << "\n Account Number : " << Client.AccountNumber;
	cout << "\n PinCode : " << Client.PinCode;
	cout << "\n Name : " << Client.Name;
	cout << "\n Phone : " << Client.Phone;
	cout << "\n Account Balance : " << Client.AccountBalance;
	cout << "\n---------------------------------\n";
}

void PrintUserCard(stUser User) {

	cout << "\nThe following are the user details:\n";
	cout << "-----------------------------------";
	cout << "\nUsername    : " << User.UserName;
	cout << "\nPassword    : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;
	cout << "\n-----------------------------------\n";
}

void PrintUserRecordLine(stUser User) {

	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(40) << left << User.Permissions;
}

void ShowAccessDeniedMessage() {
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}

void ShowEndScreen() {
	cout << "----------------------------------\n";
	cout << "\t Program Ends :-) \n";
	cout << "----------------------------------\n";
}

// ==========================================
// 7. FILE & DATABASE OPERATIONS
// ==========================================

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client) {
	for (sClient C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUsername(string Username, vector<stUser> vUsers, stUser& User) {
	for (stUser U : vUsers) {
		if (U.UserName == Username) {
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, stUser& User) {
	vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	for (stUser U : vUsers) {
		if (U.UserName == Username && U.Password == Password) {
			User = U;
			return true;
		}
	}
	return false;
}

vector<sClient> LoadCleintsDataFromFile(string FileName) {
	fstream MyFile;
	vector<sClient> vClients;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {
			vClients.push_back(ConvertLineToRecord(Line));
		}
		MyFile.close();
	}
	return vClients;
}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out); // overwrite 

	string DataLine;
	if (MyFile.is_open()) {
		for (sClient C : vClients) {
			if (C.MarkForDelete == false) {
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

vector<stUser> LoadUsersDataFromFile(string FileName) {
	vector<stUser> vUsers;
	fstream MyFile;
	MyFile.open(FileName, ios::in); // read Mode

	if (MyFile.is_open()) {
		string Line;
		stUser User;
		while (getline(MyFile, Line)) {
			User = ConvertUserLinetoRecord(Line);
			vUsers.push_back(User);
		}
		MyFile.close();
	}
	return vUsers;
}

vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser> vUsers) {
	fstream MyFile;
	MyFile.open(FileName, ios::out); // overwrite

	string DataLine;
	if (MyFile.is_open()) {
		for (stUser U : vUsers) {
			if (U.MarkForDelete == false) {
				DataLine = ConvertUserRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUsers;
}

void AddDataLineToFile(string FileName, string sDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {
		MyFile << sDataLine << endl;
		MyFile.close();
	}
}

// ==========================================
// 8. INPUT READING & VALIDATION UTILITIES
// ==========================================

short ReadNumber(short From, short To) {
	int Number = 0;
	while (true) {
		cin >> Number;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Please enter numbers only.\n";
			continue;
		}

		if (Number < From || Number > To) {
			cout << "Error: Please enter a number between "
				<< From << " and " << To << ": ";
			continue;
		}

		if (Number <= 0) {
			cout << "Error: Number must be greater than zero.\n";
			continue;
		}

		return Number;
	}
}

string ReadClientAccountNumber() {
	string AccountNumber;
	cout << "\n Please enter Account Number ? ";
	cin >> AccountNumber;
	return AccountNumber;
}

string ReadUserName() {
	string Username = "";
	cout << "\nPlease enter Username? ";
	cin >> Username;
	return Username;
}

sClient ReadNewClient() {
	sClient Client;
	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode ? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;

	return Client;
}

sClient ChangeClientRecord(string AccountNumber) {
	sClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

stUser ReadNewUser() {
	stUser User;
	cout << "Enter Username? ";
	getline(cin >> ws, User.UserName);

	while (UserExistsByUsername(User.UserName, UsersFileName)) {
		cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionsToSet();
	return User;
}

stUser ChangeUserRecord(string Username) {
	stUser User;
	User.UserName = Username;

	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionsToSet();
	return User;
}

int ReadPermissionsToSet() {
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		return -1;
	}

	cout << "\nDo you want to give access to : \n ";

	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pListClients;
	}

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pAddNewClient;
	}

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pDeleteClient;
	}

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pUpdateClients;
	}

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pFindClient;
	}

	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pTranactions;
	}

	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pManageUsers;
	}

	return Permissions;
}

double ReadDepositAmount() {
	double DepositAmount;
	cout << "Please enter Deposit Amount ? ";
	cin >> DepositAmount;
	return DepositAmount;
}

double ReadWithdraw() {
	double WithdrawAmount;
	cout << "Please enter Withdraw Amount ? ";
	cin >> WithdrawAmount;
	return WithdrawAmount;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
	vector<sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in); // read Mode

	if (MyFile.is_open()) {
		string Line;
		sClient Client;

		while (getline(MyFile, Line)) {
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

bool UserExistsByUsername(string Username, string FileName) {
	fstream MyFile;
	MyFile.open(FileName, ios::in); // read Mode

	if (MyFile.is_open()) {
		string Line;
		stUser User;

		while (getline(MyFile, Line)) {
			User = ConvertUserLinetoRecord(Line);
			if (User.UserName == Username) {
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}

bool CheckAccessPermission(enMainMenuePermissions Permission) {
	if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
		return true;

	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;
}

// ==========================================
// 9. DATA CONVERSION & STRING UTILITIES
// ==========================================

sClient ConvertLineToRecord(string Line, string Separator) {
	sClient Client;
	vector<string> vClientData;

	vClientData = SplitString(Line, Separator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator) {
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

stUser ConvertUserLinetoRecord(string Line, string Seperator) {
	stUser User;
	vector<string> vUserData;

	vUserData = SplitString(Line, Seperator);

	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[2]);

	return User;
}

string ConvertUserRecordToLine(stUser User, string Seperator) {
	string stClientRecord = "";

	stClientRecord += User.UserName + Seperator;
	stClientRecord += User.Password + Seperator;
	stClientRecord += to_string(User.Permissions);

	return stClientRecord;
}

vector<string> SplitString(string S1, string Delim) {
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable 

	while ((pos = S1.find(Delim)) != std::string::npos) {
		sWord = S1.substr(0, pos); // store the word  

		if (sWord != "") {
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "") {
		vString.push_back(S1); // it adds last word of the string.    
	}
	return vString;
}