#include "TaskManager.h"
#include <iostream>
#include <limits>

void clearInput(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Display Menu
void displayMenu(){
    std::cout << "|********************************|\n";
    std::cout << "|       TO-DO LIST MANAGER       |\n";
    std::cout << "|********************************|\n";
    std::cout << "| 1. View all tasks              |\n";
    std::cout << "| 2. Add new task                |\n";
    std::cout << "| 3. Mark task as complete       |\n";
    std::cout << "| 4. Edit task                   |\n";
    std::cout << "| 5. Delete task                 |\n";
    std::cout << "| 6. Exit                        |\n";
    std::cout << "|________________________________|\n";
    std::cout << "Choose an option: ";
}

int main(){
    TaskManager manager("data/tasks.csv");

    int choice;
    bool running = true;

    while(running) {
        displayMenu();
        std::cin >> choice;

        //Handle invalid input
        if (std::cin.fail()){
            std::cout << "invalid input! Please enter a number. \n";
            clearInput();
            continue;
        }

        clearInput();

        switch (choice){
            case 1: {
                manager.displayTasks();
                break;
            }
            case 2: {
                std::cout << "Enter task description: ";
                std::string description;
                std::getline(std::cin, description);

                if(!description.empty()){
                    manager.addTask(description);
                }
                else{
                    std::cout << "Task description cannot be empty! \n";
                }
                break;
            }

            case 3: {
                manager.displayTasks();
                std::cout << "Enter task ID to toggle:";
                int id;
                std::cin >> id;
                clearInput();
                manager.toggleComplete(id);
                break;
            }

            case 4: {
                manager.displayTasks();
                std::cout << "Enter task ID to edit:";
                int id;
                std::cin >> id;
                clearInput();

                std::cout << "Enter new description:";
                std::string newDesc;
                std::getline(std::cin, newDesc);

                if(!newDesc.empty()){
                    manager.editTask(id, newDesc);
                }
                else {
                    std::cout << "Description cannot be empty!\n";
                }
                break;
            }

            case 5: {
                manager.displayTasks();
                std::cout << "Enter task ID to delete: ";
                int id;
                std::cin >> id;
                clearInput();
                manager.deleteTask(id);
                break;
            }

            case 6: {
                std::cout << "Saving and exiting... Goodbye!\n";
                running = false;
                break;
            }

            default: {
                std::cout << "Invalid option! Please choose 1-6.\n";
            }
        }

        //Pause before next iteration
        if (running && choice != 1){
            std::cout << "Press Enter to continue....";
            std::cin.get();
        }
    }
    return 0;
}