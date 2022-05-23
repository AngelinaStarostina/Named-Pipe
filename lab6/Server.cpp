#include <windows.h>
#include <iostream>
using namespace std;

int main() {
	SECURITY_ATTRIBUTES sa; 
	SECURITY_DESCRIPTOR sd;
	HANDLE hNamedPipe;
	DWORD dwBytesRead; 
	DWORD dwBytesWrite; 

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; 
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;


	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Invalid handle value.\n";
		system("pause");
		return 0;
	}

	
	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL)) {
		cout << "Connection failed.\n";
		system("pause");
		return 0;
	}

	int n;
	char* arr;
	cout << "Enter the number of elements: ";
	cin >> n;
	arr = new char[n];
	cout << "Enter elements: ";
	for (int i = 0; i < n; i++)
		cin >> arr[i];

	if (!WriteFile(hNamedPipe, &n, sizeof(n), &dwBytesWrite, (LPOVERLAPPED)NULL)) {
		// ошибка записи
		cout << "Write n failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}
	cout << n << " is write" << endl;


	for (int i = 0; i < n; i++) 
	{
		if (!WriteFile(hNamedPipe, &arr[i], sizeof(arr[i]), &dwBytesWrite, (LPOVERLAPPED)NULL)) 
		{
			cout << "Write number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return 0;
		}
		cout << arr[i] << " is write" << endl;
	}

	int rSize;

	if (!ReadFile(hNamedPipe, &rSize, sizeof(rSize), &dwBytesRead, (LPOVERLAPPED)NULL)) 
	{
		cout << "Read size failed\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}
	cout << rSize << " is read from the pipe" << endl;

	for (int i = 0; i < rSize; i++)
	{
		char nData;
		if (!ReadFile(hNamedPipe, &nData, sizeof(nData), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			cout << "Read number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return GetLastError();
		}
		cout << nData << " is read from the pipe" << endl;
	}
	cout << endl;

	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);

	system("pause");

	return 0;
}