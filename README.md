# Smart Campus Lost & Found Management System

A menu-driven C++ console application where students can report lost items, submit found items, search for belongings, and match records automatically.

Built as **Lab Sheet-10** for BCA I Semester, COER University, Roorkee.

## Features

- Student login (3 attempts)
- Report lost items and found items
- Search by name, category, description, or location
- Auto-match lost and found items
- View all records, sorted by ID or name
- Delete your own records
- Item statistics (lost, found, matched, unmatched)
- Export a full report to a text file
- Records saved to a file, so data persists between runs

## Concepts Used

Arrays, structures, file handling, functions, loops, searching (linear search), sorting (bubble sort), and strings.

## How to Compile and Run

Requires a C++ compiler such as g++.

```bash
g++ lost_found.cpp -o lost_found
./lost_found
```

On Windows, run `lost_found.exe` instead.

## Demo Login

| Roll No | Password |
|---------|----------|
| BCA001  | coer1    |
| BCA002  | coer2    |
| BCA003  | coer3    |
| BCA004  | coer4    |
| BCA005  | coer5    |

## How Auto-Match Works

A lost item is matched with a found item when both have the same category and one item name contains the other. For example, a lost "Black Wallet" matches a found "wallet".

## Files Generated at Runtime

- `records.txt` stores all reported items
- `report.txt` holds the exported report

## Future Enhancements

- QR code verification
- Email notifications
- Face recognition
- Mobile application

## Author

Abhinav, BCA (AI/ML), COER University
