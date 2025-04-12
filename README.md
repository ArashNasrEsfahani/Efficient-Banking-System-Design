# 🏦 Efficient Banking System Design

A high-performance banking system simulation built with optimized data structures, featuring real-time account management and efficient transaction processing.

## 📑 Table of Contents
- [Features](#features)
- [Data Structures](#data-structures)
- [Commands](#commands)
- [Technical Implementation](#technical-implementation)
- [Performance](#performance)
- [File Management](#file-management)

## ✨ Features

### Core Operations
- **Account Management**
  - Register new accounts (O(log n))
  - Delete existing accounts (O(log n))
  - Process transactions (O(log n))
  - Handle balance updates

### Search Capabilities
- **Multi-mode Search**
  - Account ID lookup (O(log n))
  - Negative balance detection
  - Assets threshold filtering
  - Wealthiest customer tracking

### Data Persistence
- **File Operations**
  - Save database state
  - Load existing records
  - Transaction logging
  - Richest customer tracking

## 🔧 Data Structures

### Red-Black Tree
- Self-balancing binary search tree
- Guarantees O(log n) operations:
  - Insertion
  - Deletion
  - Search
- Color properties ensure balance
- Automatic rebalancing on modifications

### Trie
- Efficient name storage and lookup
- Prevents duplicate name entries
- Quick prefix-based searching
- Memory-efficient string storage

## 💻 Commands

```bash
# Account Management
REGISTER <first_name> <last_name> <initial_balance>
DELETE <account_id>
DELETE MINUS  # Delete all negative balance accounts

# Search Operations
SEARCH <account_id>
SEARCH MINUS  # Find negative balance accounts
SEARCH GT <value>  # Find accounts above threshold
SEARCH RICHEST  # Display wealthiest customers

# Transaction Processing
CHANGE <account_id> <amount>  # Positive or negative

# Database Operations
SAVE  # Persist current state
LOAD  # Restore from file
EXIT  # Save and terminate
```

## 🔨 Technical Implementation

### Account Node Structure
```cpp
class Node {
    int id;
    string first;
    string last;
    int balance;
    COLOR color;  // RED or BLACK
    Node *left, *right, *parent;
}
```

### Key Components
- **Balance Tracking**
  - Real-time balance updates
  - Automatic richest customer tracking
  - Negative balance monitoring

- **ID Management**
  - Automatic ID assignment
  - Recycled ID handling
  - Unique ID guarantee

- **Name Management**
  - Duplicate prevention
  - Case-sensitive storage
  - Efficient lookup

## ⚡ Performance

### Time Complexities
- Account Registration: O(log n)
- Account Deletion: O(log n)
- Balance Updates: O(log n)
- Richest Customer Lookup: O(1)
- Negative Balance Search: O(n)

### Space Complexity
- Red-Black Tree: O(n)
- Trie Structure: O(m) where m is total character count
- Additional Metadata: O(1)

## 📁 File Management

### Database Files
- **Database.txt**: Main account records
- **Richest.txt**: Top balance holders
- **Ids.txt**: ID tracking and metadata

### File Format
```
# Database.txt
<id> <first_name> <last_name> <balance>

# Richest.txt
<count>
<first_name> <last_name> <id> <balance>

# Ids.txt
<total_count>
<available_ids_count>
<available_id_1>
<available_id_2>
...
```
