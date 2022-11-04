#include <WebServer.h> //Libary for Webserver 

WebServer server(80); // tell the Webserver to listen to port 80

bool isFire = 0;  //define variable "isFire" to false/true via webserver

String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Hello cruel world</h1>\
<a href='/Worms'><button>Rainbow Worms</button></a>\
<a href='/Fire'><button>Set living room on Fire</button></a>\
</body>\
</html>";                                         // defining HTML & CSS contents which are displayed in browser



String HTMLFire = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Someone call 118</h1>\
<a href='/Worms'><button>Ranbow Worms</button></a>\
<a href='/Fire'><button>Set living room on Fire</button></a>\
</body>\
</html>";                                          // defining HTML & CSS contents which are displayed in browser



String HTMLWorms = "<!DOCTYPE html>\
<html>\
<body>\
<h1>Rainbows and Unichorns</h1>\
<a href='/Worms'><button>Rainbow Worms</button></a>\
<a href='/Fire'><button>Set living room on Fire</button></a>\
</body>\
</html>";                                           // defining HTML & CSS contents which are displayed in browser

void Fire() {
    isFire = 1; //set variable isFire to true (in FireAnd Worms.h) as on 192.168.1.22 the "fire button" is toggled
    server.send(200, "text/html", HTMLFire);
}

void Worms() {
    isFire = 0; //set variable "isFire" to false (in FireAndWorms.h)
    server.send(200, "text/html", HTMLWorms);
}

void handleRoot() {
    server.send(200, "text/html", HTML); 
}

void setup_routing() {      
  server.on("/Fire", Fire); // calls the wuermli Fire function if you access the page 192.168.1.22/Fire
  server.on("/Worms", Worms); // calls the wuermli Worms function if you access the page 192.168.1.22/Worms
  server.on("/", handleRoot);
  server.begin();  // start server  
}
