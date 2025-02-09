# IRC Server (ircserv)

## Overview
This project is an Internet Relay Chat (IRC) server written in C++98. It follows the IRC protocol and supports multiple clients, channels, and operator commands. The server is designed to handle multiple clients simultaneously using non-blocking I/O and poll (or an equivalent system call).

## Features
- **Multi-client support**: Allows multiple users to connect and interact simultaneously.
- **Authentication**: Clients must provide a password to connect.
- **Channel System**: Users can create, join, and leave channels.
- **Messaging**: Supports private messages and channel-wide messaging.
- **Operator Commands**:
  - `KICK` - Remove a user from a channel.
  - `INVITE` - Invite a user to a channel.
  - `TOPIC` - Change or view the channel topic.
  - `MODE` - Modify channel settings (invite-only, topic restrictions, passwords, operator privileges, user limits).
- **Non-blocking I/O**: Uses `poll()` (or an equivalent) to efficiently handle multiple connections.
- **TCP/IP Communication**: Fully compliant with IPv4 and IPv6.

## Compilation & Setup
### Prerequisites
- A C++98-compatible compiler
- Make
- A suitable IRC client for testing (e.g., `irssi`, `weechat`, `HexChat`, or `nc` for basic testing)

### Compilation
To build the project, run:
```bash
make
```
This will generate the executable `ircserv`.

### Running the Server
Run the server with:
```bash
./ircserv <port> <password>
```
Example:
```bash
./ircserv 6667 mypassword
```
- `<port>`: The port number the server will listen on.
- `<password>`: The password required for clients to connect.

### Connecting to the Server
You can use an IRC client to connect:
```bash
nc -C 127.0.0.1 6667
```
Alternatively, you can use an IRC client like HexChat or irssi:
```bash
/server 127.0.0.1 6667 mypassword
```

## Development Roadmap
### Completed
- Basic server setup
- Client authentication
- Channel creation and management
- Messaging system
- Operator commands
- Improved logging system
- More robust security measures
- Enhancements to error handling
- Additional IRC commands

## Contact
For inquiries or contributions:
- **GitHub**: (https://github.com/lukeslater0961)
- **Email**: luke.a.slater@outlook.com

---
