<!-- README.md -->

<h1 align="center" style="color: #4CAF50;">Client-Server Arithmetic Chat</h1>

<p align="center">
  <strong style="color: #3498DB;">Author:</strong> Huy Quoc Nguyen <br/>
  <strong style="color: #3498DB;">Class:</strong> CPSC 3500 - Operating Systems
</p>

<p align="center" style="color: #7D3C98;">
  <em>A simple project demonstrating socket programming using C++ for client-server communication.</em>
</p>

<hr style="border: 1px solid #E74C3C;"/>

<h2 style="color: #F39C12;">Project Overview</h2>

<p style="color: #2E4053;">
  This project demonstrates the use of <strong>socket programming</strong> to establish a simple client-server connection for arithmetic operations. 
  It allows a client to send arithmetic expressions (addition, subtraction, multiplication, or division) to a server, 
  which processes the expression and returns the result. This project highlights my skills in <strong>network programming</strong>, 
  <strong>C++</strong>, and <strong>systems-level programming</strong>.
</p>

<h3 style="color: #F39C12;">Key Features:</h3>
<ul style="color: #2E4053;">
  <li>Client-Server communication over TCP using <strong>sockets</strong>.</li>
  <li>Regular expression validation to ensure correct input formatting.</li>
  <li><strong>Signal handling</strong> to gracefully terminate connections.</li>
  <li>Supports basic arithmetic operations.</li>
</ul>

<h2 style="color: #F39C12;">Technologies Used</h2>
<ul style="color: #2E4053;">
  <li><strong>C++</strong>: Core programming language.</li>
  <li><strong>Socket API</strong>: For creating and managing client-server connections.</li>
  <li><strong>POSIX Libraries</strong>: 
    <ul>
      <li>For network communication: <code>sys/socket.h</code>, <code>arpa/inet.h</code>, <code>netinet/in.h</code>, <code>unistd.h</code>.</li>
      <li>For handling signals: <code>csignal</code> (e.g., <code>SIGINT</code>).</li>
      <li>For string operations: <code>cstring</code>.</li>
    </ul>
  </li>
  <li><strong>Regex</strong>: Used for validating arithmetic expressions.</li>
</ul>

<h2 style="color: #F39C12;">Client-Server Workflow</h2>

<p style="color: #2E4053;">
  The client and server communicate over TCP. Here’s how they work:
</p>

<h3 style="color: #F39C12;">1. Server:</h3>
<ul style="color: #2E4053;">
  <li>Creates a socket and binds it to a specified port.</li>
  <li>Listens for incoming client connections.</li>
  <li>Accepts a connection and waits for a valid arithmetic expression from the client.</li>
  <li>Parses the expression, performs the calculation, and sends the result back to the client.</li>
</ul>

<h3 style="color: #F39C12;">2. Client:</h3>
<ul style="color: #2E4053;">
  <li>Connects to the server by providing the server's IP address and port number.</li>
  <li>Prompts the user for an arithmetic expression.</li>
  <li>Validates the input using regular expressions.</li>
  <li>Sends the validated expression to the server and waits for the result.</li>
  <li>Receives and displays the server's response.</li>
</ul>

<h4 style="color: #E74C3C;">Example Expression:</h4>
<pre style="color: #7D3C98;"><code>Client sends: 12 + 5</code></pre>
<pre style="color: #7D3C98;"><code>Server replies: 17</code></pre>

<h3 style="color: #E74C3C;">Signal Handling:</h3>
<p style="color: #2E4053;">Both the server and client gracefully handle termination signals (<code>CTRL+C</code>). Upon receiving a <code>SIGINT</code>, the client or server closes its socket and exits safely.</p>

<h2 style="color: #F39C12;">Setup and Usage</h2>

<h3 style="color: #3498DB;">Server:</h3>
<ol style="color: #2E4053;">
  <li>Compile the server code:
    <pre style="color: #7D3C98;"><code>g++ -o server server.cpp</code></pre>
  </li>
  <li>Run the server with a port number:
    <pre style="color: #7D3C98;"><code>./server &lt;port_number&gt;</code></pre>
    <strong style="color: #E74C3C;">Example:</strong>
    <pre style="color: #7D3C98;"><code>./server 8080</code></pre>
  </li>
</ol>

<h3 style="color: #3498DB;">Client:</h3>
<ol style="color: #2E4053;">
  <li>Compile the client code:
    <pre style="color: #7D3C98;"><code>g++ -o client client.cpp</code></pre>
  </li>
  <li>Run the client with the server's IP address and port number:
    <pre style="color: #7D3C98;"><code>./client &lt;server_ip_address&gt; &lt;port_number&gt;</code></pre>
    <strong style="color: #E74C3C;">Example:</strong>
    <pre style="color: #7D3C98;"><code>./client 127.0.0.1 8080</code></pre>
  </li>
</ol>

<h3 style="color: #F39C12;">Sample Client-Server Interaction</h3>

<p><strong style="color: #E74C3C;">Client Input:</strong></p>
<pre style="color: #7D3C98;"><code>Enter expression: 10 * 3</code></pre>

<p><strong style="color: #E74C3C;">Server Output:</strong></p>
<pre style="color: #7D3C98;"><code>Server replied: 30</code></pre>

<h3 style="color: #F39C12;">Error Handling</h3>

<p>If the client sends an invalid expression:</p>
<pre style="color: #7D3C98;"><code>Enter expression: 10 & 3</code></pre>
<p>Server will reply:</p>
<pre style="color: #7D3C98;"><code>Server replied: Invalid expression</code></pre>

<h3 style="color: #F39C12;">Signal Handling</h3>
<ul style="color: #2E4053;">
  <li>If the server receives a <code>SIGINT</code> (<code>Ctrl+C</code>), it will gracefully shut down:
    <pre style="color: #7D3C98;"><code>Server is shutting down...</code></pre>
  </li>
  <li>If the client receives a <code>SIGINT</code>, it will close the connection and exit:
    <pre style="color: #7D3C98;"><code>Client disconnected.</code></pre>
  </li>
</ul>

<h2 style="color: #F39C12;">Skills Demonstrated</h2>
<ul style="color: #2E4053;">
  <li><strong>Socket Programming</strong>: Implemented client-server communication using TCP sockets.</li>
  <li><strong>Process Management</strong>: Managed concurrent clients and safe termination using signals.</li>
  <li><strong>Regex for Input Validation</strong>: Used regular expressions to ensure only valid arithmetic expressions are processed.</li>
  <li><strong>Error Handling</strong>: Included error handling for socket failures, connection issues, and invalid inputs.</li>
</ul>


</code></pre>

<h2 style="color: #F39C12;">Future Improvements</h2>
<ul style="color: #2E4053;">
  <li><strong>Multithreading</strong>: Support multiple clients concurrently.</li>
  <li><strong>Extended Operations</strong>: Add support for more advanced operations (e.g., exponentiation, modulus).</li>
  <li><strong>Security</strong>: Implement encrypted communication using SSL/TLS.</li>
</ul>

<hr style="border: 1px solid #E74C3C;"/>
<p align="center" style="color: #7D3C98;">Made with ❤️ by Huy Quoc Nguyen</p>
