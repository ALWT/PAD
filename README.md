# PAD
./client localhost port_no user_file <br>
./chat port_no <br><br>
<hr>
Protocol<br>
socket connect to server <br>
send username<br>
send password<br>
if server logins user => "read okay"<br>
else => read "wrong" client closes<br>
connection created to server <br>
send messages: (int n = 4 Bytes -> message length) + (n chars = n*1 Bytes -> message) <br>
receive messages<br>
if message = "~exit" disconnect client from server + client closes<br> 

<hr>
<ul>
<li>login user <br>
<li>server reads possible users from .txt <br>
<li>server commands(main process for commands child for server) <br>
<li>server check  clients if socket is closed <br>
</ul>
