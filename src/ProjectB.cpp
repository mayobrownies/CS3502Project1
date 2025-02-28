#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <sstream>
#include <iomanip>

// store file information parsed from directory listing
class FileInfo {
public:
    std::string name;
    size_t size;
    std::string permissions;
    std::string timestamp;

    // initializes file info with name, size, permissions and timestamp
    FileInfo(const std::string& n, size_t s, const std::string& p, const std::string& t) : name(n), size(s), permissions(p), timestamp(t) {}
};

// handles sorting and processing file data
class Sorter {
public:
    // processes input from pipe and sorts files by size
    // reads from stdin which is connected to the pipe from parent
    static void Sort() {
        std::cout << "\n[Child Process " << getpid() << "] Started receiving data from pipe" << std::endl;
        
        std::vector<FileInfo> files;
        std::string line;
        auto startTime = std::chrono::steady_clock::now();
        size_t totalBytes = 0;
        int lineCount = 0;
        
        try {
            // read each line from the pipe and parse file information
            while (std::getline(std::cin, line)) {
                lineCount++;
                std::cout << "[Child Process " << getpid() << "] Received line " << lineCount << ": " << line << std::endl;
                
                // skip directories and the "total" line from ls output
                if (line[0] != 'd' && line != "total") {
                    std::istringstream iss(line);
                    std::string perms, links, owner, group, size, month, day, time, name;
                    
                    // parse the ls -l format output
                    if (iss >> perms >> links >> owner >> group >> size >> month >> day >> time >> name) {
                        size_t fileSize = std::stoull(size);
                        totalBytes += fileSize;
                        files.emplace_back(name, fileSize, perms, month + " " + day + " " + time);
                    }
                }
            }

            std::cout << "\n[Child Process " << getpid() << "] Finished receiving data. Now Processing" << std::endl;

            // sort files by size in descending order
            std::sort(files.begin(), files.end(), 
                [](const FileInfo& a, const FileInfo& b) { return a.size > b.size; });

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - startTime).count() / 1000.0;

            // display results of the processing
            std::cout << "\n[Child Process " << getpid() << "] Results:" << std::endl;
            std::cout << "Processed " << files.size() << " files in " << duration << " ms\n";
            std::cout << "Total size: " << totalBytes << " bytes\n\n";

            std::cout << "Files sorted by size:\n";
            for (const auto& file : files) {
                std::cout << file.name << " (" << file.size << " bytes)\n";
            }
        }
        catch (const std::exception& ex) {
            // handle any errors
            std::cerr << "[Child Process " << getpid() << "] Error: " << ex.what() << std::endl;
            exit(1);
        }
    }
};

// inter-process communication using pipes
int main() {
    int pipefd[2];
    pid_t pid;

    std::cout << "[Parent Process " << getpid() << "] Starting" << std::endl;

    // create pipe for communication between processes
    if (pipe(pipefd) == -1) {
        std::cerr << "[Parent Process " << getpid() << "] Pipe creation failed: " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "[Parent Process " << getpid() << "] Pipe created successfully" << std::endl;

    // create child process
    pid = fork();
    if (pid == -1) {
        std::cerr << "[Parent Process " << getpid() << "] Fork failed: " << strerror(errno) << std::endl;
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (pid == 0) {
        // child process
        try {
            std::cout << "[Child Process " << getpid() << "] Started" << std::endl;
            
            // close write end as child only reads from pipe
            close(pipefd[1]); 
            std::cout << "[Child Process " << getpid() << "] Closed write end of pipe" << std::endl;
            
            // redirect pipe to stdin and read from it
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                throw std::runtime_error(std::string("dup2 failed: ") + strerror(errno));
            }
            std::cout << "[Child Process " << getpid() << "] Redirected pipe to stdin" << std::endl;
            
            close(pipefd[0]);
            Sorter::Sort();
        }
        catch (const std::exception& ex) {
            // handle any errors
            std::cerr << "[Child Process " << getpid() << "] Error: " << ex.what() << std::endl;
            return 1;
        }
        return 0;
    }
    else {
        // parent process
        try {
            std::cout << "[Parent Process " << getpid() << "] Created child process " << pid << std::endl;
            
            // close read end as parent only writes to pipe
            close(pipefd[0]);
            std::cout << "[Parent Process " << getpid() << "] Closed read end of pipe" << std::endl;

            // run ls -l command and get its output
            std::cout << "[Parent Process " << getpid() << "] Running 'ls -l'" << std::endl;
            FILE* ls = popen("ls -l", "r");
            if (ls == nullptr) {
                throw std::runtime_error(std::string("Failed to run ls: ") + strerror(errno));
            }

            // buffer for reading from ls command and writing to pipe
            char buffer[4096];
            size_t bytesRead;
            size_t totalBytesRead = 0;
            int chunks = 0;

            // read from ls command and write to pipe in chunks
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), ls)) > 0) {
                chunks++;
                if (write(pipefd[1], buffer, bytesRead) == -1) {
                    throw std::runtime_error(std::string("Write to pipe failed: ") + strerror(errno));
                }
                totalBytesRead += bytesRead;
                std::cout << "[Parent Process " << getpid() << "] Sent chunk " << chunks << " (" << bytesRead << " bytes) to pipe" << std::endl;
            }

            std::cout << "[Parent Process " << getpid() << "] Finished sending data. Total: " << totalBytesRead << " bytes in " << chunks << " chunks" << std::endl;

            // clean up resources
            pclose(ls);
            close(pipefd[1]);
            std::cout << "[Parent Process " << getpid() << "] Closed write end of pipe" << std::endl;

            // wait for child process to complete
            int status;
            std::cout << "[Parent Process " << getpid() << "] Waiting for child process..." << std::endl;
            
            if (waitpid(pid, &status, 0) == -1) {
                throw std::runtime_error(std::string("Wait failed: ") + strerror(errno));
            }

            // check how child process terminated
            if (WIFEXITED(status)) {
                std::cout << "[Parent Process " << getpid() << "] Child process exited with status " << WEXITSTATUS(status) << std::endl;
            }
        }
        catch (const std::exception& ex) {
            // handle any errors in parent process
            std::cerr << "[Parent Process " << getpid() << "] Error: " << ex.what() << std::endl;
            return 1;
        }
    }

    return 0;
}