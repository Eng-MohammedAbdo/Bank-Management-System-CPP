
// my solution 98% 
#include <iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenue();

enum enMainMenuOptions {

	eShowClientList = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eExit = 6
};

struct sClient {

	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

//--------------- Convert Data Function ---------------------
vector<string> SplitString(string S1, string Delim) {

	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable 
	// use find() function to get the position of the delimiters  

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

sClient ConvertLineToRecord(string Line, string Separator = "#//#") {

	sClient Client;
	vector <string> vClientData;

	vClientData = SplitString(Line, Separator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {

	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;

}

//---------------- Read Data Function -----------------------
short ReadNumber(short From, short To) {

	int Number = 0;

	while (true)
	{
		cin >> Number;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error: Please enter numbers only.\n";
			continue;
		}

		if (Number < From || Number > To)
		{
			cout << "Error: Please enter a number between "
				<< From << " and " << To << ": ";
			continue;
		}

		if (Number <= 0)
		{
			cout << "Error: Number must be greater than zero.\n";
			continue;
		}

		return Number;
	}
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}

		MyFile.close();

	}
	return false;
}

sClient ReadNewClient() {

	sClient Client;

	cout << "Enter Account Number? ";
	// Usage of std::ws will extract allthe whitespace character
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
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

string ReadClientAccountNumber() {

	string AccountNumber;

	cout << "\n Please enter Account Number ? ";
	cin >> AccountNumber;

	return AccountNumber;

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

//--------------- File Function ---------------------
vector <sClient> LoadCleintsDataFromFile(string FileName) {

	fstream MyFile;
	vector <sClient> vClients;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;

		while (getline(MyFile, Line))
		{

			vClients.push_back(ConvertLineToRecord(Line));

		}
		MyFile.close();
	}
	return vClients;
}

void AddDataLineToFile(string FileName, string sDataLine) {

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << sDataLine << endl;

		MyFile.close();
	}
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);//overwrite 

	string DataLine;

	if (MyFile.is_open()) {

		for (sClient C : vClients) {
			if (C.MarkForDelete == false) {

				//we only write records that are not marked for delete.     
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client) {

	for (sClient C : vClients) {

		if (C.AccountNumber == AccountNumber) {

			Client = C;
			return true;
		}
	}
	return false;

}

//--------------- Print Function --------------------
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

	cout << "\n Account Number : ";
	cout << Client.AccountNumber;

	cout << "\n PinCode : ";
	cout << Client.PinCode;

	cout << "\n Name : ";
	cout << Client.Name;

	cout << "\n Phone : ";
	cout << Client.Phone;

	cout << "\n Account Balance : ";
	cout << Client.AccountBalance;

	cout << "\n---------------------------------\n";
}

void PrintAllClientsData(vector <sClient> vClientsData) {

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

	if (vClientsData.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
	{
		for (sClient Client : vClientsData) {

			PrintClientRecord(Client);
			cout << endl;
		}
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

//------------------- Update Client -----------------
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
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

//------------------- Delete Client -----------------


bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients) {

	for (sClient& C : vClients) {

		if (C.AccountNumber == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}
	} return false;
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

			//Refresh Clients 
			vClients = LoadCleintsDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

//------------------- Add New Client -----------------

void AddNewClient() {

	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

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


//------------------Main Menue-------------------

void ShowEndScreen() {

	cout << "----------------------------------\n";
	cout << "\t Program Ends :-) \n";
	cout << "----------------------------------\n";

}

void FindClientScreen() {

	cout << "============================================\n";
	cout << "\t Find Client Screen \n";
	cout << "============================================\n";

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();


	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

		PrintClientCard(Client);
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

	}

}

void UpdateClientScreen() {

	cout << "============================================\n";
	cout << "\t Update Client Screen \n";
	cout << "============================================\n";

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);

}

void DeleteClientScreen() {

	cout << "============================================\n";
	cout << "\t Delete Client Screen \n";
	cout << "============================================\n";


	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClients);

}

void AddNewClientScreen() {

	AddClients();
}

void ShowClientListScreen() {

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	PrintAllClientsData(vClients);
}

void GoBackToMainMenu() {

	cout << "Press any key to go back to Main Menu...";
	system("pause > nul");
	ShowMainMenue();

}

void MainMenue(enMainMenuOptions Option) {

	switch (Option) {

	case enMainMenuOptions::eShowClientList:
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eAddNewClient:
		AddNewClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eDeleteClient:
		system("cls");
		DeleteClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eUpdateClient:
		system("cls");
		UpdateClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eFindClient:
		system("cls");
		FindClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}

}

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
	cout << "\t[6] Exit.\n";
	cout << "============================================\n";
	cout << "Choose what do you want to do? [1 to 6]? ";
	MainMenue((enMainMenuOptions)ReadNumber(1, 6));
}


int main() {


	ShowMainMenue();

	system("pause>0");
	return 0;
}