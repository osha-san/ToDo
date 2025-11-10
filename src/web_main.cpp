// src/web_main.cpp - COMPLETE VERSION
#include "../include/crow_all.h"
#include "../include/json.hpp"
#include "TaskManager.h"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

// Helper function to read file contents
std::string readFile(const std::string& path) {
    std::ifstream file(path);
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
    
    // API: Get all tasks
    CROW_ROUTE(app, "/api/tasks")
    ([&manager]() {
        auto tasks = manager.getAllTasks();
        json j = json::array();
        
        for (const auto& task : tasks) {
            j.push_back({
                {"id", task.getId()},
                {"description", task.getDescription()},
                {"completed", task.isCompleted()}
            });
        }
        
        crow::response res(j.dump());
        res.add_header("Content-Type", "application/json");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
    
    // API: Add task
    CROW_ROUTE(app, "/api/tasks")
    .methods("POST"_method)
    ([&manager](const crow::request& req) {
        try {
            auto body = json::parse(req.body);
            std::string description = body["description"];
            
            manager.addTask(description);
            
            crow::response res(200, "Task added");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        } catch (const std::exception& e) {
            return crow::response(400, "Invalid request");
        }
    });
    
    // API: Toggle complete
    CROW_ROUTE(app, "/api/tasks/<int>/toggle")
    .methods("PUT"_method)
    ([&manager](int id) {
        manager.toggleComplete(id);
        crow::response res(200, "Task updated");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
    
    // API: Delete task
    CROW_ROUTE(app, "/api/tasks/<int>")
    .methods("DELETE"_method)
    ([&manager](int id) {
        manager.deleteTask(id);
        crow::response res(200, "Task deleted");
        res.add_header("Access-Control-Allow-Origin", "*");
        return res;
    });
    
    // CORS preflight
    CROW_ROUTE(app, "/api/tasks")
    .methods("OPTIONS"_method)
    ([]() {
        crow::response res(200);
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    });
    
    std::cout << "======================================\n";
    std::cout << "  TO-DO LIST WEB SERVER RUNNING\n";
    std::cout << "======================================\n";
    std::cout << "\n";
    std::cout << "🌐 Open your browser and visit:\n";
    std::cout << "   http://localhost:8080\n";
    std::cout << "\n";
    std::cout << "Press Ctrl+C to stop the server\n";
    std::cout << "======================================\n\n";
    
    app.port(8080).multithreaded().run();
}