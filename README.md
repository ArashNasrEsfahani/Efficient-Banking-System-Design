# Efficient-Banking-System-DS-Project

This project is a simulation of a bank system built as part of a Data Structures course project, with a focus on efficient algorithms. It implements various operations to manage customer accounts, such as registering new accounts, searching for specific accounts, handling transactions, and more. 

## Features
- **Search Accounts by ID**: Efficient search using a logarithmic time complexity (O(log n)).
- **Search by Minimum Balance**: Find all customers with negative balances in linear time (O(n)).
- **Find Wealthiest Customer**: Constant time retrieval of the wealthiest customer (O(1)).
- **Add New Customers**: Efficient registration using logarithmic time complexity (O(log n)).
- **Delete Accounts**: Remove customers with logarithmic time complexity (O(log n)).
- **Handle Transactions**: Update account balances with logarithmic efficiency (O(log n)).
- **Reward Customers**: Perform a weighted lottery where higher balances increase winning chances.

## Data Structures Used
- The project utilizes **Red-Black Trees** for efficient searching, insertion, and deletion of customer accounts, ensuring low time complexities.

## Example Commands
- `SEARCH <account_no>`: Efficiently search for an account by number.
- `REGISTER <first_name> <last_name> <assets>`: Register a new customer efficiently.
- `DELETE <account_no>`: Efficiently delete a customer by account number.
- `CHANGE <account_no> +/-<value>`: Efficiently adjust the balance of a customer account.
