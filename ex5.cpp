#include <string>
#include <map>
#include <vector>
#include <set>
#include <iomanip>
#include <exception>
#include <iostream>
#include <regex>
using namespace std;

class Date {
public:
	Date() {
		year = 0;
		month = 0;
		day = 0;
	}
	Date(int new_year, int new_month, int new_day) {
		year = new_year;
		month = new_month;
		day = new_day;
	}
	int GetYear() const {
		return year;
	};
	int GetMonth() const {
		return month;
	};
	int GetDay() const {
		return day;
	};
private:
	int year;
	int month;
	int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() < rhs.GetYear()) return true;
	else {
		if (lhs.GetMonth() < rhs.GetMonth() && lhs.GetYear() == rhs.GetYear()) return true;
		else {
			if (lhs.GetDay() < rhs.GetDay() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetYear() == rhs.GetYear()) return true;
			else return false;
		}
	}

}


class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		data[date].insert(event);
	}

	bool DeleteEvent(const Date& date, const string& event)
	{
		if (data.count(date) == 0)
			return false;
		else {
			if (data[date].count(event) == 0)
				return false;
			else {
				data[date].erase(event);
				return true;
			}
		}
	}

	int  DeleteDate(const Date& date)
	{
		int N = 0;
		set<string> sets = data[date];
		for (const auto& list : sets)
		{
			++N;
			data[date].erase(list);
		}
		data.erase(date);
		return N;
	}

	set<string> Find(const Date& date) {
		return data[date];
	}

	void Print() const {
		for (const auto& list : data)
			for (const auto& events : list.second)
			{
				cout << setw(4) << setfill('0') << list.first.GetYear() << '-';
				cout << setw(2) << setfill('0') << list.first.GetMonth() << '-';
				cout << setw(2) << setfill('0') << list.first.GetDay() << " ";
				cout << events << endl;
			}
	};
private:
	map<Date, set<string>> data;
};

vector<int> GetDigitDate(const string dates) {
	bool flag = false;
	string out1, out2, out3;
	int step = 1;
	for (int i = 0; i < dates.length(); ++i)
	{
		if (dates[i] >= '0' && dates[i] <= '9')
		{
			flag = true;
			if (step == 1) out1 += dates[i];
			if (step == 2) out2 += dates[i];
			if (step == 3) out3 += dates[i];
		}
		else {
			if (step == 3 || (step != 3 && dates[i] != '-') || (dates[i] == '-' && !flag))
				throw runtime_error("Wrong date format: " + dates);
			else {
				flag = false;
				step++;
			}
		}
	}
	if (!flag || (out1.empty()) || (out2.empty()) || (out3.empty())) throw runtime_error("Wrong date format: " + dates);
	int year = stoi(out1);
	int month = stoi(out2);
	int day = stoi(out3);

	if (year < 0 || year > 9999)  throw runtime_error("Wrong date format: " + dates);
	if (month < 1 || month > 12)  throw runtime_error("Month value is invalid: " + to_string(month));
	if (day < 1 || day > 31)  throw runtime_error("Day value is invalid: " + to_string(day));
	return { year,month,day };
}

int main() {
	Database db;
	string command;
	while (getline(cin, command)) {
		if (!command.empty()) {
			string first = "";

			char buffer[20];
			string delimeter = " ";
			size_t found = command.find(delimeter);
			size_t length = command.copy(buffer, found, 0);
			buffer[length] = '\0';
			first = buffer;

			if (first == "Add") {
				try {
					string add_date = "";
					string add_event = "";
					char buffer1[20];
					char buffer2[20];
					size_t found2 = command.find(delimeter, found + 1);
					length = command.copy(buffer1, found2 - found - 1, found + 1);
					buffer1[length] = '\0';
					if (found < 100)
						add_date = buffer1;

					size_t found3 = command.find(delimeter, found2 + 1);
					length = command.copy(buffer2, found3 - found2 - 1, found2 + 1);
					buffer2[length] = '\0';
					add_event = buffer2;

					vector<int> takes = GetDigitDate(add_date);
					Date add_data(takes[0], takes[1], takes[2]);
					db.AddEvent(add_data, add_event);
				}
				catch (exception& ex)
				{
					cout << ex.what() << endl;
				}
			}
			else {
				if (first == "Find")
				{
					try {
						string find_date = "";
						char buffer1[20];
						size_t found2 = command.find(delimeter, found + 1);
						length = command.copy(buffer1, found2 - found - 1, found + 1);
						buffer1[length] = '\0';
						if (found < 100)
							find_date = buffer1;

						vector<int> takes = GetDigitDate(find_date);
						set<string> events = db.Find(Date(takes[0], takes[1], takes[2]));
						if (!events.empty())
							for (auto& list : events)
								cout << list << endl;
					}
					catch (exception& ex)
					{
						cout << ex.what() << endl;
					}

				}
				else {
					if (first == "Del")
					{
						try {
							string del_date = "";
							string del_event = "";

							char buffer1[20];
							char buffer2[20];
							size_t found2 = command.find(delimeter, found + 1);
							length = command.copy(buffer1, found2 - found - 1, found + 1);
							buffer1[length] = '\0';
							if (found < 100)
								del_date = buffer1;

							size_t found3 = command.find(delimeter, found2 + 1);
							length = command.copy(buffer2, found3 - found2 - 1, found2 + 1);
							buffer2[length] = '\0';
							if (found2 < 100)
								del_event = buffer2;

							vector<int> takes = GetDigitDate(del_date);
							if (del_event.empty())
								cout << "Deleted " << (db.DeleteDate(Date(takes[0], takes[1], takes[2]))) << " events" << endl;
							else
							{
								if (db.DeleteEvent(Date(takes[0], takes[1], takes[2]), del_event)) cout << "Deleted successfully" << endl;
								else cout << "Event not found" << endl;
							}
						}
						catch (exception& ex)
						{
							cout << ex.what() << endl;
						}
					}
					else {
						if (first == "Print")
							db.Print();
						else
						{
							if (first.length() != command.length())
								cout << "Unknown command: " << first << endl;
							else cout << "Unknown command: " << command << endl;

						}
					}
				}

			}
		}
	}
	return 0;
}