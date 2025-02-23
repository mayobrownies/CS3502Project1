#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <sstream>

// phase 1: thread operations
class BankAccount {
private:
    int accountId;
    double balance;
    std::mutex lockObject;
    static std::mutex logMutex; 

    void LogOperation(const std::string& operation, double amount) {
        std::lock_guard<std::mutex> logLock(logMutex);
        std::stringstream ss;
        ss << "Thread " << std::this_thread::get_id() << " - ";
        ss << "Account " << accountId << ": " << operation << " $" << amount;
        ss << " (Balance: $" << balance << ")" << std::endl;
        std::cout << ss.str();
    }

public:
    BankAccount(int id, double initialBalance) 
        : accountId(id), balance(initialBalance) {
        LogOperation("Created with", initialBalance);
    }
        
    BankAccount(BankAccount&& other) noexcept
        : accountId(other.accountId), balance(other.balance) {}
    BankAccount(const BankAccount&) = delete;
    BankAccount& operator=(const BankAccount&) = delete;
    BankAccount& operator=(BankAccount&&) = delete;

    // phase 2: resource protection
    void Deposit(double amount) {
        std::lock_guard<std::mutex> lock(lockObject);
        balance += amount;
        LogOperation("Deposited", amount);
    }

    void Withdraw(double amount) {
        std::lock_guard<std::mutex> lock(lockObject);
        if (balance >= amount) {
            balance -= amount;
            LogOperation("Withdrawn", amount);
        } else {
            LogOperation("Insufficient funds for withdrawal of", amount);
        }
    }

    // phase 3 and 4: deadlock creation and resolution
    bool Transfer(BankAccount& target, double amount, int timeoutMs = 1000) {
        auto start = std::chrono::steady_clock::now();
        
        {
            std::lock_guard<std::mutex> logLock(logMutex);
            std::cout << "Thread " << std::this_thread::get_id() 
                      << " attempting transfer: $" << amount 
                      << " from Account " << accountId 
                      << " to Account " << target.accountId << std::endl;
        }

        while (true) {
            if (lockObject.try_lock()) {
                if (target.lockObject.try_lock()) {
                    if (balance >= amount) {
                        balance -= amount;
                        target.balance += amount;
                        
                        {
                            std::lock_guard<std::mutex> logLock(logMutex);
                            std::cout << "Thread " << std::this_thread::get_id() 
                                      << " completed transfer: $" << amount 
                                      << " from Account " << accountId 
                                      << " to Account " << target.accountId << std::endl;
                        }

                        target.lockObject.unlock();
                        lockObject.unlock();
                        return true;
                    }
                    target.lockObject.unlock();
                }
                lockObject.unlock();
            }
            
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > timeoutMs) {
                {
                    std::lock_guard<std::mutex> logLock(logMutex);
                    std::cout << "Thread " << std::this_thread::get_id() 
                              << " transfer timed out: $" << amount 
                              << " from Account " << accountId 
                              << " to Account " << target.accountId 
                              << " (DEADLOCK PREVENTED)" << std::endl;
                }
                return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    double GetBalance() const { return balance; }
    int GetId() const { return accountId; }
};

std::mutex BankAccount::logMutex;

void Transaction(BankAccount& account) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    
    for (int i = 0; i < 5; i++) {
        if (dis(gen) == 0)
            account.Deposit(100.0);
        else
            account.Withdraw(50.0);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ConcurrencyTest() {
    std::cout << "\nConcurrency Test" << std::endl;
    BankAccount testAccount(1, 1000.0);
    std::vector<std::thread> testThreads;
    
    for (int i = 0; i < 10; i++) {
        testThreads.emplace_back([&testAccount]() {
            for (int j = 0; j < 3; j++) {
                testAccount.Withdraw(100.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    for (auto& thread : testThreads) {
        thread.join();
    }
    
    std::cout << "Final balance after concurrent withdrawals: $" << testAccount.GetBalance() << std::endl;
}

void SynchronizationTest() {
    std::cout << "\nSynchronization Test" << std::endl;
    BankAccount account1(1, 500.0);
    BankAccount account2(2, 500.0);

    std::thread thread1([&]() { account1.Transfer(account2, 300.0); });
    std::thread thread2([&]() { account2.Transfer(account1, 200.0); });

    thread1.join();
    thread2.join();

    std::cout << "Account 1 final balance: $" << account1.GetBalance() << std::endl;
    std::cout << "Account 2 final balance: $" << account2.GetBalance() << std::endl;
}

void StressTest() {
    std::cout << "\nStress Test" << std::endl;
    std::vector<BankAccount> accounts;
    std::vector<std::thread> threads;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 19);

    for (int i = 0; i < 20; i++) {
        accounts.emplace_back(i, 1000.0);
    }

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < 100; i++) {
        threads.emplace_back([&accounts, &dis, &gen]() {
            for (int j = 0; j < 5; j++) {
                int fromIdx = dis(gen);
                int toIdx = dis(gen);
                if (fromIdx != toIdx) {
                    accounts[fromIdx].Transfer(accounts[toIdx], 50.0);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto duration = std::chrono::steady_clock::now() - start;
    auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;

    std::cout << "Stress test completed in " << seconds << " seconds" << std::endl;
    int nonZeroAccounts = std::count_if(accounts.begin(), accounts.end(), 
        [](const BankAccount& acc) { return acc.GetBalance() > 0; });
    std::cout << "Total accounts with non-zero balance: " << nonZeroAccounts << std::endl;
}

void Program() {
    std::vector<BankAccount> accounts;
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        accounts.emplace_back(i, 1000.0);
    }

    for (auto& account : accounts) {
        threads.emplace_back(Transaction, std::ref(account));
    }

    for (int i = 0; i < accounts.size() - 1; i++) {
        int fromIndex = i;
        int toIndex = (i + 1) % accounts.size();
        threads.emplace_back([&accounts, fromIndex, toIndex]() {
            for (int j = 0; j < 3; j++) {
                accounts[fromIndex].Transfer(accounts[toIndex], 100.0);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "\nFinal balances:" << std::endl;
    for (const auto& account : accounts) {
        std::cout << "Account " << account.GetId() << ": $" << account.GetBalance() << std::endl;
    }
}

int main() {
    std::cout << " Program Transactions " << std::endl;
    Program();
    ConcurrencyTest();
    SynchronizationTest();
    StressTest();
    return 0;
}
