

# 🍽️ Meal Order Management System

A lightweight, console-based C++ application designed to handle meal bookings for cafeterias, dining halls, or hostels. This system features user authentication, dynamic menu selection, and persistent file-based storage for managing daily lunch and dinner orders.

## ✨ Features

* **User Authentication:** Secure sign-up and login functionality. User credentials are saved locally to prevent unauthorized access.
* **Menu Browsing & Selection:** Users can view separate menus for Lunch and Dinner, featuring items like Chicken Curry, Beef Curry, and Fish Fry.
* **Automated Billing:** Automatically calculates the total cost of the meal, factoring in a base dining fixed cost plus the prices of individual selected items.
* **Order Management:** * **Book a Meal:** Save order details including name, selected items, hall, and precise timestamp.
    * **Search Bookings:** Quickly retrieve order details by querying the user's name.
    * **Cancel Bookings:** Remove existing orders from the system easily.
* **Persistent Storage:** Utilizes C++ file handling (`fstream`) to save all users to `users.txt` and all active orders to `bookings.txt`. Bookings are automatically sorted chronologically.
* **Cross-Platform UI:** Includes pre-processor directives to cleanly clear the console on both Windows (`cls`) and Unix/Linux/macOS (`clear`) environments.

## 🛠️ Technologies Used

* **Language:** C++ (C++11 or higher recommended)
* **Standard Template Library (STL):** Utilizes `std::vector`, `std::map`, and `std::unordered_map` for efficient data handling and menu sorting.
* **File I/O:** `std::ifstream` and `std::ofstream` for persistent data management.
* **Time Library:** `<ctime>` for generating real-time booking timestamps.

## 🚀 Getting Started

### Prerequisites
You will need a C++ compiler installed on your system, such as GCC (`g++`) or MSVC.

### Compilation & Execution
1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/YourUsername/Meal-Order-Management-System.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Meal-Order-Management-System
   ```
3. Compile the source code using `g++`:
   ```bash
   g++ main.cpp -o MealSystem
   ```
4. Run the compiled executable:
   * **On Windows:**
     ```cmd
     MealSystem.exe
     ```
   * **On Linux/macOS:**
     ```bash
     ./MealSystem
     ```

## 📂 File Structure

* `main.cpp`: The core application code containing all classes (`User`, `Authentication`, `MealBookingSystem`) and application logic.
* `users.txt`: *(Auto-generated)* Stores registered usernames and passwords.
* `bookings.txt`: *(Auto-generated)* Stores all active meal bookings in a comma-separated format.
* `temp.txt`: *(Temporary)* Used briefly by the system during the cancellation process to safely update the bookings list.

## 📝 Usage Guide
1. **Launch the app** and select **Sign Up** to create an account.
2. **Log In** using your newly created credentials.
3. Select **Book a Meal**, input your name, choose between Lunch or Dinner, and enter the item numbers you wish to eat (separated by spaces).
4. Enter your designated hall. The system will print a detailed receipt with your total cost.
5. Use the main menu to **Search** or **Cancel** your booking if your plans change.
