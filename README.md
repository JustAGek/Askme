# AskMe Q&A Platform

AskMe is a command-line based Q&A platform built in C++ that enables users to sign up, log in, ask questions, reply to questions, and view a dynamic feed of interactions. The project emphasizes simple file-based persistence and a threaded question/reply structure, making it an ideal example of basic data management and user interaction in C++.

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Project Structure](#project-structure)
4. [Key Components](#key-components)
5. [Unique ID Generation](#unique-id-generation)
6. [Setup and Compilation](#setup-and-compilation)
7. [Usage](#usage)
8. [Data Persistence](#data-persistence)
9. [Future Improvements](#future-improvements)
10. [License](#license)

---

## Overview

The AskMe platform allows users to:
- **Register and Login:** Create an account with a unique username and password.
- **Ask Questions:** Pose questions to other users, with the option to ask anonymously (if the recipient allows anonymous questions).
- **Answer Questions:** Reply to questions that have been directed to the logged-in user.
- **Threaded Discussions:** Support threaded Q&A, where replies can become a nested conversation.
- **View Feed:** See a consolidated feed of questions both sent and received.

This project is designed as a console application, demonstrating concepts such as file I/O, structured data management with structs, and basic user authentication.

---

## Features

- **User Management:**
  - Sign Up: Users provide their name, email, username, password, and a flag indicating if they accept anonymous questions.
  - Sign In: Users log in using their username and password.
  - List Users: Display all system users with their IDs and names.
  - Log Out: Securely end the user session.

- **Question Management:**
  - Ask a Question: Users can ask new questions or reply to an existing thread.
  - Answer a Question: Users can respond to questions addressed to them.
  - Delete a Question: Authorized users can delete questions (either the sender or the receiver).
  - Feed Display: View a threaded feed of Q&A interactions.

- **Data Persistence:**
  - User data is stored in `users.txt`.
  - Question data is stored in `questions.txt`.
  - Both files are updated upon every significant operation, ensuring persistence across sessions.

- **Unique ID Generation:**
  - A combination of the current timestamp, process ID, and a random number is used to generate near-unique IDs for users and questions.

---

## Project Structure

```
├── main.cpp          # Contains the main code for the AskMe platform
├── users.txt         # File to store user information persistently
├── questions.txt     # File to store questions and threaded replies persistently
└── README.md         # This detailed project documentation
```

---

## Key Components

### 1. User Management (`User_manager`)
- **Data Structures:** 
  - Maintains users in an unordered map.
  - Tracks sign-in credentials and corresponding user IDs.
- **Core Functions:**
  - `sign_up()`: Handles new user registration.
  - `sign_in()`: Authenticates existing users.
  - `list_users()`: Lists all registered users.
  - `load()` / `save()`: Load and save user data from/to `users.txt`.

### 2. Question Management (`Question_manager`)
- **Data Structures:**
  - Uses a vector of `Question` structs to store questions and nested replies.
- **Core Functions:**
  - `ask_question()`: Adds a new question or reply in a threaded manner.
  - `answer_question()`: Enables a user to answer a question addressed to them.
  - `delete_question()`: Allows deletion of a question if the user is authorized.
  - `questions()`: Recursively displays questions for various views (to-me, from-me, or feed).
  - `load()` / `save()`: Load and save question data from/to `questions.txt`.

### 3. Application Controller (`askme`)
- **Responsibilities:**
  - Ties user and question management together.
  - Provides the main menu and sub-menu for user interactions.
  - Facilitates the application flow from start-up to termination.

---

## Unique ID Generation

The function `generateUniqueID()` is central to ensuring that every user and question has a distinct identifier. It achieves this by:
- **Timestamp:** Using the current time in milliseconds.
- **Process ID:** Incorporating the process ID (retrieved via `getpid()`).
- **Random Component:** Adding a random number generated using `std::mt19937` and `std::uniform_int_distribution`.

This multi-factor approach minimizes the possibility of duplicate IDs even across multiple sessions.

---

## Setup and Compilation

### Prerequisites
- A C++ compiler supporting C++11 or later.
- Windows operating system (due to inclusion of `<windows.h>` and use of `getpid()`).

### Compilation Instructions
Using **g++**:
```bash
g++ main.cpp -o askme -std=c++11
```
Alternatively, you can use an IDE like Visual Studio to open and compile the project.

---

## Usage

### Starting the Application
Run the compiled executable:
```bash
./askme
```
Upon launch, you are presented with the start menu:
- **0:** Exit the application.
- **1:** Login with your credentials.
- **2:** Sign Up to create a new account.

### User Menu Options (After Login)
Once logged in, the following options are available:
- **1: Print Questions To Me** – View questions directed to you.
- **2: Print Questions From Me** – View questions you have asked.
- **3: Answer Question** – Provide an answer to a specific question.
- **4: Delete Question** – Remove a question (if you are the sender or recipient).
- **5: Ask Question** – Ask a new question or reply in an existing thread.
- **6: List System Users** – Display a list of all registered users.
- **7: Feed** – View a combined feed of all questions and replies.
- **8: Logout** – End your session and return to the main menu.

Each option is driven by simple console inputs, making the interaction straightforward.

---

## Data Persistence

- **Users:** All user-related information is stored in `users.txt`. This file is read during startup and updated upon user registration, login, and logout events.
- **Questions:** The `questions.txt` file maintains the questions and their threaded replies. It is read at the start of each user menu iteration and saved after any modifications (like asking, answering, or deleting a question).

This file-based persistence ensures that data is retained across different sessions of the application.

---

## Future Improvements

- **Enhanced Input Validation:** Improve error handling and validation of user input.
- **Secure Password Storage:** Implement hashing for password storage instead of plain text.
- **User Interface:** Develop a more user-friendly interface, potentially using a GUI framework.
- **Cross-Platform Support:** Modify or abstract Windows-specific functions to support multiple operating systems.
- **Networking:** Consider extending the platform for network-based interactions and multi-user sessions.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
