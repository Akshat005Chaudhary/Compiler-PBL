# Use an official Node.js image with Debian base
FROM node:18-bullseye

# Install Graphviz and build tools (g++ compiler, make)
RUN apt-get update && apt-get install -y graphviz g++ make

# Set working directory inside container
WORKDIR /app

# Copy package files and install dependencies
COPY package*.json ./
RUN npm install

# Copy the entire project source code
COPY . .

# Build your C++ program
RUN g++ cpp/p1.cpp -o cpp/p1

# Expose your app port (adjust if your server listens on a different port)
EXPOSE 3000

# Start your Node.js server
CMD ["node", "server.js"]
