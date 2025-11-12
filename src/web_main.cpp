// src/web_main.cpp - FIXED VERSION

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  // Target Windows 7+
#endif

#include "../include/crow_all.h"
#include "TaskManager.h"
#include <fstream>
#include <asio.hpp>
#include <sstream>

// Helper function to read file contents
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()){
        return "File not found: " + path;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    crow::SimpleApp app;
    TaskManager manager("data/tasks.csv");
    
    // Serve HTML page
    CROW_ROUTE(app, "/")
    ([]() {
        auto html = readFile("web/index.html");
        return crow::response(html);
    });
    
    // Serve CSS
    CROW_ROUTE(app, "/style.css")
    ([]() {
        auto css = readFile("web/style.css");
        crow::response res(css);
        res.add_header("Content-Type", "text/css");
        return res;
    });
    
    // Serve JavaScript
    CROW_ROUTE(app, "/app.js")
    ([]() {
        auto js = readFile("web/app.js");
        crow::response res(js);
        res.add_header("Content-Type", "application/javascript");
        return res;
    });
    
    // API: Handle /api/tasks - GET and POST
    CROW_ROUTE(app, "/api/tasks")
    .methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post, crow::HTTPMethod::Options)
    ([&manager](const crow::request& req) {
        // Handle CORS preflight
        if (req.method == crow::HTTPMethod::Options) {
            crow::response res(200);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            return res;
        }
        
        // Handle GET - Get all tasks
        if (req.method == crow::HTTPMethod::Get) {
            auto tasks = manager.getAllTasks();
            
            crow::json::wvalue json_array;
            json_array = crow::json::wvalue::list();

            for (size_t i = 0; i < tasks.size(); i++){
                crow::json::wvalue task_json;
                task_json["id"] = tasks[i].getId();
                task_json["description"] = tasks[i].getDescription();
                task_json["completed"] = tasks[i].isCompleted();
                json_array[i] = std::move(task_json);
            }
            
            crow::response res(json_array);
            res.add_header("Content-Type", "application/json");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }
        
        // Handle POST - Add task
        if (req.method == crow::HTTPMethod::Post) {
            try {
                auto body = crow::json::load(req.body);
                if(!body){
                    crow::response res(400, "Invalid JSON");
                    res.add_header("Access-Control-Allow-Origin", "*");
                    return res;
                }
                
                std::string description = body["description"].s();

                if(description.empty()){
                    crow::response res(400, "Description cannot be empty");
                    res.add_header("Access-Control-Allow-Origin", "*");
                    return res;
                }

                manager.addTask(description);
                
                crow::response res(200, "Task added successfully");
                res.add_header("Access-Control-Allow-Origin", "*");
                res.add_header("Content-Type", "application/json");
                return res;
            } catch (const std::exception& e) {
                crow::response res(400, std::string("Error: ") + e.what());
                res.add_header("Access-Control-Allow-Origin", "*");
                return res;
            }
        }
        
        // Default response
        crow::response res(405, "Method not allowed");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
    
    // API: Toggle task completion 
    CROW_ROUTE(app, "/api/tasks/<int>/toggle")
    .methods(crow::HTTPMethod::Put, crow::HTTPMethod::Options)
    ([&manager](const crow::request& req, int id) {
        // Handle CORS preflight
        if (req.method == crow::HTTPMethod::Options) {
            crow::response res(200);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "PUT, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            return res;
        }
        
        manager.toggleComplete(id);
        crow::response res(200, "Task updated");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
    
    // API: Delete task
    CROW_ROUTE(app, "/api/tasks/<int>")
    .methods(crow::HTTPMethod::Delete, crow::HTTPMethod::Options)
    ([&manager](const crow::request& req, int id) {
        // Handle CORS preflight
        if (req.method == crow::HTTPMethod::Options) {
            crow::response res(200);
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "DELETE, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            return res;
        }
        
        manager.deleteTask(id);
        crow::response res(200, "Task deleted");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
    
    std::cout << "======================================\n";
    std::cout << "  TO-DO LIST WEB SERVER RUNNING\n";
    std::cout << "======================================\n";
    std::cout << "\n";
    std::cout << " Open your browser and visit:\n";
    std::cout << "   http://localhost:8081\n";
    std::cout << "\n";
    std::cout << "Press Ctrl+C to stop the server\n";
    std::cout << "======================================\n\n";
    
    app.port(8081).multithreaded().run();
}