#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

struct Transaction {
    string type; // "Income" or "Expense"
    string category;
    double amount;
    string date; // Format: YYYY-MM-DD
};

class FinanceTracker {
private:
    vector<Transaction> transactions;
    double balance;

public:
    FinanceTracker() : balance(0.0) {
        loadFromFile();
    }

    void addTransaction() {
        Transaction t;
        cout << "Enter type (Income/Expense): ";
        cin >> t.type;
        cout << "Enter category: ";
        cin >> t.category;
        cout << "Enter amount: ";
        cin >> t.amount;
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> t.date;

        if (t.type == "Income") {
            balance += t.amount;
        } else if (t.type == "Expense") {
            balance -= t.amount;
        } else {
            cout << "Invalid type. Must be 'Income' or 'Expense'.\n";
            return;
        }

        transactions.push_back(t);
        saveToFile();
        cout << "✅ Transaction added successfully!\n";
    }

    void viewAllSummary() {
        cout << "\n--- All Transactions ---\n";
        displayTransactions(transactions);
        cout << "\nCurrent Balance: ₹" << fixed << setprecision(2) << balance << endl;
    }

    void filterByMonthYear() {
        string yearMonth;
        cout << "Enter month and year (YYYY-MM): ";
        cin >> yearMonth;

        vector<Transaction> filtered;
        for (const auto& t : transactions) {
            if (t.date.substr(0, 7) == yearMonth) {
                filtered.push_back(t);
            }
        }

        if (filtered.empty()) {
            cout << "No transactions found for " << yearMonth << endl;
        } else {
            cout << "\n--- Transactions in " << yearMonth << " ---\n";
            displayTransactions(filtered);
        }
    }

    void searchByCategory() {
        string searchCategory;
        cout << "Enter category to search: ";
        cin >> searchCategory;

        vector<Transaction> result;
        for (const auto& t : transactions) {
            if (t.category == searchCategory) {
                result.push_back(t);
            }
        }

        if (result.empty()) {
            cout << "No transactions found in category: " << searchCategory << endl;
        } else {
            displayTransactions(result);
        }
    }

    void categoryWiseSummary() {
        map<string, double> incomeMap, expenseMap;

        for (const auto& t : transactions) {
            if (t.type == "Income")
                incomeMap[t.category] += t.amount;
            else
                expenseMap[t.category] += t.amount;
        }

        cout << "\n--- Income by Category ---\n";
        for (const auto& pair : incomeMap) {
            cout << left << setw(15) << pair.first << "₹" << fixed << setprecision(2) << pair.second << endl;
        }

        cout << "\n--- Expense by Category ---\n";
        for (const auto& pair : expenseMap) {
            cout << left << setw(15) << pair.first << "₹" << fixed << setprecision(2) << pair.second << endl;
        }
    }

    void saveToFile() {
        ofstream out("transactions.csv");
        for (const auto& t : transactions) {
            out << t.type << "," << t.category << "," << t.amount << "," << t.date << "\n";
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in("transactions.csv");
        string line, type, category, date;
        double amount;

        while (getline(in, line)) {
            stringstream ss(line);
            getline(ss, type, ',');
            getline(ss, category, ',');
            ss >> amount;
            ss.ignore(); // skip comma
            getline(ss, date);

            Transaction t{type, category, amount, date};
            transactions.push_back(t);
            if (t.type == "Income") balance += amount;
            else if (t.type == "Expense") balance -= amount;
        }
        in.close();
    }

    void displayTransactions(const vector<Transaction>& list) {
        cout << left << setw(10) << "Type"
             << setw(15) << "Category"
             << setw(10) << "Amount"
             << setw(12) << "Date" << endl;

        for (const auto& t : list) {
            cout << left << setw(10) << t.type
                 << setw(15) << t.category
                 << setw(10) << fixed << setprecision(2) << t.amount
                 << setw(12) << t.date << endl;
        }
    }
};

int main() {
    FinanceTracker tracker;
    int choice;

    do {
        cout << "\n====== Personal Finance Tracker ======\n";
        cout << "1. Add Transaction\n";
        cout << "2. View All Summary\n";
        cout << "3. Filter by Month-Year\n";
        cout << "4. Search by Category\n";
        cout << "5. Category-wise Summary\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                tracker.addTransaction();
                break;
            case 2:
                tracker.viewAllSummary();
                break;
            case 3:
                tracker.filterByMonthYear();
                break;
            case 4:
                tracker.searchByCategory();
                break;
            case 5:
                tracker.categoryWiseSummary();
                break;
            case 6: {
                char confirm;
                cout << "Do you really want to exit? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    cout << "Saving and exiting...\n";
                    return 0;
                } else {
                    choice = 0; // reset loop
                }
                break;
            }
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (true);

    return 0;
}
