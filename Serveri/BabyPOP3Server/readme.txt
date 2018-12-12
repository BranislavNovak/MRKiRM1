Baby POP3 Server 1.04 by Pablo Software Solutions 

In my daily job I have done several projects related to email (POP3/SMTP/IMAP4). One of the problems (at least in my company) is that there are never good test servers available. So that's why I decided to create this simple POP3 server.

Key Features

Supports most RFC1939 POP3 commands
Supports multiple POP3 user accounts
Integration with Microsoft SMTP server
Multi threaded
It's free!

Configuration:
To make this POP3 server usefull you must also have a SMTP server installed. Microsoft's SMTP server from ISS will be fine (included with Windows NT/2000/XP).
Usually this SMTP will have a maildrop folder were it will drop incoming messages. Microsoft's SMTP server uses c:\inetpub\mailroot\drop\ by default.
This maildrop folder is the input folder for our POP3 server, so open the Settings dialog and set the mail folder.
The timeout setting let's you specify the timeout value for receiving data. When clients become idle for too long the connection will be closed.
In User Accounts you can Add/Edit and Delete user accounts. The admin account is default and can't be removed.

