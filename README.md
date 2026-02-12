
 # Project Title : Meal Card Usage Tracker System
 
  Description   :
  This project is a console-based Meal Card Usage Tracker System developed in C++.
  It manages student meal card registration, tracks daily meal usage (Breakfast, Lunch,
  and Dinner), prevents duplicate meal consumption per session, and provides administrative analytics.
 
 ## Authentication:
   - The system uses a binary file (credentials.dat) for login credentials.
   - For evaluation purposes, the following accounts are available:
       Admin:
           Username: admin
           Password: admin123
       Staff:
           Username: staff
           Password: staff123
     NOTE: In a real deployment, credentials would be provided securely by the IT department.
 
  Supported Roles:
    1. Admin  – Full access (register students, view/edit records, track meals,
                view analytics, and manage student data).
    2. Staff  – Limited access (track student meals only).
 
  Key Features :
   - Student registration and record management
   - Meal session detection based on system time
   - Prevention of multiple meals in the same session
   - Persistent storage using CSV and text files
   - Daily meal analytics (Breakfast, Lunch, Dinner)
   - Search, filter, sort, edit, and delete student records
 
  Technologies Used :
   - Language : C++
   - File Handling : CSV (.csv), Text files (.txt), Binary files (.dat)
   - Libraries : iostream, fstream, vector, string, ctime, iomanip, cctype
 
  Date : 2026
