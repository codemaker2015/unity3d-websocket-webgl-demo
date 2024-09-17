const WebSocket = require('ws');
const server = new WebSocket.Server({ port: 8080 });

server.on('connection', (ws) => {
  console.log('Client connected');

  // Function to generate a random message
  const getRandomMessage = () => {
    const messages = [
      'Hello, world!',
      'How are you?',
      'Have a great day!',
      'WebSocket is cool!',
      'Here is a random message.',
      'Enjoy the connection!',
      'Keep it up!',
      'Nice to meet you!'
    ];
    const randomIndex = Math.floor(Math.random() * messages.length);
    return messages[randomIndex];
  };

  // Send a welcome message to the client
  ws.send('Welcome to the WebSocket server!');

  // Set an interval to send a random message every second
  const intervalId = setInterval(() => {
    const randomMessage = getRandomMessage();
    ws.send(randomMessage);
  }, 1000);

  ws.on('message', (message) => {
    console.log(`Received message: ${message}`);
    // Echo the message back to the client
    ws.send(`Server received: ${message}`);
  });

  ws.on('close', () => {
    console.log('Client disconnected');
    // Clear the interval when the client disconnects
    clearInterval(intervalId);
  });

  ws.on('error', (error) => {
    console.error(`WebSocket error: ${error}`);
  });
});

console.log('WebSocket server is running on ws://localhost:8080');
